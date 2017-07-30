#ifndef CAPTURE_CONFIG_INVALIDCONFIGURATIONEXCEPTION_H
#define CAPTURE_CONFIG_INVALIDCONFIGURATIONEXCEPTION_H

#include <exception>

namespace capture::config {
    class InvalidConfigurationException : public std::exception {

    };
}

#endif // CAPTURE_CONFIG_INVALIDCONFIGURATIONEXCEPTION_H