import argparse

from jarvis_cd.enumerations import OperationType, LogLevel


def str2bool(v):
    if isinstance(v, bool):
        return v
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected.')


class ArgumentParser(object):
    __instance = None

    @staticmethod
    def get_instance():
        """ Static access method. """
        if ArgumentParser.__instance is None:
            ArgumentParser()
        return ArgumentParser.__instance

    def __init__(self):
        super().__init__()
        """ Virtually private constructor. """
        if ArgumentParser.__instance is not None:
            raise Exception("This class is a singleton!")
        else:
            ArgumentParser.__instance = self
        self.parser = argparse.ArgumentParser(description='Apollo Deployment')
        self.parser.add_argument("target", metavar='target', type=str,
                                 help="Target to choose.")
        self.parser.add_argument("operation", metavar='operation', default=OperationType.START, type=OperationType, choices=list(OperationType),
                                 help="Operation for target")
        self.parser.add_argument("config", metavar='config', default=OperationType.START, type=str,
                                 help="Config file")
        self.parser.add_argument("experiment", metavar='experiment', default="localhost_1", type=str,
                                 help="Current Experiment to run")
        self.parser.add_argument("-ll","--log-level", default=LogLevel.ERROR, type=LogLevel,
                                 choices=list(LogLevel),
                                 help="Log level for execution")
        self.parser.add_argument("--log-path", type=str, default="/tmp/jarvis-cd/jarvis.log",
                                 help="Jarvis log file.")
        self.args = self.parser.parse_args()
        self._validate()

    def _validate(self):
        pass