#include "ConfigServer.hpp"

ConfigServer::ConfigServer()
{
	this->_port = 0;
}

ConfigServer::~ConfigServer() { }

const uint16_t	ConfigServer::getPort() const {
  return this->_port;
}

void	ConfigServer::setPort(uint16_t portNum) {
  this->_port = portNum;
}
