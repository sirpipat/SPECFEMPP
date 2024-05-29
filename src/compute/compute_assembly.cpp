
#include "compute/assembly/assembly.hpp"
#include "mesh/mesh.hpp"

specfem::compute::assembly::assembly(
    const specfem::mesh::mesh &mesh,
    const specfem::quadrature::quadratures &quadratures,
    const std::vector<std::shared_ptr<specfem::sources::source> > &sources,
    const std::vector<std::shared_ptr<specfem::receivers::receiver> >
        &receivers,
    const std::vector<specfem::enums::seismogram::type> &stypes,
    const type_real t0, const type_real dt, const int max_timesteps,
    const int max_sig_step, const specfem::simulation::type simulation) {
  this->mesh = { mesh.control_nodes, quadratures };
  this->partial_derivatives = { this->mesh };
  this->properties = { this->mesh.nspec, this->mesh.ngllz, this->mesh.ngllx,
                       mesh.materials };

  std::cout << "STEP 1 : DONE \n";
  this->kernels = { this->mesh.nspec, this->mesh.ngllz, this->mesh.ngllx,
                    mesh.materials };
  this->sources = { sources,          this->mesh, this->partial_derivatives,
                    this->properties, t0,         dt,
                    max_timesteps };
  this->receivers = { max_sig_step, receivers, stypes, this->mesh };
  this->boundaries = { this->mesh.nspec, this->properties, mesh.abs_boundary,
                       mesh.acfree_surface };

  std::cout << "STEP 2 : DONE \n";
  this->coupled_interfaces = { this->mesh, this->properties,
                               mesh.coupled_interfaces };
  this->fields = { this->mesh, this->properties, simulation };
  this->boundary_values = { max_timesteps, this->mesh, this->properties,
                            this->boundaries };

  std::cout << "STEP 3 : DONE \n";
  return;
}
