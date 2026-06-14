#include "Server.hpp"

Server::Server() {};
Server::~Server() {};

std::map<int, Client> Server::getClient() const {
  return this->_client;
}

void  Server::createListenSocket() {
  if ((this->_listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("In socket");
      exit(EXIT_FAILURE);
  }
}

void  Server::bindSocket()  {
	if (bind(this->_listen_sock, (struct sockaddr *)&this->_addr, sizeof(this->_addr))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
}

void  Server::listenSocket() {
	if (listen(this->_listen_sock, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

void  Server::fcntlSocket() {
	int flg = fcntl (this->_listen_sock, F_GETFL, 0) ;
	flg |= O_NONBLOCK;
	if (fcntl (this->_listen_sock, F_SETFL, flg)) {
        perror("In fcntl");
        exit(EXIT_FAILURE);
	}
}

void  Server::setAddr(ConfigServer &confServ) {
	this->_addr.sin_family = AF_INET;
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    this->_addr.sin_port = htons( confServ.getPort() );
	memset(this->_addr.sin_zero, '\0', sizeof this->_addr.sin_zero);
}

void  Server::createEpoll() {
 	this->_epollfd = epoll_create1(0);
    if (this->_epollfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
	}
    this->_monitor.events = EPOLLIN;
    this->_monitor.data.fd = this->_listen_sock;
    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_listen_sock, &this->_monitor) == -1) {
        perror("epoll_ctl: this->_listen_sock");
        exit(EXIT_FAILURE);
	}
}

void Server::createSocketAndEpoll(ConfigServer &confServ) {
	this->_server = confServ;
    this->createListenSocket();
	this->setAddr(confServ);
	this->bindSocket();
	this->listenSocket();
	this->fcntlSocket();
	this->createEpoll();
}

// const size_t &Server::getAddrLength() const {
//   return this->_addr.size();
// }

void Server::epollWait() {
	this->_nfds = epoll_wait(this->_epollfd, this->_events, MAX_EVENTS, -1);
    if (this->_nfds == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
	}
}

void Server::acceptListeningSocket() {
	this->_addrLen = sizeof(this->_addr);
	this->_conn_sock = accept(this->_listen_sock,
                                   (struct sockaddr *) &this->_addr, (socklen_t*)&_addrLen);
    if (this->_conn_sock == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	Client new_client;
	new_client.setServer(this->_server);
	this->_client[this->_conn_sock] = new_client;
}

void Server::setSocketNonblocking() {
	int flgcon = fcntl (this->_conn_sock, F_GETFL, 0) ;
	flgcon |= O_NONBLOCK;
	if (fcntl (this->_conn_sock, F_SETFL, flgcon)) {
		perror("In fcntl");
		exit(EXIT_FAILURE);
	}
}

void Server::setMonitorEpollin() {
    this->_monitor.events = EPOLLIN;
    this->_monitor.data.fd = this->_conn_sock;
    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_conn_sock,
                &this->_monitor) == -1) {
        perror("epoll_ctl: this->_conn_sock");
        exit(EXIT_FAILURE);
    }
}
void Server::connectToListeningSocket() {
	acceptListeningSocket();
	setSocketNonblocking();
	setMonitorEpollin();
}

void Server::getClientRequest(int n) {
	std::memset(this->_buffer, 0, sizeof(this->_buffer));
	this->len_recv = recv(this->_events[n].data.fd, this->_buffer, 30000, 0);
	this->client_recv = &this->_client[this->_events[n].data.fd];
}


void Server::closeClient(int n) {

	//perror("in recv");
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, this->_events[n].data.fd, &_monitor) == -1) {
		perror("epoll_ctl: this->_conn_sock");
		exit(EXIT_FAILURE);
	}
	close(this->_events[n].data.fd);
	this->_client.erase(this->_events[n].data.fd);
}

void Server::setMonitorEpollout(int n) {
  this->_monitor.events = EPOLLOUT;
  if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, this->_events[n].data.fd, &this->_monitor) == -1) {
	close(this->_events[n].data.fd);
  }
}

bool Server::checkFinishReceive(int n) {
	if (this->len_recv < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		closeClient(n);
		return false;
	}
	return true;
}

void Server::setAndCheckRequest() {
	this->client_recv->setRequest(this->_buffer, this->len_recv);
	if (this->client_recv->getRequest().find("\r\n\r\n") != std::string::npos)
		this->client_recv->inspectRequest();
}

bool Server::processClient(int n) {
	this->getClientRequest(n);
	std::cout << "buffer" << this->_buffer <<std::endl;

	// 受信フラグが立っている場合
	if (this->_events[n].events & EPOLLIN) {
		if (!checkFinishReceive(n)) {
			return false;
		}
		// this->setClientRequest(n);
		setAndCheckRequest();
		std::cout << "setAndCheck" << this->_buffer <<std::endl;
		// parseが終わっていたらレスポンスを作ってクライアントを送信状態にする
		if (this->client_recv->getParseCompleted()) {
			Response response;
    		response.createResponse(this->client_recv);
			this->client_recv->setResponseStr(response.getResponseStr());
			this->setMonitorEpollout(n);
			std::cout << "set epollo out" << this->_buffer <<std::endl;
		}
	} else if (this->_events[n].events & EPOLLOUT) {
		std::cout << "sending" << this->_buffer <<std::endl;

		std::string resStr = this->client_recv->getResponseStr();
		ssize_t sent = send(this->_events[n].data.fd, resStr.c_str(), resStr.size(), 0);

	}
	return true;
}

void Server::runServer() {
		for (int n = 0; n < this->_nfds; ++n) {
			if (this->_events[n].data.fd == this->_listen_sock) {
				this->connectToListeningSocket();
			// listening socket以外=clientを受信した場合
            } else {
				if (!this->processClient(n)) {
				  continue;
				}
			}
        }
}
void Server::run() {
    for (;;) {
		this->epollWait();
		this->runServer();
    }
}
