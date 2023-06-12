#ifndef ALMOSTTHERE_SRC_UI_LOADER_UILOADER_HH
#define ALMOSTTHERE_SRC_UI_LOADER_UILOADER_HH

#include "../comp/Component.hh"
#include <memory>
#include <string>

/**
 * @brief Load UI structure as a root component from a file.
 * @param uiName The name of the ui.
 * @return The parsed ui tree with root id set to the same as \c uiName.
 */
std::shared_ptr<Component> loadUITree(const std::string &uiName);

#endif // ALMOSTTHERE_SRC_UI_LOADER_UILOADER_HH
