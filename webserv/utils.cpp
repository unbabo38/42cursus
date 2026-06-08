#include "utils.hpp"



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
