#include "ConfigParser.hpp"



ConfigParser::ConfigParser()
{
	this->_nb_server = 0;
}

ConfigParser::~ConfigParser() { }

static int ft_stoi(std::string str)
{
    std::stringstream ss(str);
    if (str.length() > 10)
        throw std::runtime_error("too long");
    // for (size_t i = 0; i < str.length(); ++i)
    // {
    //     if(!isdigit(str[i])){
	// 		std::cout << "str[i]=" << str[i] << std::endl;
    //         throw std::runtime_error("out of digit");
	// 	}
    // }
    int res;
    ss >> res;
    return (res);
}

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

void ConfigParser::parseListenLine(const std::string& line, ConfigServer *confserv) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> tokens;

    // 空白（スペースやタブ）で単語をバラバラにして vector に詰める
    while (ss >> word) {
        tokens.push_back(word);
		//std::cout << word << std::endl;
    }
	for (int i = 0; i < tokens.size(); i++) {
	  if (tokens[i] == "listen" && i + 1 < tokens.size())
	    // int temp_port = ft_stoi(tokens[i + 1]);
		confserv->setPort(static_cast<uint16_t>(ft_stoi(tokens[i + 1])));
		std::cout << "inlisten" << confserv->getPort() << std::endl;
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
		std::cout << this->_server_config[this->_nb_server] << std::endl;
		parseListenLine(this->_server_config[this->_nb_server], confserv);

		this->_nb_server++;
		start = end + 1;
	}
}
