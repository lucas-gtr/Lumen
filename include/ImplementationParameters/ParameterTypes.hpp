/**
 * @file ParameterTypes.hpp
 * @brief Header file for ParameterTypes used in the implementation parameters.
 */
#ifndef IMPLEMENTATIONPARAMETERS_PARAMETERTYPES_HPP
#define IMPLEMENTATIONPARAMETERS_PARAMETERTYPES_HPP

#include <string>

#include "ImplementationParameters/ParameterValue.hpp"

/**
 * @class IntParam
 * @brief Represents an integer parameter value.
 *
 * This class is derived from ParameterValue and holds an integer value.
 */
class IntParam : public ParameterValue {
private:
  int m_value;

public:
  /**
   * @brief Constructor for IntParam.
   * @param name The name of the parameter.
   * @param value The integer value of the parameter.
   */
  IntParam(const std::string& name, int value) : ParameterValue(name), m_value(value) {}

  IntParam(const IntParam&)            = default; ///< Copy constructor.
  IntParam& operator=(const IntParam&) = default; ///< Copy assignment operator.
  IntParam(IntParam&&)                 = default; ///< Move constructor.
  IntParam& operator=(IntParam&&)      = default; ///< Move assignment operator.

  int getValue() const { return m_value; } ///< Get the integer value of the parameter.

  ~IntParam() override = default; ///< Default destructor.
};

/**
 * @class DoubleParam
 * @brief Represents a double parameter value.
 * 
 * This class is derived from ParameterValue and holds a double value.
 */
class DoubleParam : public ParameterValue {
private:
  double m_value;

public:
  /**
   * @brief Constructor for DoubleParam.
   * @param name The name of the parameter.
   * @param value The double value of the parameter.
   */
  DoubleParam(const std::string& name, double value) : ParameterValue(name), m_value(value) {}

  DoubleParam(const DoubleParam&)            = default; ///< Copy constructor.
  DoubleParam& operator=(const DoubleParam&) = default; ///< Copy assignment operator.
  DoubleParam(DoubleParam&&)                 = default; ///< Move constructor.
  DoubleParam& operator=(DoubleParam&&)      = default; ///< Move assignment operator.

  double getValue() const { return m_value; } ///< Get the double value of the parameter.

  ~DoubleParam() override = default; ///< Default destructor.
};

#endif // IMPLEMENTATIONPARAMETERS_PARAMETERTYPES_HPP