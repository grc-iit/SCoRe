loc=$(HOME)/root
MISC_FLAGS=-ggdb -g3
LINK_FLAGS=-lrpc -lredis++ -lhiredis -pthread
BUILD_DIR=build


.PHONY: json_tester
json_tester:
	g++ -std=c++17 -o ./$(BUILD_DIR)/json_tester ./json_tester.cpp $(MISC_FLAGS) -fpermissive $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/


.PHONY: redis_test
redis_test:
	g++ -std=c++17 -o ./$(BUILD_DIR)/redis_test ./redis_test.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: redis_test_other
redis_test_other:
	g++ -std=c++17 -o ./$(BUILD_DIR)/redis_test_other ./redis_test_other.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/
#	g++ -std=c++17 -o ./$(BUILD_DIR)/queue_miscc tests/queue_miscc.cpp $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/


.PHONY: queue_test
queue_test:
	g++ -std=c++17 -o ./$(BUILD_DIR)/queue_test ./queue_test.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/
#	g++ -std=c++17 -o queue_miscc tests/queue_miscc.cpp $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: rtqn_test
rtqn_test:
	g++ -std=c++17 -o  ./$(BUILD_DIR)/rtqn_test ./reverse_triequeue_node_test.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: rtqnx_test
rtqnx_test:
	g++ -std=c++17 -o  ./$(BUILD_DIR)/rtqn2_test ./reverse_triequeue_node_2_test.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/
	g++ -std=c++17 -o  ./$(BUILD_DIR)/rtqn1_test ./reverse_triequeue_node_1_test.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: rtq_test
rtq_test:
	g++ -std=c++17 -o ./$(BUILD_DIR)/rtq_test ./reverse_trie_queue_test.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: score
score:
	g++ -std=c++17 -o ./$(BUILD_DIR)/score ./score.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: score_server
score_server:
	g++ -std=c++17 -o ./$(BUILD_DIR)/score_server ./score_server.cpp $(MISC_FLAGS) $(LINK_FLAGS) -I $(loc)/include/ -I $(loc)/usr/local/include/  -L $(loc)/lib/ -L $(loc)/usr/local/lib/

.PHONY: all
all :
	make redis_test
	make queue_test
	make rtq_test
	make rtqn_test
	make rtqnx_test

clean :
	rm {redis_test,redis_test_other,queue_test,rtqn_test,rtqn1_test,rtqn2_test,rtq_test,score,score_server}

.PHONY: default
default:
	make all
	# queue test


.PHONY: list
list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'


