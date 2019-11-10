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

opt::opt( $regex re ):sub(re){}
opt::opt( const std::string& str ):opt(gentml::raw(str)){}
opt::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    return (m_name?"(":"(?:") + (std::string)*sub + ")?";
}
chainz<cname> opt::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    ret += sub->captures();
    return ret;
}
ops& ops::construct( int i, const std::string& str ) {return $regexs::construct( i, gentml::raw(str) ), *this;}
ops& ops::construct( int i, $regex reg ) {return $regexs::construct( i, reg ), *this;}

ops::ops($regexs exprs):$regexs(exprs){}
ops::operator std::__cxx11::string()const {
    if( size() == 0 ) die("empty option list");
    for( auto sub : *this ) if( !sub ) die("invalid option");
    std::string ret = m_name?"(":"(?:";
    ret += (std::string)**get(0);
    for( int i = 1; i < size(); i++ ) ret += "|" + (std::string)**get(i);
    ret += ")";
    return ret;
}
chainz<cname> ops::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    for( auto sub : *this ) ret += sub->captures();
    return ret;
}

any::any($regex expr):sub(expr){}
any::any( const std::string& str ):any(gentml::raw(str)){}
any::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    std::string ret = m_name?"(":"(?:";
    ret += "(?:";
    ret += *sub;
    ret += ")*)";
    return ret;
}
chainz<cname> any::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    ret += sub->captures();
    return ret;
}

one::one($regex expr):sub(expr){}
one::one( const std::string& str ):one(gentml::raw(str)){}
one::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    std::string ret = m_name?"(":"(?:";
    ret += "(?:";
    ret += *sub;
    ret += ")+)";
    return ret;
}
chainz<cname> one::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    ret += sub->captures();
    return ret;
}

apo::apo($regex expr):sub(expr){}
apo::apo( const std::string& str ):apo(gentml::raw(str)){}
apo::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    if( m_name ) die("name forbidden");
    return "(?=" + (std::string)*sub + ")";
}
chainz<cname> apo::captures()const {
    return sub->captures();
}

ane::ane($regex expr):sub(expr){}
ane::ane( const std::string& str ):ane(gentml::raw(str)){}
ane::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    if( m_name ) die("name forbidden");
    return "(?!" + (std::string)*sub + ")";
}
chainz<cname> ane::captures()const {
    return sub->captures();
}

bpo::bpo($regex expr):sub(expr){}
bpo::bpo( const std::string& str ):bpo(gentml::raw(str)){}
bpo::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    if( m_name ) die("name forbidden");
    return "(?<=" + (std::string)*sub + ")";
}
chainz<cname> bpo::captures()const {
    return sub->captures();
}

bne::bne($regex expr):sub(expr){}
bne::bne( const std::string& str ):bne(gentml::raw(str)){}
bne::operator std::__cxx11::string()const {
    if( !sub ) die("sub expression missing");
    if( m_name ) die("name forbidden");
    return "(?<!" + (std::string)*sub + ")";
}
chainz<cname> bne::captures()const {
    return sub->captures();
}

cat::cat($regexs exprs):$regexs(exprs){}
cat::operator std::__cxx11::string()const {
    if( size() == 0 ) die("empty cat list");
    for( auto sub : *this ) if( !sub ) die("invalid cat unit");
    std::string ret = m_name?"(":"(?:";
    for( auto sub : *this ) ret += *sub;
    ret += ")";
    return ret;
}
chainz<cname> cat::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    for( auto sub : *this ) ret += sub->captures();
    return ret;
}
cat& cat::construct( int i, const std::string& str ) {return $regexs::construct( i, gentml::raw(str) ), *this;}
cat& cat::construct( int i, $regex reg ) {return $regexs::construct( i, reg ), *this;}

sat::sat($regexs exprs):$regexs(exprs){}
sat::operator std::__cxx11::string()const {
    if( size() == 0 ) die("empty cat list");
    for( auto sub : *this ) if( !sub ) die("invalid cat unit");
    std::string ret = m_name?"(":"(?:";
    for( auto sub : *this ) ret += (std::string)*sub + "(?:\\s*)";
    ret += ")";
    return ret;
}
chainz<cname> sat::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    for( auto sub : *this ) ret += sub->captures();
    return ret;
}
sat& sat::construct( int i, const std::string& str ) {return $regexs::construct( i, gentml::raw(str) ), *this;}
sat& sat::construct( int i, $regex reg ) {return $regexs::construct( i, reg ), *this;}

sst::sst($regexs exprs):$regexs(exprs){}
sst::operator std::__cxx11::string()const {
    if( size() == 0 ) die("empty cat list");
    for( auto sub : *this ) if( !sub ) die("invalid cat unit");
    std::string ret = m_name?"(":"(?:";
    for( auto sub : *this ) ret += (std::string)*sub + "(?:\\s+)";
    ret += ")";
    return ret;
}
chainz<cname> sst::captures()const {
    chainz<cname> ret;
    if( m_name ) ret << m_name;
    for( auto sub : *this ) ret += sub->captures();
    return ret;
}
sst& sst::construct( int i, const std::string& str ) {return $regexs::construct( i, gentml::raw(str) ), *this;}
sst& sst::construct( int i, $regex reg ) {return $regexs::construct( i, reg ), *this;}

}

#endif