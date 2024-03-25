// #include "plexe/utilities/StringHelper.h"
#include "StringHelper.h"

namespace plexe {
    char* StringHelper::customDeepCopy(const char* source, int length) {
        char* copy = new char[length];
        for (int i = 0; i < length; i++) {
            copy[i] = source[i];
        }
        return copy;
    }

    char* StringHelper::customDeepCopy(std::string source) {
        return customDeepCopy(source.c_str(), static_cast<int>(source.length()));
    }

    std::string StringHelper::customCharToString(const char* source, int length) {
        std::string result;
        for (int i = 0; i < length; i++) {
            result += source[i];
        }
        return result;
    }
}
