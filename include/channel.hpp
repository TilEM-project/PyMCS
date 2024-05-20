#ifndef CHANNEL_H
#define CHANNEL_H

#include "MCSControl.h"
#include "exceptions.hpp"
#include <vector>

enum ChannelStatus {
    STOPPED = SA_STOPPED_STATUS,
    STEPPING = SA_STEPPING_STATUS,
    SCANNING = SA_SCANNING_STATUS,
    HOLDING = SA_HOLDING_STATUS,
    TARGET = SA_TARGET_STATUS,
    MOVE_DELAY = SA_MOVE_DELAY_STATUS,
    CALIBRATING = SA_CALIBRATING_STATUS,
    FINDING_REF = SA_FINDING_REF_STATUS,
    OPENING = SA_OPENING_STATUS
};

class Channel {
public:
    Channel(SA_INDEX sys_handle, SA_INDEX channel_index)
        : m_sys_handle(sys_handle),
          m_channel_index(channel_index) {};

    unsigned int getClosedLoopMoveAcceleration() {
        unsigned int acceleration;
        SA_STATUS error;
        if (error = SA_GetClosedLoopMoveAcceleration_S(m_sys_handle, m_channel_index, &acceleration))
            throw MCSException("getClosedLoopMoveAcceleration", error);
        return acceleration;
    };

    unsigned int getClosedLoopMoveSpeed() {
        unsigned int speed;
        SA_STATUS error;
        if (error = SA_GetClosedLoopMoveSpeed_S(m_sys_handle, m_channel_index, &speed))
            throw MCSException("getClosedLoopMoveSpeed", error);
        return speed;
    };

    std::vector<signed int> getPositionLimit() {
        signed int min, max;
        SA_STATUS error;
        if (error = SA_GetPositionLimit_S(m_sys_handle, m_channel_index, &min, &max))
            throw MCSException("getPositionLimit", error);
        return {min, max};
    };

    void setClosedLoopMaxFrequency(unsigned int frequency) {
        SA_STATUS error;
        if (error = SA_SetClosedLoopMaxFrequency_S(m_sys_handle, m_channel_index, frequency))
            throw MCSException("setClosedLoopMaxFrequency", error);
    };

    void setClosedLoopMoveAcceleration(unsigned int acceleration) {
        SA_STATUS error;
        if (error = SA_SetClosedLoopMoveAcceleration_S(m_sys_handle, m_channel_index, acceleration))
            throw MCSException("setClosedLoopMoveAcceleration", error);
    };

    void setClosedLoopMoveSpeed(unsigned int speed) {
        SA_STATUS error;
        if (error = SA_SetClosedLoopMoveSpeed_S(m_sys_handle, m_channel_index, speed))
            throw MCSException("setClosedLoopMoveSpeed", error);
    };

    void setPosition(signed int position) {
        SA_STATUS error;
        if (error = SA_SetPosition_S(m_sys_handle, m_channel_index, position))
            throw MCSException("setPosition", error);
    };

    void setPositionLimit(signed int minPosition, signed int maxPosition) {
        SA_STATUS error;
        if (error = SA_SetPositionLimit_S(m_sys_handle, m_channel_index, minPosition, maxPosition))
            throw MCSException("setPositionLimit", error);
    };

    void gotoPositionAbsolute(signed int position, unsigned int holdTime) {
        SA_STATUS error;
        if (error = SA_GotoPositionAbsolute_S(m_sys_handle, m_channel_index, position, holdTime))
            throw MCSException("gotoPositionAbsolute", error);
    };

    void gotoPositionRelative(signed int diff, unsigned int holdTime) {
        SA_STATUS error;
        if (error = SA_GotoPositionRelative_S(m_sys_handle, m_channel_index, diff, holdTime))
            throw MCSException("gotoPositionRelative", error);
    };

    void scanMoveAbsolute(signed int target, unsigned int scanSpeed) {
        SA_STATUS error;
        if (error = SA_ScanMoveAbsolute_S(m_sys_handle, m_channel_index, target, scanSpeed))
            throw MCSException("scanMoveAbsolute", error);
    };

    void scanMoveRelative(signed int diff, unsigned int scanSpeed) {
        SA_STATUS error;
        if (error = SA_ScanMoveRelative_S(m_sys_handle, m_channel_index, diff, scanSpeed))
            throw MCSException("scanMoveRelative", error);
    };

    void stepMove(signed int steps, unsigned int amplitude, unsigned int frequency) {
        SA_STATUS error;
        if (error = SA_StepMove_S(m_sys_handle, m_channel_index, steps, amplitude, frequency))
            throw MCSException("stepMove", error);
    };

    void stop() {
        SA_STATUS error;
        if (error = SA_Stop_S(m_sys_handle, m_channel_index))
            throw MCSException("stop", error);
    };

    signed int getPosition() {
        signed int position;
        SA_STATUS error;
        if (error = SA_GetPosition_S(m_sys_handle, m_channel_index, &position))
            throw MCSException("getPosition", error);
        return position;
    };

    ChannelStatus getStatus() {
        unsigned int status;
        SA_STATUS error;
        if (error = SA_GetStatus_S(m_sys_handle, m_channel_index, &status))
            throw MCSException("getStatus", error);
        return (ChannelStatus) status;
    }
private:
    SA_INDEX m_sys_handle;
    SA_INDEX m_channel_index;
};

#endif // CHANNEL_H