
#include "Service.hpp"

Service::Service() {};
Service::~Service() {};

std::map<int, Client> Service::getClient() const {
  return this->_client;
}

// void Service::epollManager() {
//   this->_getLaunch(i);

//   if (this->_has)
// }
