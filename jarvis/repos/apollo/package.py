from jarvis_cd.echo_node import EchoNode
from jarvis_cd.exception import Error, ErrorCode
from jarvis_cd.exec_node import ExecNode
# from repos.orangefs import package
from jarvis_cd.graph import Graph
import os
import socket
import time

from jarvis_cd.scp_node import SCPNode
from jarvis_cd.sleep_node import SleepNode
from jarvis_cd.ssh_node import SSHNode
from repos.orangefs.package import Orangefs


class Apollo(Graph):
    _default_config = "/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini"

    def __init__(self, config_file=None, experiment=None):
        super().__init__(config_file, self._default_config)
        self.experiment = experiment
        self.executable = self.config['COMMON']['EXECUTABLE']
        self.apollo_path = self.config['COMMON']['APOLLO_CONFIG']
        self.apollo_config = self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/config.json"
        self.redis_config = self.config['COMMON']['REDIS_CONFIG']
        self.redis_path = self.config['COMMON']['REDIS_PATH']

        # self.orangefs = Orangefs("/home/jcernudagarcia/Apollo/jarvis/repos/orangefs/default.ini")

        self.fact_hosts = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/fact")
        self.insight_hosts = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/insight")
        self.client_host = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/client")
        self.redis_hosts = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/redis")

        self.pid_path = self.config['COMMON']['PID_PATH']
        self.result_dir = self.config['COMMON']['RESULT_DIR']

        self.ld_path_comp = self.config['COMMON']['LD_PATH_COMP']
        self.ld_path_stor = self.config['COMMON']['LD_PATH_STOR']

        self.path="/opt/ohpc/pub/software/hdevarajan/spack3/v0.15.4.scs/var/spack/environments/apollo/.spack-env/view/bin:/opt/ohpc/pub/software/hdevarajan/spack3/v0.15.4.scs/bin:/home/jcernudagarcia/software/install/python/bin:/opt/xcat/bin:/opt/xcat/sbin:/opt/xcat/share/xcat/tools:/opt/lenovo/onecli:/usr/lib/heimdal/bin:/opt/confluent/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/opt/ibutils/bin:opt/ohpc/pub/utils/cmake/3.10.2/bin:/home/jcernudagarcia/software/install/bin:/home/jcernudagarcia/software/install/sbin:/opt/ohpc/pub/orangefs/sbin:/opt/ohpc/pub/orangefs/bin:/usr/pgsql-10/bin:/usr/lib/apache-maven-3.5.4/bin:/opt/ohpc/pub/mpi/openmpi3-gnu7/3.0.0/bin:/opt/ohpc/pub/compiler/gcc/7.3.0/bin:/opt/ohpc/pub/utils/prun/1.2:/opt/ohpc/pub/bin"

        self.ldms = False

    def _DefineClean(self):
        nodes = [SSHNode("clean Redis data", self.redis_hosts, "redis-cli flushall")]
        return nodes

    def _DefineStatus(self):
        nodes = [SSHNode("Check Redis", self.redis_hosts, "redis-cli ping", print_output=True),
                 SSHNode("Check Fact Vertex", self.fact_hosts, "ps -eaf | grep SCoRe", print_output=True)]
        return nodes

    def _DefineStop(self):
        # TODO: Check the name of the executables for both cases, so that we can kill them.
        redis_cmd = f"{self.redis_path}redis-cli -p 6379 flushall; {self.redis_path}redis-cli -p 6380 flushall; killall redis-server"
        nodes = []
        nodes += self.clean_vertex(self.insight_hosts)
        nodes += self.clean_vertex(self.insight_hosts)
        nodes += self.clean_vertex(self.fact_hosts)
        nodes += self.clean_clients(list(self.client_host))
        nodes.append(SSHNode("Stopping  Redis", list(self.redis_hosts.keys()), redis_cmd))

        return nodes

    def _DefineStart(self):
        nodes = []
        nodes += self.spawn_redis(self.redis_hosts)
        # nodes += self.orangefs.Start()
        nodes += self.spawn_tempfs(list(self.client_host.keys()))
        nodes += self.spawn_vertex(self.fact_hosts)
        nodes += self.spawn_vertex(self.insight_hosts)
        nodes += self.spawn_clients(self.client_host)
        return nodes

    def clean_vertex(self, host, out=True):
        clean_nodes = []
        for client in host.keys():
            cmd = []
            client_path = self.ld_path(client)
            for vertex_id in host[client]:
                cmd.append(
                    f"export LD_LIBRARY_PATH={client_path}:$LD_LIBRARY_PATH; echo $LD_LIBRARY_PATH; "
                    f"kill -9 `cat {self.pid_path}/score_{vertex_id}.pid`; "
                    f"rm {self.pid_path}/score_{vertex_id}.pid")
            clean_nodes.append(SSHNode("Stopping Vertex", client, cmd, print_output=out))
        return clean_nodes

    def spawn_vertex(self, host, out=True, insight=False):
        spawn_nodes = []
        for client in host.keys():
            cmd = []
            client_path = self.ld_path(client)
            for vertex_id in host[client]:
                if insight:
                    cmd.append(f"export LD_LIBRARY_PATH={client_path}:$LD_LIBRARY_PATH; echo $LD_LIBRARY_PATH; "
                               f"{self.executable}/SCoRe {self.apollo_config} {vertex_id} {self.pid_path}/score_{vertex_id}.pid; "
                               f"sleep 5")
                else:
                    cmd.append(f"export LD_LIBRARY_PATH={client_path}:$LD_LIBRARY_PATH; echo $LD_LIBRARY_PATH; "
                               f"{self.executable}/SCoRe {self.apollo_config} {vertex_id} {self.pid_path}/score_{vertex_id}.pid; "
                               f"sleep 5")
            spawn_nodes.append(SSHNode("Start Vertex", client, cmd, print_output=out))
        return spawn_nodes

    def spawn_redis(self, redis_hosts):
        redis_nodes = []
        redis_cmd = f"nohup {self.redis_path}redis-server {self.redis_config}/redis_1.conf; sleep 10; " \
                    f"{self.redis_path}redis-cli ping"
        ldms_cmd = f"nohup {self.redis_path}redis-server {self.redis_config}/ldms.conf; sleep 10; " \
                   f"{self.redis_path}redis-cli -p 6380 ping"
        for client in redis_hosts.keys():
            for node in redis_hosts[client]:
                if node == "1":
                    redis_nodes.append(SSHNode("Start Redis", client, ldms_cmd, print_output=True))
                    self.ldms = True
                elif node == "2":
                    redis_nodes.append(SSHNode("Start Redis", client, ldms_cmd, print_output=True))
                else:
                    redis_nodes.append(SSHNode("Start Redis", client, redis_cmd, print_output=True))
        return redis_nodes

    def spawn_clients(self, client_hosts):
        hosts = list(self.client_host.keys())
        num_clients = len(hosts)
        with open(f"{self.apollo_path}client_hostfile", mode='wt', encoding='utf-8') as hostfile:
            hostfile.write('\n'.join(hosts))
        if self.ldms:
            # client_cmd = f"echo Client; export LD_LIBRARY_PATH={self.ld_path_comp}:$LD_LIBRARY_PATH; echo $LD_LIBRARY_PATH; "
            mpi_cmd = f"export PATH={self.path}; export LD_LIBRARY_PATH={self.ld_path_comp}:$LD_LIBRARY_PATH; " \
                      f"mpirun -n {num_clients} -f {self.apollo_path}client_hostfile {self.executable}/ldms_client_test tcp://ares-comp-13:6380 >> {self.result_dir}/ldms_client_results"
        else:
            # client_cmd = f"echo Client; export LD_LIBRARY_PATH={self.ld_path_comp}:$LD_LIBRARY_PATH; echo $LD_LIBRARY_PATH; "
            mpi_cmd = f"export PATH={self.path}; export LD_LIBRARY_PATH={self.ld_path_comp}:$LD_LIBRARY_PATH; " \
                      f"mpirun -n {num_clients} -f {self.apollo_path}client_hostfile {self.executable}/real_client_test tcp://ares-comp-13:6380 >> {self.result_dir}/real_client-results"

        # client_nodes = [SSHNode("Start Client", hosts[0], f"echo {self.experiment} >> {self.result_dir}/real_client_test"),
        #                 SSHNode("Start mpi", hosts[0], mpi_cmd)]

        client_nodes = [ExecNode("Start Client", hosts[0], f"echo {self.experiment} >> {self.result_dir}/real_client_test")]

        return client_nodes

    def spawn_tempfs(self, tempfs_hosts):
        tempfs_nodes = []
        tempfs_cmd = "mkdir -p /mnt/nvme/jcernudagarcia/apollo_nvme/; mkdir -p /mnt/nvme/jcernudagarcia/tempfs; " \
                     "sudo mount -t tmpfs -o size=3G tmpfs /mnt/nvme/jcernudagarcia/tempfs"
        tempfs_nodes.append(SSHNode("Spawn Tempfs", tempfs_hosts, tempfs_cmd, print_output=True))
        return tempfs_nodes

    def ld_path(self, client):
        if "comp" in client:
            ld_path = self.ld_path_comp
        elif "stor" in client:
            ld_path = self.ld_path_stor
        else:
            raise Exception("Wrong host")
        return ld_path

    def clean_clients(self, client_hosts):
        client_clean = []
        for client in client_hosts:
            if "stor" in client:
                pass
            elif "comp" in client:
                cmd = "rm -rf /mnt/nvme/jcernudagarcia/tempfs/*; rm -rf /mnt/nvme/jcernudagarcia/pvfs2-mount/*; rm -rf /mnt/nvme/jcernudagarcia/apollo_nvme/* "
                client_clean.append(SSHNode("Clean Clients", client, cmd))
            else:
                raise Exception("Wrong host")
        return client_clean
