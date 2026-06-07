// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "ConfigParser.hpp"
#include "ConfigServer.hpp"


#define PORT 8080
#define MAX_EVENTS 10

ConfigParser::ConfigParser()
{
	this->_nb_server = 0;
}

ConfigParser::~ConfigParser() { }

ConfigServer::ConfigServer()
{
	this->_port = 0;
}

ConfigServer::~ConfigServer() { }

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
static void parseListenLine(const std::string& line, ConfigServer *confserv) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> tokens;

    // 空白（スペースやタブ）で単語をバラバラにして vector に詰める
    while (ss >> word) {
        tokens.push_back(word);
		std::cout << word << std::endl;
    }
	for (int i = 0; i < tokens.size(); i++) {
	  if (tokens[i] == "listen" && i + 1 < tokens.size())
	    // int temp_port = ft_stoi(tokens[i + 1]);
		confserv->setPort(static_cast<uint16_t>(ft_stoi(tokens[i + 1])));
		std::cout << "inlisten" << confserv->getPort() << std::endl;
	}

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
const uint16_t	ConfigServer::getPort() const {
  return this->_port;
}

void	ConfigServer::setPort(uint16_t portNum) {
  this->_port = portNum;
}


int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);


    struct epoll_event ev, events[MAX_EVENTS];
    int listen_sock, conn_sock, nfds, epollfd;

    //char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    // Creating socket file descriptor
	ConfigParser conf_parse;
	ConfigServer conf_serv;
  	std::ifstream istrm(argv[1], std::ios::binary);
	if (!istrm.is_open()) {
    	std::cout << "failed to open " << argv[1] << '\n';
	} else {
		double d;
		//strm.read(reinterpret_cast<char*>(&d), sizeof d); // バイナリ入力
		std::string line;
		std::stringstream stream_binding;
		stream_binding << istrm.rdbuf();
		std::string file = stream_binding.str();

		std::cout << "beforsplit" << std::endl;

		conf_parse.splitConfToServers(file, &conf_serv);



		// 3. 1行ずつファイル終端まで読み込む
		// while (std::getline(istrm, line)) {
		// 	std::cout << line << std::endl;
		// }
	}
	int read_result = 0;
	//while(read_result = getline())


    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
	std::cout << conf_serv.getPort() << std::endl;
    addr.sin_port = htons( conf_serv.getPort() );

    memset(addr.sin_zero, '\0', sizeof addr.sin_zero);


    if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(listen_sock, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }


    /* Code to set up listening socket, 'listen_sock',
       (socket(), bind(), listen()) omitted. */
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
	}
		printf("epoll_Create");
	fflush(stdout);
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
	}
	printf("%d\n", listen_sock);
	fflush(stdout);

	printf("formae\n");
	fflush(stdout);
    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
		}
		printf("nfds=%d\n", nfds);
		fflush(stdout);
		for (int i= 0; i < MAX_EVENTS; i++) {
			printf("events[i]=%d\n", events[i].data.fd);
			fflush(stdout);
		}
        for (int n = 0; n < nfds; ++n) {
			printf("n=%d\n", n);
			fflush(stdout);
            if (events[n].data.fd == listen_sock) {
                conn_sock = accept(listen_sock,
                                   (struct sockaddr *) &addr, (socklen_t*)&addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
				printf("okconsock\n");
				fflush(stdout);
                //setnonblocking(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
				char buffer[30000] = {0};
				valread = read( conn_sock , buffer, 30000);
				write(conn_sock , "hello" , 5);
				printf("%s\n",buffer );
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                            &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
													printf("okconsockadd\n");

            } else {
                //do_use_fd(events[n].data.fd);
				//return 19;
            }
			printf("%s\n", "okokok");
        }
    }
    // while(1)
    // {
    //     printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    //     if ((new_socket = accept(listen_sock, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0)
    //     {
    //         perror("In accept");
    //         exit(EXIT_FAILURE);
    //     }

    //     char buffer[30000] = {0};
    //     valread = read( new_socket , buffer, 30000);
    //     printf("%s\n",buffer );

    //     write(new_socket , hello , strlen(hello));
    //     printf("------------------Hello message sent-------------------\n");
    //     close(new_socket);
    // }
    return 0;
}
