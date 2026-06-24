#include "utils.hpp"
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <sys/types.h>

int ft_stoi(std::string str)
{
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
    int res;
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
