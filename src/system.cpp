#include "system.hpp"


std::string findSystems() {
    char buffer[2048];
    unsigned int length = 2048;
    SA_STATUS error;
    if (error = SA_FindSystems("", buffer, &length))
        throw MCSException("findSystems", error);
    return buffer;
}

System::System(std::string locator) {
    SA_STATUS error;
    if(error = SA_OpenSystem(&m_sys_handle, locator.c_str(), "sync"))
        throw MCSException("OpenSystem", error);
    if(error = SA_GetNumberOfChannels(m_sys_handle, &m_num_channels))
        throw MCSException("GetNumberOfChannels", error);
    for (int i=0; i<m_num_channels; i++) {
        m_channels.push_back(Channel(m_sys_handle, i));
    }
}