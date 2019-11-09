#ifndef __main_cpp__
#define __main_cpp__

#include "alioth.hpp"
#include <iostream>

using namespace gentml;
using namespace std;

int main( int argc, char **argv ) {
    Alioth alioth;
    cout << ((json)alioth).toJsonString() << endl;
    return 0;
}

#endif