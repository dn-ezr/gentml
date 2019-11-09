#ifndef __name__
#define __name__

#include <string>

#define def_name( p, name, args... ) struct : public cname_t {\
    operator std::string()const override{\
        auto parent = std::string(p);\
        if( parent.empty() )\
            return #name;\
        else\
            return parent + "." + #name;\
    }\
    args\
}const name;
#define def_name_s( p, str, name, args... ) struct : public cname_t {\
    operator std::string()const override{\
        auto parent = std::string(p);\
        if( parent.empty() )\
            return str;\
        else\
            return parent + "." + str;\
    }\
    args\
}const name;

namespace gentml {
struct cname_t {virtual operator std::string()const = 0;};
using cname = const cname_t*;
}

namespace gentml::n {

/**
 * for comments. */
def_name( "", comment,
    def_name( "comment", block, // multi-line comments like /* … */ and <!-- … -->.
        def_name( "comment.block", documentation)) // embedded documentation.
    def_name( "comment", line, // line comments, we specialize further so that the type of comment start character(s) can be extracted from the scope.
        def_name_s( "comment.line", "double-slash",double_slash) // // comment
        def_name_s( "comment.line", "double-dash",double_dash) // -- comment
        def_name_s( "comment.line", "number-sign",number_sign) // # comment
        def_name( "comment.line", percentage) // % comment
        def_name( "comment.line", character ))) // other types of line comments.

/**
 * various forms of constants.*/
def_name( "", constant,
    def_name( "constant", numberic) // those which represent numbers, e.g. 42, 1.3f, 0x4AB1U.
    def_name( "constant", character, // those which represent characters, e.g. &lt;, \e, \031.
        def_name( "constant.character", escape)) // escape sequences like \e would be constant.character.escape.
    def_name( "constant", language) // constants (generally) provided by the language which are “special” like true, false, nil, YES, NO, etc.
    def_name( "constant", other)) // other constants, e.g. colors in CSS.

/**
 * entity —— an entity refers to a larger part of the document
 * for example a chapter, class, function, or tag. 
 * We do not scope the entire entity as entity.* (we use meta.* for that). 
 * But we do use entity.* for the “placeholders” in the larger entity, e.g. if the entity is a chapter, 
 * we would use entity.name.section for the chapter title. */
def_name( "", entity, 
    def_name( "entity", name, // we are naming the larger entity.
        def_name( "entity.name", function) // the name of a function.
        def_name( "entity.name", type) // the name of a type declaration or class.
        def_name( "entity.name", tag) // a tag name.
        def_name( "entity.name", section)) // the name is the name of a section/heading.
    def_name( "entity", other, // other entities.
        def_name_s( "entity.other", "inherited-class", inherited_class )// the superclass/baseclass name.
        def_name_s( "entity.other", "attribute-name", attribute_name )) // the name of an attribute (mainly in tags).
)

/**
 * stuff which is “invalid”. */
def_name( "", invalid,
    def_name( "invalid", illegal) // illegal, e.g. an ampersand or lower-than character in HTML (which is not part of an entity/tag).
    def_name( "invalid", deprecated)) // for deprecated stuff e.g. using an API function which is deprecated or using styling with strict HTML.

/**
 * keywords (when these do not fall into the other groups). */
def_name( "", keyword,
    def_name( "keyword", control) // mainly related to flow control like continue, while, return, etc.
    def_name_s( "keyword", "operator", operato) // operators can either be textual (e.g. or) or be characters.
    def_name( "keyword", other)) // other keywords.

/**
 *  this is for markup languages and generally applies to larger subsets of the text. */
def_name( "", markup,
    def_name( "markup", underline, // underlined text.
        def_name( "markup.underline", link)) // this is for links, as a convenience this is derived from markup.underline so that if there is no theme rule which specifically targets markup.underline.link then it will inherit the underline style.
    def_name( "markup", bold) // bold text (text which is strong and similar should preferably be derived from this name).
    def_name( "markup", heading) // a section header. Optionally provide the heading level as the next element, for example markup.heading.2.html for <h2>…</h2> in HTML.
    def_name( "markup", italic) // italic text (text which is emphasized and similar should preferably be derived from this name).
    def_name( "markup", list, // list items.
        def_name( "markup.list", numbered) // numbered list items.
        def_name( "markup.list", unnumbered)) // unnumbered list items.
    def_name( "markup", quote) // quoted (sometimes block quoted) text.
    def_name( "markup", raw) // text which is verbatim, e.g. code listings. Normally spell checking is disabled for markup.raw.
    def_name( "markup", other))

/**
 * the meta scope is generally used to markup larger parts of the document. 
 * For example the entire line which declares a function would be meta.function and the subsets would be 
 * storage.type, entity.name.function, variable.parameter etc. and only the latter would be styled. 
 * Sometimes the meta part of the scope will be used only to limit the more general element that is styled, most of the time meta scopes are however used in scope selectors for activation of bundle items. 
 * For example in Objective-C there is a meta scope for the interface declaration of a class and the implementation, allowing the same tab-triggers to expand differently, depending on context. */
def_name( "", meta)

/**
 *  things relating to “storage”. */
def_name( "", storage,
    def_name( "storage", type) // the type of something, class, function, int, var, etc.
    def_name( "storage", modifier)) // a storage modifier like static, final, abstract, etc.

/**
 *  strings. */
def_name( "", string,
    def_name( "string", quoted, // quoted strings.
        def_name_s( "string.quoted", "double", doubl) // double quoted strings: "foo".
        def_name( "string.quoted", triple) // triple quoted strings: """Python""".
        def_name( "string.quoted", single) // single quoted strings: 'foo'.
        def_name( "string.quoted", other)) // other types of quoting: $'shell', %s{...}.
    def_name( "string", unquoted) // for things like here-docs and here-strings.
    def_name( "string", interpolated) // strings which are “evaluated”: `date`, $(pwd).
    def_name( "string", regexp) // regular expressions: /(\w+)/.
    def_name( "string", other)) // other types of strings (should rarely be used).

/**
 * things provided by a framework or library should be below support. */
def_name("", support,
    def_name( "support", function) // functions provided by the framework/library. For example NSLog in Objective-C is support.function.
    def_name_s( "support", "class", clas) // when the framework/library provides classes.
    def_name( "support", type) // types provided by the framework/library, this is probably only used for languages derived from C, which has typedef (and struct). Most other languages would introduce new types as classes.
    def_name( "support", constant) // constants (magic values) provided by the framework/library.
    def_name( "support", variable) // variables provided by the framework/library. For example NSApp in AppKit.
    def_name( "support", other)) // the above should be exhaustive, but for everything else use support.other.

/**
 *  variables. Not all languages allow easy identification (and thus markup) of these. */
def_name( "", variable,
    def_name( "variable", parameter) // when the variable is declared as the parameter.
    def_name( "variable", language) // reserved language variables like this, super, self, etc.
    def_name( "variable", other)) // other variables, like $some_variables.
}

#undef def_name
#undef def_name_s

#endif