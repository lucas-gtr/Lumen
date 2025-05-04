#include "ImplementationParameters/ParameterTypes.hpp"

#include <gtest/gtest.h>

TEST(ParametersTypesTest, SetGetIntParameter) {
  IntParam int_param("int_param", 42);
  EXPECT_EQ(int_param.getName(), "int_param");
  EXPECT_EQ(int_param.getValue(), 42);
}

TEST(ParametersTypesTest, SetGetDoubleParameter) {
  DoubleParam double_param("double_param", 3.14);
  EXPECT_EQ(double_param.getName(), "double_param");
  EXPECT_DOUBLE_EQ(double_param.getValue(), 3.14);
}