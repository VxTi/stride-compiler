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
import "external library";

define a_function() {
    call_another::function();
}

module Module {
    class Test<T> {
        define async public fn(file: io::file, socket: io::socket, timeout_ms: i32) {
            b::listen();
        }
    }
}

public class First<K, L> {
    const p: K;
    const l: L;
}

let test: auto = 5 + symbol + fn::call(3, 5) + ((3 + 5) ? 4 : 0);
let g: auto = fn::call(test, 0) + 2;

                            // subset in function call
let k: type::something = test(null, 2, mod::test());;

// Multiple variable declaration in one line
let g: char = c,
    h: i32 = 5,
    j: i32 = 5;

// If several variables are declared with 'const',
// all of them must be initialized and will be constant.
const const_g: i32 = 5,
      const_h: i32 = 5,
      const_j: i32 = 5;

module Test {
    class TestClass {
        // Something
    }
}

struct genericsTest<T, G, B> {
    val: T;
    const ref: Test::TestClass;
}

struct Vector<T> {
    x: T;
    y: T;
    z: T;
}

class Third has Second and Test {
    const p: auto = super;
}

// Inherits p and l fields
class Second<G, B> has First<i8, i32> and Test<P> {

    const p2: auto;

}

module outer::inner {

    /** Externally defined C function. */
    define external test();

    define for_test(upper: i8) {
        for (let i: i8 = 0; i < upper; i++) {
            system::println("Hello, World!");
        }

        throw Error("oops");
    }

    define main(args: string[]) {
        system::println("Hello, World!");
    }

    define func(test: string, test2: ...string) {

        let k: i8 = 0;

        // Print all variadic arguments
        for ( const auto str in test2 ) {
            system::println(str);
        }

        do {
            system::println(test);
        } while (++x < 10);

        try {
            throw Error("oops");
        } catch (e: Error) {
            system::println("Caught!");
        }

        if (kek) {
            try {
                let content: string = io::read_file("/file/path");
                let socket: socket = io::socket::create();
            }
            catch (err: Error) {
                system::println("Something went wrong");
            }
        }

        let k: i8 = 0;

        let con: socket::connection = socket::create("localhost", 8080);

        do {
            socket::listen();
        } while (!con::connected());
     }
}

```
The code shown above is a simple example of how the language syntax is going to
be. The language is still in development, so there are no guarantees that the
syntax will remain the same.

Currently all this software does is generate an AST from the input code.

Code generation is planned, but not yet implemented.

If anyone gets this far, and feels like they would want to see a specific implementation,
or have it work on the JVM, feel free to open an issue or a pull request.