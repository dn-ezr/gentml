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
        include("#definitions"),
        include("#implementations")
    };

    repository = {
        {"comments",comments()},
        {"signature",signature()},
        {"dependencies",dependencies()},
        {"definitions",definitions()},
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
        match(
            cat(
                raw("//")->name(&n::punctuation.definition.comment),
                ".*"))->name(&n::comment.line.double_slash),
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
                    bnd("module")->name(&n::punctuation.definition.keyword),
                    label()->name(&n::entity.name.namespac),
                    opt(
                        sst(
                            bnd("entry")->name(&n::keyword.control),
                            label()->name(&n::entity.name.function))),
                    opt(
                        raw(":")->name(&n::keyword.control.import))))
            ->end(apo(ops(def(),";")))
            ->patterns({
                include("#comments"),
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
                    label()->name(&n::entity.name.namespac)))),
        match(
            sat(
                raw("@")->name(&n::punctuation.accessor),
                ops(
                    label(),
                    R"(\"[^\"]+\")",
                    R"(\'[^\']+\')")
            ->name(&n::string.interpolated))),
        match(label()->name(&n::entity.name.namespac))
    };
}

$patterns Alioth::definitions() {
    return {
        class_definition(),
        operator_definition(),
        delete_operator(),
        alias_definition(),
        enum_definition(),
        attr_definition(),
        method_definition(),
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
                bnd("class")->name(&n::punctuation.definition.generic),
                any(
                    sst(
                        ops(
                            "abstract",
                            visibilities())))->name(&n::storage.modifier),
                ops(
                    label()->name(&n::entity.name.clas))))
        ->end(bpo("\\}"))
        ->name(&n::meta)
        ->patterns({
            include("#comments"),
            match(scopes()),
            match(operators()),
            match(dtypes())->name(&n::storage.type),
            match(etypes())->name(&n::support.type),
            match(label())->name(&n::entity.name.section),
            range()
                ->begin(
                    raw("\\{")->name(&n::punctuation.section.block.begin))
                ->end(
                    raw("\\}")->name(&n::punctuation.section.block.end))
                ->patterns({
                    include("#comments"),
                    include("#definitions"),
                    match(etypes())->name(&n::support.type),
                    match(dtypes())->name(&n::storage.type),
                    match(scopes()),
                    match(operators())})
                ->patterns(
                    constants())
                ->patterns({
                    match(label())->name(&n::entity.name.clas)})
        });
}

$pattern Alioth::attr_definition() {
    return match(
        sat(
            //bne("\\)\\s*"),
            etypes()->name(&n::punctuation.definition.variable),
            opt(
                ops(
                    bnd("meta"),
                    bnd("const"),
                    visibilities())->name(&n::storage.modifier)),
            label()->name(&n::variable.other.member)));
}

$pattern Alioth::alias_definition() {
    return range()
        ->begin(
            sat(
                bnd("let")->name(&n::punctuation.definition.generic),
                opt(visibilities()->name(&n::storage.modifier)),
                label()->name(&n::entity.name.clas),
                raw("=")->name(&n::keyword.operato)))
        ->end(
            apo(ops(def(),"\\}")))
        ->patterns({
            include("#comments"),
            match(scopes()),
            match(operators()),
            match(dtypes())->name(&n::storage.type),
            match(etypes())->name(&n::support.type),
            match(label())->name(&n::support.clas)
        });
}

$pattern Alioth::enum_definition() {
    return range()
        ->begin(
            sat(
                bnd("enum")->name(&n::punctuation.definition.generic),
                opt(visibilities()->name(&n::storage.modifier)),
                label()->name(&n::entity.name.enu),
                raw("\\{")->name(&n::punctuation.section.block.begin)))
        ->end(raw("\\}")->name(&n::punctuation.section.block.end))
        ->patterns({
            include("#comments"),
            match(label())->name(&n::variable.annotation)
        });
}

$pattern Alioth::method_definition() {
    return range()
        ->begin(
            sat(
                bnd("method")->name(&n::punctuation.definition.generic),
                any(
                    sat(
                        ops(
                            visibilities(),
                            bnd("const"),
                            bnd("atomic"),
                            bnd("meta"))))->name(&n::storage.modifier),
                label()->name(&n::entity.name.function),
                raw("\\(")->name(&n::punctuation.section.parens.begin)))
        ->end(raw("\\)")->name(&n::punctuation.section.parens.end))
        ->patterns(
            parameter_list());
}

$pattern Alioth::delete_operator() {
    return match(
        sat(
            sst("delete", "default", "operator")->name(&n::punctuation.definition.keyword),
            ops("{...}","sctor","mctor","cctor")->name(&n::entity.name.function)));
}

$pattern Alioth::operator_definition() {
    return range()
        ->begin(
            sat(
                bnd("operator")->name(&n::punctuation.definition.generic),
                opt(
                    ops(
                        bnd("default"),
                        bnd("const"),
                        bnd("prefix"),
                        bnd("suffix"),
                        bnd("ism"),
                        bnd("rev")))->name(&n::storage.modifier),
                oplabels()->name(&n::entity.name.function),
                raw("\\(")->name(&n::punctuation.section.parens.begin)))
        ->end(
            raw("\\)")->name(&n::punctuation.section.parens.end))
        ->patterns(
            parameter_list());
}

$pattern Alioth::method_implementation() {
    return range()
        ->begin(apo("method"))
        ->end(raw("\\)")->name(&n::punctuation.section.parens.end))
        ->patterns({
            range()
                ->begin(bnd("method")->name(&n::punctuation.definition.generic))
                ->end(raw("\\(")->name(&n::punctuation.section.parens.begin))
                ->patterns({
                    match(
                        ops(
                            bnd("const"),
                            bnd("atomic"),
                            bnd("meta")))->name(&n::storage.modifier),
                    range()
                        ->begin(raw("\\(")->name(&n::storage.type))
                        ->end(raw("\\)")->name(&n::storage.type))
                        ->patterns({
                            include("$self"),
                            match(dtypes())->name(&n::storage.type),
                            match("\\*|\\^|\\(|\\<|\\>|\\,\\:\\:|\\=\\>")->name(&n::punctuation.accessor),
                            match(label())->name(&n::entity.name.clas)
                        }),
                    range()
                        ->begin(raw("\\<")->name(&n::punctuation.section.group.begin))
                        ->end(raw("\\>")->name(&n::punctuation.section.group.end))
                        ->patterns({
                            include("$self"),
                            match(dtypes())->name(&n::storage.type),
                            match("\\*|\\^|\\<|\\(|\\)|\\,\\::|\\=\\>")->name(&n::punctuation.accessor),
                            match(label())->name(&n::entity.name.clas)
                        }),
                    match(
                        sat(
                            label()->name(&n::entity.name.clas),
                            raw("\\:\\:")->name(&n::punctuation.accessor)
                            )),
                    match(">")->name(&n::punctuation.section.group.end),
                    match(label()->name(&n::entity.name.function))
                })
        })
        ->patterns(
            parameter_list());
}

$pattern Alioth::operator_implementation() {
    return range()
        ->begin(
            sat(
                bnd("operator")->name(&n::punctuation.definition.generic),
                opt(
                    ops(
                        bnd("const"),
                        bnd("prefix"),
                        bnd("suffix"),
                        bnd("ism"),
                        bnd("rev")))->name(&n::storage.modifier),
                oplabels()->name(&n::entity.name.function),
                raw("\\(")->name(&n::punctuation.section.parens.begin)))
        ->end(
            raw("\\)")->name(&n::punctuation.section.parens.end))
        ->patterns(parameter_list());
}

$patterns Alioth::parameter_list() {
    return {
        range()
            ->begin(raw("\\(")->name(&n::storage.type))
            ->end(raw("\\)")->name(&n::storage.type))
            ->patterns({
                include("$self"),
                match(dtypes())->name(&n::storage.type),
                match("\\*|\\^|\\(|\\<|\\>|\\,\\:\\:|\\=\\>")->name(&n::punctuation.accessor),
                match(label())->name(&n::entity.name.clas)
            }),
        range()
            ->begin(raw("\\<")->name(&n::punctuation.section.group.begin))
            ->end(raw("\\>")->name(&n::punctuation.section.group.end))
            ->patterns({
                include("$self"),
                match(dtypes())->name(&n::storage.type),
                match("\\*|\\^|\\<|\\(|\\)|\\,\\::|\\=\\>")->name(&n::punctuation.accessor),
                match(label())->name(&n::entity.name.clas)
            }),
        match(
            sat(
                raw("\\:\\:")->name(&n::punctuation.accessor),
                label()->name(&n::entity.name.clas))),
        match(">")->name(&n::punctuation.section.group.end),
        match(
            sat(
                opt(
                    raw("\\,")->name(&n::punctuation.separator)),
                opt(
                    ops(
                        bnd("const")->name(&n::storage.modifier),
                        etypes()->name(&n::support.type))),
                label()->name(&n::variable.parameter),
                any(
                    ops(
                        "\\*",
                        "\\^"))->name(&n::storage.modifier),
                opt(
                    ops(
                        dtypes()->name(&n::storage.type),
                        label()->name(&n::entity.name.clas)
                    )))),
        match(scopes()),
        match(operators())
    };
}

$regex Alioth::operators() {
    return ops(
        raw("\\=\\>")->name(&n::punctuation.definition.annotation),
        cat(
            opt(
                ops(
                    "\\^"
                    "\\&"
                    "\\*"
                    "\\-"
                    "\\+"
                    "\\/"
                    "\\|"
                    "\\<\\<"
                    "\\>\\>")),
            "=")->name(&n::keyword.operato.assignment),
        ops(
            "\\~",
            "\\&",
            "\\|"
            "\\^",
            "\\<\\<",
            "\\>\\>")->name(&n::keyword.operato.bitwise),
        ops(
            "\\!\\=",
            "\\=\\=",
            "\\>\\=",
            "\\<\\=",
            "\\>",
            "\\+",
            "\\-",
            "\\*",
            "\\/",
            "\\%",
            "\\<")->name(&n::keyword.operato.arithmetic),
        raw("\\!")->name(&n::keyword.control.conditional),
        raw("\\@")->name(&n::punctuation.accessor),
        raw("\\#")->name(&n::punctuation.accessor),
        raw("\\$")->name(&n::punctuation.definition.variable),
        raw("\\&")->name(&n::punctuation.accessor),
        raw("\\?")->name(&n::invalid.illegal),
        ops(
            bnd("and"),
            bnd("or"),
            bnd("not"),
            bnd("xor"))->name(&n::keyword.operato.word)
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
        raw("\\-\\>")->name(&n::punctuation.accessor),
        raw("\\.")->name(&n::punctuation.accessor),
        raw("\\:\\:")->name(&n::punctuation.accessor),
        raw("\\(")->name(&n::punctuation.section.parens.begin),
        raw("\\)")->name(&n::punctuation.section.parens.end),
        raw("\\[")->name(&n::punctuation.section.brackets.begin),
        raw("\\]")->name(&n::punctuation.section.brackets.end),
        // raw("\\{")->name(&n::punctuation.section.block.begin),
        // raw("\\}")->name(&n::punctuation.section.block.end),
        raw("\\:")->name(&n::punctuation.separator),
        raw("\\;")->name(&n::punctuation.separator),
        raw("\\,")->name(&n::punctuation.separator)
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

$regex Alioth::def() {
    return ops(
        bnd("class"),
        bnd("let"),
        bnd("enum"),
        bnd("method"),
        bnd("operator"),
        bnd("var"),
        bnd("obj"),
        bnd("ptr"),
        bnd("ref"),
        bnd("rel")
    );
}

$regex Alioth::oplabels() {
    return ops(
        "\\+",
        "\\-",
        "\\*",
        "\\/",
        "\\%",
        "\\&",
        "\\|",
        "\\^",
        "\\<\\<",
        "\\>\\>",
        "\\=\\=",
        "\\!\\=",
        "\\<\\=",
        "\\>\\=",
        "\\>",
        "\\<",
        "and",
        "or",
        "not",
        "xor",
        "\\=",
        "\\+\\=",
        "\\-\\=",
        "\\*\\=",
        "\\/\\=",
        "\\%\\=",
        "\\&\\=",
        "\\|\\=",
        "\\^\\=",
        "\\<\\<\\=",
        "\\>\\>\\=",
        "\\[\\]",
        "increment",
        "decrement",
        "negative",
        "add",
        "sub",
        "mul",
        "div",
        "mol",
        "bitand",
        "bitor",
        "bitxor",
        "bitnot",
        "shr",
        "shl",
        "assign",
        "eq",
        "ne",
        "le",
        "ge",
        "lt",
        "gt",
        "aspect",
        "member",
        "move",
        "sctor",
        "lctor",
        "mctor",
        "cctor",
        "dtor",
        "\\{\\.\\.\\.\\}",
        "\\[\\.\\.\\.\\]",
        "\\{\\~\\}"
    );
}

$patterns Alioth::constants() {
    return {
        match(
            ops(
                bnd("true"),
                bnd("false"),
                bnd("null")))->name(&n::constant.language),
        match(
            bnd("this"))->name(&n::variable.language),
        match(
            cat(
                one("[0-9]"),
                cat("\\.",one("[0-9]")),
                cat("e",opt("\\-"),one("[0-9]"))))->name(&n::constant.numberic.floa.decimal),
        match(
            bnd(
                one("[0-9]")))->name(&n::constant.numberic.integer.decimal),
        match(
            bnd(
                cat(
                    "0b",one("[01]"))))->name(&n::constant.numberic.integer.binary),
        match(
            bnd(
                cat(
                    "0o",one("[01234567]"))))->name(&n::constant.numberic.integer.octal),
        match(
            bnd(
                cat(
                    "0x",one("[0-9a-fA-F]"))))->name(&n::constant.numberic.integer.hexadecimal),
        range()
            ->begin(raw("\"")->name(&n::punctuation.definition.string))
            ->end(raw("\"")->name(&n::punctuation.definition.string))
            ->patterns({
                match("\\\\(x\\h\\h|.)")->name(&n::constant.character.escape)
            }),
        range()
            ->begin(raw("\'")->name(&n::punctuation.definition.string))
            ->end(raw("\'")->name(&n::punctuation.definition.string))
            ->patterns({
                match("\\\\(x\\h\\h|.)")->name(&n::constant.character.escape)
            })
    };
}

#endif