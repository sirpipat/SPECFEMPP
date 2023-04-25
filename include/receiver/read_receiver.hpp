#ifndef _READ_RECEIVER_HPP
#define _READ_RECEIVER_HPP

#include "receiver.hpp"
#include "specfem_setup.hpp"
#include <vector>

namespace specfem {
namespace receivers {

std::vector<specfem::receivers::receiver *>
read_receivers(const std::string stations_file, const type_real angle);
} // namespace receivers
} // namespace specfem

#endif
