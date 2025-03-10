#ifndef _DOMAIN_IMPL_RECEIVERS_ELASTIC2D_ISOTROPIC_HPP_
#define _DOMAIN_IMPL_RECEIVERS_ELASTIC2D_ISOTROPIC_HPP_

#include "constants.hpp"
// #include "domain/impl/receivers/elastic/elastic2d.hpp"
#include "domain/impl/receivers/receiver.hpp"
#include "enumerations/interface.hpp"
#include "kokkos_abstractions.h"
#include "quadrature/interface.hpp"
#include <Kokkos_Core.hpp>

namespace specfem {
namespace domain {
namespace impl {
namespace receivers {
/**
 * @brief Elemental receiver specialization for 2D elastic isotropic spectral
 * elements with static quadrature points
 *
 * @tparam NGLL Number of Gauss-Lobatto-Legendre quadrature points defined at
 * compile time
 */
template <int NGLL, bool using_simd>
class receiver<
    specfem::dimension::type::dim2, specfem::element::medium_tag::elastic,
    specfem::element::property_tag::isotropic,
    specfem::enums::element::quadrature::static_quadrature_points<NGLL>,
    using_simd> {
private:
  constexpr static auto DimensionType = specfem::dimension::type::dim2;
  constexpr static auto MediumTag = specfem::element::medium_tag::elastic;

  using ElementQuadratureViewType = typename specfem::element::quadrature<
      NGLL, DimensionType, specfem::kokkos::DevScratchSpace,
      Kokkos::MemoryTraits<Kokkos::Unmanaged>, true, false>::ViewType;

  using ElementFieldViewType = typename specfem::element::field<
      NGLL, DimensionType, MediumTag, specfem::kokkos::DevScratchSpace,
      Kokkos::MemoryTraits<Kokkos::Unmanaged>, true, true, true, false,
      using_simd>::ViewType;

public:
  /**
   * @name Typedefs
   */
  ///@{
  /**
   * @brief Dimension of the element
   *
   */
  using dimension =
      specfem::dimension::dimension<specfem::dimension::type::dim2>;
  using medium_type =
      specfem::medium::medium<specfem::dimension::type::dim2,
                              specfem::element::medium_tag::elastic,
                              specfem::element::property_tag::isotropic>;
  /**
   * @brief Number of Gauss-Lobatto-Legendre quadrature points
   */
  using quadrature_points_type =
      specfem::enums::element::quadrature::static_quadrature_points<NGLL>;

  KOKKOS_FUNCTION receiver() = default;

  //   /**
  //    * @brief Construct a new elemental receiver object
  //    *
  //    * @param sin_rec sin of the receiver angle
  //    * @param cos_rec cosine of the receiver angle
  //    * @param receiver_array receiver array containing pre-computed lagrange
  //    * interpolants
  //    * @param partial_derivatives struct used to store partial derivatives at
  //    * quadrature points
  //    * @param properties struct used to store material properties at
  //    quadrature
  //    * points
  //    * @param receiver_field view to store compute receiver field at all GLL
  //    * points where the receiver is located
  //    */
  //   KOKKOS_FUNCTION
  //   receiver(const specfem::kokkos::DeviceView1d<type_real> sin_rec,
  //            const specfem::kokkos::DeviceView1d<type_real> cos_rec,
  //            const specfem::kokkos::DeviceView4d<type_real> receiver_array,
  //            const specfem::compute::partial_derivatives
  //            &partial_derivatives, const specfem::compute::properties
  //            &properties, specfem::kokkos::DeviceView6d<type_real>
  //            receiver_field);

  /**
   * @brief Compute and populate the receiver field at all GLL points where the
   * receiver is located for a given time step
   *
   * @param ireceiver Index of the receiver
   * @param iseis Index of the seismogram
   * @param ispec Index of the element
   * @param siesmogram_type Type of the seismogram
   * @param xz Index of the quadrature point
   * @param isig_step Seismogram step. Seismograms step = current time step /
   * seismogram sampling rate
   * @param field Global wavefield
   * @param field_dot Global wavefield time derivative
   * @param field_dot_dot Global wavefield second time derivative
   * @param hprime_xx Derivates of Lagrange interpolants in the x direction
   * @param hprime_zz Derivates of Lagrange interpolants in the z direction
   */
  KOKKOS_FUNCTION
  void get_field(
      const int iz, const int ix,
      const specfem::point::partial_derivatives<specfem::dimension::type::dim2,
                                                false, using_simd>
          partial_derivatives,
      const specfem::point::properties<specfem::dimension::type::dim2,
                                       medium_type::medium_tag,
                                       medium_type::property_tag, using_simd>
          properties,
      const ElementQuadratureViewType hprime,
      const ElementFieldViewType active_field,
      Kokkos::View<type_real[2], Kokkos::LayoutStride,
                   specfem::kokkos::DevMemSpace>
          receiver_field) const;

  //   /**
  //    * @brief Compute the seismogram components for a given receiver and
  //    * seismogram
  //    *
  //    * @param ireceiver Index of the receiver
  //    * @param iseis Index of the seismogram
  //    * @param seismogram_type Type of the seismogram
  //    * @param xz Index of the quadrature point
  //    * @param isig_step Seismogram step. Seismograms step = current time step
  //    /
  //    * seismogram sampling rate
  //    * @param l_seismogram_components Local seismogram components
  //    */
  //   KOKKOS_FUNCTION void compute_seismogram_components(
  //       const int &ireceiver, const int &iseis,
  //       const specfem::enums::seismogram::type &seismogram_type, const int
  //       &xz, const int &isig_step, specfem::kokkos::array_type<type_real, 2>
  //       &l_seismogram_components) const;

  //   /**
  //    * @brief Store the computed seismogram components in the global
  //    seismogram
  //    * view
  //    *
  //    * @param ireceiver Index of the receiver
  //    * @param seismogram_components Local seismogram components
  //    * @param receiver_seismogram Gloabl seismogram view
  //    */
  //   KOKKOS_FUNCTION void compute_seismogram(
  //       const int &ireceiver,
  //       const specfem::kokkos::array_type<type_real, 2>
  //       &seismogram_components, specfem::kokkos::DeviceView1d<type_real>
  //       receiver_seismogram) const;
};

} // namespace receivers
} // namespace impl
} // namespace domain
} // namespace specfem

#endif /* _DOMAIN_IMPL_RECEIVERS_ELASTIC2D_ISOTROPIC_HPP_ */
