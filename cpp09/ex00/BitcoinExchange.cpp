#include "BitcoinExchange.hpp"

int process_line(std::string line) {
  
}

int exchange(const std::string &filename) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "Error: could not open file." << std::endl;
    return 1;
  }
  std::string line;
  while(std::getline(ifs, line)) {
    std::cout << line << std::endl;
    process_line(line);
  }
  ifs.close();
}