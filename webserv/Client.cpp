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
	_isCookie(false)
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
  std::cout << "buf:" << buf << std::endl;
  this->_request.append(buf, len);
  std::cout << "_request:" << this->_request << std::endl;

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
  return this->_server;
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
  this->_server = server;
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
//   std::cout << "request:" << request << std::endl;

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
  for (int i = 0; i < 9; i++)
  {
	if (this->_method == methodsUse[i]) {
	  this->_statusCode = 200;
	  return ;
	}
	if (this->_method == methodsNotUse[i]) {
	  this->_statusCode = 501;
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
  std::cout <<"httpversion"<< this->_httpVersion << std::endl;
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
  std::cout << "first_line" << first_line << std::endl;
  size_t first_space = first_line.find(' ');
  if (first_space == std::string::npos) return;

  size_t second_space = first_line.find(' ', first_space + 1);
  if (second_space == std::string::npos) return;

  this->_method        = first_line.substr(0, first_space);
  this->_requestTarget = first_line.substr(first_space + 1, second_space - (first_space + 1));
  this->_httpVersion   = first_line.substr(second_space + 1);

  this->parseFields(request, first_line_end + 2);
  if (this->_isParseCompleted) {
    this->checkMethod();
    this->checkRequestTarget();
    this->checkHttpVersion();
	std::cout << this->_statusCode << std::endl;
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
  std::cout << "processChunkedRequest" << std::endl;
  while(true) {
  if (this->_chunkState == CHUNK_SIZE) {
	size_t chunkeSize = chunkedRequest.find("\r\n");
	if (chunkeSize == std::string::npos) return;
	else {
	  std::string rawChunke = chunkedRequest.substr(0, chunkeSize);
	  this->_expectedChunkSize = ftHexaToDecimal(rawChunke);
	  if (this->_expectedChunkSize == 0) {
		this->_chunkState = CHUNK_END;
		this->parseCompleted();
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

void Client::processNormalBody(std::string request) {
	this->_body = request;
	this->_phase = COMPLETE;
	this->parseCompleted();
}

void Client::parseFields(std::string &request, size_t i) {
  	while(i < request.size()) {
		size_t lineLen = request.find("\r\n", i);
		if (lineLen == std::string::npos)
			return;
		if (lineLen == i) {
      		break;
    	}
		std::string line = request.substr(i, lineLen - i);
		this->divideKeyAndValue(line);
		i += line.size() + 2;
  	}
	request = request.substr(i + 2);
	std::string cookie = this->_fields["Cookie"];
	if (cookie != "" && cookie != "session_id=") {
		std::cout << "Cookie found:" << this->_fields["Cookie"] << std::endl;
		this->_isCookie = true;
		this->_sessionId = this->_fields["Cookie"];
	}
	if (this->_fields.find("Content-Length") != this->_fields.end()) {
		this->_contentLength = ft_stoi(this->_fields["Content-Length"]);
		this->processNormalBody(request.substr(i + 2, this->_contentLength));
	}
	else if (this->_fields["Transfer-Encoding"] == " chunked") {
    	this->_isChunked = true;
		this->processChunkedRequest(request);
	}
	else
		this->parseCompleted();
	std::cout << this->_body << std::endl;
}

void Client::divideKeyAndValue(std::string line) {
  size_t keyPos = line.find(":");
	if (keyPos == std::string::npos)
		return;
  std::string key = line.substr(0, keyPos);
  std::string value = line.substr(keyPos + 1);
  this->_fields[key] = value;
  std::cout << "line = [" << line << "]\n";
	std::cout << "key = [" << key << "]\n";
	std::cout << "value = [" << value << "]\n";
  if (this->_fields.find("Host") == this->_fields.end())
    this->_statusCode = 400;
}

void Client::parseCompleted() {
  std::cout << "method = " << this->_method << std::endl;
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
}

std::string Client::getResponseStr() const {
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
  this->_cgiOutput.append(buf, n);
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
std::string Client::getCgiOutput() {
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
