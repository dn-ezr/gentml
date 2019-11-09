#ifndef __alioth_cpp__
#define __alioth_cpp__

#include "alioth.hpp"

Alioth::Alioth() {

    name = "alioth";
    fileTypes = {"alioth",".alioth"};
    scopeName = "source.alioth";
    patterns = {
        new p::include("#comments"),
        new p::include("#signature"),
        new p::include("#global-definitions"),
        new p::include("#implementations"),
        new p::include("#keywords")
    };

    repository = {
        {"comments",comments()},
        {"signature",{
            //new p::range()
            }},
        {"dependencies",{
            //new p::match(),
            //new p::match(),
            //new p::match()
            }},
        {"global-definitions",{

            }},
        {"implementations",{

            }},
        {"keywords",{

            }}
    };
}

$regex Alioth::label() {
    return one(
        options(
            raw("\\w"),
            raw("_")));
}

$regex Alioth::space() {
    return one(
        raw("\\s"));
}

$regex Alioth::tag() {
    return one(
        options(
            raw("\\w"),
            raw("-"),
            raw("_")));
}

$patterns Alioth::comments() {
    return {
        match(raw("//.*"))
            ->name(&n::comment.line.double_slash),
        range()
            ->begin(raw("/\\*"))
            ->end(raw("\\*/"))
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
            ->patterns({
                match(
                    cat(
                        raw("@")->name(&n::keyword.other),
                        tag()->name(&n::entity.name.section),
                        space(),
                        one(raw("[^:]"))->name(&n::entity.name.function),
                        raw(":")))})
    };
}

#endif