#pragma once

#include "acoustic_properties.hpp"
#include "elastic_properties.hpp"
#include "enumerations/specfem_enums.hpp"
#include "point/properties.hpp"
#include "properties.hpp"
#include "specfem_setup.hpp"
#include <ostream>
#include <tuple>

namespace specfem {
namespace material {

/**
 * @brief Material properties for a given medium and property
 *
 * @tparam MediumTag Medium tag for the material
 * @tparam PropertyTag Property tag for the material
 */
template <specfem::element::medium_tag MediumTag,
          specfem::element::property_tag PropertyTag>
class material : public specfem::material::properties<MediumTag, PropertyTag> {
public:
  constexpr static auto medium_tag = MediumTag;     ///< Medium tag
  constexpr static auto property_tag = PropertyTag; ///< Property tag

  /**
   * @name Constructors
   */
  ///@{

  /**
   * @brief Construct a new material object
   *
   */
  material() = default;

  /**
   * @brief Construct a new material object
   *
   * @tparam Args Arguments to forward to the properties constructor
   * @param args Properties of the material (density, wave speeds, etc.)
   */
  template <typename... Args>
  material(Args &&...args)
      : specfem::material::properties<MediumTag, PropertyTag>(
            std::forward<Args>(args)...) {}
  ///@}

  ~material() = default;

  /**
   * @brief Get the medium tag of the material
   *
   * @return constexpr specfem::element::medium_tag Medium tag
   */
  constexpr specfem::element::medium_tag get_type() const {
    return medium_tag;
  };
};

} // namespace material
} // namespace specfem
