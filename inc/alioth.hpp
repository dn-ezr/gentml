#ifndef __alioth__
#define __alioth__

#include "gentml.hpp"
using namespace gentml;

class Alioth : public Tml {

    private:
        $regex label();
        $regex space();
        $regex tag();
        $patterns comments();

    public:
        Alioth();
};

#endif