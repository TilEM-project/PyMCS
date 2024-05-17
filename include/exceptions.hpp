#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <stdexcept>
#include <sstream>

inline std::string error_msg(SA_STATUS error) {
    switch (error) {
        case SA_OK: return "SA_OK";
        case SA_INITIALIZATION_ERROR: return "SA_INITIALIZATION_ERROR";
        case SA_NOT_INITIALIZED_ERROR: return "SA_NOT_INITIALIZED_ERROR";
        case SA_NO_SYSTEMS_FOUND_ERROR: return "SA_NO_SYSTEMS_FOUND_ERROR";
        case SA_TOO_MANY_SYSTEMS_ERROR: return "SA_TOO_MANY_SYSTEMS_ERROR";
        case SA_INVALID_SYSTEM_INDEX_ERROR: return "SA_INVALID_SYSTEM_INDEX_ERROR";
        case SA_INVALID_CHANNEL_INDEX_ERROR: return "SA_INVALID_CHANNEL_INDEX_ERROR";
        case SA_TRANSMIT_ERROR: return "SA_TRANSMIT_ERROR";
        case SA_WRITE_ERROR: return "SA_WRITE_ERROR";
        case SA_INVALID_PARAMETER_ERROR: return "SA_INVALID_PARAMETER_ERROR";
        case SA_READ_ERROR: return "SA_READ_ERROR";
        case SA_INTERNAL_ERROR: return "SA_INTERNAL_ERROR";
        case SA_WRONG_MODE_ERROR: return "SA_WRONG_MODE_ERROR";
        case SA_PROTOCOL_ERROR: return "SA_PROTOCOL_ERROR";
        case SA_TIMEOUT_ERROR: return "SA_TIMEOUT_ERROR";
        case SA_ID_LIST_TOO_SMALL_ERROR: return "SA_ID_LIST_TOO_SMALL_ERROR";
        case SA_SYSTEM_ALREADY_ADDED_ERROR: return "SA_SYSTEM_ALREADY_ADDED_ERROR";
        case SA_WRONG_CHANNEL_TYPE_ERROR: return "SA_WRONG_CHANNEL_TYPE_ERROR";
        case SA_CANCELED_ERROR: return "SA_CANCELED_ERROR";
        case SA_INVALID_SYSTEM_LOCATOR_ERROR: return "SA_INVALID_SYSTEM_LOCATOR_ERROR";
        case SA_INPUT_BUFFER_OVERFLOW_ERROR: return "SA_INPUT_BUFFER_OVERFLOW_ERROR";
        case SA_QUERYBUFFER_SIZE_ERROR: return "SA_QUERYBUFFER_SIZE_ERROR";
        case SA_DRIVER_ERROR: return "SA_DRIVER_ERROR";
        case SA_COMPATIBILITY_ERROR: return "SA_COMPATIBILITY_ERROR";
        case SA_NO_SUCH_SLAVE_ERROR: return "SA_NO_SUCH_SLAVE_ERROR";
        case SA_NO_SENSOR_PRESENT_ERROR: return "SA_NO_SENSOR_PRESENT_ERROR";
        case SA_AMPLITUDE_TOO_LOW_ERROR: return "SA_AMPLITUDE_TOO_LOW_ERROR";
        case SA_AMPLITUDE_TOO_HIGH_ERROR: return "SA_AMPLITUDE_TOO_HIGH_ERROR";
        case SA_FREQUENCY_TOO_LOW_ERROR: return "SA_FREQUENCY_TOO_LOW_ERROR";
        case SA_FREQUENCY_TOO_HIGH_ERROR: return "SA_FREQUENCY_TOO_HIGH_ERROR";
        case SA_SCAN_TARGET_TOO_HIGH_ERROR: return "SA_SCAN_TARGET_TOO_HIGH_ERROR";
        case SA_SCAN_SPEED_TOO_LOW_ERROR: return "SA_SCAN_SPEED_TOO_LOW_ERROR";
        case SA_SCAN_SPEED_TOO_HIGH_ERROR: return "SA_SCAN_SPEED_TOO_HIGH_ERROR";
        case SA_SENSOR_DISABLED_ERROR: return "SA_SENSOR_DISABLED_ERROR";
        case SA_COMMAND_OVERRIDDEN_ERROR: return "SA_COMMAND_OVERRIDDEN_ERROR";
        case SA_END_STOP_REACHED_ERROR: return "SA_END_STOP_REACHED_ERROR";
        case SA_WRONG_SENSOR_TYPE_ERROR: return "SA_WRONG_SENSOR_TYPE_ERROR";
        case SA_COULD_NOT_FIND_REF_ERROR: return "SA_COULD_NOT_FIND_REF_ERROR";
        case SA_WRONG_END_EFFECTOR_TYPE_ERROR: return "SA_WRONG_END_EFFECTOR_TYPE_ERROR";
        case SA_MOVEMENT_LOCKED_ERROR: return "SA_MOVEMENT_LOCKED_ERROR";
        case SA_RANGE_LIMIT_REACHED_ERROR: return "SA_RANGE_LIMIT_REACHED_ERROR";
        case SA_PHYSICAL_POSITION_UNKNOWN_ERROR: return "SA_PHYSICAL_POSITION_UNKNOWN_ERROR";
        case SA_OUTPUT_BUFFER_OVERFLOW_ERROR: return "SA_OUTPUT_BUFFER_OVERFLOW_ERROR";
        case SA_COMMAND_NOT_PROCESSABLE_ERROR: return "SA_COMMAND_NOT_PROCESSABLE_ERROR";
        case SA_WAITING_FOR_TRIGGER_ERROR: return "SA_WAITING_FOR_TRIGGER_ERROR";
        case SA_COMMAND_NOT_TRIGGERABLE_ERROR: return "SA_COMMAND_NOT_TRIGGERABLE_ERROR";
        case SA_COMMAND_QUEUE_FULL_ERROR: return "SA_COMMAND_QUEUE_FULL_ERROR";
        case SA_INVALID_COMPONENT_ERROR: return "SA_INVALID_COMPONENT_ERROR";
        case SA_INVALID_SUB_COMPONENT_ERROR: return "SA_INVALID_SUB_COMPONENT_ERROR";
        case SA_INVALID_PROPERTY_ERROR: return "SA_INVALID_PROPERTY_ERROR";
        case SA_PERMISSION_DENIED_ERROR: return "SA_PERMISSION_DENIED_ERROR";
        case SA_CALIBRATION_FAILED_ERROR: return "SA_CALIBRATION_FAILED_ERROR";
        case SA_UNKNOWN_COMMAND_ERROR: return "SA_UNKNOWN_COMMAND_ERROR";
        case SA_OTHER_ERROR: return "SA_OTHER_ERROR";
        default: return "";
    }
}

class MCSException : public std::runtime_error {
public:
    MCSException(const std::string &what_arg)
        : std::runtime_error(what_arg) {};

    MCSException(std::string function, SA_STATUS error)
        : std::runtime_error(construct_msg(function, error)) {};

    static std::string construct_msg(std::string function, SA_STATUS error) {
        std::stringstream message;
        message << "MCS failure on function \""
                << function
                << "\" with error code "
                << error
                << ": "
                << error_msg(error)
                << ".";
        return message.str();
    }
};

#endif // EXCEPTIONS_H