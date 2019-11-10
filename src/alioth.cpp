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
    return one(ops("\\w","_"));
}

$regex Alioth::space() {
    return one("\\s");
}

$regex Alioth::tag() {
    return one(ops("\\w","-","_"));
}

$patterns Alioth::comments() {
    return {
        match("//.*")
            ->name(&n::comment.line.double_slash),
        range()
            ->begin("/\\*\\*doc")
            ->end("end\\*\\*/")
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
            ->patterns({
                include("text.html.markdown")
            }),
        range()
            ->begin("/\\*\\*")
            ->end("\\*/")
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
            ->patterns({
                match(
                    cat(
                        raw("@")->name(&n::keyword.other),
                        tag()->name(&n::storage.type),
                        space(),
                        cat(
                            ".*",
                            apo(raw(": ")))->name(&n::entity.name.function),
                        ":"))}),
        range()
            ->begin("/\\*")
            ->end("\\*/")
            ->name(&n::comment.block)
            ->contentName(&n::comment.block.documentation)
    };
}

$patterns Alioth::signature() {
    return {
        range()
            ->begin(
                sat(
                    bnd("module")->name(&n::keyword),
                    label()->name(&n::entity.name),
                    opt(
                        sst(
                            bnd("entry")->name(&n::keyword),
                            label()->name(&n::entity.name.tag))),
                    opt(
                        raw(":")->name(&n::keyword))))
            ->end(
                apo(
                    ops(
                        "class",
                        "method",
                        "operator",
                        "enum",
                        "let",
                        ";")))
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
                    sat("this","module")->name(&n::constant.language),
                    label()->name(&n::entity.name)))),
        match(
            sat(
                raw("@")->name(&n::keyword),
                ops(
                    label(),
                    R"(\"[^\"]+\")",
                    R"(\'[^\']+\')")
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

$pattern Alioth::class_definition() {
    return range()
        ->begin(
            sat(
                bnd("class")->name(&n::storage.type),
                any(
                    sst(
                        ops(
                            "abstract",
                            visibilities())))->name(&n::storage.modifier),
                label()->name(&n::entity.name.type)))
        ->end(bpo("\\}"))
        ->name(&n::meta)
        ->patterns({
            match(scopes())->name(&n::constant.character.escape),
            match(operators())->name(&n::keyword.operato),
            match(dtypes())->name(&n::storage.type),
            match(etypes())->name(&n::support.type),
            match(label())->name(&n::entity.name.section),
            range()
                ->begin(
                    raw("\\{")->name(&n::keyword.control))
                ->end(
                    raw("\\}")->name(&n::keyword.control))
                ->patterns({
                    include("#global-definitions"),
                    include("#inner-definitions")
                })
        });
}

$pattern Alioth::alias_definition() {
    return range()
        ->begin(
            sat(
                bnd("let")->name(&n::storage.type),
                visibilities()->name(&n::storage.modifier),
                label()->name(&n::support.clas),
                raw("=")->name(&n::keyword.operato)))
        ->end(
            apo("class|let|enum|method|operator|var|obj|ptr|ref|rel|\\}|\\d"))
        ->patterns({
            match(scopes())->name(&n::constant.character.escape),
            match(operators())->name(&n::keyword.operato),
            match(dtypes())->name(&n::storage.type),
            match(etypes())->name(&n::support.type),
            match(label())->name(&n::support.clas)
        });
}

$pattern Alioth::enum_definition() {
    // return match(raw("enum"))->name(&n::storage.type);
    return range()
        ->begin(bnd("enum")->name(&n::storage.type))
        ->end(bpo("\\}"));
}

$pattern Alioth::method_definition() {
    return match(bnd("method"))->name(&n::storage.type);
}

$pattern Alioth::operator_definition() {
    return match(bnd("operator"))->name(&n::storage.type);
}

$pattern Alioth::method_implementation() {
    return match(bnd("method"))->name(&n::storage.type);
}

$pattern Alioth::operator_implementation() {
    return match(bnd("operator"))->name(&n::storage.type);
}

$regex Alioth::operators() {
    return ops(
        "\\~",
        "\\!",
        "\\@",
        "\\#",
        "\\$",
        "\\%",
        "\\^",
        "\\&",
        "\\*",
        "\\-",
        "\\+",
        "\\=",
        "\\|",
        "\\<",
        "\\>",
        "\\?",
        "\\/",
        bnd("and"),
        bnd("or"),
        bnd("not"),
        bnd("xor")
    );
}

$regex Alioth::dtypes() {
    return ops(
        ops(
            bnd("int8"),
            bnd("int16"),
            bnd("int32"),
            bnd("int64"),
            bnd("uint8"),
            bnd("uint16"),
            bnd("uint32"),
            bnd("uint64"),
            bnd("float32"),
            bnd("float64"),
            bnd("bool"),
            bnd("void"))
    );
}

$regex Alioth::etypes() {
    return ops(
        bnd("var"),
        bnd("obj"),
        bnd("ptr"),
        bnd("ref"),
        bnd("rel")
    );
}

$regex Alioth::visibilities() {
    return ops(
        bnd("public"),
        bnd("private"),
        bnd("protected"),
        "\\+",
        "\\-",
        "\\*"
    );
}

$regex Alioth::scopes() {
    return ops(
        "\\-\\>",
        "\\.",
        "\\:\\:",
        "\\(",
        "\\)",
        "\\[",
        "\\]",
        "\\{",
        "\\}",
        "\\:",
        "\\;",
        "\\,"
    );
}

$regex Alioth::bnd($regex expr) {
    return cat(
        expr,
        apo("\\b")
    );
}
$regex Alioth::bnd(const std::string& s) {
    return bnd(raw(s));
}

#endif