# Kyanite
This is an overview language specification for Kyanite.

Note: this is not the main [README.md](README.md) of the repository. See there for information relating to the codebase itself.

## Syntax
The syntax is described in-depth in [SYNTAX.md](SYNTAX.md).

## Specification
* Dynamically and strongly typed
    * Only values have a type and variables can store any value.
    * Function parameters and object variables function like other variables, having no explicit type.
    * Types can, however, be checked at runtime if necessary (using `is` syntax).
    * Types are restricted to int, float, bool, string, list and object.
* Light-weight in implementation
* Simple and intuitive to write in