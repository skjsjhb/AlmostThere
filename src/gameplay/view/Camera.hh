#ifndef GAMEPLAY_VIEW_CAMERA
#define GAMEPLAY_VIEW_CAMERA

#include "gameplay/objs/TickObject.hh"
#include <glm/glm.hpp>

class CameraObject;

class Game;

class Camera : public TickObject {
public:
  glm::mat4 getViewMatrix();

  glm::mat4 getViewMatrixInv();

  glm::mat4 getProjectionMatrix();

  glm::mat4 getProjectionMatrixInv();

  void setState(glm::vec3 pos, glm::vec3 direction, glm::vec3 up, double fov, double aspect);

  glm::vec3 getPosition();

  glm::vec3 getDir();

  void tick() override;

  // Constructor
  using TickObject::TickObject;

  // A camera is an always-loaded object
  [[nodiscard]] bool shouldTick() const override { return true; };

  [[nodiscard]] double getGenTime() const override { return 0; };

  static std::shared_ptr<Camera> create(Game &g, std::shared_ptr<Controller> ct);

protected:
  glm::vec3 pos;
  glm::vec3 direction;
  glm::vec3 up;
  double fov = 45.0;
  double aspect = 1920.0 / 1080.0;
  glm::mat4 _cProjectionMatrix;    // Projection Matrix
  glm::mat4 _cProjectionMatrixInv; // Inversed
  glm::mat4 _cViewMatrix;          // View Matrix
  glm::mat4 _cViewMatrixInv;       // Inversed
};

#endif /* GAMEPLAY_VIEW_CAMERA */
