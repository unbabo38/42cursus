
#include "Response.hpp"
#include "Client.hpp"
#include <fstream>
#include "utils.cpp"

Response::Response() {};
Response::~Response() {};


std::string ft_to_string(size_t num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string Response::errorResponse(Client *client, int errorNum) {
  std::string errorResponseHtml;
  //std::map<int, std::string> errorPageMap = client->getErrorPagesMap();
  std::string errorCode = this->_errorMap[errorNum];
  //std::string errorCode = "404 Not Found";
  std::cout << "errorCode:" << errorCode << std::endl;
  std::string body =
    "<html><body><h1>" + errorCode + "</h1></body></html>";

  errorResponseHtml = "HTTP/1.1 " + errorCode + "\r\n";
  errorResponseHtml += "Content-Type: text/html\r\n";
  errorResponseHtml += "Content-Length: " + ft_to_string(body.size()) + "\r\n\r\n";
  errorResponseHtml += body;
  return errorResponseHtml;
}

std::string Response::errorResponse405(Client *client, std::set<std::string> &methods) {
  std::string errorResponseHtml;
  std::map<int, std::string> errorPageMap = client->getErrorPagesMap();
  //std::string errorCode = errorPageMap[client->getStatusCode()];
  std::string body =
    "<html><body><h1>405 Method Not Allowed</h1></body></html>";

  errorResponseHtml = "HTTP/1.1 405 Method Not Allowed\r\n";
  errorResponseHtml += "Allow: ";
  for (std::set<std::string>::iterator it = methods.begin();
     it != methods.end(); ++it)
  {
    errorResponseHtml += *it;
  }
  errorResponseHtml += "\r\n";
  errorResponseHtml += "Content-Type: text/html\r\n";
  errorResponseHtml += "Content-Length: " + ft_to_string(body.size()) + "\r\n\r\n";
  errorResponseHtml += body;
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
  size_t matched_length = 0;
  Location best_match;
  int pos = 0;
  for (int i = 0; i < locations.size(); i++) {
	std::string loc_path = locations[i].getLocationPath();
	if (loc_path.empty()) {
        continue;
    }
	if (requestTarget.compare(0, loc_path.size(), loc_path) == 0)
	{
		  std::cout << "candidate: " << loc_path << std::endl;
			std::cout << "loc_path.size() = " << loc_path.size()
					<< ", matched_length = " << matched_length << std::endl;

			if (loc_path.size() > matched_length) {
				std::cout << ">>> UPDATE <<<" << std::endl;
				matched_length = loc_path.size();
				pos = i;
			}
	}
  }
  std::cout << "🔍 [Match Result] Returning Path: [" << locations[pos].getLocationPath()
          << "] with Root: [" << locations[pos].getRoot() << "]" << std::endl;
  return locations[pos];
}

std::string Response::getContentType(const std::string& filePath)
{
    if (filePath.size() >= 5 &&
        filePath.substr(filePath.size() - 5) == ".html")
        return "text/html";

    if (filePath.size() >= 4 &&
        filePath.substr(filePath.size() - 4) == ".css")
        return "text/css";

    if (filePath.size() >= 3 &&
        filePath.substr(filePath.size() - 3) == ".js")
        return "application/javascript";

    if (filePath.size() >= 4 &&
        filePath.substr(filePath.size() - 4) == ".png")
        return "image/png";

    if (filePath.size() >= 4 &&
        filePath.substr(filePath.size() - 4) == ".jpg")
        return "image/jpeg";

    return "application/octet-stream";
}
std::string Response::getFileType(const std::string &filePath) {
    size_t dot_pos = filePath.rfind('.');

    if (dot_pos == std::string::npos || dot_pos == filePath.size() - 1) {
        return "";
    }
    return filePath.substr(dot_pos);
}

std::string Response::regularResponse(Client *client, std::vector<ConfigServer> servers) {

  std::string raw_host = client->getFields("Host"); // 例: "virtual_server:8081"
  std::string tmp_header = ft_trim(raw_host);
  std::string host_header;
  Location loc;
  std::string filePath;
  size_t colon_pos = tmp_header.find(":");
  int hostFlg = 0;
  if (colon_pos != std::string::npos) {
      // コロンが見つかったら、その手前までを切り出す
      host_header = tmp_header.substr(0, colon_pos);
  } else {
      // コロンがなければそのまま
      host_header = tmp_header;
  }
	ConfigServer server = client->getServer();
	loc = this->longestPrefixMatch(client->getRequestTarget(), server.getLocation());
	std::string root_path = loc.getRoot();
	if (root_path.empty()) {
      root_path = server.getRoot();
    }
    std::string request_target = client->getRequestTarget();
	std::cout << "request_path:" << root_path << std::endl;
	//   if (!root_path.empty() && root_path.back() == '/' && !request_target.empty() && request_target[0] == '/') {
    //         request_target = request_target.substr(1);
    //     }
	std::string relative = request_target.substr(loc.getLocationPath().size());
	filePath = root_path + "/" + relative;
	if (!request_target.empty() && request_target[request_target.length() - 1] == '/') {
		// 本来なら longestPrefixMatch で見つけた Location (loc) の index 設定ファイルを見る
		if (!loc.getIndex().empty()) {
			filePath += loc.getIndex(); // 設定ファイルに書かれた index.html 等を足す
		} else {

			filePath += "index.html"; // 設定がなければデフォルトのフォールバック
		}
	}

	// 🎯 【綺麗にする】スラッシュが重複して "//" になっていたら一本化する
	size_t double_slash;
	while ((double_slash = filePath.find("//")) != std::string::npos) {
		filePath.replace(double_slash, 2, "/");
	}

	std::cout << "🎯 [Final File Path] -> " << filePath << std::endl;
  std::ifstream target(filePath.c_str());

  std::string filetype = getFileType(filePath);
  std::cout << "filetype" << filetype << std::endl;
  std::string query_string = "test";
  std::map<std::string, std::string> cgifile = loc.getCgiHandlersMap();


  std::string body;
  std::map<std::string, std::string>::iterator it = cgifile.find(filetype);
  if (it != cgifile.end()) {
	    std::string cgi_path = it->second;
		body = this->cgi.do_cgi(cgi_path, filePath, query_string, client);
		//  std::stringstream body_length;
		// body_length << body.size();
		// std::string content_length = body_length.str();
		// std::string response;
		// response = "HTTP/1.1 200 OK\r\n";
		// response += "Content-Type: " + getContentType(filePath) + "\r\n";
		// response += "Content-Length: " + content_length + "\r\n\r\n";
		// response += body;
		// return response;
  }
  else if (!target.is_open()) {
    //client->setStatusCode(404);   // 必要なら
    return this->errorResponse(client, 404);
  }

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
  response += "Content-Type: " + getContentType(filePath) + "\r\n";
  response += "Content-Length: " + content_length + "\r\n\r\n";
  response += body;
  return response;
}

void Response::initErrorMap() {
    this->_errorMap[400] = "400 Bad Request";
    this->_errorMap[403] = "403 Forbidden";
    this->_errorMap[404] = "404 Not Found";
    this->_errorMap[405] = "405 Method Not Allowed";
	this->_errorMap[413] = "413 Payload Too Large";
    this->_errorMap[500] = "500 Internal Server Error";
    this->_errorMap[501] = "501 Not Implemented";
    this->_errorMap[505] = "505 HTTP Version Not Supported";
}

bool Response::checkFileName(std::string filename) {
	std::cout << "filenmae:" << filename << std::endl;
  if (filename.empty()) {
	std::cout << "empty" << std::endl;
	return false;
  }
  if (filename == "." || filename == "..") {
	std::cout << "filename" << std::endl;
	return false;
  }
  std::string invalid_chars = "/\\:*?\"<>|\0";
  if (filename.find_first_of(invalid_chars) != std::string::npos) {
	std::cout << "invalid_cahrs" << std::endl;
	return false;
  }
  return true;
}

std::string Response::postResponse(Client *client) {
  ConfigServer server = client->getServer();
  Location loc = this->longestPrefixMatch(client->getRequestTarget(), server.getLocation());
  std::string root_path = loc.getRoot();
  if (root_path.empty())
      root_path = server.getRoot();

  std::string request_target = client->getRequestTarget();
  std::map<std::string, std::string> cgi_file = loc.getCgiHandlersMap();

  std::string cgi_path = cgi_file[".sh"];
  std::cout << "cgifile[.sh]" << cgi_file[".sh"] << std::endl;
  std::string filePath = root_path + client->getRequestTarget();
  std::string relative = request_target.substr(loc.getLocationPath().size());
  if (!checkFileName(relative)) {
	return errorResponse(client, 400);
  }
  filePath = root_path + relative;

  // ★ CGI判定
  if (!loc.getCgiHandlersMap().empty()
      && loc.getCgiHandlersMap().count(".sh")) {
	  std::string body = this->cgi.do_cgi_post(
							cgi_path,
							filePath,
							client->getBody(),
							client
						);
		std::stringstream ss;
		ss << body;
	    ss << "HTTP/1.1 200 OK\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: " << body.size() << "\r\n" // bodyのサイズを直接流し込めます
		<< "\r\n"
		<< body; // もしボディがあれば

		std::string response_str = ss.str();
		return response_str;
  }

    std::string response;
  	int fileStatus = is_file_exist_posix(filePath);
	if (fileStatus == 1) {
		std::ofstream ofs(filePath.c_str());
		if (!ofs.is_open())
			perror("open");
		ofs << client->getBody();
		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Length: 0\r\n\r\n";
	} else if (fileStatus == -1){
		std::ofstream ofs(filePath.c_str());
		if (!ofs.is_open())
			perror("open");
		ofs << client->getBody();
		response = "HTTP/1.1 201 Created\r\n";
		response += "Content-Length: 0\r\n\r\n";
	} else if (fileStatus == 0) {
		response = errorResponse(client, 403);
	}
	return response;
  //権限は
}
#include <cstdio>
std::string Response::deleteResponse(Client *client) {
  ConfigServer server = client->getServer();
  Location loc = this->longestPrefixMatch(client->getRequestTarget(), server.getLocation());
  std::string root_path = loc.getRoot();
  if (root_path.empty())
      root_path = server.getRoot();
  std::string filePath = root_path + client->getRequestTarget();
  if (std::remove(filePath.c_str()) == 0) {
	return "HTTP/1.1 204 No Content\r\n"
       "Content-Length: 0\r\n\r\n";
  } else {
	//これはやる
	//client->setStatusCode(404);
    return errorResponse(client, 404);
  }
  //権限は
}

std::string Response::redirect301Response(Client *client, std::string url) {
  std::string response;
  response = "HTTP/1.1 301 Moved Permanently\r\n";
  response += "Location:";
  response += url;
  response += "Content-Length: 0\r\n\r\n";
  return response;

}

std::string Response::responseBodyTooLong(Client *client) {
  std::string response;
  response = "HTTP/1.1 413 Payload Too Large\r\n";
  response += "Content-Type: text/html";
  response += "Content-Length:";
  response += client->getBody().size();
  response += "\r\n\r\n";
  return response;
}

void Response::createResponse(Client *client, std::vector<ConfigServer> servers){
  this->initErrorMap();
  std::cout << "statuscode:" << client->getStatusCode() << std::endl;
  std::map<int, std::string>::iterator it = this->_errorMap.find(client->getStatusCode());
  if (it != _errorMap.end()) {
	std::string content_length = it->second; // 安全に値を取得
	std::cout << "値は: " << content_length << std::endl;
	this->_response = this->errorResponse(client, client->getStatusCode());
	return ;
  } else {
	// キーが存在しない場合
  	std::cout << "Content-Lengthヘッダーはありません" << std::endl;
  }
  if (this->_errorMap.find(client->getStatusCode()) != _errorMap.end()) {
    this->_response = this->errorResponse(client, 501);
  }

  ConfigServer server = client->getServer();
  Location loc = this->longestPrefixMatch(client->getRequestTarget(), server.getLocation());
  std::cout << "matched location = " << loc.getLocationPath() << std::endl;
  std::cout << "client method = " << client->getMethod() << std::endl;
  std::set<std::string> methods = loc.getLimitExcept();
  std::string client_method = client->getMethod();
  if (client->getContentLength() > loc.getClientMaxBodySize()) {
	//this->_response = this->responseBodyTooLong(client);
	this->_response = this->errorResponse(client, 413);
	return;
  }
  if (methods.size() == 0)
  {
	methods.insert("GET");
	methods.insert("POST");
	methods.insert("DELETE");
  }
  for (std::set<std::string>::iterator it = methods.begin();
  	it != methods.end(); ++it)
  {
  	std::cout << "allowed = [" << *it << "]" << std::endl;
  }
  if (client->getStatusCode() == 200) {
	if (loc.getReturn().first == 301) {
      this->_response = this->redirect301Response(client, loc.getReturn().second);
	  return;
	}
	if (methods.find(client_method) == methods.end()) {
    	client->setStatusCode(405);
    	this->_response = this->errorResponse405(client, methods);
		return ;
	}
	if (client_method== "POST")
		this->_response = this->postResponse(client);
	else if (client_method == "GET")
		this->_response = this->regularResponse(client, servers);
	else if (client_method == "DELETE") {
		std::cout << "method=" << client->getMethod() << std::endl;
    	this->_response = deleteResponse(client);
	}
  } else {
    client->setStatusCode(405);
    this->_response = this->errorResponse405(client, methods);
  }

  std::cout << "this->response:" << this->_response << std::endl;
};

const std::string &Response::getResponseStr() const {
  return this->_response;
}

void Response::setResponseStr(std::string response) {
  this->_response = response;
}
