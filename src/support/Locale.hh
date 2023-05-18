/**
 * @brief Locale management and translation providing module.
 *
 * @author skjsjhb
 */

#ifndef SUPPORT_LOCALE
#define SUPPORT_LOCALE

#include <string>

/**
 * @brief Sets the active locale.
 *
 * The corresponding locale file will be loaded from `assets/locale/<name>.lang`
 *
 * @param name The identifier of the locale
 */
void setLocale(const std::string &name);

/**
 * @brief Gets the translation text corresponding to the key provided.
 * @param key Translation key
 * @return Translated text, or `` if not found
 */
std::string getLocaleText(const std::string &key);

#endif /* SUPPORT_LOCALE */
