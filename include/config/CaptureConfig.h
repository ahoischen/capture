#ifndef CAPTURE_CONFIG_CAPTURECONFIG_H
#define CAPTURE_CONFIG_CAPTURECONFIG_H

#include <memory>
#include <string>
#include <vector>

#include "config/CaptureKeys.h"
#include "config/CatchOption.h"
#include "config/CatchValueOption.h"

namespace capture::config {
    // Represents the configuration for capture.
    class CaptureConfig {
        std::vector<std::unique_ptr<CatchOption>> _options;
        CaptureKeys _captureKeys;
        std::string _exitCodeFile;
        std::string _testSpec;

        friend CaptureConfig CreateConfigFromFile(std::string);

    public:
        const CaptureKeys &GetCaptureKeys() const { return _captureKeys; }
        void SetCaptureKeys(CaptureKeys value) { _captureKeys = value; }

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