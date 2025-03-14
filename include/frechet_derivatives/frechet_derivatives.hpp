#pragma once

#include "compute/assembly/assembly.hpp"
#include "enumerations/dimension.hpp"
#include "enumerations/medium.hpp"
#include "impl/frechet_element.hpp"

namespace specfem {
namespace frechet_derivatives {
/**
 * @brief Compute Kernels to compute Frechet derivatives elements within a
 * medium
 *
 * @tparam DimensionType Dimension for elements within this kernel
 * @tparam MediumTag Medium for elements within this kernel
 * @tparam NGLL Number of Gauss-Lobatto-Legendre points for elements within this
 * kernel
 */
template <specfem::dimension::type DimensionType,
          specfem::element::medium_tag MediumTag, int NGLL>
class frechet_derivatives {
public:
  using dimension = specfem::dimension::dimension<DimensionType>;
  using medium_type = specfem::medium::medium<DimensionType, MediumTag>;

  /**
   * @name Constructor
   */
  ///@{

  /**
   * @brief Construct a Freclet Derivatives kernels from spectral element
   * assembly
   *
   * @param assembly Spectral element assembly
   */
  frechet_derivatives(const specfem::compute::assembly &assembly)
      : isotropic_elements(assembly) {}

  ///@}

  /**
   * @brief Compute Frechet derivatives
   *
   * @param dt Time step
   */
  void compute(const type_real &dt) { isotropic_elements.compute(dt); }

private:
  specfem::frechet_derivatives::impl::frechet_elements<
      DimensionType, MediumTag, specfem::element::property_tag::isotropic, NGLL>
      isotropic_elements; ///< Frechet derivatives kernels for isotropic
                          ///< elements
};
} // namespace frechet_derivatives
} // namespace specfem
