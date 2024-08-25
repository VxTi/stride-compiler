# Stride Language Compiler

---

A compiler for the Stride programming language.

This project is dedicated to a language that I've created.
The main purpose of this project is to learn more about compilers and the behind
the scenes of programming languages.

Currently, the compiler is still a work in progress, so there are no binaries
to be generated yet.

A syntax example of the language can be found below:

```
const var externally_invisible: string = "I'm hidden";

define test(p1: i8, p2: i32, p3: string[]) {}

/*
 * Functions have return type i32 and return 0 by default,
 * if no type or return value is provided.
 *
 * Classes, modules or variables are shared through the 'shared' keyword.
 * This allows objects to be exposed to other modules.
 * Below is a representation of how one can write code in this language.
 */

shared module::name {

    // Reference external (C) library function
    declare shared external shared_function();

    declare f3(const param: i8) {}

    declare fname(varname: i8) {
        return 0;
    }

    declare ts(variadic: ...string) {}

    // Enumerables
    enum ename {
        MEMBER_0 = 0,
        MEMBER_1 = 1;
    }
}

// Single function sharing
declare shared external fname();

// publicly shared class
shared class cname {
    declare main() {
    
       // Automatically gets transformed to 3 by syntax analysis
       var len: i8 = [0, 1, 2].length;
    
       for ( var i: i32 = 0; i < 10; i++) {
    
       }
    
       return 0;
    }
}

```

The code shown above is a simple example of how the language syntax is going to
be. The language is still in development, so there are no guarantees that the
syntax will remain the same.