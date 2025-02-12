cmake_minimum_required(VERSION 3.25)

set(VCPKG_DIR $ENV{VCPKG_DIR})
# Check if the variable is set
if(VCPKG_DIR)
    message(STATUS "Using path: ${VCPKG_DIR}")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
else()
    message(STATUS "VCPKG_DIR not set. Not using vcpkg.")
endif()

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

include(ExternalProject)

ExternalProject_Add(
    patchelf
    URL https://github.com/NixOS/patchelf/archive/refs/tags/0.18.0.tar.gz
    URL_MD5 39be87713c18bfb3b2d27f4b23e28a18
    CONFIGURE_COMMAND cd <SOURCE_DIR> && ./bootstrap.sh && ./configure
    BUILD_COMMAND make -C <SOURCE_DIR>
    INSTALL_COMMAND ""
)

ExternalProject_Get_property(patchelf SOURCE_DIR)

ExternalProject_Add(
    mcs
    URL MCS_3.0.28_Linux.tgz
    URL_MD5 e8bf5aa5ebe1d4c4215617c6f4688679
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    DEPENDS patchelf
    PATCH_COMMAND sh -c "${SOURCE_DIR}/src/patchelf --set-rpath '$ORIGIN' <SOURCE_DIR>/*/lib/*.so"
)

ExternalProject_Get_property(mcs SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

link_directories(${SOURCE_DIR}/${MCS_ARCH}/lib/)

include_directories(include)

find_package(pybind11 CONFIG)
if(NOT pybind11_FOUND)
    execute_process(COMMAND pybind11-config --cmake OUTPUT_VARIABLE PYBIND_DIR)
    find_package(pybind11 CONFIG REQUIRED PATHS ${PYBIND_DIR}/..)
endif()

file(GLOB SRC src/*.cpp)

pybind11_add_module(PyMCS MODULE ${SRC})
target_link_libraries(
    PyMCS
    PRIVATE
    ${MCS_LIBRARIES}
)

add_custom_command(TARGET PyMCS POST_BUILD COMMAND cp ${SOURCE_DIR}/${MCS_ARCH}/lib/*.so* $<TARGET_FILE_DIR:PyMCS>/)

set_target_properties(PyMCS PROPERTIES INSTALL_RPATH "$ORIGIN" INSTALL_RPATH_USE_LINK_PATH false BUILD_WITH_INSTALL_RPATH true)