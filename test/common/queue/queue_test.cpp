//
// Created by neeraj on 4/17/20.
//

#include <score/common/queue/Queue.h>
#include <iostream>
#include <unistd.h>
//#include "debug.h"
int te=0;
double cap(){
	return te*322.14;
}

int main(){

    auto url = "tcp://127.0.0.1:6379";
    auto topic1 = "q_test1";

    Queue queue(url, topic1);

    d_dict val1;
    d_dict val2;
    d_dict val3;
    val1.insert(std::make_pair <std::string ,std::string> ("1", "test1"));
    val2.insert(std::make_pair <std::string ,std::string> ("2", "test2"));
    val2.insert(std::make_pair <std::string ,std::string> ("3", "test3"));
    val3.insert(std::make_pair <std::string ,std::string> ("4", "test4"));
    val3.insert(std::make_pair <std::string ,std::string> ("5", "test5"));
    val3.insert(std::make_pair <std::string ,std::string> ("6", "test6"));

    queue.publish(val1);
    queue.publish(val2);
    item_stream res1 = queue.subscribe();

    queue.publish(val3);
    item_stream res2 = queue.subscribe();

    item_stream res3 = queue.subscribe();

	return 0;
}