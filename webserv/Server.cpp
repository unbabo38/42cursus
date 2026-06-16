#include "Server.hpp"

Server::Server() {};
Server::~Server() {};

std::map<int, Client> Server::getClient() const {
  return this->_client;
}

// void  Server::createListenSocket() {
//   if ((this->_listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//   {
//       perror("In socket");
//       exit(EXIT_FAILURE);
//   }
// }

// void  Server::bindSocket()  {
// 	if (bind(this->_listen_sock, (struct sockaddr *)&this->_addr, sizeof(this->_addr))<0)
// 	{
// 		perror("In bind");
// 		exit(EXIT_FAILURE);
// 	}
// }

// void  Server::listenSocket() {
// 	if (listen(this->_listen_sock, 10) < 0)
//     {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }
// }

// void  Server::fcntlSocket() {
// 	int flg = fcntl (this->_listen_sock, F_GETFL, 0) ;
// 	flg |= O_NONBLOCK;
// 	if (fcntl (this->_listen_sock, F_SETFL, flg)) {
//         perror("In fcntl");
//         exit(EXIT_FAILURE);
// 	}
// }

// void  Server::setAddr(ConfigServer &confServ) {
// 	this->_addr.sin_family = AF_INET;
//     this->_addr.sin_addr.s_addr = INADDR_ANY;
//     this->_addr.sin_port = htons( confServ.getPort() );
// 	memset(this->_addr.sin_zero, '\0', sizeof this->_addr.sin_zero);
// }

// void  Server::createEpoll() {
//  	this->_epollfd = epoll_create1(0);
//     if (this->_epollfd == -1) {
//         perror("epoll_create");
//         exit(EXIT_FAILURE);
// 	}
//     this->_monitor.events = EPOLLIN;
//     this->_monitor.data.fd = this->_listen_sock;
//     if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_listen_sock, &this->_monitor) == -1) {
//         perror("epoll_ctl: this->_listen_sock");
//         exit(EXIT_FAILURE);
// 	}
// }

// epollを単体で作るように分離
void Server::createEpollBase() {
    this->_epollfd = epoll_create1(0);
    if (this->_epollfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
}

// ソケットを作って返す
int Server::createSingleListenSocket() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    // 🎯 42課題で必須の、ポート即時再利用フラグもここに仕込んどく
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return fd;
}

// 指定されたポート用にアドレスを設定
void Server::setAddrForPort(int port) {
    this->_addr.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    this->_addr.sin_port = htons(port); // 🎯 渡されたポートをセット
    memset(this->_addr.sin_zero, '\0', sizeof this->_addr.sin_zero);
}

// 指定されたfdをbindする
void Server::bindSingleSocket(int fd)  {
    if (bind(fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
}

// 指定されたfdをlistenする
void Server::listenSingleSocket(int fd) {
    if (listen(fd, 128) < 0) { // 待ち受けキューは多めの128とかが安全
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

// 指定されたfdをノンブロッキングにする
void Server::fcntlSingleSocket(int fd) {
    int flg = fcntl(fd, F_GETFL, 0);
    flg |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flg)) {
        perror("In fcntl");
        exit(EXIT_FAILURE);
    }
}

// 指定されたfdをepollに登録する
void Server::addSocketToEpoll(int fd) {
    struct epoll_event ev; // 🚨 メンバ変数の _monitor を使い回すとバグるのでローカルで
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl: add listen sock");
        exit(EXIT_FAILURE);
    }
}

void Server::createSocketAndEpoll(const std::vector<ConfigServer>& parsed_servers) {
    // 🎯 1. 重複のないポートリストを set で作る
    std::set<int> unique_ports;
    for (size_t i = 0; i < parsed_servers.size(); i++) {
        unique_ports.insert(parsed_servers[i].getPort());
    }

    // 🎯 2. epoll は最初に1回だけ作ればOK！
    this->createEpollBase();

    // 🎯 3. ポートの数だけループして、既存の関数を使い回す
    for (std::set<int>::iterator it = unique_ports.begin(); it != unique_ports.end(); ++it) {
        int port = *it;

        // 既存の細切れ関数に「ポート」や「作ったFD」を渡して使い回す！
        int fd = this->createSingleListenSocket();
        this->setAddrForPort(port);
        this->bindSingleSocket(fd);
        this->listenSingleSocket(fd);
        this->fcntlSingleSocket(fd);

        // 🎯 4. クラスのメンバ変数 (std::set<int> _listen_fds) にストック
        this->_listen_fds.insert(fd);

        // 🎯 5. epoll に登録
        this->addSocketToEpoll(fd);

        std::cout << "🚀 Listening on port: " << port << " (FD: " << fd << ")" << std::endl;
    }
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

bool Server::processClient(int n, std::vector<ConfigServer> servers) {
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
    		response.createResponse(this->client_recv, servers);
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

void Server::runServer(std::vector<ConfigServer> servers) {
		for (int n = 0; n < this->_nfds; ++n) {
			if (this->_events[n].data.fd == this->_listen_sock) {
				this->connectToListeningSocket();
			// listening socket以外=clientを受信した場合
            } else {
				if (!this->processClient(n, servers)) {
				  continue;
				}
			}
        }
}
void Server::run(std::vector<ConfigServer> servers) {
    for (;;) {
		this->epollWait();
		this->runServer(servers);
    }
}
