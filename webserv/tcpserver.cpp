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

#include "config.hpp"

#define PORT 8080
#define MAX_EVENTS 10

ConfigParser::ConfigParser()
{
	this->_nb_server = 0;
}

ConfigParser::~ConfigParser() { }

// void ConfigParser::createServer(std::string &config, ServerConfig &server)
// {
// 	std::vector<std::string>	parametrs;
// 	std::vector<std::string>	error_codes;
// 	int		flag_loc = 1;
// 	bool	flag_autoindex = false;
// 	bool	flag_max_size = false;

// 	parametrs = splitParametrs(config += ' ', std::string(" \n\t"));
// 	if (parametrs.size() < 3)
// 		throw  ErrorException("Failed server validation");
// 	for (size_t i = 0; i < parametrs.size(); i++)
// 	{
// 		if (parametrs[i] == "listen" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (server.getPort())
// 				throw  ErrorException("Port is duplicated");
// 			server.setPort(parametrs[++i]);
// 		}
// 		else if (parametrs[i] == "location" && (i + 1) < parametrs.size())
// 		{
// 			std::string	path;
// 			i++;
// 			if (parametrs[i] == "{" || parametrs[i] == "}")
// 				throw  ErrorException("Wrong character in server scope{}");
// 			path = parametrs[i];
// 			std::vector<std::string> codes;
// 			if (parametrs[++i] != "{")
// 				throw  ErrorException("Wrong character in server scope{}");
// 			i++;
// 			while (i < parametrs.size() && parametrs[i] != "}")
// 				codes.push_back(parametrs[i++]);
// 			server.setLocation(path, codes);
// 			if (i < parametrs.size() && parametrs[i] != "}")
// 				throw  ErrorException("Wrong character in server scope{}");
// 			flag_loc = 0;
// 		}
// 		else if (parametrs[i] == "host" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (server.getHost())
// 				throw  ErrorException("Host is duplicated");
// 			server.setHost(parametrs[++i]);
// 		}
// 		else if (parametrs[i] == "root" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (!server.getRoot().empty())
// 				throw  ErrorException("Root is duplicated");
// 			server.setRoot(parametrs[++i]);
// 		}
// 		else if (parametrs[i] == "error_page" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			while (++i < parametrs.size())
// 			{
// 				error_codes.push_back(parametrs[i]);
// 				if (parametrs[i].find(';') != std::string::npos)
// 					break ;
// 				if (i + 1 >= parametrs.size())
// 					throw ErrorException("Wrong character out of server scope{}");
// 			}
// 		}
// 		else if (parametrs[i] == "client_max_body_size" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (flag_max_size)
// 				throw  ErrorException("Client_max_body_size is duplicated");
// 			server.setClientMaxBodySize(parametrs[++i]);
// 			flag_max_size = true;
// 		}
// 		else if (parametrs[i] == "server_name" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (!server.getServerName().empty())
// 				throw  ErrorException("Server_name is duplicated");
// 			server.setServerName(parametrs[++i]);
// 		}
// 		else if (parametrs[i] == "index" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (!server.getIndex().empty())
// 				throw  ErrorException("Index is duplicated");
// 			server.setIndex(parametrs[++i]);
// 		}
// 		else if (parametrs[i] == "autoindex" && (i + 1) < parametrs.size() && flag_loc)
// 		{
// 			if (flag_autoindex)
// 				throw ErrorException("Autoindex of server is duplicated");
// 			server.setAutoindex(parametrs[++i]);
// 			flag_autoindex = true;
// 		}
// 		else if (parametrs[i] != "}" && parametrs[i] != "{")
// 		{
// 			if (!flag_loc)
// 				throw  ErrorException("Parametrs after location");
// 			else
// 				throw  ErrorException("Unsupported directive");
// 		}
// 	}
// 	if (server.getRoot().empty())
// 		server.setRoot("/;");
// 	if (server.getHost() == 0)
// 		server.setHost("localhost;");
// 	if (server.getIndex().empty())
// 		server.setIndex("index.html;");
// 	if (ConfigFile::isFileExistAndReadable(server.getRoot(), server.getIndex()))
// 		throw ErrorException("Index from config file not found or unreadable");
// 	if (server.checkLocaitons())
// 		throw ErrorException("Locaition is duplicated");
// 	if (!server.getPort())
// 		throw ErrorException("Port not found");
// 	server.setErrorPages(error_codes);
// 	if (!server.isValidErrorPages())
// 		throw ErrorException("Incorrect path for error page or number of error");
// }

static void parseListenLine(const std::string& line) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> tokens;

    // 空白（スペースやタブ）で単語をバラバラにして vector に詰める
    while (ss >> word) {
        tokens.push_back(word);
		std::cout << word << std::endl;
    }

    // 例: "listen 8080 8080 80808;"
    // tokens[0] = "listen"
    // tokens[1] = "8080"
    // tokens[2] = "8080"
    // tokens[3] = "80808;" (末尾にセミコロンが残る)

    // 🛑 弾くべきチェック1：引数の数が絶対におかしい
    // 正しい listen は ["listen", "ポート番号;"] の2つだけのはず

    // // 🛑 弾くべきチェック2：末尾にちゃんとセミコロンがあるか？
    // std::string port_str = tokens[1];
    // if (port_str.empty() || port_str[port_str.length() - 1] != ';') {
    //     throw std::runtime_error("Error: missing ';' at the end of directive");
    // }

    // // セミコロンを切り落として純粋な数字にする
    // port_str = port_str.substr(0, port_str.length() - 1);

    // // 🛑 弾くべきチェック3：数字以外が混ざってないか？（"80a80" 対策）
    // for (size_t i = 0; i < port_str.length(); i++) {
    //     if (!isdigit(port_str[i])) {
    //         throw std::runtime_error("Error: host not found in listen directive");
    //     }
    // }

    // // 最後に int に変換してポート範囲内（0〜65535）かチェック
    // int port = std::stoi(port_str); // C++98なら atoi(port_str.c_str())
    // if (port < 0 || port > 65535) {
    //     throw std::runtime_error("Error: invalid port range");
    // }

    // std::cout << "SUCCESS: Port is " << port << std::endl;
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
void ConfigParser::splitServers(std::string &content)
{
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
		parseListenLine(this->_server_config[this->_nb_server]);

		this->_nb_server++;
		start = end + 1;
	}
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
		//std::cout << stream_binding.str();

		ConfigParser conf;
		conf.splitServers(file);


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
    addr.sin_port = htons( PORT );

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
