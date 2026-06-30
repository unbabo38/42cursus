

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

        //std::cout << "🚀 Listening on port: " << port << " (FD: " << fd << ")" << std::endl;
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

int Server::acceptListeningSocket(int listen_fd, std::vector<ConfigServer>& servers) {
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
    ConfigServer *default_server_for_port =NULL;
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].getPort() == connected_port) {
            default_server_for_port = &servers[i];
            break; // 最初に見つかったブロック（先頭）をデフォルトとする
        }
    }
	if (default_server_for_port != NULL) {
      new_client.setServer(*default_server_for_port);
	}

    // 3. クライアントに、このポート用のデフォルトサーバー設定を初期値として握らせる

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
        perror("epoll_ctl ADD(setMonitorEpollin)");
        exit(EXIT_FAILURE);
    }
}

void Server::modMonitorEpollin(int conn_sock) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = conn_sock;

    if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, conn_sock, &ev) == -1) {
        perror("epoll_ctl MOD->IN(modMonitorEpollin)");
        exit(EXIT_FAILURE);
    }
}

void Server::modMonitorEpollout(int conn_sock) {
    struct epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.fd = conn_sock;

    if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, conn_sock, &ev) == -1) {
        perror("epoll_ctl MOD->OUT(modMonitorEpollout)");
        exit(EXIT_FAILURE);
    }
}
void Server::connectToListeningSocket(int listen_fd, std::vector<ConfigServer>& servers) {
	while (true) {
      int conn_sock = acceptListeningSocket(listen_fd, servers); // 🎯 serversを渡す
	  if (conn_sock == -1)
		break;
      setSocketNonblocking(conn_sock);
      setMonitorEpollin(conn_sock);
	  //std::cout << "🚀 Accepted connection on FD: " << conn_sock << " (via Listen FD: " << listen_fd << ")" << std::endl;
	}
}

void Server::getClientRequest(int n) {
	//std::cout << "getClientRequest: " << std::endl;
	int fd = this->_events[n].data.fd;
    std::map<int, Client>::iterator it = this->_client.find(fd);
    if (it == this->_client.end()) {     // 既に閉じた fd → 何もしない
        this->client_recv = NULL;
        this->len_recv = -1;
        return;
    }
    this->client_recv = &it->second;
	std::cout << "request client address: " << &this->client_recv << std::endl;
	std::memset(this->_buffer, 0, sizeof(this->_buffer));
	this->len_recv = recv(fd, this->_buffer, sizeof(this->_buffer) - 1, 0);
	fprintf(stderr, "recv len=%zd errno=%d phase=%d req_size=%zu\n",
        this->len_recv, errno, this->client_recv->getPhase(),
        this->client_recv->getRefRequest().size());
	if (this->len_recv <= 0)             // 0=切断 / <0=EAGAIN・エラー。どちらも append しない
        return;
	//std::cout << "recv returned: " << this->len_recv << std::endl;
	// if (this->len_recv == 0) {
	//   closeClient(this->_events[n].data.fd);
	//   return;
	// }

	this->client_recv->setRequest(this->_buffer, this->len_recv);
	//std::cout << "getPhase: " << this->client_recv->getPhase() << std::endl;
	//std::cout << "PARSE_HEADER" << PARSE_HEADER << std::endl;

	if (this->client_recv->getPhase() == PARSE_HEADER) {
        std::string &req = this->client_recv->getRefRequest();
		size_t s = req.find_first_not_of("\r\n");
		if (s != std::string::npos && s > 0)
			req = req.substr(s);
		else if (s == std::string::npos)
			return;   // 全部改行なら待つ

		if (req.find("\r\n\r\n") != std::string::npos)
			this->client_recv->parseHeader(req);
		else
			return;
    }
	if (this->client_recv->getPhase() == PARSE_BODY) {
		std::map<std::string, std::string> fields = this->client_recv->getFields();
        if (this->client_recv->getIsChunked()) {
            // 2回目以降にボディだけが届いたときは、上のヘッダー処理をスルーしてダイレクトにここに来る！
			//std::cout << "getischunked: " << std::endl;
            this->client_recv->processChunkedRequest(this->client_recv->getRefRequest());
        } else if (fields.find("Content-Length") != fields.end()) {

            this->client_recv->processNormalBody(this->client_recv->getRefRequest());
        }
    }
}


void Server::closeClient(int client_fd) {
    std::map<int, Client>::iterator it = this->_client.find(client_fd);
    if (it == this->_client.end()) {
        // 既に閉じられている → 二重close。何もしない
        return;
    }
    std::cout << "[DEBUG] closeClient fd=" << client_fd << std::endl;   // ← これ

    if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
        perror("epoll_ctl DEL in closeClient");
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
	if (this->len_recv == 0) {            // 相手が接続を閉じた
        closeClient(n);    // ← これが無いと無限ループ
        return false;
    }
	if (this->len_recv < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		closeClient(n);
		return false;
	}

	return true;
}

void Server::setAndCheckRequest() {
	//this->client_recv->setRequest(this->_buffer, this->len_recv);
	// if (this->client_recv->getRequest().find("\r\n\r\n") != std::string::npos)
	// 	this->client_recv->inspectRequest();
}

#include "utils.cpp"
// bool Server::readCgi(int n, std::vector<ConfigServer> &servers) {

// }

bool Server::processClient(int n, std::vector<ConfigServer> &servers) {
    int current_fd = this->_events[n].data.fd;

    // =================================================================
    // 🌟 ルートA: もし発火したFDが「誰かのCGIのパイプFD」だった場合
    // =================================================================
	//this->readCgi();
	for (std::map<int, Client>::iterator it = this->_client.begin(); it != this->_client.end(); ++it) {
        if (it->second.getIsCgiRunning() && it->second.getCgiOutFd() == current_fd) {
		std::cerr << "[DEBUG] processing fd=" << current_fd << std::endl;
		if (current_fd == it->second.getCgiOutFd()) std::cout << " (CGI_OUT)";
		else if (current_fd == it->second.getCgiInFd()) std::cout << " (CGI_IN)";
		else std::cout << " (SOCKET)";
		std::cout << std::endl;
		std::cout << "[DEBUG] Reading CGI Pipe (fd=" << current_fd << ")" << std::endl;

		char buf[4096];
		ssize_t readnum = read(current_fd, buf, sizeof(buf));

		if (readnum > 0) {
			std::cout << "[DEBUG] Successfully read " << readnum << " bytes from pipe" << std::endl;
			it->second.appendCgiOutput(buf, readnum);
			return true;
		}

		if (readnum == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				std::cout << "[DEBUG] Pipe empty (EAGAIN), waiting for more data..." << std::endl;
				return true;
			} else {
				std::cerr << "[ERROR] Read error: " << strerror(errno) << std::endl;
			}
		}

		if (readnum == 0) {
			std::cout << "[DEBUG] EOF reached. Closing pipe fd=" << current_fd << std::endl;
				int status;
            	pid_t res = waitpid(it->second.getCgiPid(), &status, 0);

				std::cout << "cgi_read_finished\n" << std::endl;
                // 用済みのCGIパイプを epoll から削除して閉じる
                epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, current_fd, NULL);
                close(current_fd);
                it->second.setIsCgiRunning(false);

				// cgi_output は参照で受ける（コピーしない）
				std::string &cgi_output = it->second.getCgiOutput();

				size_t delimiter = cgi_output.find("\r\n\r\n");
				size_t offset = 4;
				if (delimiter == std::string::npos) {
					delimiter = cgi_output.find("\n\n");
					offset = 2;
				}

				// ヘッダ部分（小さいのでコピーOK）
				std::string cgi_headers;
				if (delimiter != std::string::npos)
					cgi_headers = cgi_output.substr(0, delimiter);

				// ボディの開始位置だけ覚える（substrでコピーしない）
				size_t body_start = (delimiter != std::string::npos) ? delimiter + offset : 0;
				size_t body_len = cgi_output.size() - body_start;

				// レスポンスヘッダを組み立てる（ヘッダは小さい）
				std::stringstream ss;
				ss << "HTTP/1.1 200 OK\r\n";
				if (!cgi_headers.empty())
					ss << cgi_headers << "\r\n";
				else
					ss << "Content-Type: text/html\r\n";
				ss << "Content-Length: " << body_len << "\r\n\r\n";

				// full_response = ヘッダ + ボディ。ヘッダ文字列を作ってから append で1回だけ足す
				std::string full_response = ss.str();              // ヘッダ部のみ（小）
				full_response.append(cgi_output, body_start, body_len);  // ボディを1回だけコピーして追記

				it->second.setResponseStr(full_response);
				{ std::string tmp; tmp.swap(it->second.getCgiOutput()); }
				// 元の「ソケットFD」を送信可能状態（EPOLLOUT）にする！
				this->modMonitorEpollout(it->first);
            }
            return true; // CGIパイプの処理をしたので、ここで安全にイベントを抜ける
        }
    }
	for (std::map<int, Client>::iterator it = this->_client.begin(); it != this->_client.end(); ++it) {
        if (it->second.getIsCgiRunning() && it->second.getCgiInFd() == current_fd) {

            // 🎯 ここでついに2つの変数が火を吹きます！
            std::string &body = it->second.getBody();
            size_t already_written = it->second.getBodyBytesWritten(); // 💡何バイト目まで送ったか
			std::cout << "already_written" << already_written << std::endl;
            while (already_written < body.size()) {
				std::cout << "[DEBUG] Writing ALL data to CGI (100MB processed)" << std::endl;
				ssize_t writenum = write(current_fd, body.c_str() + already_written, body.size() - already_written);
					std::cout
						<< "written "
						<< already_written
						<< "/"
						<< body.size()
						<< std::endl;
				if (writenum > 0) {
					already_written += writenum;
					it->second.addBodyBytesWritten(writenum);
				} else if (writenum == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
					// パイプがいっぱいになった。またイベントが来るのを待つ
					return true;
				} else {
					// エラーまたは切断
					close(current_fd);
					it->second.setCgiInFd(-1);
					std::cout << "[DEBUG] CGI InPipe CLOSED. Waiting for CGI output..." << std::endl;
					return true;
				}
			}

			// 全書き込み完了
			std::cout << "[DEBUG] CGIへの書き込み完了！" << std::endl;
			epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, current_fd, NULL);
			close(current_fd);
			it->second.setCgiInFd(-1);
			{ std::string tmp; tmp.swap(it->second.getBody()); }
			std::cout << "[DEBUG] CGI InPipe CLOSED. Waiting for CGI output..." << std::endl;
			return true;
			// size_t total_size = body.size();

            // // 残りのデータから32KBだけ切り出す
            // size_t rem = total_size - already_written;
            // size_t chunk_size = (rem > 32768) ? 32768 : rem;

            // // ノンブロッキングなので、送れる分だけ write する
            // ssize_t writenum = write(current_fd, body.c_str() + already_written, chunk_size);

            // if (writenum > 0) {
            //     // 💡進捗カウンターを更新（次のループではこの続きから write できる）
            //     it->second.addBodyBytesWritten(writenum);

            //     // 🎯 100MB 全部を書き終えたら
            //     if (it->second.getBodyBytesWritten() == total_size) {
            //         // 用済みの入力パイプを epoll から削除して閉じる
            //         // これにより、CGI側に「データ送信完了（EOF）」が伝わる
			// 		std::cout << "[DEBUG] CGIへの書き込み完了！" << std::endl;
            //         epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, current_fd, NULL);
            //         close(current_fd);
            //         it->second.setCgiInFd(-1); // 終わった目印
            //     }
            // }
            // return true; // イベント処理を抜ける
        }
    }

    // =================================================================
    // 🌐 ルートB: 純粋なクライアントソケットFDの処理（従来のロジック）
    // =================================================================
    Client& curr_client = this->_client[current_fd];

    // 1. 受信フラグ（EPOLLIN）
    if (this->_events[n].events & EPOLLIN) {
        this->getClientRequest(n);
        if (!checkFinishReceive(this->_events[n].data.fd)) {
            return false;
        }
		this->client_recv->inspectRequest();
        //setAndCheckRequest();

        if (this->client_recv->getPhase() == COMPLETE && !this->client_recv->getIsCgiRunning()) {
            Response response;
            response.createResponse(this->client_recv, servers);

            if (this->client_recv->getIsCgiRunning()) {
                // 💡 CGIが起動した場合、ソケットFDはそのまま静かに眠らせておく（EPOLLOUTにはしない）
                // その代わり、CGIのパイプFDを epoll に登録する！
                struct epoll_event ev;
                std::memset(&ev, 0, sizeof(ev));
                ev.events = EPOLLIN; // CGIが文字を吐き出すのを待つ
                ev.data.fd = this->client_recv->getCgiOutFd();
                epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->client_recv->getCgiOutFd(), &ev);

				std::memset(&ev, 0, sizeof(ev));
				ev.events = EPOLLOUT;
				ev.data.fd = this->client_recv->getCgiInFd();
				epoll_ctl(_epollfd, EPOLL_CTL_ADD, this->client_recv->getCgiInFd(), &ev);
                // ソケット側は空回りを防ぐため、EPOLLINのまま（OUTは付けない）維持
                return true;
            } else {
                // 通常の静的ファイルなら、即座に送信フェーズ（EPOLLOUT）へ
                this->client_recv->setResponseStr(response.getResponseStr());
				std::cout
    << "body.size=" << this->client_recv->getBody().size()
    << std::endl;
				std::cout << this->client_recv->getResponseStr() << std::endl;

                this->setMonitorEpollout(n);
            }
        }
        return true;

    // 2. 送信フラグ（EPOLLOUT）
    } else if (this->_events[n].events & EPOLLOUT) {
        //std::cout << "sendingINEOPLLOUT" << std::endl;

        // 💡 ここに来るということは、ルートAによってCGIが100%完了し、
        // レスポンス文字列が完成した状態で叩き起こされたということです。
        // 面倒なCGI待ちのif文はここには一切不要になり、純粋に send するだけになります！

        const std::string &resStr = curr_client.getResponseStr();
        size_t &sent_sum = curr_client.getResponseBytesSent();
        size_t total_send = resStr.size();
        const char *ptr = resStr.c_str();
				std::cout << "response client address: " << &curr_client << std::endl;

        while(sent_sum < total_send) {
            ssize_t sent = send(current_fd, ptr + sent_sum, total_send - sent_sum, 0);
			std::cout << "[DEBUG] Sent " << sent << " bytes. Progress: "
          << sent_sum + sent << "/" << total_send << std::endl;
            if (sent < 0) {
				    std::cout << "send error errno=" << errno << std::endl;
                if (errno == EAGAIN || errno == EWOULDBLOCK) return true;
                this->closeClient(current_fd);
                return false;
            } else if (sent == 0) {
                this->closeClient(current_fd);
                return false;
            }
            sent_sum += sent;
        }
        if (sent_sum >= total_send) {
			curr_client.clearForNextRequest();
		    this->modMonitorEpollin(current_fd);
			return true;
			// std::cout << "[DEBUG] Full response sent successfully!" << std::endl;
			// shutdown(current_fd, SHUT_WR);
			// this->closeClient(current_fd);
		}
        return true;
    }
    return true;
}

void Server::runServer(std::vector<ConfigServer> &servers) {
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
void Server::run(std::vector<ConfigServer> &servers) {
    for (;;) {
		this->epollWait();
		this->runServer(servers);
    }
}
