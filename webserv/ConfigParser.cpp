#include "ConfigParser.hpp"
#include "Location.hpp"
#include "utils.cpp"


ConfigParser::ConfigParser()
{
	this->_nb_server = 0;
}

ConfigParser::~ConfigParser() { }


size_t ConfigParser::findStartServer (size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  std::runtime_error("Wrong character out of server scope{}");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw std::runtime_error("Wrong character out of server scope{}");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  std::runtime_error("Wrong character out of server scope{}");

}

/* finding a server end and return the index of } end of server */
size_t ConfigParser::findEndServer (size_t start, std::string &content)
{
	size_t	i;
	size_t	scope;

	scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}



size_t ConfigParser::getBlockSize(const std::vector<std::string>& tokens, size_t start_pos) {
    size_t brace_count = 0;
    size_t pos = start_pos;

    while (pos < tokens.size()) {
        if (tokens[pos] == "{") {
            brace_count++;
        } else if (tokens[pos] == "}") {
            brace_count--;
            if (brace_count == 0) {
                return pos - start_pos + 1;
            }
        }
        pos++;
    }
    throw std::runtime_error("Error: Unclosed curly brace '{'");
}

void ConfigParser::parseServer(const std::string& line, ConfigServer *confserv) {
    std::string formatted_line = "";
	for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == ';' || line[i] == '{' || line[i] == '}') {
            formatted_line += " ";
            formatted_line += line[i];
            formatted_line += " ";
        } else {
            formatted_line += line[i];
        }
    }
	std::stringstream ss(formatted_line);
    std::string word;
    std::vector<std::string> tokens;

    while (ss >> word)
	{
        tokens.push_back(word);
		//std::cout << word << std::endl;
	}
	for (size_t i = 0; i < tokens.size(); i++) {
	  if (tokens[i] == "listen" && i + 1 < tokens.size()) {
		confserv->setPort(static_cast<uint16_t>(ft_stoi(tokens[i + 1])));
		i++;
	  } else if (tokens[i] == "server_name" && i + 1 < tokens.size()) {
		confserv->setServerName(tokens[i + 1]);
		i++;
	  } else if (tokens[i] == "root" && i + 1 < tokens.size()) {
		confserv->setRoot(tokens[i + 1]);
		i++;
	  } else if (tokens[i] == "location" && i + 1 < tokens.size())
	  {
		size_t len = getBlockSize(tokens, i);
		Location::parseLocation(tokens, i, i + len, confserv);
		i+=len;
	  } else if (tokens[i] == "error_page" && i + 2 < tokens.size()) {
	  	short code = static_cast<short>(ft_stoi(tokens[i + 1]));
	  	std::string path = tokens[i + 2];
	  	confserv->addErrorPage(code, path);
	  	i += 3;
	  }
	}
}


void ConfigParser::splitConfToServers(std::string &content, ConfigServer *confserv)
{
	std::cout << "insplit" << std::endl;
	size_t start = 0;
	size_t end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw std::runtime_error("Server did not find");
	while (start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		std::cout << start << " " << end << std::endl;
		if (start == end)
			std::cout << "start=end" << std::endl;
		//throw std::runtime_error("problem with scope");
		//std::string con = static_cast<std::string> (content);
		this->_server_config.push_back(content.substr(start, end - start + 1));
		parseServer(this->_server_config[this->_nb_server], confserv);
        //confserv->printServerConfig();
		this->_servers.push_back(*confserv);
		this->_servers.back().printServerConfig();
		this->_nb_server++;
		start = end + 1;
	}
}
