cmake_minimum_required(VERSION 3.25)

project(PyMCS VERSION 0.1.0 LANGUAGES CXX)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(FATAL_ERROR "Only Linux is supported!")
endif()

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "amd64.*|x86_64.*|AMD64.*")
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(MCS_ARCH arch_x86_64)
    else()
        set(MCS_ARCH arch_x86)
    endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686.*|i386.*|x86.*")
    set(MCS_ARCH arch_x86)
else()
    message(FATAL_ERROR "The wrapped MCS library only supports X86 systems. Unknown processor architecture '${CMAKE_SYSTEM_PROCESSOR}'.")
endif()

if(${MCS_ARCH} STREQUAL "arch_x86")
    set(MCS_LIBRARIES libd2xx_table.so libftchipid.so libftd2xx.so libmcscontrol.so libsmaractio.so)
elseif(${MCS_ARCH} STREQUAL "arch_x86_64")
    set(MCS_LIBRARIES libftchipid.so libftd2xx.so libmcscontrol.so libsmaractio.so)
else()
    message(FATAL_ERROR "Unknown processor architecture '${MCS_ARCH}'.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/MCS/${MCS_ARCH}/lib/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(pybind11 CONFIG)
if(NOT pybind11_FOUND)
    execute_process(COMMAND pybind11-config --cmake OUTPUT_VARIABLE PYBIND_DIR)
    find_package(pybind11 CONFIG REQUIRED PATHS ${PYBIND_DIR}/..)
endif()

pybind11_add_module(_PyMCS MODULE src/PyMCS/PyMCS.cpp src/system.cpp)
target_link_libraries(
    _PyMCS
    PRIVATE
    ${MCS_LIBRARIES}
)

set_target_properties(_PyMCS PROPERTIES INSTALL_RPATH "$ORIGIN" INSTALL_RPATH_USE_LINK_PATH false BUILD_WITH_INSTALL_RPATH true)

install(TARGETS _PyMCS DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/MCS/${MCS_ARCH}/lib/ DESTINATION .)
