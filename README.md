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
import "system";

fn do_print(content: string, args: ...unknown) {
    system::printf(content, args);
}

fn main(): void {

    var a: i32 = 10;
    var b: f32 = 3.1, c: f32 = 4.1;

    var k: f32 = a = a * b + c;

    do_print("Calculated value for k: %d", k);

}
```

In this example, we import the system module, define a function that prints
a formatted string to the console, and then we define the main function that
declares some variables and calculates a value for `k`.

The code shown above is a simple example of how the language syntax is going to
be. The language is still in development, so there are no guarantees that the
syntax will remain the same.