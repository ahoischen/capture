#ifndef CAPTURE_CONFIG_CATCHVALUEOPTION
#define CAPTURE_CONFIG_CATCHVALUEOPTION

#include <string>
#include <vector>

#include "config/CatchOption.h"

namespace capture::config {
    // Represents an option for catch that has a value.
    class CatchValueOption : public CatchOption {
        // The option's value encoded as a string.
        std::string _value;

    public:
        std::string GetValue() const { return _value; }
        void SetValue(const std::string value) { _value = value; }

        // Appends its string representation to an Argument vector.
        void AddTo(std::vector<std::string> &catchArgv) const override {
            CatchOption::AddTo(catchArgv);
            catchArgv.push_back(_value);
        }

        CatchValueOption(std::string name, std::string value, bool isEnabled) : _value(value), CatchOption(name, isEnabled) {}
    };
}

#endif // CAPTURE_CONFIG_CATCHVALUEOPTION