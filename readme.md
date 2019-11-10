---
title: gentml
author: GodGnidoc
date: 2019/11/7
---

# GentmL

The project `GentmL` means generate `tmLanguage` configure file. The major purpose of this project is to generate `json` formated `tmLanguage` configuration file for the vscode `IDE`. It makes you write the regular expressions more structural, so that you can work on more complicated syntaxs.

## Usage

The class `Tml` in the namespace `gentml` represents a tmLanguage configure file. Inherit it or just use it directly is fine. Fill fields you need, and then convert it into `json`, the overrided operator can help you with this.

This project is developed for the `Alioth` project, and the `jsonz` and `chainz` module are all parts come from the `Alioth` project. I reserved all source documents involved when generating the syntax configure file for the Alioth programming language to be the example shows the way using this project.

There are several concepts you maybe need to know.

### Pattern

There are three kinds of patterns `match`, `range` and `include` pattern.

All these three kinds of patterns inherits from the base class `pattern`, the type `$pattern` can be used to pointer to them, and there's no need to worry about memory space releasing.

The patterns are all defined in the namespace `gentml::p`, use helper functions defined in namespace `genteml` to create patterns and abtain pointers pointing to them.

- `gentml::match(...)`
  
  Used to generate a pattern with field `match`.

- `gentml::range(...)`

  Used to generate a pattern with field `begin` and field `end`.

- `gentml::include(...)`
  
  Used to generate a pattern with field `include`.

Member functions can be used to modify other fields of patterns, and all those member functions returns `this` pointer, so that you can modify more than one fields in single expression.

### Regex

There are several classes inherits the class `regex`. Which are defined in the namespace `r`, they are used to make it easier to write regular expressions.

- `gentml::r::raw`
  
  Used to represent the raw regular expression.
- `gentml::r::opt`
  
  Used to represent a section of regular expression matchs an optional unit.
- `gentml::r::ops`
  
  Used to represent a section of regular expression matchs one or more options.
- `gentml::r::any`
  
  Use the suffix `*` to make the unit can be matched for any times.
- `gentml::r::one`
  
  Use the suffix `+` to make the unit can be matched for one or more times.
- `gentml::r::apo`

  Uncaptured group, look ahead positive assert: `(?=xxxx)`.
- `gentml::r::ane`

  Uncaptured group, look ahead negative assert: `(?!xxxx)`.
- `gentml::r::bpo`

  Uncaptured group, look back positive assert: `(?<=xxxx)`.
- `gentml::r::bne`

  Uncaptured group, look back negative assert: `(?<!xxxx)`.
- `gentml::r::cat`
  
  Used to link regular expressions together.

Also, there corresponding helper functions defined in the namespace `gentml`, which helps generate the regular expression and abtain the pointer.

The member function `name` can be used to set the `capture name` of this unit. If the `capture name` is set, the corresponding section in `captures` field of the owner pattern of this regular expression will be filled automatically.

According to the kind of the owner pattern of the regular expression, the corresponding `captures` section may be varies within `captures`, `beginCaptures` and `endCaptures`.

### Name conversion

The name conversion for `tmLanguage` is welly defined in the namespace `gentml::n`. They are designed to be single-instance classes, so that they can be used as object and namespace at the same time.