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
        self.apollo_config = self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + ".json"
        self.redis_config = self.config['COMMON']['REDIS_CONFIG']
        self.redis_path = self.config['COMMON']['REDIS_PATH']
        self.hosts = self._convert_pairs_toMap(self.config['COMMON']['APOLLO_CONFIG'] + self.experiment)
        self.redis_hosts = self._make_list_unique(self.hosts.keys())
        self.pid_path = self.config['COMMON']['PID_PATH']
        self.result_dir = self.config['COMMON']['RESULT_DIR']
        self.ld_path = self.config['COMMON']['LD_PATH']

    def _DefineClean(self):
        nodes = [SSHNode("clean Redis data", self.redis_hosts,
                         "redis-cli flushall")]
        return nodes

    def _DefineStatus(self):
        nodes = [SSHNode("check Fact Vertex", self.redis_hosts, "redis-cli ping", print_output=True)]
        return nodes

    def _DefineStop(self):
        # TODO: Check the name of the executables for both cases, so that we can kill them.
        redis_cmds = [
            f"{self.redis_path}redis-cli flushall",
            f"killall redis-server"
        ]
        nodes = []
        for client in self.hosts.keys():
            cmd = []
            for vertex_id in self.hosts[client]:
                if vertex_id != "-1":
                    cmd.append(
                        f"export LD_LIBRARY_PATH={self.ld_path}; echo $LD_LIBRARY_PATH; "
                        f"kill -9 `cat {self.pid_path}/score_{vertex_id}.pid`; "
                        f"rm {self.pid_path}/score_{vertex_id}.pid")
            nodes.append(SSHNode("Start Vertex", client, cmd))
        nodes.append(SSHNode("stop SCoRe", self.redis_hosts, redis_cmds))
        return nodes

    def _DefineStart(self):
        print(os.environ['LD_LIBRARY_PATH'])
        nodes = []
        # set pvfstab on clients
        for client in self.redis_hosts:
            redis_cmd = f"nohup {self.redis_path}redis-server {self.redis_config}"
            nodes.append(SSHNode("Start reddis", client, redis_cmd, print_output=True))
            nodes.append(SSHNode("Start reddis", client, f"sleep 30; {self.redis_path}redis-cli ping; sleep 10", print_output=True))

        for client in self.hosts.keys():
            cmd = []
            for vertex_id in self.hosts[client]:
                if vertex_id == "-1":
                    cmd.append(f"mkdir -p {self.result_dir}i/; "
                               f"echo {self.experiment} >> {self.result_dir}/client-results"
                               f"{self.executable}/client_test >> {self.result_dir}/client-results")
                else:
                    cmd.append(f"export LD_LIBRARY_PATH={self.ld_path}; echo $LD_LIBRARY_PATH; "
                               f"{self.executable}/SCoRe {self.apollo_config} {vertex_id} {self.pid_path}/score_{vertex_id}.pid; "
                               f"sleep 10")
            nodes.append(SSHNode("Start Vertex", client, cmd, print_output=True))
        return nodes
