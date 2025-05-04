/**
 * @file ParameterValue.hpp
 * @brief Header file for the ParameterValue class.
 */
#ifndef IMPLEMENTATIONPARAMETERS_PARAMETERVALUE_HPP
#define IMPLEMENTATIONPARAMETERS_PARAMETERVALUE_HPP

#include <string>

/**
 * @class ParameterValue
 * @brief Base class for parameter values.
 *
 * This class serves as a base class for different types of parameter values.
 * It provides a common interface for derived classes to implement specific parameter types.
 */
class ParameterValue {
private:
  std::string m_name;

public:
  /**
   * @brief Constructor for ParameterValue.
   * @param name The name of the parameter.
   */
  explicit ParameterValue(const std::string& name) : m_name(name) {}
  ParameterValue(const ParameterValue&)            = default; ///< Copy constructor.
  ParameterValue& operator=(const ParameterValue&) = default; ///< Copy assignment operator.
  ParameterValue(ParameterValue&&)                 = default; ///< Move constructor.
  ParameterValue& operator=(ParameterValue&&)      = default; ///< Move assignment operator.

  /**
   * @brief Gets the name of the parameter.
   * @return The name of the parameter.
   */
  const std::string& getName() const { return m_name; }

  virtual ~ParameterValue() = default; ///< Default destructor.
};

#endif // IMPLEMENTATIONPARAMETERS_PARAMETERVALUE_HPP