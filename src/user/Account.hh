/**
 * @brief Player account document module.
 *
 * @author skjsjhb
 */
#ifndef USER_ACCOUNT
#define USER_ACCOUNT

#include <string>

enum AccountType {
  AC_LOCAL,  // A local profile without authentication
  AC_ONLINE, // An authenticated account
  AC_REMOTE, // An account with limited information
};

class Account {
public:
  /**
   * @brief Gets the uid of the user.
   * @return The uid number.
   */
  [[nodiscard]] unsigned int getUID() const;

  /**
   * @brief Gets the username.
   * @return The name of the user.
   */
  [[nodiscard]] std::string getUserName() const;

  /**
   * @brief Create a local profile for playing locally.
   *
   * @note Local profiles cannot be used for online matching, etc. However, they can
   * still obtain a map from our server and play solo.
   * @return The created profile.
   */
  static Account createLocalProfile(const std::string &name);

  /**
   * @brief Gets the account type.
   * @return The type of the account.
   */
  [[nodiscard]] AccountType getAccountType() const;

protected:
  unsigned int uid;
  AccountType acType;
  std::string userName;
  std::string token; // Authorized token for online users
};

#endif /* USER_ACCOUNT */
