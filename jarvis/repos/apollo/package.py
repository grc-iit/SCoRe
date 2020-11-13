from jarvis_cd.echo_node import EchoNode
from jarvis_cd.exception import Error, ErrorCode
from jarvis_cd.exec_node import ExecNode
from jarvis_cd.graph import Graph
import os
import socket
import time

from jarvis_cd.scp_node import SCPNode
from jarvis_cd.sleep_node import SleepNode
from jarvis_cd.ssh_node import SSHNode


class Apollo(Graph):
    _default_config = "/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini"

    def __init__(self, config_file=None, experiment=None):
        super().__init__(config_file, self._default_config)
        self.experiment = experiment
        self.executable = self.config['COMMON']['EXECUTABLE']
        self.apollo_config = self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/config.json"
        self.redis_config = self.config['COMMON']['REDIS_CONFIG']
        self.redis_path = self.config['COMMON']['REDIS_PATH']

        self.fact_hosts = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/fact")
        self.insight_hosts = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/insight")
        self.client_host = self._convert_pairs_toMap(
            self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + "/client")
        all_hosts = list(self.fact_hosts.keys()) + list(self.insight_hosts.keys()) + list(self.client_host.keys())
        self.redis_hosts = self._make_list_unique(all_hosts)

        self.pid_path = self.config['COMMON']['PID_PATH']
        self.result_dir = self.config['COMMON']['RESULT_DIR']
        self.ld_path = self.config['COMMON']['LD_PATH']

    def _DefineClean(self):
        nodes = [SSHNode("clean Redis data", self.redis_hosts, "redis-cli flushall")]
        return nodes

    def _DefineStatus(self):
        nodes = [SSHNode("Check Redis", self.redis_hosts, "redis-cli ping", print_output=True),
                 SSHNode("Check Fact Vertex", self.fact_hosts, "ps -eaf | grep SCoRe", print_output=True)]
        return nodes

    def _DefineStop(self):
        # TODO: Check the name of the executables for both cases, so that we can kill them.
        redis_cmd = f"{self.redis_path}redis-cli flushall; killall redis-server"
        nodes = []
        nodes += self.clean_vertex(self.insight_hosts)
        nodes += self.clean_vertex(self.fact_hosts)
        nodes.append(SSHNode("Stopping  Redis", self.redis_hosts, redis_cmd))

        return nodes

    def _DefineStart(self):
        nodes = []
        redis_cmd = f"nohup {self.redis_path}redis-server {self.redis_config}; sleep 30; " \
                    f"{self.redis_path}redis-cli ping"
        client_cmd = f"export LD_LIBRARY_PATH={self.ld_path}; echo $LD_LIBRARY_PATH; mkdir -p {self.result_dir}/; " \
                     f"echo {self.experiment} >> {self.result_dir}/client-results; " \
                     f"echo {self.experiment}; " \
                     f"{self.executable}/client_test >> {self.result_dir}/client-results "

        nodes.append(SSHNode("Start redis", self.redis_hosts, redis_cmd, print_output=True))
        nodes += self.spawn_vertex(self.fact_hosts)
        nodes += self.spawn_vertex(self.insight_hosts)
        nodes.append(SSHNode("Start Client", list(self.client_host.keys()), client_cmd, print_output=True))
        return nodes

    def clean_vertex(self, host, out=True):
        clean_nodes = []
        for client in host.keys():
            cmd = []
            for vertex_id in host[client]:
                cmd.append(
                    f"export LD_LIBRARY_PATH={self.ld_path}; echo $LD_LIBRARY_PATH; "
                    f"kill -9 `cat {self.pid_path}/score_{vertex_id}.pid`; "
                    f"rm {self.pid_path}/score_{vertex_id}.pid")
            clean_nodes.append(SSHNode("Stopping Fact Vertex", client, cmd, print_output=out))
        return clean_nodes

    def spawn_vertex(self, host, out=True):
        spawn_nodes = []
        for client in host.keys():
            cmd = []
            for vertex_id in host[client]:
                cmd.append(f"export LD_LIBRARY_PATH={self.ld_path}; echo $LD_LIBRARY_PATH; "
                           f"{self.executable}/SCoRe {self.apollo_config} {vertex_id} {self.pid_path}/score_{vertex_id}.pid; "
                           f"sleep 10")
            spawn_nodes.append(SSHNode("Start Vertex", client, cmd, print_output=out))
        return spawn_nodes
