#ifndef __regex_cpp__
#define __regex_cpp__

#include "regex.hpp"
#include "gentml.hpp"

namespace gentml {
regex* regex::name( cname cn ) {
    m_name = cn;
    return this;
}
}
namespace gentml::r {

raw::raw( std::string ex ):sub(ex){}
raw::operator std::string()const {
    return (m_name?"(":"(?:") + sub + ")";
}
chainz<cname> raw::captures()const {
    if( m_name ) return {m_name};
    else return {};
}

optional::optional( $regex re ):sub(re){}
optional::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    return (m_name?"(":"(?:") + (std::string)*sub + ")?";
}
chainz<cname> optional::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    ret += sub->captures();
    return ret;
}

options::options($regexs exprs):$regexs(exprs){}
options::operator std::__cxx11::string()const {
    if( size() == 0 ) die("empty option list");
    for( auto sub : *this ) if( !sub ) die("invalid option");
    std::string ret = m_name?"(":"(?:";
    ret += (std::string)**get(0);
    for( int i = 1; i < size(); i++ ) ret += "|" + (std::string)**get(i);
    ret += ")";
    return ret;
}
chainz<cname> options::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    for( auto sub : *this ) ret += sub->captures();
    return ret;
}

kleene::kleene($regex expr):sub(expr){}
kleene::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    std::string ret = m_name?"(":"(?:";
    ret += "(?:";
    ret += *sub;
    ret += ")*)";
    return ret;
}
chainz<cname> kleene::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    ret += sub->captures();
    return ret;
}

positive::positive($regex expr):sub(expr){}
positive::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    std::string ret = m_name?"(":"(?:";
    ret += "(?:";
    ret += *sub;
    ret += ")+)";
    return ret;
}
chainz<cname> positive::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    ret += sub->captures();
    return ret;
}

cat::cat($regexs exprs):subs(exprs){}
cat::operator std::__cxx11::string()const {
    if( subs.size() == 0 ) die("empty cat list");
    for( auto sub : subs ) if( !sub ) die("invalid cat unit");
    std::string ret = m_name?"(":"(?:";
    for( auto sub : subs ) ret += *sub;
    ret += ")";
    return ret;
}
chainz<cname> cat::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    for( auto sub : subs ) ret += sub->captures();
    return ret;
}

}

#endif