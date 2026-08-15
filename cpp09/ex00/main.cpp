#include "BitcoinExchange.hpp"

int  main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Error: could not open file." << std::endl;
    return (1);
  }
  std::string file = argv[1];
  exchange(file);
}