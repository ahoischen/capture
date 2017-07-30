#include <string>
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <exception>
#include <cstring>

#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_COLOUR_NONE
#define CATCH_CONFIG_NO_CPP11
#include "catch.hpp"
#include "3ds.h"

#include "config/CaptureConfig.h"

const std::string kConfigFilename="romfs:/CaptureConfig.toml";

void SetupTestingEnvironment() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    Result romfs_code = romfsInit();
    gspWaitForVBlank();

    if (romfs_code < 0) {
        printf("Romfs Init failed: %08X", romfs_code);
    }
}

void TearDownTestingEnvironment() {
    gfxExit();
}

#ifndef CAPTURE_OPTION_NONINTERACTIVE
    #define CAPTURE_INTERACTIVE
#endif

namespace capture {
    const std::string kDefaultTestTags = "[citra]";
    const std::string kDefaultTestLocation = "/Test Log.txt";
    const std::string kDefaultExitLocation = "/Test Finished.bin";
    const std::string kDefaultTestReporter = "junit";
    
    // This functions interface is likely to be changed to abstract away from 
    // Catch's command line arguments.
    int RunTests(const std::vector<std::string> &args) {
        char** cArgs = new char*[args.size()]; 

        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << args[i] << std::endl;
            
            char* copied_string = new char[args[i].length()+1];
            strcpy(copied_string, args[i].c_str());

            cArgs[i] = copied_string;
        }

        svcSleepThread(1000000000);

        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << cArgs[i] << std::endl;
        }

        
        svcSleepThread(5000000000);        

        return Catch::Session().run(args.size(), cArgs);
    }


    // Executes the default tests. Assumes to be on citra, if unable to detect the
    // platform. Writes test output to a file on the sdmc for later use and runs
    // tests tagged with the platform's default string.
    int RunAutomaticTests() {
        config::CaptureConfig config(config::CreateConfigFromFile(kConfigFilename));
        return RunTests(config.CreateCatchArgumentVector());
    }


    // Detects if the test is running automatically or with human input.
    // It currently does this by asking the user do hold any button at startup.
    bool IsRunningInteractively() {
        hidScanInput();
        return hidKeysDown() != 0;
    }

    #ifdef CAPTURE_INTERACTIVE

    namespace interactive {
        // Keys to press for configuration.
        constexpr uint32_t kLogLocationKey = KEY_B;
        const std::string kLogLocationKeyName = "B";
        constexpr uint32_t kReporterKey = KEY_A;
        const std::string kReporterKeyName = "A";
        constexpr uint32_t kTagKey = KEY_Y;
        const std::string kTagKeyName = "Y";
        constexpr uint32_t kStartTestKey = KEY_START;
        const std::string kStartTestKeyName = "START";
        constexpr uint32_t kStopApplicationKey = KEY_SELECT;
        const std::string kStopApplicationKeyName = "SELECT";

        void DrawSettingInfo(
                const std::string variable_info,
                const std::string value,
                const std::string key_name)
        {
            std::cout << "(\x1b[32m" << key_name << "\x1b[0m) "
                << variable_info << std::endl
                << "\x1b[33m" << value << "\x1b[0m" << std::endl
                << std::endl;
        }

        void DrawStatusInfo(
                const std::string tags, 
                const std::string test_log_location, 
                const std::string test_reporter) 
        {
            std::cout << "\x1b[2J\x1b[20CTEST CONFIG" << std::endl << std::endl;

            DrawSettingInfo("Log File", test_log_location, kLogLocationKeyName);
            DrawSettingInfo("Test Reporter", test_reporter, kReporterKeyName);
            DrawSettingInfo("Tags", tags, kTagKeyName);

            std::cout << "(\x1b[32m" << kStartTestKeyName << "\x1b[0m) "
                << "Start the test.";
        }
        
        // Opens the software keyboard with an initial value and returns
        // the string the user entered. 
        std::string GetStringKeyboard(std::string initial_value) {
            SwkbdState swkbd;
            swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, -1);
            swkbdSetInitialText(&swkbd, initial_value.c_str());
            swkbdSetHintText(&swkbd, "Please enter value");
            swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Submit", true);

            constexpr int kMaxLength = 255;
            char out_chars[kMaxLength] = {};

            swkbdInputText(&swkbd, out_chars, kMaxLength);

            return out_chars;
        }


        // Returns true if the user has changed a setting and false if he has 
        // started the test run.
        bool LetUserChangeSettings(
                std::string* tags,
                std::string* test_location,
                std::string* test_reporter) 
        {
            // Loops because the user might take more than one frame to input
            // something.
            while(aptMainLoop()) {
                hidScanInput();
                uint32_t pressed_buttons = hidKeysDown();

                // Let the user change settings or start the test by pressing
                // the corresponding buttons.
                if (pressed_buttons & kLogLocationKey) {
                    *test_location = GetStringKeyboard(*test_location);
                    return true;
                }
                else if (pressed_buttons & kReporterKey) {
                    *test_reporter = GetStringKeyboard(*test_reporter);
                    return true;
                }
                else if (pressed_buttons & kTagKey) {
                    *tags = GetStringKeyboard(*tags);
                    return true;
                }
                else if (pressed_buttons & kStartTestKey) {
                    return false;
                }
            }

            return false;
        }

        // Displays a menu for configuration for the user and lets them run the tests.
        int RunInteractiveMenu() {
            std::string tags{kDefaultTestTags};
            std::string test_log_location{kDefaultTestLocation};
            std::string test_reporter{kDefaultTestReporter};

            // Notify the user that the test runner is in interactive mode.
            std::cout << "Interactive session started" << std::endl;

            // Print the current configuration at least once and let the user
            // change it as much as they want.
            do {
                DrawStatusInfo(tags, test_log_location, test_reporter);
                gfxFlushBuffers();
                gfxSwapBuffers();
                gspWaitForVBlank();
            } while (LetUserChangeSettings(&tags, &test_log_location, &test_reporter));

            // Execute the now configured tests.
            int test_exit_code = //RunTests(tags, test_log_location, test_reporter);
                                    0;
            std::cout << "\x1b[2J\x1b[20C" << "Tests finished!" << std::endl 
                << std::endl
                << "(\x1b[32m" << kStopApplicationKeyName  << "\x1b[0m) "
                << "Close application.";

            // Wait for the user to close the app.
            while(aptMainLoop()) {
                hidScanInput();
                gfxFlushBuffers();
                gfxSwapBuffers();
                gspWaitForVBlank();
                if (hidKeysDown() & kStopApplicationKey) {
                    break;
                }
            }

            return test_exit_code;
        }

    } // namespace interactive

    // Writes the exit code of the program to a file on the sdmc for later
    // consumption by CI services.
    // A test run might later exit with this code itself to signal test success
    // or failure.
    void WriteExitCode(const int exit_code, const std::string filename) {
        std::ofstream test_exit_file (filename);
        test_exit_file.open(filename);
        test_exit_file << std::to_string(exit_code);
        test_exit_file.close();
    } // namespace interactive

    #endif // CAPTURE_INTERACTIVE

} // namespace capture

int main(int argc, char** argv) 
{
    SetupTestingEnvironment();

    int exit_code = 0x1ff;

    if (capture::IsRunningInteractively()) {
        exit_code = capture::interactive::RunInteractiveMenu();
    }
    else {
        exit_code = capture::RunAutomaticTests();
    }
    
    TearDownTestingEnvironment();

    capture::WriteExitCode(exit_code, capture::kDefaultExitLocation);


    return exit_code;
}
