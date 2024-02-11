#include "JSONParser.h"
#include <iostream>

int main() {
    // create a map of key-value pairs
    std::map<std::string, std::string> map;
    map["name"] = "John";
    map["age"] = "30";
    map["city"] = "New York";
    map["country"] = "USA";
    // // pretty print the map
    // std::cout << plexe::JSONParser::prettyPrint(map) << std::endl;
    // convert the map to a JSON string
    std::string json = plexe::JSONParser::stringify(map);
    std::cout << json << std::endl;
    // parse the JSON string and print the map
    std::map<std::string, std::string> parsedMap = plexe::JSONParser::parse(json);
    std::cout << plexe::JSONParser::prettyPrint(parsedMap) << std::endl;
    return 0;
}
