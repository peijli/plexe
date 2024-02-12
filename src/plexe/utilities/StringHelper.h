#ifndef STRINGHELPER_H_
#define STRINGHELPER_H_

#include <string>

namespace plexe {
    class StringHelper {
        public:
            static char* customDeepCopy(const char* source, int length);
            static char* customDeepCopy(std::string source);
            static std::string customCharToString(const char* source, int length);
    };
}

#endif /* STRINGHELPER_H_ */
