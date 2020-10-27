#! /usr/bin/env python3

import json
import networkx as nx
import matplotlib.pyplot as plt
import pydot
from networkx.drawing.nx_pydot import graphviz_layout
from argparse import ArgumentParser

from ascii_tree import make_and_print_tree


class NTreeNode:
    def __init__(self, val):
        self.val = val
        self.children = []
        
def get_value(no:NTreeNode):
        return no.val

def get_children(no:NTreeNode):
        return no.children

def node_validate(t:dict)-> bool:
    node_required = ['hostname', 'key', 'queues', 'children', 'parent']
    node_key_required = ['node_index', 'level', 'type']
    queue_required = ['key', 'url', 'hook', 'topic']
    queue_key_required = ['node_index', 'tier_index', 'mode', 'type']
    queue_key_type_required = ['value', 'interval']
    
    def queues_validator(qt:list)->bool:
        for i in qt:
            if list(i.keys()) == queue_required:
                if list(i['key']) == queue_key_required:
                    if list(i['key']['type'].keys()) == queue_key_type_required:
                        continue
                    else:
                        print(f"Error in QUEUE {i['key']}, Missing TYPE needs {queue_key_type_required}")
                        return False
                        
                else :
                    print(f"Error in QUEUE {i['key']}, Missing KEY needs {queue_key_required}")
                    return False
            else :
                print(f"Error in QUEUE {i['key']}, MISSING QUEUE CONFIG REQUIREMENTS needs {queue_required}")
                return False
        return True
    
    if list(t.keys()) != node_required:
        print(f"Error in {t['key']} , GENERAL MESSED UP, needs {node_required}")
        return False
    elif list(t['key'].keys())!= node_key_required:
        print(f"Error in {t['key']['node_index']} , KEY ERROR needs {node_key_required}")
        return False
    elif not queues_validator(t['queues']):
        print(f"Error in NODE {t['key']['node_index']} , QUEUE ERROR needs {queue_required}")        
        return False
    
    return True


def nodes_validator(configs:list)-> int:
    return True if sum(list(map(node_validate, configs))) == len(configs) else False

def main(config_filename:str, draw_tree:bool)-> int:
    conf_file =open(config_filename, "r")
    file_contents = conf_file.read()
    configs = json.loads(file_contents)
    is_good = nodes_validator(configs)
    if is_good:
        print("Everything looks good chief")
    else :
        print("You done messed up A-Aron")
        
    if draw_tree and is_good:
        draw_tree_config(configs)
    return 0



def draw_tree_config(configs:list)-> None:
    nodes = {}
    for i in configs:
        if i["key"]["node_index"] not in nodes.keys():
            nodes[i["key"]["node_index"]] = NTreeNode(f'{i["key"]["node_index"]}')
        for j in i["children"]:
            if j not in nodes.keys():
                nodes[j] = NTreeNode(f'{j}')
            nodes[i["key"]["node_index"]].children.append(nodes[j])
    make_and_print_tree(nodes[0], get_value, get_children)

if __name__ == "__main__":
    
    def str2bool(v):
       if v.lower() in ('yes', 'true', 't', 'y', '1'):
           return True
       elif v.lower() in ('no', 'false', 'f', 'n', '0'):
           return False
       else:
           raise argparse.ArgumentTypeError('Boolean value expected.')
   
    parser = ArgumentParser()
    parser.add_argument('-c', '--conf',type=str,
                help="Config file to validate", required=True)
    parser.add_argument('-d','--draw_tree' ,type=str2bool, nargs='?',const=True,
                help="If set will draw the tree of the config", default=False)
    args = parser.parse_args()

    main(args.conf, args.draw_tree)

