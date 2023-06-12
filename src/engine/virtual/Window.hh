/**
 * @brief The engine game window spec.
 *
 * The implementation should contain window management and coordinates transforming.
 *
 * @author skjsjhb
 */

#ifndef ENGINE_VIRTUAL_WINDOW
#define ENGINE_VIRTUAL_WINDOW

/**
 * @brief Initialize game window.
 *
 * @note The window instance created is unique. Therefore, this
 * method should not be called multiple times.
 */
extern void vtInitWindow();

/**
 * @brief Gets the window pointer.
 *
 * @return A \c void* pointer to the window instance.
 * @note The type of the pointer is based on the backend. If not intended
 * to tweak the engine, DO NOT try to cast it.
 */
extern void *vtGetWindow();

/**
 * @brief Destroy window and deinitialize the graphics library.
 */
extern void vtStopWindow();

/**
 * @brief Run the main window loop once in order to complete stuffs other than
 * game loop and drawing.
 *
 * @return \c true if the window shoule be closed.
 */
extern bool vtWindowLoop();

/**
 * @brief Gets the real size of the window.
 *
 * @param[out] x,y References to the width and the height to write to.
 */
extern void vtGetWindowSize(int &x, int &y);

/**
 * @brief Transforms the virtual coord to real coord.
 *
 * The virtual coord is a coordinate in virtual 2D space 1600x900, where
 * (0,0) is the bottom left corner. The real coord is the corresponding coord
 * in the viewport. If the viewport also has a coord system that sets (0,0) as
 * the bottom left corner, then a simple scale will do the trick.
 *
 * @see \c vtDeCoord
 * @param sx,sy Virtual coord
 * @param[out] rx,ry References to the real coord to write to.
 */
extern void vtGetCoord(int sx, int sy, int &rx, int &ry);

/**
 * @brief Transforms the real coord to virtual coord.
 *
 * @see \c vtGetCoord
 * @param rx,ry Real coord
 * @param[out] sx,sy References to the virtual coord to write to.
 */
extern void vtDeCoord(int rx, int ry, int &sx, int &sy);

/**
 * @brief Sets the max number of frames rendered per second.
 *
 * This functions guarantees that no more than specified count of frames are rendered, FPS
 * may become lower if hardware failed to support.
 *
 * @note Even by setting fps to the same as screen this does not enable VSYNC.
 * @note FPS cannot be higher than TPS, since frames are rendered inside the loop of each tick.
 * @see \c vtSetTPSCap
 * @param fps The max fps cap.
 */
extern void vtSetFPSCap(unsigned int fps);

/**
 * @brief Sets the max number of ticks to be run per second.
 *
 * This functions guarantees that no more than specified count of ticks are processed. On
 * low-end devices the actual TPS might get even slower.
 *
 * @note Values lower than 500 are not accepted.
 * @note Due to the limit of the hardware (timings are not that accurate), the actual tps
 * might be slightly lower than the set value.
 * @param tps The max tps cap.
 */
extern void vtSetTPSCap(unsigned int tps);

/**
 * @brief Check if this frame should be drawn.
 *
 * @return \c true if engine considers its time to update a frame.
 */
extern bool vtShouldDraw();

/**
 * @brief Flush the render buffer.
 *
 * VMC Turbo uses double-buffered output, this function updated the display.
 */
extern void vtDisplayFlip();

#endif /* ENGINE_VIRTUAL_WINDOW */
