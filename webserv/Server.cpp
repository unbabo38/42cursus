#include "Server.hpp"
#include <cstdio>   // perror を使うために必要
#include <cstdlib>  // exit, EXIT_FAILURE を使うために必要
#include <cerrno>   // errno, EAGAIN, EWOULDBLOCK を使うために必要
Server::Server() {};
Server::~Server() {};

std::map<int, Client> Server::getClient() const {
  return this->_client;
}

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
    if (fd == -1) {
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
    this->_addr.sin_addr.s_addr = INADDR_ANY;
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
		this->_fd_to_port_map[fd] = port;
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

int Server::acceptListeningSocket(int listen_fd, const std::vector<ConfigServer>& servers) {
    this->_addrLen = sizeof(this->_addr);

    // 🎯 渡された listen_fd を使って accept する
    int conn_sock = accept(listen_fd, (struct sockaddr *) &this->_addr, (socklen_t*)&_addrLen);
    if (conn_sock == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1; // ➔ 呼び出し元の while ループを安全に抜けるための -1 を返す
        }
        perror("accept");
		return -1;
    }

    Client new_client;

    // ===================================================================
    // 🎯【超重要】ここがバーチャルホスト対応の命
    // このクライアントが「どのlistenソケット（ポート）」から来たかを判定し、
    // それに応じたサーバー設定（8080用か8081用か）をセットする！
    // ===================================================================
    // ※ 簡易的には、getsocknameを使うか、
    // ここで一時的に _server を適切なものに切り替えるロジックを入れます。
    // 今回は一旦既存の this->_server を入れますが、後でここをポート基準で選べるようにします。
    int connected_port = this->_fd_to_port_map[listen_fd];

    // 2. 全サーバー設定の中から、このポートを担当している「最初のサーバー」を探す
    // (Hostヘッダーが一致しなかった場合のデフォルトサーバーになります)
    ConfigServer default_server_for_port;
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].getPort() == connected_port) {
            default_server_for_port = servers[i];
            break; // 最初に見つかったブロック（先頭）をデフォルトとする
        }
    }

    // 3. クライアントに、このポート用のデフォルトサーバー設定を初期値として握らせる
    new_client.setServer(default_server_for_port);

    // 4. 後ほど Host ヘッダーを見てバーチャルホストを厳密に切り替えるため、
    //    このクライアントが「物理的に何番ポートから来たか」もクライアント自身に覚えさせておく
    new_client.setServerPort(connected_port); // 🎯 Clientクラスにこのセッターを追加


    this->_client[conn_sock] = new_client;

    return conn_sock; // 🎯 次の関数に渡すためにFDを返す
}

void Server::setSocketNonblocking(int conn_sock) {
    int flgcon = fcntl (conn_sock, F_GETFL, 0) ;
    flgcon |= O_NONBLOCK;
    if (fcntl (conn_sock, F_SETFL, flgcon)) {
        perror("In fcntl");
        exit(EXIT_FAILURE);
    }
}

void Server::setMonitorEpollin(int conn_sock) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = conn_sock;

    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
    }
}

void Server::modMonitorEpollin(int conn_sock) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = conn_sock;

    if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, conn_sock, &ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
    }
}

void Server::connectToListeningSocket(int listen_fd, const std::vector<ConfigServer>& servers) {
	while (true) {
      int conn_sock = acceptListeningSocket(listen_fd, servers); // 🎯 serversを渡す
	  if (conn_sock == -1)
		break;
      setSocketNonblocking(conn_sock);
      setMonitorEpollin(conn_sock);
	  std::cout << "🚀 Accepted connection on FD: " << conn_sock << " (via Listen FD: " << listen_fd << ")" << std::endl;
	}
}

void Server::getClientRequest(int n) {
	std::memset(this->_buffer, 0, sizeof(this->_buffer));
	this->len_recv = recv(this->_events[n].data.fd, this->_buffer, 30000, 0);
	this->client_recv = &this->_client[this->_events[n].data.fd];
}


void Server::closeClient(int client_fd) {

	//perror("in recv");
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		perror("epoll_ctl: this->_conn_sock");
	}
	close(client_fd);
	this->_client.erase(client_fd);
}

void Server::setMonitorEpollout(int n) {
int current_fd = this->_events[n].data.fd; // 今処理している正しいFD

  struct epoll_event ev;
  std::memset(&ev, 0, sizeof(ev));

  ev.events = EPOLLOUT;
  ev.data.fd = current_fd;

  if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, current_fd, &ev) == -1) {
    perror("epoll_ctl: MOD to EPOLLOUT failed");
    close(current_fd);
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
	//this->getClientRequest(n);
	int current_fd = this->_events[n].data.fd;
	Client& curr_client = this->_client[current_fd];
	// 受信フラグが立っている場合
	if (this->_events[n].events & EPOLLIN) {
		this->getClientRequest(n);
		if (!checkFinishReceive(n)) {
			return false;
		}
		// this->setClientRequest(n);
		setAndCheckRequest();
		std::cout << "setAndCheck" << this->_buffer <<std::endl;
		// parseが終わっていたらレスポンスを作ってクライアントを送信状態にする
		if (curr_client.getParseCompleted()) {

			Response response;
    		response.createResponse(&curr_client, servers);
			if (!curr_client.getIsCgiRunning()) {
				curr_client.setResponseStr(response.getResponseStr());
			}
			this->setMonitorEpollout(n);
			std::cout << "set epollo out" << this->_buffer <<std::endl;
		}
		return true;
	} else if (this->_events[n].events & EPOLLOUT) {
		std::cout << "sendingINEOPLLOUT" << this->_buffer <<std::endl;
		if (curr_client.getIsCgiRunning()) {
		std::cout << "isCgirunning" << this->_buffer <<std::endl;
			int status;
			// ⭕ WNOHANGだから実行中なら即0が返る。絶対にハングしない！
			pid_t res = waitpid(curr_client.getCgiPid(), &status, WNOHANG);

			char buf[4096];
			// ⭕ Step 1でO_NONBLOCKを仕込んだので、データがまだ来てなくても即-1(EAGAIN)で戻る！
			ssize_t readnum = read(curr_client.getCgiOutFd(), buf, sizeof(buf));

			if (readnum > 0) {
				// パイプから出てきたCGIの出力を、クライアントのバッファに少しずつ貯める
				curr_client.appendCgiOutput(buf, readnum);
			}

			// ⭕ 子プロセスが終了(res > 0)し、かつパイプのデータもすべて読み尽くした(readnum <= 0)なら、完全終了！
			if (res > 0 && readnum <= 0) {
				close(curr_client.getCgiOutFd());

				curr_client.setIsCgiRunning(false); // CGIフェーズ完了！
				std::string full_response = "HTTP/1.1 200 OK\r\n";
				full_response += "Content-Type: text/html\r\n";
				// CGIの出力（curr_client.getCgiOutput()）にすでにヘッダーが含まれている場合は、
				// 重複しないように適宜調整してください。
				std::stringstream ss;
				ss << curr_client.getCgiOutput().size();
				full_response += "Content-Length: " + ss.str() + "\r\n\r\n";
				full_response += curr_client.getCgiOutput();

				curr_client.setResponseStr(full_response);
			} else {
				//this->modMonitorEpollin(current_fd);
			    return true;
			}
		}


		std::string resStr = curr_client.getResponseStr();
		size_t sent_sum = 0;
		size_t total_send = resStr.size();
		const char *ptr = resStr.c_str();

		while(sent_sum < total_send) {
			ssize_t sent = send(this->_events[n].data.fd, ptr + sent_sum, total_send - sent_sum, 0);
			if (sent < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					// closeしない！
					return true;
				}
				perror("send");
				closeClient(this->_events[n].data.fd);
				return false;
			} else if (sent == 0) {
				std::cout << "Successfully sent " << sent << " bytes." << std::endl;
				std::cout << "resStr.size() = " << resStr.size() << std::endl;
				std::cout << "sent = " << sent << std::endl;
				this->closeClient(this->_events[n].data.fd);
        		return false;
			}
			sent_sum += sent;
		}
		this->closeClient(this->_events[n].data.fd);

		return true;
	}
	return true;
}

void Server::runServer(std::vector<ConfigServer> servers) {
		for (int n = 0; n < this->_nfds; ++n) {
			int event_fd = this->_events[n].data.fd;
			if (this->_listen_fds.count(event_fd)) {
				this->connectToListeningSocket(event_fd, servers);
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
