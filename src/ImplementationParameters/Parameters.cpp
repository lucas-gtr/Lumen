#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ImplementationParameters/ParameterTypes.hpp"
#include "ImplementationParameters/ParameterValue.hpp"
#include "ImplementationParameters/Parameters.hpp"

void Parameters::setParameter(const std::string& name, int value) {
  std::unique_ptr<ParameterValue> param = std::make_unique<IntParam>(name, value);
  setParameter(name, std::move(param));
}

void Parameters::setParameter(const std::string& name, double value) {
  std::unique_ptr<ParameterValue> param = std::make_unique<DoubleParam>(name, value);
  setParameter(name, std::move(param));
}

void Parameters::setDefaultParameter(const std::string& name, int value) {
  std::unique_ptr<ParameterValue> param = std::make_unique<IntParam>(name, value);
  setDefaultParameter(name, std::move(param));
}

void Parameters::setDefaultParameter(const std::string& name, double value) {
  std::unique_ptr<ParameterValue> param = std::make_unique<DoubleParam>(name, value);
  setDefaultParameter(name, std::move(param));
}

void Parameters::setDefaultParameter(const std::string& name, std::unique_ptr<ParameterValue> param) {
  auto it = std::find_if(m_parameters.begin(), m_parameters.end(),
                         [&name](const std::unique_ptr<ParameterValue>& p) { return p->getName() == name; });

  if(it == m_parameters.end()) {
    m_parameters.push_back(std::move(param));
  }
}

void Parameters::setParameter(const std::string& name, std::unique_ptr<ParameterValue> param) {
  auto it = std::find_if(m_parameters.begin(), m_parameters.end(),
                         [&name](const std::unique_ptr<ParameterValue>& p) { return p->getName() == name; });

  if(it != m_parameters.end()) {
    *it = std::move(param);
  } else {
    m_parameters.push_back(std::move(param));
  }
}
