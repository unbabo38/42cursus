#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "ConfigServer.hpp"
#include "Response.hpp"
enum RequestPhase {
    PARSE_HEADER, // ヘッダーを読んでいるステージ
    PARSE_BODY,   // ボディ（通常またはチャンク）を読んでいるステージ
    COMPLETE      // 全部読み終わったステージ
};

enum ChunkState {
    CHUNK_SIZE, // 16進数のサイズを読み取っている最中
    CHUNK_DATA, // 実際のボディデータを読み取っている最中
    CHUNK_END   // すべてのチャンクを読み終えた（0\r\n\r\nが来た）
};


class Client {
  private:
    std::vector<std::string> methodsUse;
    std::vector<std::string> methodsNotUse;
	ConfigServer& _server;
	int 		 _phase;
	int			 _socket;
	int			 _statusCode;
	bool		 _sentRequest;
	bool 		 _isParseCompleted;
	bool 		 _isChunked;
	int 		 _cgiPid;
	int 		 _cgiOutFd;
	bool		 _isCgiRunning;
	int 		 _expectedChunkSize;
	int			 _chunkState;
	bool		 _isCookie;

 	std::string  _cgiOutput;
	std::string	 _request;
	time_t		 _lastRequest;
	std::string	 _requestTarget;
	std::string	 _method;
	std::string  _resource;
	std::string  _httpVersion;
	std::string  _body;
	std::string  _recv;
    std::map<int, std::string>         _error_pages_map;

	std::map<std::string, std::string> _fields;
	size_t		 _contentLength;
	Response 	_res;
	int _server_port;
	std::string _sessionId;
  public:
    Client();
    ~Client();

	const std::string &getMethod() const;
	void setRequest(const char* buf, const int &len);
	const std::string &getRequest() const;
	std::string &getRefRequest();
	const std::string &getRequestTarget() const;
	void inspectRequest();
	void parseHeader(std::string &header);
    void checkRequestTarget();
    void checkHttpVersion();
	void divideKeyAndValue(std::string line);
	void parseFields(std::string &request, size_t i);
	void parseCompleted();
	void checkMethod();
	const int &getStatusCode() const;
    const std::map<int, std::string>&   getErrorPagesMap() const;
    void setErrorPagesMap(const std::map<int, std::string>& error_pages);
    const size_t getContentLength() const;
	const bool &getParseCompleted() const;
	void setResponseStr(const std::string& str);

    std::string getResponseStr() const;
	ConfigServer &getServer();
	void setServer(ConfigServer &server);
	const std::string &getFields(const std::string &key) const;
	void setServerPort(int port);
    int  getServerPort() const ;
	const std::string &getBody() const;
	void setStatusCode(const int statusCode);
	void setCgiPid(int pid);
	void setCgiOutFd(int fd);
	void setIsCgiRunning(bool tr);
	void appendCgiOutput(const char *buf, size_t n);
	int getCgiPid();
	int getCgiOutFd();
	bool getIsCgiRunning();
	bool getIsChunked();
	std::string& getReceive();
	void appendRequest(std::string recv, ssize_t size);
	int getPhase();
	void setPhase(int phase);
	std::map<std::string, std::string> getFields();
	std::string getCgiOutput();
	const std::string getField(std::string key) const;
	void processChunkedRequest(std::string &chunkedRequest);
	size_t ftHexaToDecimal(std::string rawChunke);
	void processNormalBody(std::string request);
	bool getIsCookie();
	std::string getSessionId();
	void setSessionId(std::string sessionId);
	void setIsCookie();
};

#endif
