#ifndef GAMEPLAY_OBJS_TICKOBJECT
#define GAMEPLAY_OBJS_TICKOBJECT

#include "gameplay/control/Controller.hh"
#include <memory>

#define OBJ_UNLOAD_DELAY 1

struct DrawContext;

class TickObject
{
public:
    virtual bool shouldTick(double absTime) const
    {
        auto ref = controller->getReference();
        return absTime >= ref.genTime && absTime <= ref.endTime + ref.length + OBJ_UNLOAD_DELAY;
    };

    virtual double getTickTime() { return controller->getReference().genTime; };

    /**
     * @brief Main tick method for any tickable object.
     *
     * Tickable objects will update their state on every 'tick'. In default sceneraio a 'tick' has
     * the same period as a frame (when drawing), as the tick is limited by `vtWindowLoop`, considering
     * the latter one will call functions like `glfwSwapBuffers`. However, the tick speed can be
     * limited by the game loop for system stability.
     *
     * Call this method to update the state of the object.
     *
     * @note Passing `absTime` in a non-increasing order is considered a bad practice, as objects
     * which implements this might not always be able to handle reversed timeline correctly.
     *
     * @param absTime Current time.
     */
    virtual void tick(double absTime) { controller->tick(absTime); };

    // Virtual plain destructor
    virtual ~TickObject() = default;

    /**
     * @brief Object draw method.
     *
     * This method draw the object to the draw context using the current status.
     * The context will be used by the engine to render it to the screen.
     *
     * @param[in,out] ctx A reference to the active context for drawing.
     */
    virtual void draw(DrawContext &ctx){};

    /**
     * @brief Get the controller for necessary access.
     *
     * @note A controller should be fully managed by the owner in most cases. Modifying a controller
     * manually might cause potential desync.
     * @return A weak ptr to the controller object.
     */
    std::weak_ptr<ObjController> getController() { return controller; };

protected:
    std::shared_ptr<ObjController> controller;
};

#endif /* GAMEPLAY_OBJS_TICKOBJECT */
