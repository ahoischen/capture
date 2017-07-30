#include "config/CaptureKeys.h"

#include <string>

#include "config/InvalidConfigurationException.h"

namespace capture::config {
    const std::string kStartInteractiveKey = "StartInteractive";
    const std::string kEnableOptionKey = "EnableOption";
    const std::string kChangeOptionKey = "ChangeOption";
    const std::string kMoveUpKey = "MoveUp";
    const std::string kMoveDownKey = "MoveDown";
    const std::string kCloseApplicationKey = "CloseApplication";
    const std::string kStartTestsKey = "StartTests";


    void CaptureKeys::InitFromTable(std::shared_ptr<cpptoml::table> table) {
        assert(table);
        
        if (
            table->contains(kStartInteractiveKey) &&
            table->contains(kEnableOptionKey) &&
            table->contains(kChangeOptionKey) &&
            table->contains(kMoveUpKey) &&
            table->contains(kMoveDownKey) &&
            table->contains(kCloseApplicationKey) &&
            table->contains(kStartTestsKey))
        {
            _startInteractive = interactive::Keys::State(table->get_as<std::string>(kStartInteractiveKey).value_or(""));
            _enableOption = interactive::Keys::State(table->get_as<std::string>(kEnableOptionKey).value_or(""));
            _changeOption = interactive::Keys::State(table->get_as<std::string>(kChangeOptionKey).value_or(""));
            _moveUp = interactive::Keys::State(table->get_as<std::string>(kMoveUpKey).value_or(""));
            _moveDown = interactive::Keys::State(table->get_as<std::string>(kMoveDownKey).value_or(""));
            _closeApplication = interactive::Keys::State(table->get_as<std::string>(kCloseApplicationKey).value_or(""));
            _startTests = interactive::Keys::State(table->get_as<std::string>(kStartTestsKey).value_or(""));
        }
        else {
            throw InvalidConfigurationException();
        }
        
    }
}
