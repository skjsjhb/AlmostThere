/**
 * @brief Abstract controller module.
 *
 * This module acts as a replacement of the old draw controller module, and supports multiple control mode.
 * Controller is a map-generated object. It acts as an 'indicator', which means it generates output according
 * to the map, telling the main game loop how to draw/load objects. However, it does not control the object
 * directly.
 *
 * @author skjsjhb
 */
#ifndef ALMOSTTHERE_SRC_GAMEPLAY_MAP_CONTROLLER_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_MAP_CONTROLLER_HH

#include <glm/glm.hpp>

struct ControllerOutput {
  // Movement
  glm::vec3 pos = {0, 0, 0}, up = {0, 1, 0}, norm = {0, 0, 1};

  // Apperance
  double len = 0, alpha = 1, size = 1;
};

struct ControllerLifeTime {
  double genTime = 0, hitTime = std::numeric_limits<double>::max(), unloadTime = std::numeric_limits<double>::max(),
      length = 0;
};

/**
 * @brief A controller class manages the lifetime and status of an object.
 */
class Controller {
public:
  /**
   * @brief Tick method to update the status of the controller.
   * @param absTime Current time.
   */
  virtual void tick(double absTime) = 0;

  /**
   * @brief Get the output status.
   * @return A const reference to the status.
   */
  [[nodiscard]] virtual const ControllerOutput &getOutput() const = 0;

  /**
   * @brief Get the lifetime of the controlled object.
   * @return A const reference to the lifetime indicator.
   */
  [[nodiscard]] virtual const ControllerLifeTime &getLifeTime() const = 0;

  // Virtual destructor
  virtual ~Controller() = default;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_MAP_CONTROLLER_HH
