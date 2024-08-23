#pragma once

#include "enumerations/dimension.hpp"
#include "specfem_setup.hpp"
#include <Kokkos_Core.hpp>
#include <cstddef>

namespace specfem {
namespace point {

/**
 * @brief Struct to store local coordinates associated with a quadrature point
 *
 * @tparam DimensionType Dimension of the element where the quadrature point is
 * located
 */
template <specfem::dimension::type DimensionType> struct local_coordinates;

/**
 * @brief Template specialization for 2D elements
 *
 */
template <> struct local_coordinates<specfem::dimension::type::dim2> {
  int ispec;       ///< Index of the spectral element
  type_real xi;    ///< Local coordinate \f$ \xi \f$
  type_real gamma; ///< Local coordinate \f$ \gamma \f$

  /**
   * @brief Default constructor
   *
   */
  KOKKOS_FUNCTION
  local_coordinates() = default;

  /**
   * @brief Construct a new local coordinates object
   *
   * @param ispec Index of the spectral element
   * @param xi Local coordinate \f$ \xi \f$
   * @param gamma Local coordinate \f$ \gamma \f$
   */
  KOKKOS_FUNCTION
  local_coordinates(const int &ispec, const type_real &xi,
                    const type_real &gamma)
      : ispec(ispec), xi(xi), gamma(gamma) {}
};

/**
 * @brief Struct to store global coordinates associated with a quadrature point
 *
 * @tparam DimensionType Dimension of the element where the quadrature point is
 * located
 */
template <specfem::dimension::type DimensionType> struct global_coordinates;

/**
 * @brief Template specialization for 2D elements
 *
 */
template <> struct global_coordinates<specfem::dimension::type::dim2> {
  type_real x; ///< Global coordinate \f$ x \f$
  type_real z; ///< Global coordinate \f$ z \f$

  /**
   * @brief Default constructor
   *
   */
  KOKKOS_FUNCTION
  global_coordinates() = default;

  /**
   * @brief Construct a new global coordinates object
   *
   * @param x Global coordinate \f$ x \f$
   * @param z Global coordinate \f$ z \f$
   */
  KOKKOS_FUNCTION
  global_coordinates(const type_real &x, const type_real &z) : x(x), z(z) {}
};

/**
 * @brief Struct to store the index associated with a quadrature point
 *
 * @tparam DimensionType Dimension of the element where the quadrature point is
 * located
 */
template <specfem::dimension::type DimensionType> struct index;

/**
 * @brief Template specialization for 2D elements
 *
 */
template <> struct index<specfem::dimension::type::dim2> {
  int ispec; ///< Index of the spectral element
  int iz;    ///< Index of the quadrature point in the z direction within the
             ///< spectral element
  int ix;    ///< Index of the quadrature point in the x direction within the
             ///< spectral element

  constexpr static bool using_simd =
      false; ///< Flag to indicate that SIMD is not being used

  /**
   * @brief Default constructor
   *
   */
  KOKKOS_FUNCTION
  index() = default;

  /**
   * @brief Construct a new index object
   *
   * @param ispec Index of the spectral element
   * @param iz Index of the quadrature point in the z direction within the
   * spectral element
   * @param ix Index of the quadrature point in the x direction within the
   * spectral element
   */
  KOKKOS_FUNCTION
  index(const int &ispec, const int &iz, const int &ix)
      : ispec(ispec), iz(iz), ix(ix) {}
};

/**
 * @brief Struct to store the SIMD indices associated with a quadrature point
 *
 * SIMD indices are used to access data into SIMD vector. Generally,
 * you'd pass the SIMD index to a @c load_on_device or @c load_on_host function
 * to load data from the device or host memory into the SIMD vector.
 *
 * @tparam DimensionType Dimension of the element where the quadrature point is
 * located
 */
template <specfem::dimension::type DimensionType> struct simd_index;

/**
 * @brief Template specialization for 2D elements
 *
 * @copydoc simd_index
 *
 */
template <> struct simd_index<specfem::dimension::type::dim2> {
  int ispec; ///< Index associated with the spectral element at the start
             ///< of the SIMD vector
  int number_elements; ///< Number of elements stored in the SIMD vector
  int iz; ///< Index of the quadrature point in the z direction within
          ///< the spectral element
  int ix; ///< Index of the quadrature point in the x direction within
          ///< the spectral element

  constexpr static bool using_simd =
      true; ///< Flag to indicate that SIMD is being used

  /**
   * @brief Default constructor
   *
   */
  KOKKOS_FUNCTION
  simd_index() = default;

  /**
   * @brief Construct a new simd index object
   *
   * @param ispec Index of the spectral element
   * @param number_elements Number of elements
   * @param iz Index of the quadrature point in the z direction within the
   * spectral element
   * @param ix Index of the quadrature point in the x direction within the
   * spectral element
   */
  KOKKOS_FUNCTION
  simd_index(const int &ispec, const int &number_elements, const int &iz,
             const int &ix)
      : ispec(ispec), number_elements(number_elements), iz(iz), ix(ix) {}

  /**
   * @brief Returns a boolean mask to check if the SIMD index is within the SIMD
   * vector
   *
   * @param lane SIMD lane
   * @return bool True if the SIMD index is within the SIMD vector
   */
  KOKKOS_INLINE_FUNCTION
  bool mask(const std::size_t &lane) const {
    return int(lane) < number_elements;
  }
};

/**
 * @brief Distance between two global coordinates
 *
 * @tparam DimensionType Dimension of the element where the quadrature point is
 * located
 * @param p1 Coordinates of the first point
 * @param p2 Coordinates of the second point
 * @return type_real Distance between the two points
 */
template <specfem::dimension::type DimensionType>
KOKKOS_FUNCTION type_real
distance(const specfem::point::global_coordinates<DimensionType> &p1,
         const specfem::point::global_coordinates<DimensionType> &p2);

} // namespace point
} // namespace specfem
