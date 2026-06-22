#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>  // close() 用
#include <cstring>
#include <netinet/in.h>
#include "ConfigServer.hpp"
#include "Client.hpp"
#include "Response.hpp"
#include <fcntl.h>
#include <sys/wait.h>
#include <cstdlib>
#define MAX_EVENTS 10

class Server {
  private:
	ConfigServer _server;
	int			 _socket;
	bool		 _sentRequest;
	std::string	 _request;
	time_t		 _lastRequest;
	std::string	 _requestPayload;
	std::string	 _method;
	std::string  _resource;
	//stringMap	 _headers;
	size_t		 _contentLength;
    std::map<int, Client> _client;
	struct sockaddr_in _addr;
	int 		 _addrLen;
    struct epoll_event _monitor;
	struct epoll_event _events[MAX_EVENTS];
	Response response;
	int _listen_sock;
	int _conn_sock;
	int _nfds;
	int _epollfd;
	ssize_t len_recv;
	Client *client_recv;
	char _buffer[30000];
	std::set<int>            _listen_fds;
	std::map<int, int>       _fd_to_port_map;
	void createEpollBase();
    int  createSingleListenSocket();
    void setAddrForPort(int port);
    void bindSingleSocket(int fd);
    void listenSingleSocket(int fd);
    void fcntlSingleSocket(int fd);
    void addSocketToEpoll(int fd);

  public:
    Server();
	~Server();
	std::map<int, Client> getClient() const;
	void  createListenSocket();
	void  bindSocket();
	void  listenSocket();
	void  fcntlSocket();
	void  setAddr(ConfigServer &confServ);
	void  createEpoll();
	void  createSocketAndEpoll(const std::vector<ConfigServer>& parsed_servers);
	// const size_t &getAddrLength() const;
	void  run(std::vector<ConfigServer> );
	void runServer(std::vector<ConfigServer> );
	void setMonitorEpollout(int n);
	void closeClient(int n);
	void getClientRequest(int n);
	void connectToListeningSocket(int listen_fd, const std::vector<ConfigServer>& servers);
	void setMonitorEpollin(int fd);
	void setSocketNonblocking(int conn_sock);
	int acceptListeningSocket(int listen_fd, const std::vector<ConfigServer>& servers);
	void epollWait();
	bool checkFinishReceive(int n);
	void setAndCheckRequest();
	bool processClient(int n, std::vector<ConfigServer> servers);
	void modMonitorEpollin(int conn_sock);
};

#endif
