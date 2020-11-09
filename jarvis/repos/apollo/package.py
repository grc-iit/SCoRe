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
    _default_config = "repos/apollo/default.ini"

    def __init__(self, config_file=None, experiment=None):
        super().__init__(config_file, self._default_config)
        self.experiment = experiment
        self.executable = self.config['COMMON']['EXECUTABLE']
        self.apollo_config = self.config['COMMON']['APOLLO_CONFIG'] + self.experiment + ".json"
        self.redis_config = self.config['COMMON']['REDIS_CONFIG']
        self.redis_path = self.config['COMMON']['REDIS_PATH']
        self.hosts = self._convert_pairs_toMap(self.config['COMMON']['APOLLO_CONFIG'] + self.experiment)
        self.redis_hosts = self._make_list_unique(self.hosts.keys())

    def _DefineClean(self):
        nodes = [SSHNode("clean Redis data", self.redis_hosts,
                         "redis-cli flushall")]
        return nodes

    def _DefineStatus(self):
        nodes = [SSHNode("check Fact Vertex", self.redis_hosts, "redis-cli ping", print_output=True)]
        return nodes

    def _DefineStop(self):
        # TODO: Check the name of the executables for both cases, so that we can kill them.
        cmds = [
            "redis-cli flushall",
            "killall reddis-server",
            "killall SCoRe"
        ]
        nodes = [SSHNode("stop SCoRe", self.redis_hosts, cmds)]
        return nodes

    def _DefineStart(self):
        nodes = []
        # set pvfstab on clients
        for client in self.redis_hosts:
            redis_cmd = f"{self.redis_path}redis-server {self.redis_config}"
            nodes.append(SSHNode("Start reddis", client, redis_cmd))

        for client in self.hosts.keys():
            cmd = []
            for vertex_id in self.hosts[client]:
                if vertex_id == -1:
                    cmd.append(f"{self.executable}/client_test >> client_results")
                else:
                    cmd.append(f"{self.executable}/SCoRe {self.apollo_config} {vertex_id}")
            nodes.append(SSHNode("Start Vertex", client, cmd))
        return nodes
