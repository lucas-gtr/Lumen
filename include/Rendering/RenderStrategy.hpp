/**
 * @file RenderStrategy.hpp
 * @brief Header file for the RenderStrategy class.
 */
#ifndef RENDERING_RENDERSTRATEGY_HPP
#define RENDERING_RENDERSTRATEGY_HPP

class Renderer; // Forward declaration of Renderer class

/**
 * @class RenderStrategy
 * @brief Abstract base class for rendering strategies.
 *
 * This class defines the interface for different rendering strategies that can be used by the Renderer.
 * It allows for flexible rendering implementations, such as single-threaded or multi-threaded strategies.
 */
class RenderStrategy {
private:
  Renderer* m_renderer = nullptr;

protected:
  Renderer* renderer() const { return m_renderer; }

public:
  RenderStrategy() = default; ///< Default constructor.

  RenderStrategy(const RenderStrategy&)            = delete; ///< Deleted copy constructor.
  RenderStrategy& operator=(const RenderStrategy&) = delete; ///< Deleted copy assignment operator.
  RenderStrategy(RenderStrategy&&)                 = delete; ///< Deleted move constructor.
  RenderStrategy& operator=(RenderStrategy&&)      = delete; ///< Deleted move assignment operator.

  /**
   * @brief Renders the scene using the specified rendering strategy.
   * @return True if rendering was successful, false otherwise.
   */
  virtual bool render() = 0;

  /**
   * @brief Sets the renderer for this rendering strategy.
   * @param renderer Pointer to the Renderer instance that will use this strategy.
   */
  void setRenderer(Renderer* renderer) { this->m_renderer = renderer; }

  virtual ~RenderStrategy() = default; ///< Virtual destructor for proper cleanup of derived classes.
};

#endif // RENDERING_RENDERSTRATEGY_HPP