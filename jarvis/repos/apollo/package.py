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

        self.ldms = True

    def _DefineClean(self):
        nodes = [SSHNode("clean Redis data", self.redis_hosts, "redis-cli flushall")]
        return nodes

    def _DefineStatus(self):
        nodes = [SSHNode("Check Redis", self.redis_hosts, "redis-cli ping", print_output=True),
                 SSHNode("Check Fact Vertex", self.fact_hosts, "ps -eaf | grep SCoRe", print_output=True)]
        return nodes

    def _DefineStop(self):
        # TODO: Check the name of the executables for both cases, so that we can kill them.
        redis_cmd = f"{self.redis_path}redis-cli flushall; {self.redis_path}redis-cli -p 6380 flushall; killall redis-server"
        nodes = []
        nodes += self.clean_vertex(self.insight_hosts)
        # nodes += self.orangefs.Stop()
        nodes += self.clean_vertex(self.fact_hosts)
        nodes.append(SSHNode("Stopping  Redis", self.redis_hosts, redis_cmd))

        return nodes

    def _DefineStart(self):
        nodes = []
        nodes += self.spawn_redis(self.redis_hosts)
        # nodes += self.orangefs.Start()
        nodes += self.spawn_tempfs(list(self.client_host.keys()))
        nodes += self.spawn_vertex(self.fact_hosts)
        nodes += self.spawn_vertex(self.insight_hosts)
        nodes += self.spawn_clients(list(self.client_host.keys()))
        return nodes

    def clean_vertex(self, host, out=True):
        clean_nodes = []
        for client in host.keys():
            cmd = []
            client_path = self.ld_path(client)
            for vertex_id in host[client]:
                cmd.append(
                    f"export LD_LIBRARY_PATH={client_path}; echo $LD_LIBRARY_PATH; "
                    f"kill -9 `cat {self.pid_path}/score_{vertex_id}.pid`; "
                    f"rm {self.pid_path}/score_{vertex_id}.pid")
            clean_nodes.append(SSHNode("Stopping Fact Vertex", client, cmd, print_output=out))
        return clean_nodes

    def spawn_vertex(self, host, out=True, insight=False):
        spawn_nodes = []
        for client in host.keys():
            cmd = []
            client_path = self.ld_path(client)
            for vertex_id in host[client]:
                if insight:
                    cmd.append(f"sleep 30; export LD_LIBRARY_PATH={client_path}; echo $LD_LIBRARY_PATH; "
                               f"{self.executable}/SCoRe {self.apollo_config} {vertex_id} {self.pid_path}/score_{vertex_id}.pid; "
                               f"sleep 10")
                else:
                    cmd.append(f"export LD_LIBRARY_PATH={client_path}; echo $LD_LIBRARY_PATH; "
                               f"{self.executable}/SCoRe {self.apollo_config} {vertex_id} {self.pid_path}/score_{vertex_id}.pid; "
                               f"sleep 10")
            spawn_nodes.append(SSHNode("Start Vertex", client, cmd, print_output=out))
        return spawn_nodes

    def spawn_redis(self, redis_hosts):
        redis_nodes = []
        redis_cmd = f"nohup {self.redis_path}redis-server {self.redis_config}/redis_1.conf; sleep 30; " \
                    f"{self.redis_path}redis-cli ping"
        ldms_cmd = f"nohup {self.redis_path}redis-server {self.redis_config}/ldms.conf; sleep 30; " \
                   f"{self.redis_path}redis-cli -p 6380 ping"
        for client in redis_hosts.keys():
            if redis_hosts[client] == "1":
                redis_nodes.append(SSHNode("Start Client", client, ldms_cmd, print_output=True))
            else:
                redis_nodes.append(SSHNode("Start Client", client, redis_cmd, print_output=True))
        return redis_nodes

    def spawn_clients(self, client_hosts):
        if self.ldms:
            client_cmd = f"export LD_LIBRARY_PATH={self.ld_path_comp}; echo $LD_LIBRARY_PATH; " \
                         f"mkdir -p {self.result_dir}/; " \
                         f"echo {self.experiment} >> {self.result_dir}/client-results; " \
                         f"echo {self.experiment}; " \
                         f"{self.executable}/client_test >> {self.result_dir}/client-results "
        else:
            client_cmd = f"export LD_LIBRARY_PATH={self.ld_path_comp}; echo $LD_LIBRARY_PATH; " \
                         f"mkdir -p {self.result_dir}/; " \
                         f"echo {self.experiment} >> {self.result_dir}/client-results; " \
                         f"echo {self.experiment}; " \
                         f"{self.executable}/client_test >> {self.result_dir}/client-results "
        client_nodes = []
        SSHNode("Start Client", client_hosts, client_cmd, print_output=True)
        return client_nodes

    def spawn_tempfs(self, tempfs_hosts):
        redis_nodes = []
        tempfs_cmd = "mkdir /tmp/jaime_apollo; sudo mount -t tmpfs -o size=2G tmpfs /tmp/jaime_apollo/"
        SSHNode("Start Client", tempfs_hosts, tempfs_cmd, print_output=True)
        return redis_nodes

    def ld_path(self, client):
        if "comp" in client:
            ld_path = self.ld_path_comp
        elif "stor" in client:
            ld_path = self.ld_path_stor
        else:
            raise Exception("Wrong host")
        return ld_path
