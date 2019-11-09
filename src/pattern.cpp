#ifndef __pattern_cpp__
#define __pattern_cpp__

#include "gentml.hpp"
#include "pattern.hpp"

namespace gentml {

pattern* pattern::comment(const std::string& c ) {
    m_comment = c;
    return this;
}

}

namespace gentml::p {

match::match($regex ex):expr(ex){}
match::operator json()const {
    if( !expr ) die("expression missing");
    json obj = json::object;
    if( m_comment.size() ) obj["comment"] = m_comment;
    obj["match"] = *expr;
    if( m_name ) obj["name"] = *m_name;

    if( auto cp = expr->captures(); cp.size() ) {
        json& jcp = obj["captures"] = json(json::object);
        for( int i = 0; i < cp.size(); i++ ) {
            auto& jcpo = jcp[std::to_string(i+1)] = json(json::object);
            jcpo["name"] = *cp[i];
        }
    }
    return obj;
}
match* match::name(cname cn) {
    m_name = cn;
    return this;
}

range::operator json()const {
    if( !m_begin ) die("begin expression missing");
    if( !m_end ) die("end expression missing");
    json obj = json::object;
    if( m_comment.size() ) obj["comment"] = m_comment;
    obj["begin"] = *m_begin;
    obj["end"] = *m_end;
    if( m_name ) obj["name"] = *m_name;
    if( m_contentName ) obj["contentName"] = *m_contentName;
    if( m_patterns.size() ) {
        json& jpts = obj["patterns"] = json(json::array);
        for( auto pt : m_patterns ) jpts[jpts.count()] = *pt;
    }
    if( auto cp = m_begin->captures(); cp.size() ) {
        json& jcp = obj["beginCaptures"] = json(json::object);
        for( int i = 0; i < cp.size(); i++ ) {
            auto& jcpo = jcp[std::to_string(i+1)] = json(json::object);
            jcpo["name"] = *cp[i];
        }
    }
    if( auto cp = m_end->captures(); cp.size() ) {
        json& jcp = obj["endCaptures"] = json(json::object);
        for( int i = 0; i < cp.size(); i++ ) {
            auto& jcpo = jcp[std::to_string(i+1)] = json(json::object);
            jcpo["name"] = *cp[i];
        }
    }
    return obj;
}
range* range::begin($regex reg) {
    m_begin = reg;
    return this;
}
range* range::end($regex reg) {
    m_end = reg;
    return this;
}
range* range::name(cname cn) {
    m_name = cn;
    return this;
}
range* range::contentName(cname cn) {
    m_contentName = cn;
    return this;
}
range* range::patterns($patterns pts ) {
    m_patterns = pts;
    return this;
}

include::include( const std::string& anc ):anchor(anc) {}
include::operator json()const {
    json obj = json::object;
    obj["include"] = anchor;
    return obj;
}

}

#endif