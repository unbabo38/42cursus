#include "Client.hpp"
#include "utils.cpp"
#include <fstream>

#include <iostream>
Client::Client() :
    _isCgiRunning(false),
    _contentLength(0),
    _statusCode(200),          // ◄ 200 (OK) で初期化
    _phase(PARSE_HEADER),      // ◄ 最初のステージはヘッダーパース
    _chunkState(CHUNK_SIZE),   // ◄ 最初のチャンク状態はサイズ読み込み
    _isParseCompleted(false),  // ◄ まだパースは完了していない
    _isChunked(false),         // ◄ 最初は通常の想定
	_sessionId(""),
	_isCookie(false),
	_server(NULL),
	_cgiInFd(-1),
	_bodyBytesWritten(0),
	_response_bytes_sent(0),
	_isSending(false)
{
    this->methodsUse.push_back("GET");
    this->methodsUse.push_back("DELETE");
    this->methodsUse.push_back("POST");

    this->methodsNotUse.push_back("HEAD");
    this->methodsNotUse.push_back("OPTIONS");
    this->methodsNotUse.push_back("TRACE");
    this->methodsNotUse.push_back("PATCH");
    this->methodsNotUse.push_back("CONNECT");
    this->methodsNotUse.push_back("PUT");
}
Client::~Client() {};

void Client::setRequest(const char *buf, const int &len) {
  //std::cout << "buf:" << buf << std::endl;
  this->_request.append(buf, len);
  //std::cout << "_request:" << this->_request << std::endl;

}

const std::string &Client::getRequest() const {
  return this->_request;
}
std::string &Client::getRefRequest() {
  return this->_request;
}
const std::string &Client::getRequestTarget() const {

  return this->_requestTarget;
}

ConfigServer &Client::getServer() {
  return *(this->_server);
}

const std::string &Client::getFields(const std::string &key) const {
    // 1. findを使って要素を検索する（これならconstマップでも使える）
    std::map<std::string, std::string>::const_iterator it = this->_fields.find(key);

    // 2. もしキーが存在しなかったら、空文字列の参照などを返す（または例外）
    if (it == this->_fields.end()) {
        static const std::string empty_string = "";
        return empty_string;
    }

    // 3. 発見できたらその値（value）を返す
    return it->second;
}

void Client::setServer(ConfigServer &server) {
  this->_server = &server;
}

void Client::setServerPort(int port) {
	this->_server_port = port;
}
int  Client::getServerPort() const {
	return this->_server_port;
}

const std::string &Client::getMethod() const {
  return this->_method;
}


void Client::inspectRequest() {
//   std::string request = getRequest();
//   //std::cout << "request:" << request << std::endl;
	//std::cout << "🔍 [DEBUG RESPONSE] クライアントの現在のMethod: [" << this->getMethod() << "]" << std::endl;

	if (this->_isParseCompleted == true) {
        return;
    }
  //this->parseHeader(request);
  if (this->_statusCode != 200) {
    this->_isParseCompleted = true;
    return ;
  }
  return ;
}

const std::string &Client::getBody() const {
  return this->_body;
}


void Client::checkMethod() {
  for (int i = 0; i < 3; i++)
  {
	if (this->_method == methodsUse[i]) {
	  this->_statusCode = 200;
	  return ;
	}
  }
  for (int i = 0; i < 6; i++) {
	if (this->_method == methodsNotUse[i]) {
	  this->_statusCode = 405;
	  return ;
	}
  }
  this->_statusCode = 400;
  return ;
}

void Client::checkRequestTarget() {
  if (this->_requestTarget[0] != '/') {
	this->_statusCode = 400;
	return ;
  }
  int i = 0;
  while (this->_requestTarget[i]) {
	if (this->_requestTarget == " ") {
		this->_statusCode = 400;
		return ;
  	}
	i++;
  }
}

void Client::checkHttpVersion() {
  //std::cout <<"httpversion"<< this->_httpVersion << std::endl;
  if (this->_httpVersion != "HTTP/1.1") {
	this->_statusCode = 505;
	return ;
  }
}
void Client::parseHeader(std::string &request) {
  size_t first_line_end = request.find("\r\n");
  if (first_line_end == std::string::npos)
	return;
  bool flg;

  std::string first_line = request.substr(0, first_line_end);
  //std::cout << "first_line" << first_line << std::endl;
  size_t first_space = first_line.find(' ');
  if (first_space == std::string::npos) return;

  size_t second_space = first_line.find(' ', first_space + 1);
  if (second_space == std::string::npos) return;

  this->_method        = first_line.substr(0, first_space);
  this->_requestTarget = first_line.substr(first_space + 1, second_space - (first_space + 1));
  this->_httpVersion   = first_line.substr(second_space + 1);

  this->parseFields(request, first_line_end + 2);
  std::cout << "[" << this->_fields["Content-Length"] << "]" << std::endl;
  if (this->_isParseCompleted) {
    this->checkMethod();
    this->checkRequestTarget();
    this->checkHttpVersion();
	//std::cout << this->_statusCode << std::endl;
  }
  this->setPhase(PARSE_BODY);
}

size_t Client::ftHexaToDecimal(std::string rawChunke) {
  size_t decimalValue;
  std::stringstream ss;

  ss << std::hex << rawChunke;
  ss >> decimalValue;

  if (ss.fail()) {
      return 0;
  }

  return decimalValue;
}

void Client::processChunkedRequest(std::string &chunkedRequest) {
  //std::cout << "processChunkedRequest" << std::endl;
  while(true) {
  if (this->_chunkState == CHUNK_SIZE) {
	size_t chunkeSize = chunkedRequest.find("\r\n");
	if (chunkeSize == std::string::npos) return;
	else {
	  std::string rawChunke = chunkedRequest.substr(0, chunkeSize);
	  this->_expectedChunkSize = ftHexaToDecimal(rawChunke);
	  if (this->_expectedChunkSize == 0) {
		this->_chunkState = CHUNK_END;
		this->_phase = COMPLETE;
	    return;
	  }
	  chunkedRequest = chunkedRequest.substr(chunkeSize + 2);
	  this->_chunkState = CHUNK_DATA;
	}
  }
  if (this->_chunkState == CHUNK_DATA) {
	if (chunkedRequest.size() < this->_expectedChunkSize + 2) {
	  return ;
	}
	this->_body += chunkedRequest.substr(0, this->_expectedChunkSize);
	chunkedRequest = chunkedRequest.substr(this->_expectedChunkSize + 2);
	this->_chunkState = CHUNK_SIZE;
  }
  if (this->_chunkState == CHUNK_END) {
	this->_phase = COMPLETE;
        return;
    }
  }
}

void Client::processNormalBody(const std::string &request) {
    if (request.size() >= _contentLength) {
        _body = request.substr(0, _contentLength);   // ボディはピッタリ切る
        _phase = COMPLETE;
    } else {
        _body = request;   // まだ足りない、待つ
    }
}

void Client::parseFields(std::string &request, size_t i) {
    while (i < request.size()) {
        size_t lineLen = request.find("\r\n", i);
        if (lineLen == std::string::npos)
            return;                       // ヘッダがまだ全部来てない
        if (lineLen == i)                 // 空行 = ヘッダ終端
            break;
        std::string line = request.substr(i, lineLen - i);
        this->divideKeyAndValue(line);
        i += line.size() + 2;
    }

    // ヘッダ部分を捨てて、残り（ボディ）だけを request に残す
    size_t body_start = i + 2;
    if (body_start <= request.size())
        request = request.substr(body_start);
    else
        request.clear();

    // --- Host 必須チェック（全フィールド読み終わってから1回だけ） ---
    if (this->_fields.find("Host") == this->_fields.end())
        this->_statusCode = 400;

    // --- Cookie / session_id 抽出（今のロジックそのまま） ---
    std::string cookie = this->_fields["Cookie"];
    size_t last_valid = cookie.find_last_not_of(" \t\r\n");
    cookie = (last_valid != std::string::npos) ? cookie.substr(0, last_valid + 1) : "";
    std::string prefix = "session_id=";
    size_t pos = cookie.find(prefix);
    if (pos != std::string::npos) {
        std::string id_value = cookie.substr(pos + prefix.length());
        size_t semi_pos = id_value.find(";");
        if (semi_pos != std::string::npos)
            id_value = id_value.substr(0, semi_pos);
        size_t first = id_value.find_first_not_of(" \t");
        size_t last  = id_value.find_last_not_of(" \t");
        if (first != std::string::npos && last != std::string::npos)
            id_value = id_value.substr(first, last - first + 1);
        if (!id_value.empty()) {
            this->_isCookie = true;
            this->_sessionId = id_value;
        } else {
            this->_isCookie = false;
        }
    } else {
        this->_isCookie = false;
    }

    // --- ボディの「種類」だけ判定。append はしない！ ---
    if (this->_fields.find("Content-Length") != this->_fields.end()) {
        this->_contentLength = ft_stoi(this->_fields["Content-Length"]);
        // processNormalBody は呼ばない（getClientRequest の PARSE_BODY に任せる）
    }
    else if (ft_trim(this->_fields["Transfer-Encoding"]) == "chunked") {
        this->_isChunked = true;
    }
    else {
        this->_phase = COMPLETE;   // ボディ無し（GET/DELETE等）→即完了
    }
}

// void Client::parseFields(std::string &request, size_t i) {
//   	while(i < request.size()) {
// 		size_t lineLen = request.find("\r\n", i);
// 		if (lineLen == std::string::npos)
// 			return;
// 		if (lineLen == i) {
//       		break;
//     	}
// 		std::string line = request.substr(i, lineLen - i);
// 		this->divideKeyAndValue(line);
// 		i += line.size() + 2;
//   	}
// 	request = request.substr(i + 2);
// 	std::string cookie = this->_fields["Cookie"];

//     // 1. まずCookieヘッダー全体の末尾にある目に見えない改行（\r, \n）や空白を削る
//     size_t last_valid = cookie.find_last_not_of(" \t\r\n");
//     if (last_valid != std::string::npos) {
//         cookie = cookie.substr(0, last_valid + 1);
//     } else {
//         cookie = "";
//     }

//     // 2. "session_id=" という文字列が含まれているか探す
//     std::string prefix = "session_id=";
//     size_t pos = cookie.find(prefix);

//     if (pos != std::string::npos) {
//         // "session_id=" の直後から末尾までの文字列（純粋なID）を切り出す
//         std::string id_value = cookie.substr(pos + prefix.length());

//         // セミコロン「;」で区切られて他のCookieが続く場合を考慮（例: session_id=xxx; path=/）
//         size_t semi_pos = id_value.find(";");
//         if (semi_pos != std::string::npos) {
//             id_value = id_value.substr(0, semi_pos);
//         }

//         // 念のため、抽出したID自体の前後の余計な空白も削る
//         size_t first = id_value.find_first_not_of(" \t");
//         size_t last = id_value.find_last_not_of(" \t");
//         if (first != std::string::npos && last != std::string::npos) {
//             id_value = id_value.substr(first, (last - first + 1));
//         }
// 		//std::cout << "id_value:" << id_value << std::endl;

//         // 最終的に中身が空でなければ、正しくCookieありとしてセット
//         if (!id_value.empty()) {
//             //std::cout << "🎯 [Parsed Clean Session ID] -> [" << id_value << "]" << std::endl;
//             this->_isCookie = true;
//             this->_sessionId = id_value;
//         } else {
//             this->_isCookie = false;
//         }
//     } else {
//         this->_isCookie = false;
//     }
// 	if (this->_fields.find("Content-Length") != this->_fields.end()) {

// 		this->_contentLength = ft_stoi(this->_fields["Content-Length"]);
// 		this->processNormalBody(request.substr(i + 2, this->_contentLength));
// 	}
// 	else if (this->_fields["Transfer-Encoding"] == " chunked") {
//     	this->_isChunked = true;
// 		this->processChunkedRequest(request);
// 	}
// 	else
// 		this->_body = COMPLETE;
// 	////std::cout << this->_body << std::endl;
// }

void Client::divideKeyAndValue(std::string line) {
    size_t keyPos = line.find(":");
    if (keyPos == std::string::npos)
        return;
    std::string key   = line.substr(0, keyPos);
    std::string value = line.substr(keyPos + 1);
    this->_fields[key] = value;
    // Host チェックはここではやらない
}

void Client::parseCompleted() {
  //std::cout << "method = " << this->_method << std::endl;
  if (this->_method == "POST") {
	if (!this->_isChunked) {
	  this->_contentLength = this->_body.size();
	  this->_isParseCompleted = true;
	}
  }
  this->_isParseCompleted = true;
}

const int &Client::getStatusCode() const {
  return this->_statusCode;
}

const std::map<int, std::string>& Client::getErrorPagesMap() const {
    return this->_error_pages_map;
}

void Client::setErrorPagesMap(const std::map<int, std::string>& error_pages) {
    this->_error_pages_map = error_pages;
}
// if (client_recv.getRequest().size() >= 4)
// {
// 	if (client_recv.getRequest().substr(client_recv.getRequest().size() - 4, 4) == "\r\n\r\n")
// }

const size_t Client::getContentLength() const {
  return this->_contentLength;
}

const std::string Client::getField(std::string key) const {
  return this->_fields.at(key);
}

const bool &Client::getParseCompleted() const {
  return this->_isParseCompleted;
}

void Client::setResponseStr(const std::string& str) {
    this->_res.setResponseStr(str);
	this->_response_bytes_sent = 0;
}

const std::string &Client::getResponseStr() const {
    return this->_res.getResponseStr();
}

void Client::setStatusCode(const int statusCode) {
  this->_statusCode = statusCode;
}


void Client::setCgiPid(int pid) {
  this->_cgiPid = pid;
}
void Client::setCgiOutFd(int fd) {
  this->_cgiOutFd = fd;
}
void Client::setIsCgiRunning(bool tr) {
  this->_isCgiRunning = tr;
}
bool Client::getIsChunked() {
  return this->_isChunked;
}

// std::string& Client::getReceive() {
//   return this->_request;
// }
void Client::appendRequest(std::string recv, ssize_t size) {
  this->_request.append(recv, size);
}

void Client::setPhase(int phase) {
	this->_phase = phase;
}
int Client::getPhase() {
	return this->_phase;
}

std::map<std::string, std::string> Client::getFields() {
  return this->_fields;
}
void Client::appendCgiOutput(const char *buf, size_t n) {
    if (this->_cgiOutput.find("500 Internal Server Error") != std::string::npos)
        return;
    this->_cgiOutput.append(buf, n);   // 上限チェックを削除
}

int Client::getCgiPid() {
  return this->_cgiPid;
}
int Client::getCgiOutFd() {
  return this->_cgiOutFd;
}
bool Client::getIsCgiRunning() {
  return this->_isCgiRunning;
}
std::string &Client::getCgiOutput() {
  return this->_cgiOutput;
}

bool Client::getIsCookie() {
  return this->_isCookie;
}

void Client::setIsCookie() {
  this->_isCookie = true;
}

void Client::setSessionId(std::string sessionId) {
  this->_sessionId = sessionId;
}

std::string Client::getSessionId() {
  return this->_sessionId;
}

// =================================================================
// 💡 CGI入力用FD (CgiInFd) のゲッター / セッター
// =================================================================
int Client::getCgiInFd() const {
    return this->_cgiInFd;
}

void Client::setCgiInFd(int fd) {
    this->_cgiInFd = fd;
}

// =================================================================
// 💡 書き込み進捗カウンター (BodyBytesWritten) の制御
// =================================================================
size_t Client::getBodyBytesWritten() const {
    return this->_bodyBytesWritten;
}

void Client::setBodyBytesWritten(size_t bytes) {
    this->_bodyBytesWritten = bytes;
}

// 🎯 write が成功したバイト数をそのまま加算するためのヘルパー
void Client::addBodyBytesWritten(size_t bytes) {
    this->_bodyBytesWritten += bytes;
}

std::string &Client::getBody() {
    // 参照を返すことで、100MBのデータも「コピー時間ゼロ」で安全に扱えます
    return this->_body;
}


size_t &Client::getResponseBytesSent() {
    return this->_response_bytes_sent;
}

void Client::addResponseBytesSent(size_t bytes) {
    this->_response_bytes_sent += bytes;
}

void Client::setResponseBytesSent(size_t bytes) {
    this->_response_bytes_sent = bytes;
}


void Client::clearForNextRequest() {
    // 1. 各種ステージ・フラグのリセット
	  this->_isSending = false;
	this->_phase = PARSE_HEADER;         // 最初のヘッダー読み込みステージに戻す
    this->_statusCode = 200;             // デフォルトのステータスにリセット
    this->_sentRequest = false;
    this->_isParseCompleted = false;     // パース完了フラグを戻す
    this->_isChunked = false;

    // 2. チャンク関連のリセット
    this->_expectedChunkSize = 0;
    this->_chunkState = CHUNK_SIZE;      // チャンクサイズ読み取りステージに戻す

    // 3. CGI関連のリセット（念のため）
    this->_cgiPid = -1;
    this->_cgiOutFd = -1;
    this->_cgiInFd = -1;
    this->_isCgiRunning = false;
    this->_bodyBytesWritten = 0;
    this->_cgiOutput.clear();

    // 4. 文字列バッファ・マップの完全クリア
    this->_request.clear();
    this->_requestTarget.clear();
    this->_method.clear();
    this->_resource.clear();
    this->_httpVersion.clear();
    this->_body.clear();
    this->_recv.clear();
    this->_fields.clear();               // HTTPヘッダーのマップをクリア

    // 5. カウンター・サイズ系のリセット
    this->_contentLength = 0;
    this->_response_bytes_sent = 0;      // 送信済みバイト数をリセット

    // 6. レスポンスオブジェクト自体の初期化（必要であれば）
    // もし Response クラスに独自の文字列バッファがあるなら、ここでクリアする関数を呼ぶか、再生成する
    this->_res = Response();

    // タイムスタンプの更新
    this->_lastRequest = time(NULL);
}
