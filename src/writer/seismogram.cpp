#include "compute/interface.hpp"
#include "writer/interface.hpp"
#include <fstream>

void specfem::writer::seismogram::write() {

  const int nsig_types = this->receivers.h_seismogram_types.extent(0);
  const int nsig_steps = this->receivers.h_seismogram.extent(0);
  const auto h_seismogram = this->receivers.h_seismogram;
  const type_real dt = this->dt;
  const type_real t0 = this->t0;
  const type_real nstep_between_samples = this->nstep_between_samples;

  this->receivers.sync_seismograms();

  std::cout << "output folder : " << this->output_folder << "\n";

  switch (this->type) {
  case specfem::enums::seismogram::ascii:
    // Open stream
    for (int irec = 0; irec < nreceivers; irec++) {
      std::string network_name = receivers.network_names[irec];
      std::string station_name = receivers.station_names[irec];
      for (int isig = 0; isig < nsig_types; isig++) {
        std::vector<std::string> filename;
        auto stype = this->receivers.h_seismogram_types(isig);
        switch (stype) {
        case specfem::enums::seismogram::type::displacement:
          filename = { this->output_folder + "/" + network_name + station_name +
                           "BXX" + ".semd",
                       this->output_folder + "/" + network_name + station_name +
                           "BXZ" + ".semd" };
          break;
        case specfem::enums::seismogram::type::velocity:
          filename = { this->output_folder + "/" + network_name + station_name +
                           "BXX" + ".semv",
                       this->output_folder + "/" + network_name + station_name +
                           "BXZ" + ".semv" };
          break;
        case specfem::enums::seismogram::type::acceleration:
          filename = { this->output_folder + "/" + network_name + station_name +
                           "BXX" + ".sema",
                       this->output_folder + "/" + network_name + station_name +
                           "BXZ" + ".sema" };
          break;
        }

        for (int iorientation = 0; iorientation < filename.size();
             iorientation++) {
          std::ofstream seismo_file;
          seismo_file.open(filename[iorientation]);
          for (int isig_step = 0; isig_step < nsig_steps; isig_step++) {
            const type_real time_t =
                isig_step * dt * nstep_between_samples + t0;
            const type_real value =
                h_seismogram(isig_step, isig, irec, iorientation);

            seismo_file << std::scientific << time_t << " " << std::scientific
                        << value << "\n";
          }
          seismo_file.close();
        }
      }
    }
    break;
  default:
    std::ostringstream message;
    message << "seismogram output type " << this->type
            << " has not been implemented yet.";
    throw std::runtime_error(message.str());
  }

  std::cout << std::endl;
}
