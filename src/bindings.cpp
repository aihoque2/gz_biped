#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // std::vector binding support
#include "Simulator.h"

namespace py=pybind11;

PYBIND11_MODULE(blackbird_rl, m){
    py::class_<TrainSimulator>(m, "TrainSimulator")
    .def(py::init<bool>())
    .def("step", &TrainSimulator::Step, py::arg("inputAction"),"Step the environment 1ms in time with given action")
    .def("reset_sim", &TrainSimulator::ResetSim, "reset the simulation")
    .def("is_terminal", &TrainSimulator::isTerminal, "check if the robot is in a terminal state")
    .def("pause", &TrainSimulator::Pause, "pause the simulation");
}