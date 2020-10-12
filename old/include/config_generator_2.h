//
// Created by neeraj on 4/30/20.
//

#ifndef SCORE_CONFIG_GENERATOR_2_H
#define SCORE_CONFIG_GENERATOR_2_H

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"


#if defined(__GNUC__)
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(effc++)
#endif

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(switch-enum)
#endif



#define CHRONOLOG_CONF basket::Singleton<chronolog::ConfigurationManager>::GetInstance()
namespace chronolog{
	class ConfigurationManager{

	private:
		void config(rapidjson::Document &doc, const char* member, uint16_t &variable){
			assert(doc.HasMember(member));
			assert(doc[member].IsInt());
			variable = doc[member].GetInt();
		}

		void config(rapidjson::Document &doc, const char* member, really_long &variable){
			assert(doc.HasMember(member));
			assert(doc[member].IsUint64());
			variable = doc[member].GetUint64();
		}

		void config(rapidjson::Document &doc, const char* member, CharStruct &variable){
			assert(doc.HasMember(member));
			assert(doc[member].IsString());
			variable = doc[member].GetString();
		}
		int CountServers(CharStruct server_list_path){
			fstream file;
			int total=0;
			file.open(server_list_path.c_str(), ios::in);
			if (file.is_open()) {
				std::string file_line;
				std::string server_node_name;
				int count;
				while (getline(file, file_line)) {
					if (!file_line.empty()) {
						int split_loc = file_line.find(':');  // split to node and net
						if (split_loc != std::string::npos) {
							server_node_name = file_line.substr(0, split_loc);
							count = atoi(file_line.substr(split_loc+1, std::string::npos).c_str());
						} else {
							// no special network
							server_node_name = file_line;
							count = 1;
						}
						// server list is list of network interfaces
						for(int i=0;i<count;++i){
							total++;
						}
					}
				}
			} else {
				printf("Error: Can't open server list file %s\n", server_list_path.c_str());
				exit(EXIT_FAILURE);
			}
			file.close();
			return total;
		}

	public:
		CharStruct DJOURNAL_SERVER_LISTS, PKV_SERVER_LISTS, CHRONOLOG_SERVER_LISTS,CHRONOPLAYER_SERVER_LISTS,CHRONOPLAYER_CLIENT_LISTS;
		uint16_t DJOURNAL_SERVER_PORT,DJOURNAL_SERVER_OUT_PORT,DJOURNAL_SERVER_INT_PORT, PKV_SERVER_PORT, CHRONOLOG_SERVER_PORT,CHRONOPLAYER_SERVER_PORT,CHRONOPLAYER_CLIENT_PORT;
		uint16_t DJOURNAL_RPC_THREADS, PKV_RPC_THREADS, CHRONOLOG_RPC_THREADS,CHRONOPLAYER_SERVER_RPC_THREADS,CHRONOPLAYER_CLIENT_RPC_THREADS;
		CharStruct DJOURNAL_DIR, PKV_SERVER_DIR, CHRONOLOG_DIR,CHRONOPLAYER_DIR;
		CharStruct CONFIGURATION_FILE;
		really_long NVME_CAPACITY,SSD_CAPACITY;
		size_t PKV_KEY_SPACE_LEN;
		int JOURNAL_SERVERS_COUNT,PKV_SERVERS_COUNT,CHRONOLOG_SERVERS_COUNT,CHRONOPLAYER_SERVERS_COUNT;


		ConfigurationManager(): CONFIGURATION_FILE("/home/hdevarajan/projects/chronolog/conf/config/chronolog.json"),
		                        DJOURNAL_SERVER_LISTS("/home/hdevarajan/projects/chronolog/server_lists/journal"),
		                        PKV_SERVER_LISTS("/home/hdevarajan/projects/chronolog/server_lists/pkv"),
		                        CHRONOLOG_SERVER_LISTS("/home/hdevarajan/projects/chronolog/server_lists/chronolog"),
		                        CHRONOPLAYER_CLIENT_LISTS("/home/hdevarajan/projects/chronolog/server_lists/chronoplayer_client"),
		                        CHRONOPLAYER_SERVER_LISTS("/home/hdevarajan/projects/chronolog/server_lists/chronoplayer_server"),
		                        DJOURNAL_DIR("/dev/shm/hari/journal"),
		                        PKV_SERVER_DIR("/mnt/ssd/hdevarajan/ssd"),
		                        CHRONOLOG_DIR("/dev/shm/hari/chronolog"),
		                        CHRONOPLAYER_DIR("/dev/shm/hari/chronoplayer"),
		                        DJOURNAL_SERVER_PORT(8000),
		                        DJOURNAL_SERVER_OUT_PORT(9000),
		                        DJOURNAL_SERVER_INT_PORT(10000),
		                        PKV_SERVER_PORT(11000),
		                        CHRONOLOG_SERVER_PORT(12000),
		                        CHRONOPLAYER_SERVER_PORT(13000),
		                        CHRONOPLAYER_CLIENT_PORT(14000),
		                        DJOURNAL_RPC_THREADS(4),
		                        PKV_RPC_THREADS(4),
		                        CHRONOLOG_RPC_THREADS(4),
		                        CHRONOPLAYER_SERVER_RPC_THREADS(4),
		                        CHRONOPLAYER_CLIENT_RPC_THREADS(1),
				//NVME_CAPACITY(128ULL*1024ULL),
				                NVME_CAPACITY(200ULL*1024ULL*1024ULL*1024ULL),
				                SSD_CAPACITY(1024ULL*1024ULL*1024ULL*1024ULL),
				                PKV_KEY_SPACE_LEN(1024ULL*1024ULL),
				                JOURNAL_SERVERS_COUNT(1),
				                PKV_SERVERS_COUNT(1),
				                CHRONOLOG_SERVERS_COUNT(1),
				                CHRONOPLAYER_SERVERS_COUNT(1){}

		void LoadConfiguration(){
			using namespace rapidjson;

			FILE *outfile = fopen(CONFIGURATION_FILE.c_str(), "r");
			if(outfile == NULL){
				std::cout << "HLog configuration not found" << std::endl;
				exit(EXIT_FAILURE);
			}
			char buf[65536];
			FileReadStream instream(outfile, buf, sizeof(buf));
			Document doc;
			doc.ParseStream<kParseStopWhenDoneFlag>(instream);
			if(!doc.IsObject()) {
				std::cout << "HLog - Canfiguration JSON is invalid" << std::endl;
				fclose(outfile);
				exit(EXIT_FAILURE);
			}
			config(doc, "DJOURNAL_SERVER_LISTS", DJOURNAL_SERVER_LISTS);
			config(doc, "PKV_SERVER_LISTS", PKV_SERVER_LISTS);
			config(doc, "CHRONOLOG_SERVER_LISTS", CHRONOLOG_SERVER_LISTS);
			config(doc, "CHRONOPLAYER_CLIENT_LISTS", CHRONOPLAYER_CLIENT_LISTS);
			config(doc, "CHRONOPLAYER_SERVER_LISTS", CHRONOPLAYER_SERVER_LISTS);
			config(doc, "DJOURNAL_DIR", DJOURNAL_DIR);
			config(doc, "PKV_SERVER_DIR", PKV_SERVER_DIR);
			config(doc, "CHRONOLOG_DIR", CHRONOLOG_DIR);
			config(doc, "CHRONOPLAYER_DIR", CHRONOPLAYER_DIR);
			config(doc, "DJOURNAL_SERVER_PORT", DJOURNAL_SERVER_PORT);
			config(doc, "DJOURNAL_SERVER_OUT_PORT", DJOURNAL_SERVER_OUT_PORT);
			config(doc, "DJOURNAL_SERVER_INT_PORT", DJOURNAL_SERVER_INT_PORT);
			config(doc, "PKV_SERVER_PORT", PKV_SERVER_PORT);
			config(doc, "CHRONOLOG_SERVER_PORT", CHRONOLOG_SERVER_PORT);
			config(doc, "CHRONOPLAYER_SERVER_PORT", CHRONOPLAYER_SERVER_PORT);
			config(doc, "CHRONOPLAYER_CLIENT_PORT", CHRONOPLAYER_CLIENT_PORT);
			config(doc, "DJOURNAL_RPC_THREADS", DJOURNAL_RPC_THREADS);
			config(doc, "PKV_RPC_THREADS", PKV_RPC_THREADS);
			config(doc, "CHRONOLOG_RPC_THREADS", CHRONOLOG_RPC_THREADS);
			config(doc, "CHRONOPLAYER_CLIENT_RPC_THREADS", CHRONOPLAYER_CLIENT_RPC_THREADS);
			config(doc, "CHRONOPLAYER_SERVER_RPC_THREADS", CHRONOPLAYER_SERVER_RPC_THREADS);
			config(doc, "NVME_CAPACITY", NVME_CAPACITY);
			config(doc, "SSD_CAPACITY", SSD_CAPACITY);
			config(doc, "PKV_KEY_SPACE_LEN", PKV_KEY_SPACE_LEN);
			JOURNAL_SERVERS_COUNT = CountServers(DJOURNAL_SERVER_LISTS);
			PKV_SERVERS_COUNT = CountServers(PKV_SERVER_LISTS);
			CHRONOLOG_SERVERS_COUNT = CountServers(CHRONOLOG_SERVER_LISTS);
			CHRONOPLAYER_SERVERS_COUNT = CountServers(CHRONOPLAYER_SERVER_LISTS);
		}
	};
}


namespace  config2{
	using namespace rapidjson;
	using namespace std;

	typedef map<string, string> MessageMap;





	static void ParseMessages(const char* json, MessageMap& messages) {
		Reader reader;
		MessageHandler handler;
		StringStream ss(json);
		if (reader.Parse(ss, handler))
			messages.swap(handler.messages_);   // Only change it if success.
		else {
			ParseErrorCode e = reader.GetParseErrorCode();
			size_t o = reader.GetErrorOffset();
			cout << "Error: " << GetParseError_En(e) << endl;;
			cout << " at offset " << o << " near '" << string(json).substr(o, 10) << "...'" << endl;
		}
	}

}

#if defined(__GNUC__)
RAPIDJSON_DIAG_POP
#endif

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

#endif //SCORE_CONFIG_GENERATOR_2_H
