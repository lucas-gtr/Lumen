#include "ImplementationParameters/Parameters.hpp"
#include "ImplementationParameters/ParameterTypes.hpp"

#include <gtest/gtest.h>

TEST(ParametersTest, SetGetIntParameter) {
  Parameters params;
  params.setParameter("int_param", 42);
  auto int_param = params.get<IntParam>("int_param");
  EXPECT_NE(int_param, nullptr);
  EXPECT_EQ(int_param->getName(), "int_param");
  EXPECT_EQ(int_param->getValue(), 42);
}

TEST(ParametersTest, SetGetDoubleParameter) {
  Parameters params;
  params.setParameter("double_param", 3.14);
  auto double_param = params.get<DoubleParam>("double_param");
  EXPECT_NE(double_param, nullptr);
  EXPECT_EQ(double_param->getName(), "double_param");
  EXPECT_DOUBLE_EQ(double_param->getValue(), 3.14);
}

TEST(ParametersTest, SetDefaultIntParameter) {
  Parameters params;
  params.setDefaultParameter("default_int_param", 100);
  auto int_param = params.get<IntParam>("default_int_param");
  EXPECT_NE(int_param, nullptr);
  EXPECT_EQ(int_param->getName(), "default_int_param");
  EXPECT_EQ(int_param->getValue(), 100);
}

TEST(ParametersTest, SetDefaultDoubleParameter) {
  Parameters params;
  params.setDefaultParameter("default_double_param", 2.718);
  auto double_param = params.get<DoubleParam>("default_double_param");
  EXPECT_NE(double_param, nullptr);
  EXPECT_EQ(double_param->getName(), "default_double_param");
  EXPECT_DOUBLE_EQ(double_param->getValue(), 2.718);
}

TEST(ParametersTest, SetParameterOverwrite) {
  Parameters params;
  params.setParameter("overwrite_param", 1);
  auto int_param = params.get<IntParam>("overwrite_param");
  EXPECT_EQ(int_param->getValue(), 1);
  params.setParameter("overwrite_param", 2);
  int_param = params.get<IntParam>("overwrite_param");
  EXPECT_EQ(int_param->getValue(), 2);
}

TEST(ParametersTest, SetDefaultParameterDontOverwrite) {
  Parameters params;
  params.setParameter("dont_overwrite_param", 1);
  auto int_param = params.get<IntParam>("dont_overwrite_param");
  EXPECT_EQ(int_param->getValue(), 1);
  params.setDefaultParameter("dont_overwrite_param", 2);
  int_param = params.get<IntParam>("dont_overwrite_param");
  EXPECT_EQ(int_param->getValue(), 1);
}