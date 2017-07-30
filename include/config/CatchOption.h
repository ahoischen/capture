#ifndef CAPTURE_CONFIG_CATCHOPTION_H
#define CAPTURE_CONFIG_CATCHOPTION_H

#include <string>
#include <vector>

namespace capture::config {
    // Represents an option for catch
    class CatchOption {
        // The option's long name without the `--`.
        std::string _name;

        // Whether the option is initially enabled.
        bool _isEnabled;

    public:
        std::string GetName() const { return _name; }
        bool GetIsEnabled() const { return _isEnabled; }
        void SetIsEnabled(bool isEnabled) { _isEnabled = isEnabled; }

        // Appends its string representation to an Argument vector.
        virtual void AddTo(std::vector<std::string> &catchArgv) const {
            catchArgv.push_back("--" + _name);
        }

        CatchOption (std::string name, bool isEnabled) : _name(name), _isEnabled(isEnabled) {}
   };
}

#endif // CAPTURE_CONFIG_CATCHOPTION_H