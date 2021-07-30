# Snag
A small (single-header, header-only) C++ unit testing library. Snag is the culmination of my attempts to understand how the Catch2 unit testing library works and has ended up being a sort-of Catch2-lite.

## Features
Snag is designed to be lightweight and minimalistic so it really only supports the core functionality that's needed to be useful: the `UNIT_TEST` macro is used to declare a new unit test and the `REQUIRE` macro checks whether an expression is true and counts the test as a failure if it's not.

Snag always provides a `main` function for the unit test executable (something that's configurable in Catch2).

Simple sample code showing how this works is provided in the arithmetic example.

## Future work
I don't want to bloat the code too much as this library is really for didactic purposes, but there are a few features that I would like to implement:
* Statistics showing overall test pass/failure
* Nicer output (maybe some colourisation of the text)
