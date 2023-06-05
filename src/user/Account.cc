#include "Account.hh"

unsigned int Account::getUID() const {
  return uid;
}

std::string Account::getUserName() const {
  return userName;
}

Account Account::createLocalProfile(const std::string &name) {
  Account ac;
  ac.token = "";
  ac.acType = AC_LOCAL;
  ac.uid = 0;
  ac.userName = name;
  return ac;
}

AccountType Account::getAccountType() const {
  return acType;
}
