/**
 * @brief Object controller bridge module.
 *
 * This module evaluate the control script and calculate related state for the owner to
 * retreive and update itself.
 *
 * @author skjsjhb
 */

#ifndef GAMEPLAY_CONTROL_CONTROLLER
#define GAMEPLAY_CONTROL_CONTROLLER

#include <vector>
#include <cglm/cglm.h>
#include <map>
#include "gameplay/map/MapDef.hh"
#include <string>
#include <memory>
#include <list>

/**
 * @brief A minimal reference kept by a controller.
 *
 * The controller copies part of the values in the map object to
 * initialize itself. The following is the values to be copied.
 */
class ObjMinimalRef
{
public:
    std::string id;
    int tickScript;
    int player;
    double genTime, endTime;
    double length; // The real length (in seconds), for duration notes only.
    ObjMinimalRef(const MapObject &m);
};

/**
 * @brief Object state info.
 *
 * This is a state kept in the controller and will be read by the owner of the
 * controller.
 */
struct ObjState
{
    vec3 pos = {0, 0, 0}, up = {0, 0, 0}, normal = {0, 0, 0};
    double len = 0, alpha = 1;
};

/**
 * @brief A controller of an object.
 *
 * This controller acts as a bridge connecting the map script and each object it generated.
 * The owner of this controller will use this controller to retreive its state and update itself
 * as part of its ticking logic.
 */
class ObjController
{
public:
    /**
     * @brief The status update ('ticking') function.
     *
     * This function evaluates the script and extracts assigned value to inner
     * state object.
     *
     * @param absTime Current time.
     */
    virtual void tick(double absTime);

    /**
     * @brief Initialize a controller using a map object.
     *
     * The values of the map object is copied and used for initialization. Any
     * changes are not synced later.
     *
     * @param o The object to be used for initialization.
     */
    explicit ObjController(const MapObject &o) : objRef(o){};

    /**
     * @brief Get the evaluated state.
     *
     * This function returns a copy of the inner status to avoid invalid changes.
     *
     * @return A copy of the state object.
     */
    ObjState getState() const;

    /**
     * @brief Get the reference values of this controllers.
     *
     * A status of a note is decided (mostly or exclusively) by its controller. Values
     * like `hitTime` and `genTime` are stored here.
     *
     * @return A const reference of the ref obejct.
     */
    const ObjMinimalRef &getReference() const;

    /**
     * @brief Set the controller this controller ref to.
     * @param cr Target ptr to be referenced.
     */
    void setRel(std::weak_ptr<ObjController> cr) { rel = cr; };

    // Virtual destructor
    virtual ~ObjController() = default;

protected:
    /**
     * @brief The status of the target.
     *
     * This state information will be read passively by the owner of this controller.
     */
    ObjState currentState;

    /**
     * @brief The minimal required values set.
     */
    ObjMinimalRef objRef;

    /**
     * @brief A pointer to the controller this object reference to.
     *
     * The reference relationship is built from the map.
     *
     * @see `MapDef`
     * @see `loadMap`
     */
    std::weak_ptr<ObjController> rel;

    // Internal value, defining last time synced
    double lastTickTime = -1;
};

/**
 * @brief Bind control utilities to Lua engine in order to run the tick script.
 */
void initControllerLuaExt();

#endif /* GAMEPLAY_CONTROL_CONTROLLER */
