#ifndef __pattern__
#define __pattern__

#include "agent.hpp"
#include "regex.hpp"
#include "jsonz.hpp"

namespace gentml {
using namespace alioth;

struct pattern;
using $pattern = agent<pattern>;
using $patterns = chainz<$pattern>;

/**
 * @struct pattern : 模式
 * @desc :
 *  匹配模式
 */
struct pattern : public basic_thing {

    /**
     * @member comment : [可选]注释 */
    std::string m_comment;

    /**
     * @operator json : 将模式转换为json格式 */
    virtual operator json()const = 0;

    /**
     * @method comment : 设置注释 */
    pattern* comment(const std::string& c );
};
}

/**
 * @namespace p : pattern */
namespace gentml::p {

struct match : public pattern {

    $regex expr;
    // $patterns m_patterns;
    cname m_name;

    match( $regex m = nullptr );

    operator json()const override;

    /**
     * @method name : 设置模式名称 */
    match* name( cname cn );

    /**
     * @method patterns : 设置内部匹配模式 */
    // match* patterns( $patterns pts );
};

struct range : public pattern {
    
    $regex m_begin;
    $regex m_end;
    cname m_name;
    cname m_contentName;
    $patterns m_patterns;

    operator json()const override;

    /**
     * @method begin : 设置起始正则表达式 */
    range* begin($regex);

    /**
     * @method end : 设置终止正则表达式 */
    range* end($regex);

    /**
     * @method name : 设置名称 */
    range* name(cname);

    /**
     * @method contentName : 设置内容名称 */
    range* contentName(cname);

    /**
     * @method patterns : 设置内容模式 */
    range* patterns($patterns);
};

struct include : public pattern {

    std::string anchor;

    include( const std::string& anc );

    operator json()const override;
};

}

namespace gentml {

#define def_pattern(name)\
template<typename ... Args>\
p::name* name(Args&& ...args) {\
    return new p::name(std::forward<Args>(args)...);\
}

def_pattern(match)
def_pattern(range)
def_pattern(include)

#undef def_regex
}

#endif