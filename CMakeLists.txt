cmake_minimum_required(VERSION 3.25)

set(VCPKG_DIR $ENV{VCPKG_DIR})
# Check if the variable is set
if(NOT VCPKG_DIR)
    message(FATAL_ERROR "VCPKG_DIR is not set")
endif()

message(STATUS "Using path: ${VCPKG_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(PyMCS VERSION 0.1.0 LANGUAGES CXX)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(FATAL_ERROR "Only Linux is supported!")
endif()

if(NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686.*|i386.*|x86.*|x86_64.*|amd64.*|AMD64.*")
    message(FATAL_ERROR "The wrapped MCS library only supports X86 systems.")
endif()

include(ExternalProject)
ExternalProject_Add(
    mcs
    URL https://download.smaract.com/Downloads/Installer/MCS_3.0.28_Linux.tgz
    URL_MD5 e8bf5aa5ebe1d4c4215617c6f4688679
    # PATCH_COMMAND sed -i "s/    SRCPATH=.*/    SRCPATH=$base/g" <SOURCE_DIR>/install_mcs.sh
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Add_Step(
    mcs
    install-lib
    COMMAND ./install_mcs.sh
    COMMENT "Installing MCS library"
    EXCLUDE_FROM_MAIN true
    WORKING_DIRECTORY <SOURCE_DIR>
    DEPENDEES download
    ALWAYS true
)
ExternalProject_Add_StepTargets(mcs install-lib)
ExternalProject_Get_property(mcs SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "amd64.*|x86_64.*|AMD64.*")
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(MCS_ARCH arch_x86_64)
    else()
        set(MCS_ARCH arch_x86)
    endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686.*|i386.*|x86.*")
    set(MCS_ARCH arch_x86)
else()
    message(FATAL_ERROR "Unknown processor architecture '${CMAKE_SYSTEM_PROCESSOR}'.")
endif()

file(GLOB MCS_LIBRARIES ${SOURCE_DIR}/${MCS_ARCH}/lib/*.so)

include_directories(include)

find_package(pybind11 CONFIG REQUIRED)

file(GLOB SRC src/*.cpp)

pybind11_add_module(PyMCS MODULE ${SRC})
target_link_libraries(
    PyMCS
    PRIVATE
    ${MCS_LIBRARIES}
)