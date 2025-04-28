#ifndef SYSTEM_H
#define SYSTEM_H

#include "MCS/include/MCSControl.h"
#include "exceptions.hpp"
#include "channel.hpp"
#include <vector>

std::string findSystems();

class System {
public:
    System(std::string locator);
    void close() {
        SA_STATUS error;
        if (error = SA_CloseSystem(m_sys_handle))
            throw MCSException("close", error);
    };

    unsigned int num_channels() {
        return m_num_channels;
    };

    Channel& channel(unsigned int channel_index) {
        if (channel_index >= m_num_channels)
            throw MCSException("Channel does not exist.");
        return m_channels[channel_index];
    };
private:
    SA_INDEX m_sys_handle;
    unsigned int m_num_channels;
    std::vector<Channel> m_channels;
};

#endif // SYSTEM_H
