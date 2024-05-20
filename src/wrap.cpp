#include <pybind11/pybind11.h>
#include "system.hpp"
#include "channel.hpp"
#include "exceptions.hpp"

namespace py = pybind11;

PYBIND11_MODULE(PyMCS, m) {
    m.doc() = "Python bindings for SmarAct MCS API.";

    m.def("findSystems", &findSystems);

    py::class_<System>(m, "System")
        .def(py::init<std::string>(), py::arg("locator"))
        .def("close", &System::close)
        .def("num_channels", &System::num_channels)
        .def("channel", &System::channel)
        .def("__getitem__", &System::channel)
        .def("__enter__", [](const System &s){ return s; })
        .def("__exit__", &System::close);

    py::class_<Channel>(m, "Channel")
        .def("getClosedLoopMoveAcceleration", &Channel::getClosedLoopMoveAcceleration)
        .def("getClosedLoopMoveSpeed", &Channel::getClosedLoopMoveSpeed)
        .def("getPositionLimit", &Channel::getPositionLimit)
        .def("setClosedLoopMaxFrequency", &Channel::setClosedLoopMaxFrequency)
        .def("setClosedLoopMoveAcceleration", &Channel::setClosedLoopMoveAcceleration)
        .def("setClosedLoopMoveSpeed", &Channel::setClosedLoopMoveSpeed)
        .def("setPosition", &Channel::setPosition)
        .def("setPositionLimit", &Channel::setPositionLimit)
        .def("gotoPositionAbsolute", &Channel::gotoPositionAbsolute)
        .def("gotoPositionRelative", &Channel::gotoPositionRelative)
        .def("scanMoveAbsolute", &Channel::scanMoveAbsolute)
        .def("scanMoveRelative", &Channel::scanMoveRelative)
        .def("stepMove", &Channel::stepMove)
        .def("stop", &Channel::stop)
        .def("getPosition", &Channel::getPosition)
        .def("getStatus", &Channel::getStatus);

    py::enum_<ChannelStatus>(m, "ChannelStatus")
        .value("STOPPED", ChannelStatus::STOPPED)
        .value("STEPPING", ChannelStatus::STEPPING)
        .value("SCANNING", ChannelStatus::SCANNING)
        .value("HOLDING", ChannelStatus::HOLDING)
        .value("TARGET", ChannelStatus::TARGET)
        .value("MOVE_DELAY", ChannelStatus::MOVE_DELAY)
        .value("CALIBRATING", ChannelStatus::CALIBRATING)
        .value("FINDING_REF", ChannelStatus::FINDING_REF)
        .value("OPENING", ChannelStatus::OPENING)
        .export_values();

    py::register_local_exception<MCSException>(m, "MCSException");
}