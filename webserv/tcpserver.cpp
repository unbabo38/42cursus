#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "ConfigParser.hpp"
#include "ConfigServer.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Response.hpp"
#define PORT 8080
#define MAX_EVENTS 10



int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);

	std::map<int, Client> client;




    struct epoll_event ev, events[MAX_EVENTS];
    int listen_sock, conn_sock, nfds, epollfd;

    //char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    // Creating socket file descriptor
	ConfigParser conf_parse;
	ConfigServer conf_serv;
	Response response;
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


		conf_parse.splitConfToServers(file, &conf_serv);
		std::vector<Location> loc;
		loc = conf_serv.getLocation();
		//std::cout << loc[0].getRoot() << std::endl;
		std::cout << "autoindex" << loc[0].getAutoindex() << std::endl;

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
	//std::cout << conf_serv.getPort() << std::endl;
    addr.sin_port = htons( conf_serv.getPort() );
	std::cout << "servername:" << conf_serv.getServerName() << std::endl;
	//conf_serv.printServerConfig();

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
	int flg = fcntl (listen_sock, F_GETFL, 0) ;
	flg |= O_NONBLOCK;
	if (fcntl (listen_sock, F_SETFL, flg)) {
        perror("In fcntl");
        exit(EXIT_FAILURE);
	}

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
				int flgcon = fcntl (conn_sock, F_GETFL, 0) ;
				flgcon |= O_NONBLOCK;
				if (fcntl (conn_sock, F_SETFL, flgcon)) {
					perror("In fcntl");
					exit(EXIT_FAILURE);
				}
                ev.events = EPOLLIN;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                            &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
				char buffer[30000] = {0};
				ssize_t len = recv(events[n].data.fd, buffer, 30000, 0);
				Client &client_recv = client[events[n].data.fd];

				if (ev.events & EPOLLIN) {
					if (len < 0) {
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						continue;
						perror("in recv");
						if (epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1) {
							perror("epoll_ctl: conn_sock");
							exit(EXIT_FAILURE);
						}
						close(events[n].data.fd);
						client.erase(events[n].data.fd);
						continue;
					}
					std::cout << "setrequest:" << buffer << std::endl;
					client_recv.setRequest(buffer, len);
					if (client_recv.getRequest().find("\r\n\r\n") != std::string::npos)
					client_recv.inspectRequest();
					if (client_recv.getParseCompleted()) {
						response.createResponse(&client_recv);
						ev.events = EPOLLOUT;

						if (epoll_ctl(epollfd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1) {
							close(events[n].data.fd);
//							client_map.erase(events[n].data.fd);
						}
					}
				} else if (events[n].events & EPOLLOUT) {
					std::string resStr = response.getResponseStr();
					ssize_t sent = send(events[n].data.fd, resStr.c_str(), resStr.size(), 0);
					std::cout << sent <<std::endl;
				}
			}
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
