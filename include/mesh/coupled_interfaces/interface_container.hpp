#pragma once

#include "enumerations/medium.hpp"
#include "kokkos_abstractions.h"
#include "specfem_mpi/interface.hpp"

namespace specfem {
namespace mesh {
/**
 * @brief Information about interfaces between two media
 *
 * @tparam Medium1 Medium type 1
 * @tparam Medium2 Medium type 2
 */
template <specfem::element::medium_tag Medium1,
          specfem::element::medium_tag Medium2>
struct interface_container {
  constexpr static auto medium1_tag = Medium1; ///< Medium 1 tag
  constexpr static auto medium2_tag = Medium2; ///< Medium 2 tag

  /**
   * @name Constructors
   *
   */
  ///@{
  /**
   * @brief Default constructor
   *
   */
  interface_container(){};

  /**
   * @brief Constructor to read and assign values from fortran binary database
   * file
   *
   * @param num_interfaces Number of interfaces
   * @param stream Stream object for fortran binary file buffered to coupled
   * interfaces section
   * @param mpi Pointer to MPI object
   */
  interface_container(const int num_interfaces, std::ifstream &stream,
                      const specfem::MPI::MPI *mpi);
  ///@}

  int num_interfaces = 0; ///< Number of edges within this interface
  Kokkos::View<int *, Kokkos::HostSpace>
      medium1_index_mapping; ///< spectral element index for edges in medium 1

  Kokkos::View<int *, Kokkos::HostSpace>
      medium2_index_mapping; ///< spectral element index for edges in medium 2

  /**
   * @brief get the spectral element index for the given edge index in the given
   * medium
   *
   * @tparam medium Medium where the edge is located
   * @param interface_index Edge index
   * @return int Spectral element index
   */
  template <specfem::element::medium_tag medium>
  int get_spectral_elem_index(const int interface_index) const;
};
} // namespace mesh
} // namespace specfem
