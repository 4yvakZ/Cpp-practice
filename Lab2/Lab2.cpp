#include <iostream>
#include "Array.h"


int main()
{
    Array<std::string> a;
    for (int i = 0; i < 10; ++i)
        std::cout << a.insert("i + 1") << "\n";

}