#include "plexe/utilities/JSONParser.h"
// #include "JSONParser.h"

namespace plexe{
    std::map<std::string, std::string> JSONParser::parse(std::string json){
        std::map<std::string, std::string> map;
        std::string key;
        std::string value;
        bool isKey = true;
        for (int i = 0; i < json.length(); i++){
            if (json[i] == '\"'){
                i++;
                std::string temp = "";
                while (json[i] != '\"'){
                    temp += json[i];
                    i++;
                }
                if (isKey){
                    key = temp;
                } else {
                    value = temp;
                    map[key] = value;
                }
                isKey = !isKey;
            }
        }
        return map;
    }

    std::string JSONParser::stringify(std::map<std::string, std::string> map){
        std::string json = "{";
        for (auto it = map.begin(); it != map.end(); it++){
            json += "\"" + it->first + "\":\"" + it->second + "\",";
        }
        json.pop_back();
        json += "}";
        return json;
    }

    std::string JSONParser::prettyPrint(std::map<std::string, std::string> map){
        std::string json = "{\n";
        for (auto it = map.begin(); it != map.end(); it++){
            json += "  \"" + it->first + "\": \"" + it->second + "\",\n";
        }
        json.pop_back();
        json.pop_back();
        json += "\n}";
        return json;
    }
}
