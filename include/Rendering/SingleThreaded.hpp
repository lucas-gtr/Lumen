/**
 * @file SingleThreaded.hpp
 * @brief Header file for the SingleThreaded class.
 */
#ifndef RENDERING_SINGLETHREADED_HPP
#define RENDERING_SINGLETHREADED_HPP

#include "Rendering/RenderExecution.hpp"

/**
 * @class SingleThreaded
 * @brief A rendering strategy that renders the scene using a single thread.
 *
 * This class implements the RenderStrategy interface and provides a simple rendering method
 * that processes the entire scene in a single thread without parallelization.
 */
class SingleThreaded : public RenderStrategy {
public:
  /**
   * @brief Renders the scene using a single thread.
   * @return True if rendering was successful, false otherwise.
   */
  bool render() override;
};

#endif // RENDERING_SINGLETHREADED_HPP