/**
 * @file Observer.hpp
 * @brief Header file for the Observer class, which implements the observer pattern.
 */
#ifndef CORE_OBSERVER_HPP
#define CORE_OBSERVER_HPP

#include <atomic>
#include <cstddef>
#include <functional>
#include <unordered_map>

/**
 * @class Observer
 * @brief A class that implements the observer pattern.
 * @tparam Args The types of arguments that the observer callbacks will accept.
 *
 * This class allows you to register callbacks that will be notified when an event occurs.
 * Callbacks can be added and removed dynamically, and they will be called with the provided arguments
 * when the `notify` method is called.
 */
template <typename... Args> class Observer {
public:
  using Callback   = std::function<void(Args...)>;
  using CallbackID = std::size_t;

  /**
   * @brief Adds a callback to the observer.
   * @param callback The callback function to add.
   * @return The ID of the added callback.
   */
  CallbackID add(const Callback& callback) {
    const CallbackID id = m_next_id++;
    m_callbacks[id]       = callback;
    return id;
  }

  /**
   * @brief Removes a callback from the observer.
   * @param id The ID of the callback to remove.
   */
  void remove(CallbackID id) { m_callbacks.erase(id); }

  /**
   * @brief Notifies all registered callbacks with the provided arguments.
   * @param args The arguments to pass to the callbacks.
   */
  void notify(Args... args) const {
    for(const auto& [id, callback] : m_callbacks) {
      if(callback) {
        callback(args...);
      }
    }
  }

private:
  std::unordered_map<CallbackID, Callback> m_callbacks;
  std::atomic<CallbackID>                  m_next_id{0};
};

#endif // CORE_OBSERVER_HPP
