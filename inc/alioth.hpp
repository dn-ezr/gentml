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
        $patterns signature();
        $patterns dependencies();
        $patterns global_definitions();
        $patterns inner_definitions();
        $patterns implementations();
        $pattern class_definition();
        $pattern alias_definition();
        $pattern enum_definition();
        $pattern method_definition();
        $pattern operator_definition();

        $pattern method_implementation();
        $pattern operator_implementation();

        $regex operators();
        $regex dtypes();
        $regex etypes();
        $regex visibilities();
        $regex scopes();
        $regex bnd($regex);
        $regex bnd(const std::string&);

    public:
        Alioth();
};

#endif