#ifndef _SPECFEM_RUNTIME_CONFIGURATION_SOLVER_SOLVER_TPP_
#define _SPECFEM_RUNTIME_CONFIGURATION_SOLVER_SOLVER_TPP_

#include "kernels/kernels.hpp"
#include "solver.hpp"
#include "solver/time_marching.hpp"
#include "timescheme/interface.hpp"
#include <iostream>
#include <memory>

template <typename qp_type>
std::shared_ptr<specfem::solver::solver>
specfem::runtime_configuration::solver::solver::instantiate(
    const specfem::compute::assembly &assembly,
    std::shared_ptr<specfem::time_scheme::time_scheme> time_scheme,
    const qp_type &quadrature) const {

  if (this->simulation_type == "forward") {
    std::cout << "Instantiating Kernels \n";
    std::cout << "-------------------------------\n";
    const auto kernels = specfem::kernels::kernels<specfem::wavefield::type::forward,
                                                   specfem::dimension::type::dim2, qp_type>(
        assembly, quadrature);
    return std::make_shared<
        specfem::solver::time_marching<specfem::simulation::type::forward,
                                       specfem::dimension::type::dim2, qp_type>>(
        kernels, time_scheme);
  } else if (this->simulation_type == "adjoint") {
    std::cout << "Instantiating Kernels \n";
    std::cout << "-------------------------------\n";
    const auto adjoint_kernels = specfem::kernels::kernels<specfem::wavefield::type::adjoint,
                                                   specfem::dimension::type::dim2, qp_type>(
        assembly, quadrature);
    const auto backward_kernels = specfem::kernels::kernels<specfem::wavefield::type::backward,
                                                   specfem::dimension::type::dim2, qp_type>(
        assembly, quadrature);
    return std::make_shared<
        specfem::solver::time_marching<specfem::simulation::type::adjoint,
                                       specfem::dimension::type::dim2, qp_type>>(
        adjoint_kernels, backward_kernels, time_scheme);
  } else {
    throw std::runtime_error("Simulation type not recognized");
  }
}

#endif
