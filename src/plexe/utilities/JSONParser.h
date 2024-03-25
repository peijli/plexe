#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <string>
#include <map>

namespace plexe {
    class JSONParser {
    public:
        // function to parse a JSON string and return a map of key-value pairs
        static std::map<std::string, std::string> parse(std::string json);
        // function to convert a map of key-value pairs to a JSON string
        static std::string stringify(std::map<std::string, std::string> map);
        // function to pretty print a map of key-value pairs
        static std::string prettyPrint(std::map<std::string, std::string> map);
    };

} // namespace plexe

#endif /* JSONPARSER_H_ */
