
#ifndef CONFIGOBLOCK_H
#define CONFIGOBLOCK_H

#include <string>
#include <array>
enum WorldCountry{
    IRAN,
    USA,
    UK,
    WALES
};

struct Zoo{
    int AnimalCounter;
    float Temperature;
    WorldCountry Country;
};

struct ConfigOBlock{
    bool a;
    int b;
    float c;
    double d;
    WorldCountry e;
    struct Zoo z;
    bool f;
    std::array<int,4> aa;
}__attribute__((packed));

#endif // CONFIGOBLOCK_H
