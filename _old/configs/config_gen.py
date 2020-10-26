import json


with open("nodelist") as node_list_file:
    node_list = list(map(lambda x: x.strip(),node_list_file.readlines()))
    
with open("queuelist") as queue_list_file:
    queue_list = list(map(lambda x: x.strip(),queue_list_file.readlines()))
    
with open("tierlist") as tier_list_file:
    tier_list = list(map(lambda x: x.strip(),tier_list_file.readlines()))
    
with open("clusterlist") as tier_list_file:
    cluster_list = list(map(lambda x: x.strip(),tier_list_file.readlines()))


nodes = {}
for k in node_list:
    node = {}

    queues = []
    
    node_hostname = k
    node_key_node_index = k.split('-')[-1]
    node_key_type = "LEAF"
    node_children_list = []
    node_parent = 0
    queue_template_list = '['

    for ti, tl in enumerate(tier_list):
        tier_level = tl.split(':')[0][tl.find('T')+1:-1]
        node_key_level = tier_level if node_key_type != "LEAF" else -1 
        queue = {}
        for i, j in enumerate(queue_list):
            queue = {}
        
            queue_node_index = i
            queue_node_tier_index = tier_level
            queue_node_type_value = j
            queue_node_type_interval = 10000
            queue_mode = "SERVER"
            queue_url = f"tcp://{k}"
            queue_hook = j
            queue_topic = f'{node_key_type}_{j}'
            
            queue["key"] = {"node_index": queue_node_index,
                            "tier_index": queue_node_tier_index,
                            "mode" : queue_mode,
                            "type": {
                                "value": queue_node_type_value,
                                "interval": queue_node_type_interval
                            }
                           }
            queue["url"] = queue_url
            queue["hook"] = queue_hook
            queue["topic"] = queue_topic
            
            queues.append(queue)
    
    node["hostname"] = node_hostname
    node["key"] = { "node_index" : node_key_node_index, "level" : node_key_level, "type" : node_key_type }
    node["queues"] = queues
    node["children"] = node_children_list
    node["parent"] = node_parent
    

    nodes[int(node_key_node_index)] = node


tier_nodes={}
node_key_type = "TIER"
node_children_list = []
node_parent = 0

for ti, tl in enumerate(tier_list):
    tier_level = tl.split(':')[0][tl.find('T')+1:-1]
    k = tier_list[0].split(':')[1].strip()
    node_hostname = k

    node_key_level = tier_level if node_key_type != "LEAF" else -1 
    node = {}
    queues = []
    for i, j in enumerate(queue_list):
        queue = {}

        queue_node_index = i
        queue_node_tier_index = tier_level
        queue_node_type_value = j
        queue_node_type_interval = 10000
        queue_mode = "SERVER"
        queue_url = f"tcp://{k}"
        queue_hook = j
        queue_topic = f'{node_key_type}_{j}'

        queue["key"] = {"node_index": queue_node_index,
                        "tier_index": queue_node_tier_index,
                        "mode" : queue_mode,
                        "type": {
                            "value": queue_node_type_value,
                            "interval": queue_node_type_interval
                        }
                       }
        queue["url"] = queue_url
        queue["hook"] = queue_hook
        queue["topic"] = queue_topic
        queues.append(queue)

    node["hostname"] = node_hostname
    node["key"] = { "node_index" : int(node_key_node_index)+1+ti, "level" : node_key_level, "type" : node_key_type }
    node["queues"] = queues
    node["children"] = node_children_list
    node["parent"] = node_parent

    tier_nodes[int(node_key_node_index)+1+ti] = node




cluster_nodes={}
node_key_type = "CLUSTER"
node_children_list = []
node_parent = -1

for ti, tl in enumerate(cluster_list):
    tier_level = node_key_node_index = 0
    k = tl
    node_hostname = k

    node_key_level = tier_level if node_key_type != "LEAF" else -1 
    node = {}
    queues = []
    for i, j in enumerate(queue_list):
        queue = {}

        queue_node_index = i
        queue_node_tier_index = tier_level
        queue_node_type_value = j
        queue_node_type_interval = 10000
        queue_mode = "SERVER"
        queue_url = f"tcp://{k}"
        queue_hook = j
        queue_topic = f'{node_key_type}_{j}'

        queue["key"] = {"node_index": queue_node_index,
                        "tier_index": queue_node_tier_index,
                        "mode" : queue_mode,
                        "type": {
                            "value": queue_node_type_value,
                            "interval": queue_node_type_interval
                        }
                       }
        queue["url"] = queue_url
        queue["hook"] = queue_hook
        queue["topic"] = queue_topic
        queues.append(queue)

    node["hostname"] = node_hostname
    node["key"] = { "node_index" : node_key_node_index, "level" : node_key_level, "type" : node_key_type }
    node["queues"] = queues
    node["children"] = node_children_list
    node["parent"] = node_parent

    cluster_nodes[0-ti] = node

for i in tier_nodes.keys():
    for j in nodes.keys():
        tier_nodes[i]['children'].append(j)
        nodes[j]['parent'] = i

for i in cluster_nodes.keys():
    for j in tier_nodes.keys():
        cluster_nodes[i]['children'].append(j)
        tier_nodes[j]['parent'] = i

node_dict = {**nodes, **tier_nodes, **cluster_nodes}

with open(f'config_{len(node_list)}.json', 'w', encoding='utf-8') as f:
    json.dump(list(node_dict.values()), f, ensure_ascii=False, indent=4)
 
