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

/**
 * @struct opt : optional */
struct opt : public regex {

    $regex sub;

    opt($regex re = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct ops : options */
struct ops : public regex, public $regexs {

    template<typename ...Args>
    ops(Args... args){(construct(-1,args), ...);}
    ops($regexs exprs = {});

    operator std::string()const override;

    chainz<cname> captures()const override;
};

struct any : public regex {

    $regex sub;

    any($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

struct one : public regex {

    $regex sub;

    one($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct apo : look ahead positive */
struct apo : public regex {

    $regex sub;

    apo($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct ane : look ahead negative */
struct ane : public regex {

    $regex sub;

    ane($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct bpo : look back positive */
struct bpo : public regex {

    $regex sub;

    bpo($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct bne : look back negative */
struct bne : public regex {

    $regex sub;

    bne($regex expr = nullptr);

    operator std::string()const override;

    chainz<cname> captures()const override;
};

struct cat : public regex, public $regexs {

    template<typename ...Args>
    cat(Args... args){(construct(-1,args), ...);}
    cat($regexs exprs = {});

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct sat : space saperated cat, spaces are optional */
struct sat : public regex, public $regexs {

    template<typename ...Args>
    sat(Args ... args){(construct(-1,args), ...);}
    sat($regexs exprs = {});

    operator std::string()const override;

    chainz<cname> captures()const override;
};

/**
 * @struct sst : space saperated cat, spaces are necessary */
struct sst : public regex, public $regexs {

    template<typename ...Args>
    sst(Args ... args){(construct(-1,args), ...);}
    sst($regexs exprs = {});

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
def_regex(opt)
def_regex(ops)
def_regex(any)
def_regex(one)
def_regex(cat)
def_regex(sat)
def_regex(sst)
def_regex(apo)
def_regex(ane)
def_regex(bpo)
def_regex(bne)

#undef def_regex

}
#endif