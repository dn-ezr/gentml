#ifndef __tml_cpp__
#define __tml_cpp__

#include "tml.hpp"

namespace gentml {

Tml::operator json()const {
    json tml = json::object;
    tml["$schema"] = std::string("https://raw.githubusercontent.com/martinring/tmlanguage/master/tmlanguage.json");
    tml["name"] = name;
    tml["scopeName"] = scopeName;
    json& jfileTypes = tml["fileTypes"] = json(json::array);
    json& jpatterns = tml["patterns"] = json(json::array);
    json& jrepository = tml["repository"] = json(json::object);

    for( auto ft : fileTypes ) jfileTypes[jfileTypes.count()] = ft;
    for( auto pt : patterns ) jpatterns[jpatterns.count()] = *pt;
    for( auto [nm,pts] : repository ) {
        json& repo = jrepository[nm] = json(json::object);
        json& jpts = repo["patterns"] = json(json::array);
        for( auto pt : pts ) jpts[jpts.count()] = *pt;
    }

    return tml;
}

}

#endif