
#include "Response.hpp"
#include "Client.hpp"
#include <fstream>

Response::Response() {};
Response::~Response() {};

std::string ft_to_string(size_t num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string Response::errorResponse(Client *client) {
  std::string errorResponseHtml;
  std::map<int, std::string> errorPageMap = client->getErrorPagesMap();
  std::string errorCode = errorPageMap[client->getStatusCode()];
  errorResponseHtml = "HTTP/1.1 " + errorCode + "\r\n";
  errorResponseHtml += "Content-Type: text/html\r\n";
  errorResponseHtml += "Content-Length: " + ft_to_string(client->getContentLength()) + "\r\n\r\n";
  errorResponseHtml += "<html><body><h1>" + errorCode + "</h1></body></html>";
  return errorResponseHtml;
}

std::string ft_trim(const std::string& str) {
    // 狩り取る対象（半角スペース、タブ、改行、キャリッジリターン）
    const std::string whitespace = " \t\r\n";
    
    // 文字列の先頭から、空白以外の文字が最初に現れる位置を探す
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return ""; // 全部空白だった場合は空文字を返す
    }
    
    // 文字列の末尾から、空白以外の文字が最初に現れる位置を探す
    size_t end = str.find_last_not_of(whitespace);
    
    // 有効な文字の区間だけを切り出す
    return str.substr(start, end - start + 1);
}

std::string Response::regularResponse(Client *client, std::vector<ConfigServer> servers) {

  std::string raw_host = client->getFields("Host"); // 例: "virtual_server:8081"
  std::string host_header = ft_trim(raw_host);
  std::string pure_host;

  size_t colon_pos = host_header.find(":");
  if (colon_pos != std::string::npos) {
      // コロンが見つかったら、その手前までを切り出す
      pure_host = host_header.substr(0, colon_pos);
  } else {
      // コロンがなければそのまま
      pure_host = host_header;
  }
  for (size_t i = 0; i < servers.size(); i++) {
    std::cout << "servers[i]" << servers[i].getPort() << std::endl;
    std::cout << "pure_host[i]" << pure_host << std::endl;
    if (pure_host == servers[i].getServerName())
      std::cout << servers[i].getServerName() << std::endl;

  }
  
  std::string root = client->getServer().getRoot();
    std::string target_path = client->getRequestTarget();
    std::string filepath = root;

    if (target_path == "/") {
        std::string index_file = "virtual.html";
        if (!filepath.empty() && filepath.at(filepath.size() - 1) != '/') {
            filepath += "/";
        }
        filepath += index_file;
    } else {
        if (!filepath.empty() && filepath.at(filepath.size() - 1) == '/' && target_path.at(0) == '/') {
            filepath += target_path.substr(1);
        } else {
            filepath += target_path;
        }
    }
	std::cout << "🎯 [Final File Path] -> " << filepath << std::endl;
  std::ifstream target(filepath.c_str());
  if (!target.is_open())
    this->errorResponse(client);
  std::string body;
  if (target) {
	std::stringstream ss;
	ss << target.rdbuf();
	body = ss.str();
  }
  std::stringstream body_length;
  body_length << body.size();
  std::string content_length = body_length.str();



  std::string response;

  response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/html\r\n";
  response += "Content-Length: " + content_length + "\r\n\r\n";
  response += body;
  return response;
}

void Response::initErrorMap() {
    this->_errorMap[400] = "400 Bad Request";
    this->_errorMap[403] = "403 Forbidden";
    this->_errorMap[404] = "404 Not Found";
    this->_errorMap[405] = "405 Method Not Allowed";
    this->_errorMap[500] = "500 Internal Server Error";
    this->_errorMap[501] = "501 Not Implemented";
    this->_errorMap[505] = "505 HTTP Version Not Supported";
}

void Response::createResponse(Client *client, std::vector<ConfigServer> servers){
  this->initErrorMap();
  if (this->_errorMap.find(client->getStatusCode()) != _errorMap.end()) {
    this->_response = this->errorResponse(client);
  }
  if (client->getStatusCode() == 200) {
	this->_response = this->regularResponse(client, servers);
  }
  std::cout << "this->response:" << this->_response << std::endl;
};

const std::string &Response::getResponseStr() const {
  return this->_response;
}

void Response::setResponseStr(std::string response) {
  this->_response = response;
}

// void Response::epollManager() {
//   this->_getLaunch(i);

//   if (this->_has)
// }
