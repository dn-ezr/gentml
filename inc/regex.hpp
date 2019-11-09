#ifndef __regex__
#define __regex__

#include <string>
#include "agent.hpp"
#include "name.hpp"

namespace gentml {
using namespace alioth;

struct regex;
using $regex = agent<regex>;
using $regexs = chainz<$regex>;

struct regex : public basic_thing {

    cname m_name = nullptr;

    virtual operator std::string()const = 0;

    virtual chainz<cname> captures()const = 0;
    
    regex* name( cname );
};
}

/**
 * @namespace r : regular expressions */
namespace gentml::r {

struct raw : public regex {

    std::string sub;

    raw( std::string sub = "" );

    operator std::string()const override;

    chainz<cname> captures()const override;
};

struct optional : public regex {

    $regex sub;

    optional($regex re = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

struct options : public regex, public $regexs {

    template<typename ...Args>
    options(Args... args){(construct(-1,args), ...);}
    options($regexs exprs = {});

    operator std::string()const override;

    chainz<cname> captures()const override;
};

struct kleene : public regex {

    $regex sub;

    kleene($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};using any = kleene;

struct positive : public regex {

    $regex sub;

    positive($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};using one = positive;

struct cat : public regex {

    $regexs subs;

    template<typename ...Args>
    cat(Args... args){(subs.construct(-1,args), ...);}
    cat($regexs exprs = {});

    operator std::string()const override;

    chainz<cname> captures()const override;
};

}

namespace gentml {

#define def_regex(name)\
template<typename ... Args>\
r::name* name(Args&& ...args) {\
    return new r::name(std::forward<Args>(args)...);\
}

def_regex(raw)
def_regex(optional)
def_regex(options)
def_regex(kleene)
def_regex(any)
def_regex(positive)
def_regex(one)
def_regex(cat)

#undef def_regex

}
#endif