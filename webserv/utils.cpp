#include "utils.hpp"
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <sys/types.h>

size_t ft_stoi(std::string str)
{
    if (str == "")
      return 0;
    std::stringstream ss(str);
    if (str.length() > 10)
        throw std::runtime_error("too long");
    // for (size_t i = 0; i < str.length(); ++i)
    // {
    //     if(!isdigit(str[i])){
	// 		std::cout << "str[i]=" << str[i] << std::endl;
    //         throw std::runtime_error("out of digit");
	// 	}
    // }
    size_t res;
    ss >> res;
    return (res);
}

std::string ft_to_string(int num)
{
    std::stringstream ss;
	ss << num;
	return ss.str();
}


int is_file_exist_posix(const std::string& filename) {
  struct stat buffer;
  if (stat(filename.c_str(), &buffer) != 0) {
	return -1;
  }
  if (buffer.st_mode & 0200) {
        return 1;
    } else {
        return 0;
  }
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
