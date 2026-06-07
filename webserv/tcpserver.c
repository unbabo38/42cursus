// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>

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

	parse_config(argv[1]);

	

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
                                   (struct sockaddr *) &addr, &addrlen);
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
