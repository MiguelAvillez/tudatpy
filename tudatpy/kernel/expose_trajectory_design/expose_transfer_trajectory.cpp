/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "tudat/astro/mission_segments/createTransferTrajectory.h"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <tudat/astro/mission_segments/createTransferTrajectory.h>
#include <tudat/simulation/propagation_setup/accelerationSettings.h>

#include "tudatpy/docstrings.h"

namespace py = pybind11;
namespace tms = tudat::mission_segments;
namespace tss = tudat::simulation_setup;

namespace tudatpy {
namespace trajectory_design {
namespace transfer_trajectory {

void expose_transfer_trajectory(py::module &m) {

    m.attr("DEFAULT_MINIMUM_PERICENTERS") = tms::DEFAULT_MINIMUM_PERICENTERS;

    py::enum_<tms::TransferLegTypes>(m, "TransferLegTypes",
                                     get_docstring("TransferLegTypes").c_str())
            .value("unpowered_unperturbed_leg_type",
                   tms::TransferLegTypes::unpowered_unperturbed_leg,
                   get_docstring("TransferLegTypes.unpowered_unperturbed_leg_type").c_str())
            .value("dsm_position_based_leg_type",
                   tms::TransferLegTypes::dsm_position_based_leg,
                   get_docstring("TransferLegTypes.dsm_position_based_leg_type").c_str())
            .value("dsm_velocity_based_leg_type",
                   tms::TransferLegTypes::dsm_velocity_based_leg,
                   get_docstring("TransferLegTypes.dsm_velocity_based_leg_type").c_str())
            .export_values();

    py::class_<
            tms::TransferNodeSettings,
            std::shared_ptr<tms::TransferNodeSettings> >(m, "TransferNodeSettings",
                                                         get_docstring("TransferNodeSettings").c_str());

    py::class_<
            tms::SwingbyNodeSettings,
            std::shared_ptr<tms::SwingbyNodeSettings>,
            tms::TransferNodeSettings >(m, "SwingbyNodeSettings",
                                        get_docstring("SwingbyNodeSettings").c_str());

    py::class_<
            tms::EscapeAndDepartureNodeSettings,
            std::shared_ptr<tms::EscapeAndDepartureNodeSettings>,
            tms::TransferNodeSettings >(m, "EscapeAndDepartureNodeSettings",
                                        get_docstring("EscapeAndDepartureNodeSettings").c_str());

    py::class_<
            tms::CaptureAndInsertionNodeSettings,
            std::shared_ptr<tms::CaptureAndInsertionNodeSettings>,
            tms::TransferNodeSettings >(m, "CaptureAndInsertionNodeSettings",
                                        get_docstring("CaptureAndInsertionNodeSettings").c_str());

    py::class_<
            tms::TransferLegSettings,
            std::shared_ptr<tms::TransferLegSettings> >(m, "TransferLegSettings",
                                                        get_docstring("TransferLegSettings").c_str());

    m.def("mga_transfer_settings",
          py::overload_cast<
          const std::vector< std::string >&,
          const tms::TransferLegTypes,
          const std::pair< double, double >,
          const std::pair< double, double >,
          const std::map< std::string, double >
          >( &tms::getMgaTransferTrajectorySettings ),
          py::arg( "body_order" ),
          py::arg( "leg_type" ),
          py::arg( "departure_orbit" ) = std::make_pair( TUDAT_NAN, TUDAT_NAN ),
          py::arg( "arrival_orbit" ) = std::make_pair( TUDAT_NAN, TUDAT_NAN ),
          py::arg( "minimum_pericenters" ) = tms::DEFAULT_MINIMUM_PERICENTERS,
          get_docstring("mga_transfer_settings").c_str()
          );

    py::class_<
            tms::TransferTrajectory,
            std::shared_ptr<tms::TransferTrajectory> >(m, "TransferTrajectory",
                                                       get_docstring("TransferTrajectory").c_str() )
            .def_property_readonly("delta_v", &tms::TransferTrajectory::getTotalDeltaV,
                                   get_docstring("TransferTrajectory.delta_v").c_str() )
            .def_property_readonly("time_of_flight", &tms::TransferTrajectory::getTotalTimeOfFlight,
                                   get_docstring("TransferTrajectory.time_of_flight").c_str())
            .def("evaluate", &tms::TransferTrajectory::evaluateTrajectory,
                 py::arg( "node_times" ),
                 py::arg( "leg_parameters" ),
                 py::arg( "node_parameters" ),
                 get_docstring("TransferTrajectory.evaluate").c_str() )
            .def("single_node_delta_v", &tms::TransferTrajectory::getNodeDeltaV,
                 py::arg( "node_index" ),
                 get_docstring("TransferTrajectory.single_node_delta_v").c_str() )
            .def("single_leg_delta_v", &tms::TransferTrajectory::getLegDeltaV,
                 py::arg( "leg_index" ),
                 get_docstring("TransferTrajectory.single_leg_delta_v").c_str() )
            .def("states_along_trajectory",
                 py::overload_cast<const int> (&tms::TransferTrajectory::getStatesAlongTrajectory),
                 py::arg("number_of_data_points_per_leg"),
                 get_docstring("TransferTrajectory.states_along_trajectory").c_str() )
            .def_property_readonly("delta_v_per_node", &tms::TransferTrajectory::getDeltaVPerNode,
                                   get_docstring("TransferTrajectory.delta_v_per_node").c_str() )
            .def_property_readonly("delta_v_per_leg", &tms::TransferTrajectory::getDeltaVPerLeg,
                                   get_docstring("TransferTrajectory.delta_v_per_leg").c_str() )
            .def_property_readonly( "number_of_nodes", &tms::TransferTrajectory::getNumberOfNodes,
                                    get_docstring("TransferTrajectory.number_of_nodes").c_str() )
            .def_property_readonly( "number_of_legs", &tms::TransferTrajectory::getNumberOfLegs,
                                    get_docstring("TransferTrajectory.number_of_legs").c_str() );

    m.def("unpowered_leg",
          &tms::unpoweredLeg,
          get_docstring("unpowered_leg").c_str() );

    m.def("dsm_position_based_leg",
          &tms::dsmPositionBasedLeg,
          get_docstring("dsm_position_based_leg").c_str() );

    m.def("dsm_velocity_based_leg",
          &tms::dsmVelocityBasedLeg,
          get_docstring("dsm_velocity_based_leg").c_str() );

    m.def("swingby_node",
          &tms::swingbyNode,
          py::arg( "minimum_periapsis" ) = TUDAT_NAN,
          get_docstring("swingby_node").c_str() );

    m.def("departure_node",
          &tms::escapeAndDepartureNode,
          py::arg( "departure_semi_major_axis" ),
          py::arg( "departure_eccentricity" ),
          get_docstring("departure_node").c_str() );

    m.def("capture_node",
          &tms::captureAndInsertionNode,
          py::arg( "capture_semi_major_axis" ),
          py::arg( "capture_eccentricity" ),
          get_docstring("capture_node").c_str() );


    m.def("print_parameter_definitions",
          &tms::printTransferParameterDefinition,
          py::arg( "leg_settings" ),
          py::arg( "node_settings" ),
          get_docstring("print_parameter_definitions").c_str() );

    m.def("create_transfer_trajectory",
          &tms::createTransferTrajectory,
          py::arg( "bodies" ),
          py::arg( "leg_settings" ),
          py::arg( "node_settings" ),
          py::arg( "node_names" ),
          py::arg( "central_body" ),
          get_docstring("create_transfer_trajectory").c_str());


    m.def("get_low_thrust_acceleration_settings",
          &tss::getLowThrustLegAccelerationSettings,
          py::arg("low_thrust_leg"),
          py::arg("bodies"),
          py::arg("body_to_propagate"),
          py::arg("specific_impulse_function"),
          py::arg("low_thrust_leg_initial_time") );
};

} // namespace transfer_trajectory
} // namespace trajectory_design
} // namespace tudatpy
