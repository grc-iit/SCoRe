//
// Created by neeraj on 4/24/20.
//

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"

#include <fstream>
#include <unordered_map>

using namespace rapidjson;

void config(Document &doc, const char* member, Document &variable){
	assert(doc.HasMember(member));
	assert(doc[member].IsString());
//	variable = doc[member];

//	std::string t(doc[member]);
	variable.Parse((const char *) doc[member]);

}



void config(Document &doc, const char* member, uint16_t &variable){
	assert(doc.HasMember(member));
	assert(doc[member].IsInt());
	variable = doc[member].GetInt();
}

int main(){
	auto *config_filename = "../configs/config_1.json";
//	std::ifstream file_(config_filename);
	FILE* file_ = fopen(config_filename, "rb");

	char buf[65536];
	FileReadStream instream(file_, buf, sizeof(buf));
	Document doc;
//	IStreamWrapper isw(file_);
//	Document json_doc;
//	json_doc.ParseStream(file_);
	doc.config(doc, "", DJOURNAL_SERVER_LISTS);



//	std::unordered_map<int, json> jason_map;
//	nlohmann::json jayson;
//	file_ >> jayson;
//	json jason = jayson[0];
	/*for (nlohmann::json i : jayson){
		jason_map.insert({{i["key"]["node_index"], i}});
	}*/

	/*
	auto t = conf::map_all_to_ReverseTriequeueNodeConfig(jason_map);
*/

	return 0;
}