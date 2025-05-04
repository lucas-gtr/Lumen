/**
 * @file Parameters.hpp
 * @brief Header file for the Parameters class.
 */
#ifndef IMPLEMENTATIONPARAMETERS_PARAMETERS_HPP
#define IMPLEMENTATIONPARAMETERS_PARAMETERS_HPP

#include <memory>
#include <string>
#include <vector>

#include "ImplementationParameters/ParameterValue.hpp"

/**
 * @class Parameters
 * @brief Holds implementation-specific parameters for classes derived from abstract interfaces.
 *
 * This class manages a collection of parameters, allowing for setting and getting parameter values.
 * It provides methods to set parameters with default values and retrieve parameters by name.
 */
class Parameters {
private:
  std::vector<std::unique_ptr<ParameterValue>> m_parameters;

  void setParameter(const std::string& name, std::unique_ptr<ParameterValue> param);
  void setDefaultParameter(const std::string& name, std::unique_ptr<ParameterValue> param);

public:
  Parameters() = default; ///< Default constructor.

  Parameters(const Parameters&)            = delete;
  Parameters& operator=(const Parameters&) = delete;
  Parameters(Parameters&&)                 = delete;
  Parameters& operator=(Parameters&&)      = delete;

  /**
   * @brief Sets a parameter (an integer) for the class.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setParameter(const std::string& name, int value);

  /**
   * @brief Sets a parameter (a double) for the class.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setParameter(const std::string& name, double value);

  /**
   * @brief Sets a default parameter (an integer) for the class.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setDefaultParameter(const std::string& name, int value);

  /**
   * @brief Sets a default parameter (a double) for the class.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setDefaultParameter(const std::string& name, double value);

  /**
   * @brief Retrieves a parameter by name.
   * @param name The name of the parameter to retrieve.
   * @return A pointer to the parameter value, or nullptr if not found.
   */
  template <typename T> T* get(const std::string& name) const {
    for(const auto& param : m_parameters) {
      if(param->getName() == name) {
        return dynamic_cast<T*>(param.get());
      }
    }
    return nullptr;
  }
  ~Parameters() = default; ///< Default destructor.
};

#endif // IMPLEMENTATIONPARAMETERS_PARAMETERS_HPP