#include "Location.hpp"
#include "ConfigServer.hpp"
#include "utils.cpp"

Location::Location() {
    this->_autoindex = false;
    this->_client_max_body_size = 1000000;
    this->_root = "./html";
}

Location::~Location() {
}

void Location::parseLocation(const std::vector<std::string>& tokens, size_t start, size_t end, ConfigServer *confserv) {
	//std::cout <<"parseLocation:  start:"<< start << "end:" << end << std::endl;
	Location new_location;
	for (int i = start; i < end; i++)
	{
		//std::cout <<"tokens[i]:  " << tokens[i] <<std::endl;
		if (tokens[i] == "root" && i + 1 < end) {
		  new_location.setRoot(tokens[i + 1]);


		} else if (tokens[i] == "autoindex") {

		  if (tokens[i + 1] == "on")
	          new_location.setAutoindex(true);
		  else if (tokens[i + 1] == "off")
	          new_location.setAutoindex(false);
		  else
		    throw std::runtime_error("invalid setting in autoindex");
		} else if (tokens[i] == "index") {
			std::vector<std::string> indexes;
			if (tokens[i + 1] != ";" && i + 1 < end) {
			  indexes.push_back(tokens[i + 1]);
			}
			new_location.setIndexFilesVec(indexes);
		} else if ((tokens[i] == "max_body_size" || tokens[i] == "client_max_body_size") && i + 1 < end) {
            new_location.setClientMaxBodySize(static_cast<uint16_t>(ft_stoi(tokens[i + 1])));
            i++;
        } else if ((tokens[i] == "method" || tokens[i] == "allow_methods") && i + 1 < end) {
            std::set<std::string> methods;
            i++;
            while (i < end && tokens[i] != ";") {
                methods.insert(tokens[i]);
                i++;
            }
            new_location.setLimitExcept(methods);
        } else if ((tokens[i] == "upload_path" || tokens[i] == "upload_store") && i + 1 < end) {
            new_location.setUploadStore(tokens[i + 1]);
            i++;
        } else if (tokens[i] == "return" && i + 2 < end) {
            int code = std::atoi(tokens[i + 1].c_str());
            std::string url = tokens[i + 2];
            new_location.setReturn(code, url);
            i += 2;
        } else if (tokens[i] == "cgi_path" && i + 1 < end) {
            std::map<std::string, std::string> cgi_map;
            cgi_map[".sh"] = tokens[i + 1];
            cgi_map[".py"] = tokens[i + 1];
            new_location.setCgiHandlersMap(cgi_map);
            i++;
        }
	}
	confserv->addLocation(new_location);
}

const std::string& Location::getRoot() const {
    return this->_root;
}

const std::vector<std::string>& Location::getIndexFilesVec() const {
    return this->_index_files_vec;
}

std::size_t Location::getClientMaxBodySize() const {
    return this->_client_max_body_size;
}

bool Location::getAutoindex() const {
    return this->_autoindex;
}

const std::map<int, std::string>& Location::getErrorPagesMap() const {
    return this->_error_pages_map;
}

const std::map<std::string, std::string>& Location::getCgiHandlersMap() const {
    return this->_cgi_handlers_map;
}

const std::set<std::string>& Location::getLimitExcept() const {
    return this->_limit_except;
}

const std::string& Location::getUploadStore() const {
    return this->_upload_store;
}

std::pair<int, std::string> Location::getReturn() const {
    return this->_return;
}

// ==========================================
//                   Setters
// ==========================================

void Location::setRoot(const std::string& root) {
    this->_root = root;
}

void Location::setIndexFilesVec(const std::vector<std::string>& index_files) {
    this->_index_files_vec = index_files;
}

void Location::setClientMaxBodySize(std::size_t size) {
    this->_client_max_body_size = size;
}

void Location::setAutoindex(bool autoindex) {
    this->_autoindex = autoindex;
}

void Location::setErrorPagesMap(const std::map<int, std::string>& error_pages) {
    this->_error_pages_map = error_pages;
}

void Location::setCgiHandlersMap(const std::map<std::string, std::string>& cgi_handlers) {
    this->_cgi_handlers_map = cgi_handlers;
}

void Location::setLimitExcept(const std::set<std::string>& limit_except) {
    this->_limit_except = limit_except;
}

void Location::setUploadStore(const std::string& upload_store) {
    this->_upload_store = upload_store;
}

void Location::setReturn(int code, const std::string& url) {
    this->_return = std::make_pair(code, url);
}
