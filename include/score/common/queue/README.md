### Question 1 (Constructor): 
So on Queue() you pass an URL plus topic. I'm guessing this is the ip and the topic is the name of the queue.
So if you are on ares_comp_01 and need memory_capacity from stor_01 and stor_02 and hdd_capcity from stor_02, you do:
* client1 = queue(ares_stor_01:port, "memory_capacity")
* client2 = queue(ares_stor_02:port, "memory_capacity")
* client3 = queue(ares_stor_02:port, "hdd_capcity")

Where client2 and client3 are talking to the same instance of redis (one per node) but to different queues, yes?. 

Meaning that you have as many queue instances as queues you are connected to, yes?

### Question 2 (Publish): 
* You cast everything to strings because redis requires so, yeah?
### Question 3 (Subscribe):
* Subscribe returns a std::vector of item from latest_sub_id+1 (position wise) to latest. 
Is this orderd, like if i only need the last one, can i do, item_strem.last()?
* Is there any way to only get the latest if it is different? I just dont know redis, do you have a quick reference to this logic?
### Question 3 (is_synced):
* Retuns true if lat_sub_id is equal to the last element of all the queues in the map?
* This is shared with question 4, What is this std::vector of std::map?
    - Was this intended to represent the Score struct where a map holds a layer and the vector holds layers?
    - Or is the map a weird attempt at an std::Pair?
    - In general, who creates them? the user? what are they supoused to represent?
### Question 4 (populate):
populate(vector<map>) just calls populate() on every queue on the vector<map>, yes?