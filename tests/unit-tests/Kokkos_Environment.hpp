#include <Kokkos_Core.hpp>
#include <gtest/gtest.h>

class KokkosEnvironment : public ::testing::Environment {
public:
  void SetUp();
  void TearDown();
};
