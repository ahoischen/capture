#include "config/CaptureConfig.h"

#include <memory>
#include <string>
#include <vector>

#include "cpptoml.h"

#include "config/InvalidConfigurationException.h"

namespace capture::config {
    // Configuration file key definitions:
    const std::string kExitCodeFileKey = "Capture.ExitCodeFile";
    const std::string kTestSpecKey = "Catch.TestSpec";
    const std::string kCatchOptionKey = "Catch.Option";
    const std::string kCatchOptionNameKey = "Name";
    const std::string kCatchOptionValueKey = "Value";
    const std::string kCatchOptionEnabledKey = "Enabled";

    std::vector<std::string> CaptureConfig::CreateCatchArgumentVector() {
        std::vector<std::string> args;
        
        args.push_back("catch");

        for (int i = 0; i < GetOptions().size(); ++i) {
            if (GetOptions()[i]->GetIsEnabled()) {
                GetOptions()[i]->AddTo(args);
            }
        }

        args.push_back(GetTestSpec());

        return args;
    }

    // Parses the config file into CatchOptions. Takes a pointer to the root of the TOML document's table and
    // a vector to output the options to.
    void ParseCatchOptions(
        const cpptoml::table *configFile,
        std::vector<std::unique_ptr<CatchOption>> &options) {

        auto config = configFile->get_table_array_qualified(kCatchOptionKey);

        // Each CatchOption represents one argument with or without value to catch.
        for (const auto& table : *config)
        {
            // Each must contain a Name and an Enabled key.
            if (table->contains(kCatchOptionNameKey) && table->contains(kCatchOptionEnabledKey))
            {
                // CatchValueOptions must contain a Value key.
                if(table->contains(kCatchOptionValueKey))
                {
                    // This isn't correct error handling:
                    options.push_back(std::make_unique<CatchValueOption>(
                        table->get_as<std::string>(kCatchOptionNameKey).value_or("ERROR"),
                        table->get_as<std::string>(kCatchOptionValueKey).value_or(""),
                        table->get_as<bool>(kCatchOptionEnabledKey).value_or(false)
                    ));
                }
                else 
                {
                    options.push_back(std::make_unique<CatchOption>(
                        table->get_as<std::string>(kCatchOptionNameKey).value_or("ERROR"),
                        table->get_as<bool>(kCatchOptionEnabledKey).value_or(false)
                    ));
                }
            }
            else
            {
                throw InvalidConfigurationException();
            }
        }
    }

    CaptureConfig CreateConfigFromFile(std::string configFileName) {
        auto configFile = cpptoml::parse_file(configFileName);
        CaptureConfig captureConfig;

        // Ensure the config file is properly formatted.
        if(!configFile->contains_qualified(kExitCodeFileKey)) {
            throw InvalidConfigurationException();
        }

        // Fill fields.
        captureConfig.SetExitCodeFile(configFile->get_qualified_as<std::string>(kExitCodeFileKey).value_or(""));
        captureConfig.SetTestSpec(configFile->get_qualified_as<std::string>(kTestSpecKey).value_or(""));
        ParseCatchOptions(configFile.get(), captureConfig.GetOptions());

        return captureConfig;
    }
}