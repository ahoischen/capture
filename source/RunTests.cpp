#include "RunTests.h"

#include <cstring>
#include <iostream>

#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_COLOUR_NONE
#define CATCH_CONFIG_NO_CPP11
#include "catch.hpp"

namespace capture {
    int RunTests(const std::vector<std::string>& args) {
        char** cArgs = new char*[args.size()]; 

        for (size_t i = 0; i < args.size(); ++i) {
            char* copied_string = new char[args[i].length()+1];
            strcpy(copied_string, args[i].c_str());

            cArgs[i] = copied_string;
        }

        return Catch::Session().run(args.size(), cArgs);
    }
}
