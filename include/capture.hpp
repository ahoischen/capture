#include <string>
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdint>

#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_COLOUR_NONE
#define CATCH_CONFIG_NO_CPP11
#include "catch.hpp"
#include "3ds.h"


void SetupTestingEnvironment();
void TearDownTestingEnvironment();

namespace {

    const std::string kDefaultTestTags = "[citra]";
    const std::string kDefaultTestLocation = "/Test Log.txt";
    const std::string kDefaultExitLocation = "/Test Finished.bin";
    const std::string kDefaultTestReporter = "junit";
    
    // This functions interface is likely to be changed to abstract away from 
    // Catch's command line arguments.
    int RunTests(
            const std::string tags,
            const std::string test_log_location,
            const std::string test_reporter) {
        // Build the argv in an arrray so that its number of elements is preserved
        // throughout the function.
        std::array<const char*, 6> catch_arguments{
            "catch",
            "--out",
            test_log_location.c_str(),
            "--reporter",
            test_reporter.c_str(),
            tags.c_str()
        };

        return Catch::Session().run(catch_arguments.size(), catch_arguments.data());
    }


    // Executes the default tests. Assumes to be on citra, if unable to detect the
    // platform. Writes test output to a file on the sdmc for later use and runs
    // tests tagged with the platform's default string.
    int RunAutomaticTests() {
        return RunTests(kDefaultTestTags,kDefaultTestLocation, kDefaultTestReporter);
    }


    // Detects if the test is running automatically or with human input.
    // It currently does this by asking the user do hold any button at startup.
    bool IsRunningInteractively() {
        hidScanInput();
        return hidKeysDown() != 0;
    }

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
        //constexpr uint32_t kStopApplicationKey = KEY_SELECT;
        //const string kStopApplicationKeyName = "SELECT";

        void DrawStatusInfo(
                const std::string tags, 
                const std::string test_log_location, 
                const std::string test_reporter) 
        {
            std::cout << "\x1b[2J";

            std::cout << "Press " << kLogLocationKeyName 
                << " to change the location of the log file. Current value:" 
                << std::endl << test_log_location << std::endl;

            std::cout << "Press " << kReporterKeyName
                << " to change the test reporter. Current value:"
                << std::endl << test_reporter << std::endl;

            std::cout << "Press" << kTagKeyName
                << " to change the test tags. Current value:"
                << std::endl << tags << std::endl;

            std::cout << "Press" << kStartTestKeyName
                << " to start the tests." << std::endl;

            //std::cout << "Press" << kStopApplicationKeyName
            //    << " to stop the application." << std::endl;
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
            int test_exit_code = RunTests(tags, test_log_location, test_reporter);

            // Wait for the user to close the app.
            while(aptMainLoop()) {
                hidScanInput();
                gfxFlushBuffers();
                gfxSwapBuffers();
                gspWaitForVBlank();
                if (hidKeysDown() & KEY_SELECT) {
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
    }

} // namespace

int main(int argc, char** argv) 
{
    SetupTestingEnvironment();

    int exit_code = 0x1ff;

    if (IsRunningInteractively()) {
        exit_code = interactive::RunInteractiveMenu();
    }
    else {
        exit_code = RunAutomaticTests();
    }
    
    TearDownTestingEnvironment();

    WriteExitCode(exit_code, kDefaultExitLocation);


    return exit_code;
}
