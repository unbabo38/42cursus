
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

Location Response::longestPrefixMatch(std::string requestTarget, const std::vector<Location> &locations) {
  int matched_length = -1;
  Location best_match;
  int pos = 0;
  for (int i = 0; i < locations.size(); i++) {
	std::string loc_path = locations[i].getLocationPath();
	if (loc_path.empty()) {
        continue;
    }
	if (requestTarget.compare(0, loc_path.size(), loc_path) == 0)
	{
		  std::cout << "Valid match candidate found: " << loc_path << " (size: " << loc_path.size() << ")" << std::endl;
		  if (loc_path.size() > matched_length) {
		  matched_length = loc_path.size();
		  pos = i;
		}
	}
  }
  std::cout << "🔍 [Match Result] Returning Path: [" << locations[pos].getLocationPath()
          << "] with Root: [" << locations[pos].getRoot() << "]" << std::endl;
  return locations[pos];
}

std::string Response::regularResponse(Client *client, std::vector<ConfigServer> servers) {

  std::string raw_host = client->getFields("Host"); // 例: "virtual_server:8081"
  std::string tmp_header = ft_trim(raw_host);
  std::string host_header;
  Location loc;
  std::string filepath;
  size_t colon_pos = tmp_header.find(":");
  int hostFlg = 0;
  if (colon_pos != std::string::npos) {
      // コロンが見つかったら、その手前までを切り出す
      host_header = tmp_header.substr(0, colon_pos);
  } else {
      // コロンがなければそのまま
      host_header = tmp_header;
  }
  for (size_t i = 0; i < servers.size(); i++) {
    std::cout << "servers[i]" << servers[i].getPort() << std::endl;
    std::cout << "host_header[i]:" << host_header << std::endl;
    if (host_header == servers[i].getServerName()) {
	  hostFlg = i;
	}
	std::cout << "server_name[i]" << servers[i].getServerName() << std::endl;
	std::cout << "locationPath[i]:" << servers[i].getLocation()[0].getLocationPath() << std::endl;
	loc = this->longestPrefixMatch(client->getRequestTarget(), servers[i].getLocation());
	std::string root_path = loc.getRoot();
	std::cout << "root_path:" << root_path << std::endl;

    if (root_path.empty()) {
      root_path = servers[i].getRoot();
    }
    std::string request_target = client->getRequestTarget();
	//   if (!root_path.empty() && root_path.back() == '/' && !request_target.empty() && request_target[0] == '/') {
    //         request_target = request_target.substr(1);
    //     }
	std::cout << "root_path:" << root_path << std::endl;
	std::cout << "request_target:" << request_target << std::endl;
    filepath = root_path + request_target;
	std::cout << "filepath:" << filepath << std::endl;
	if (!request_target.empty() && request_target[request_target.length() - 1] == '/') {
		// 本来なら longestPrefixMatch で見つけた Location (loc) の index 設定ファイルを見る
		if (!loc.getIndex().empty()) {
			filepath += loc.getIndex(); // 設定ファイルに書かれた index.html 等を足す
		} else {
			filepath += "index.html"; // 設定がなければデフォルトのフォールバック
		}
	}

	// 🎯 【綺麗にする】スラッシュが重複して "//" になっていたら一本化する
	size_t double_slash;
	while ((double_slash = filepath.find("//")) != std::string::npos) {
		filepath.replace(double_slash, 2, "/");
	}
        break;
  }

  	// std::string root = client->getServer().getRoot();
    // std::string target_path = client->getRequestTarget();
  	// std::cout << loc.getLocationPath() << std::endl;
    // if (target_path == "/") {
    //     std::string index_file = "virtual.html";
    //     if (!filepath.empty() && filepath.at(filepath.size() - 1) != '/') {
    //         filepath += "/";
    //     }
    //     filepath += index_file;
    // } else {
    //     if (!filepath.empty() && filepath.at(filepath.size() - 1) == '/' && target_path.at(0) == '/') {
    //         filepath += target_path.substr(1);
    //     } else {
    //         filepath += target_path;
    //     }
    // }
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
