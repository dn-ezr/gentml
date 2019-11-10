#ifndef __alioth_cpp__
#define __alioth_cpp__

#include "alioth.hpp"

Alioth::Alioth() {

    name = "alioth";
    fileTypes = {"alioth",".alioth"};
    scopeName = "source.alioth";
    patterns = {
        include("#comments"),
        include("#signature"),
        include("#global-definitions"),
        include("#implementations")
    };

    repository = {
        {"comments",comments()},
        {"signature",signature()},
        {"dependencies",dependencies()},
        {"global-definitions",global_definitions()},
        {"inner-definitions",inner_definitions()},
        {"implementations",implementations()}
    };
}

$regex Alioth::label() {
    return one(
        ops(
            raw("\\w"),
            raw("_")));
}

$regex Alioth::space() {
    return one(
        raw("\\s"));
}

$regex Alioth::tag() {
    return one(
        ops(
            raw("\\w"),
            raw("-"),
            raw("_")));
}

$patterns Alioth::comments() {
    return {
        match(raw("//.*"))
            ->name(&n::comment.line.double_slash),
        range()
            ->begin(raw("/\\*\\*doc"))
            ->end(raw("end\\*\\*/"))
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
            ->patterns({
                include("text.html.markdown")
            }),
        range()
            ->begin(raw("/\\*\\*"))
            ->end(raw("\\*/"))
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
            ->patterns({
                match(
                    cat(
                        raw("@")->name(&n::keyword.other),
                        tag()->name(&n::storage.type),
                        space(),
                        cat(
                            raw(".*"),
                            apo(raw(": ")))->name(&n::entity.name.function),
                        raw(":")))}),
        range()
            ->begin(raw("/\\*"))
            ->end(raw("\\*/"))
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
    };
}

$patterns Alioth::signature() {
    return {
        range()
            ->begin(
                sat(
                    raw("module")->name(&n::keyword),
                    space(),
                    label()->name(&n::entity.name),
                    opt(
                        sst(
                            raw("entry")->name(&n::keyword),
                            label()->name(&n::entity.name.tag))),
                    opt(
                        raw(":")->name(&n::keyword))))
            ->end(
                apo(
                    ops(
                        raw("class"),
                        raw("method"),
                        raw("operator"),
                        raw("enum"),
                        raw("let"),
                        raw(";"))))
            ->patterns({
                include("#dependencies")
            })
    };
}

$patterns Alioth::dependencies() {
    return {
        match(
            sat(
                raw("as")->name(&n::keyword),
                ops(
                    sat(raw("this"),raw("module"))->name(&n::constant.language),
                    label()->name(&n::entity.name)))),
        match(
            sat(
                raw("@")->name(&n::keyword),
                ops(
                    label(),
                    raw(R"(\"[^\"]+\")"),
                    raw(R"(\'[^\']+\')"))
            ->name(&n::string.interpolated))),
        match(label()->name(&n::support.clas))
    };
}

$patterns Alioth::global_definitions() {
    return {
        class_definition(),
        alias_definition(),
        enum_definition()
    };
}

$patterns Alioth::inner_definitions() {
    return {
        method_definition(),
        operator_definition()
    };
}

$patterns Alioth::implementations() {
    return {
        method_implementation(),
        operator_implementation()
    };
}

$pattern Alioth::basic_type() {
    return match(
        cat(
            ops(
                opt(
                    raw("u")),
                raw("int"),
                ops(
                    raw("8"),
                    raw("16"),
                    raw("32"),
                    raw("64")),
                cat(
                    raw("float"),
                    ops(
                        raw("32"),
                        raw("64"))),
                raw("bool"),
                raw("void")),
            raw("\\b")))->name(&n::storage.type);
}

$pattern Alioth::class_definition() {
    return range()
        ->begin(
            sat(
                raw("class")->name(&n::storage.type),
                space(),
                any(
                    sst(
                        ops(
                            raw("abstract"),
                            raw("public"),
                            raw("protected"),
                            raw("private"),
                            raw("\\+"),
                            raw("\\-"),
                            raw("\\*"))))->name(&n::storage.modifier),
                label()->name(&n::entity.name.type)))
        ->end(bpo(raw("\\}")))
        ->name(&n::meta)
        ->patterns({
            match(
                ops(
                    raw("\\<"),
                    raw("\\>"),
                    raw("\\["),
                    raw("\\]"),
                    raw("\\:"),
                    raw("\\="),
                    raw("\\!"),
                    raw("and"),
                    raw(",")))->name(&n::keyword.operato),
            match(
                ops(
                    raw("int8"),
                    raw("int16"),
                    raw("int32"),
                    raw("int64"),
                    raw("uint8"),
                    raw("uint16"),
                    raw("uint32"),
                    raw("uint64"),
                    raw("float32"),
                    raw("float64"),
                    raw("bool"),
                    raw("void")))->name(&n::storage.type),
            match(
                ops(
                    raw("obj"),
                    raw("ptr"),
                    raw("ref"),
                    raw("rel")))->name(&n::support.type),
            match(
                label())->name(&n::entity.name.section),
            range()
                ->begin(
                    raw("\\{")->name(&n::keyword.other))
                ->end(
                    raw("\\}")->name(&n::keyword.other))
                ->patterns({
                    include("#global-definitions"),
                    include("#inner-definitions")
                })
        });
}

$pattern Alioth::alias_definition() {
    return match(raw("let"))->name(&n::storage.type);
}

$pattern Alioth::enum_definition() {
    return match(raw("enum"))->name(&n::storage.type);
}

$pattern Alioth::method_definition() {
    return match(raw("method"))->name(&n::storage.type);
}

$pattern Alioth::operator_definition() {
    return match(raw("operator"))->name(&n::storage.type);
}

$pattern Alioth::method_implementation() {
    return match(raw("method"))->name(&n::storage.type);
}

$pattern Alioth::operator_implementation() {
    return match(raw("operator"))->name(&n::storage.type);
}


#endif