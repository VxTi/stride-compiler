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
const externally_invisible: string = "I'm hidden";

define async test(p1: i8, p2: i32, p3: string[]) {}

/*
 * Functions have return type i32 and return 0 by default,
 * if no type or return current is provided.
 *
 * Classes, modules or letiables are isPublic through the 'isPublic' keyword.
 * This allows objects to be exposed to other modules.
 * Below is a representation of how one can write code in this language.
 */

module module0::name {

    // Reference external (C) library function
    define public external shared_function();

    define f3(const param: i8) {
        return 0;
    }

    define fname(letname: i8) {
        return 0;
    }

    define ts(letiadic: ...string) {}

    // Enumerables
    enum ename {
        MEMBER_0 = 0,
        MEMBER_1 = 1;
    }
}

// Single function sharing
define public external fname();

// Structures
struct Structure<A, B> {
    coordX: A;
    coordY: B;
}

// publicly isPublic class
public class ClassName {
    define main() {
    
       // Automatically gets transformed to 3 by syntax analysis
       let len: i8 = [0, 1, 2].length;
    
       for ( let i: i32 = 0; i < 10; i++) {
    
       }
    
       return 0;
    }
}

public class Secondary {
    // Define something here
}

// Inheritance
public class InheritedClass has Secondary and ClassName {
    // Do something with the inheritance.
}

```

The code shown above is a simple example of how the language syntax is going to
be. The language is still in development, so there are no guarantees that the
syntax will remain the same.