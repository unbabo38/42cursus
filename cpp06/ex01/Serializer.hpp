#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <climits>
#include <iostream>
#include <stdint.h>

struct Data {
    int k;
};

class Serializer {
  private:
    Serializer();
    Serializer(const Serializer &other);
    Serializer &operator=(const Serializer &other);
    virtual ~Serializer();

  public:
    static uintptr_t serialize(Data *ptr);
    static Data *deserialize(uintptr_t raw);
};

#endif
