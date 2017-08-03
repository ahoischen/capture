#include "capture/WriteExitCode.h"

#include "fstream"

namespace capture {
    void WriteExitCode(const int exit_code, const std::string filename) {
        std::ofstream test_exit_file(filename);
        test_exit_file.open(filename);
        test_exit_file << std::to_string(exit_code);
        test_exit_file.close();
    }
}
