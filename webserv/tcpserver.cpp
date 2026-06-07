#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>

// 自作のC++ヘッダはシステムヘッダの後に読み込むのが安全です
#include "ConfigParser.hpp"
#include "ConfigServer.hpp"

#define PORT 8080
#define MAX_EVENTS 10

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
		//std::cout << stream_binding.str();

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
