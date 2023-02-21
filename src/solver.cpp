#include "../include/solver.h"
#include "../include/domain.h"
#include "../include/timescheme.h"
#include <Kokkos_Core.hpp>

void specfem::solver::time_marching::run() {

  specfem::TimeScheme::TimeScheme *it = this->it;
  specfem::Domain::Domain *domain = this->domain;

  const int nstep = it->get_max_timestep();

  std::cout << "Excuting time loop\n"
            << "----------------------------\n";

  while (it->status()) {
    int istep = it->get_timestep();

    type_real timeval = it->get_time();

    Kokkos::Profiling::pushRegion("Stiffness calculation");
    it->apply_predictor_phase(domain);

    domain->compute_stiffness_interaction();
    domain->compute_source_interaction(timeval);
    domain->divide_mass_matrix();

    it->apply_corrector_phase(domain);
    Kokkos::Profiling::popRegion();

    if (istep % 10 == 0) {
      std::cout << "Progress : executed " << istep << " steps of " << nstep
                << " steps\n";
    }

    it->increment_time();
  }

  return;
}
