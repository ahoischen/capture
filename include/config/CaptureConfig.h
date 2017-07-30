#ifndef CAPTURE_CONFIG_CAPTURECONFIG_H
#define CAPTURE_CONFIG_CAPTURECONFIG_H

#include <memory>
#include <string>
#include <vector>

#include "config/CatchOption.h"
#include "config/CatchValueOption.h"

namespace capture::config {
    // Represents the configuration for capture.
    class CaptureConfig {
        std::vector<std::unique_ptr<CatchOption>> _options;
        std::string _exitCodeFile;
        std::string _testSpec;

    public:
        const std::vector<std::unique_ptr<CatchOption>> &GetOptions() const { return _options; }
        std::vector<std::unique_ptr<CatchOption>> &GetOptions() { return _options; }
        
        std::string GetExitCodeFile() const { return _exitCodeFile; }
        void SetExitCodeFile(std::string exitCodeFile) { _exitCodeFile = exitCodeFile; }

        std::string GetTestSpec() const { return _testSpec; }
        void SetTestSpec(std::string testSpec) { _testSpec = testSpec; }

        std::vector<std::string> CreateCatchArgumentVector();
    };

    CaptureConfig CreateConfigFromFile(std::string configFileName);
}

#endif // CAPTURE_CONFIG_CAPTURECONFIG_H