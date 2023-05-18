/**
 * @brief The UI event hook system.
 *
 * This system accepts the registry requests from caller, and notify
 * them when a UI event happened in specified area.
 *
 * @author skjsjhb
 */

#ifndef ENGINE_VIRTUAL_UIHOOK
#define ENGINE_VIRTUAL_UIHOOK

#include <functional>
#include "Input.hh"

typedef std::function<void()> UIHookXT;

/**
 * @brief A hook containing necessary data for backend to handle UI actions on
 * a certain area.
 */
struct HookSt
{
    /**
     * @brief The border parameters of the trigger area.
     */
    int xbegin, xend, ybegin, yend;

    /**
     * @brief Executor (aka. callback)
     */
    UIHookXT xt;

    /**
     * @brief An internal flag to prevent duplicated event firing.
     */
    bool cleared = true;
};

/**
 * @brief Register a UI hook for triggering.
 *
 * @see `HookSt`, `vtDeUIHook`
 * @param st The hook to be registered
 * @return A unique number as the handler
 */
extern unsigned int vtAddUIHook(HookSt st);

/**
 * @brief Remove a UI hook using the handler ID.
 *
 * @see `vtAddUIHook`
 * @param hid The handler ID returned by `vtAddUIHook`.
 */
extern void vtDeUIHook(unsigned int hid);

/**
 * @brief Fire an update event and call related callbacks using the
 * stats cached in the specified input buffer.
 *
 * @param isp The input buffer object (`InputSet`).
 */
extern void vtNotifyUIHooks(const InputSet &isp);

#endif /* ENGINE_VIRTUAL_UIHOOK */
