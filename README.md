Finch is a simple bytecode interpreted, purely object-oriented,
prototype-based, dynamically-typed programming language. It's mostly
inspired by Smalltalk, Self, and Javascript.

It is written in C++ with a hand-written lexer and parser. It has
minimal dependencies. I want Finch to be:

*   Syntactically expressive yet minimal. Your code should look beautiful and
    do what you want.

*   An example of a small, clean C++ codebase for an interpreter. If you can
    read C++ and want to learn more about programming languages, I hope Finch's
    code will be a good primer.

*   A language in the Smalltalk family that's friendly to people coming from a
    text file and curly brace background.

*   A minimal prototype-based dynamic language. I think prototypes are a really
    cool paradigm, but Self goes too far, and Javascript doesn't go far enough.

*   Easily embeddable in other applications. I don't know if Finch ever will
    have real use, but if it does, it will likely be as a configuration or
    scripting language within a larger application, much like Lua.


A Taste of the Language
-----------------------

Here's a little example to get you going. This little program doesn't
draw, but it will tell you what turns to make to draw a dragon curve:

    // create an object and put it in a variable "dragon"
    dragon <- [
      // define a "trace:" method for outputting the series of left and
      // right turns needed to draw a dragon curve.
      trace: depth {
        self traceDepth: depth turn: "R"
        writeLine: "" // end the line
      }

      // the main recursive method
      traceDepth: n turn: turn {
        if: n > 0 then: {
          self traceDepth: n - 1 turn: "R"
          write: turn
          self traceDepth: n - 1 turn: "L"
        }
      }
    ]

    // now lets try it
    dragon trace: 5


Getting Started
---------------

Finch lives on github here: https://github.com/munificent/finch

To play around with it, sync it down. Finch uses GYP to generate projects or
makefiles for your platform, which you then build to get an executable.

1. Download GYP from: http://code.google.com/p/gyp/
2. Clone the finch repo from github.
3. In a terminal/command prompt, navigate to the root finch/ directory.
4. Run GYP on this file: `<path to gyp>/gyp --depth=1`
   Where `<path to gyp>` is wherever you downloaded GYP to in step 1.
   This should spit out a project/makefile in the root directory for your
   platform.
5. Open that project in XCode or VS and build, or build the makefile.
6. Ta-da! You should now have a Finch executable under a build/ directory.

Let me know if you run into any problems.


Running Finch
-------------

Once you've got it built and running, you'll be at the main
interpreter prompt. Finch is a command-line app. If you run it without
any arguments, it drops you into a REPL, an interactive session. You
can type in chunks of code and it will interpret them immediately. (If
you run it with a single argument, it expects that to be a path to a
.fin script, and it will load and run that script.)

Once you're in the REPL, you can load and execute a script using
load:. The path must be relative to where the executable is right now (lame!).
You can run the tests like this:

    >> load: "../../test/test.fin"


Where to Go from Here
---------------------

You should be good to start hacking some Finch code now. There is some
documentation here:

* http://finch.stuffwithstuff.com
* http://journal.stuffwithstuff.com/category/finch/

If you have any questions or comments, holler at me.