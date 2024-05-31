# Design Specifications

## Optional Features
Optional features have to always be 'opt-in' by the user, unless it matches these specifications:
- The feature does not change overall functionality of the program
- The feature is a quality of life improvement
- The feature does not introduce additional security risks
- The feature does not have any significant performance impact (IO operations are acceptable)
- The feature does not require additional dependencies
- The feature does not introduce additional side effects (e.g. changes to configurations, variables, etc.)

## Exception Handling
- All exceptions not manually triggered by our code should be caught and logged (e.g. IO errors)
- Avoid exceptions unless for critical errors (use the `NebulaCore::Result` type for non-critical errors)
- Any function that returns a `Result` type should be checked for errors
- Any function that returns a boolean should be checked for errors (e.g. C functions), otherwise explcitly ignored using `NEBULA_IGNORE_RETURN`
- All exceptions should be acknowledged, and not silent, except for the following exceptions:
  - Exceptions that are expected to be thrown (e.g. file not found)
  - Exceptions that are not critical to the program (e.g. a file not being found)
  - Exceptions that are not recoverable (e.g. out of memory)

## Entry and Exit Points
- There should only be one entrypoint to the program
- Program entry and exit points should be clearly defined, and logged (This is to make sure there are no silent crashes)
- Memory should be cleaned up before the program exits
- The main exit point should be thread-safe
- The program should exit gracefully, and not crash, unless for critical errors (e.g. out of memory)

## Memory Management
- When possible use smart pointers, unless for performance-critical code (Renderers, Physics, etc.)
- Raw pointers should be wrapped with a Nebula::Ptr type, which should provide zero-cost abstractions for raw pointers, and error checking in debug mode
- Use the stack when the size of the object is known at compile time
- Always free memory before the program exits
- Move semantics should be used when possible

## Object Oriented Programming
- Avoid inheritance unless necessary
- Prefer composition over inheritance
- Avoid function pointers unless necessary
- Add noexcept to functions that do not throw exceptions
- Use the `override` keyword when overriding functions
- Use the `final` keyword when a class should not be inherited from
- Always define:
  - Destructor
  - Copy Constructor
  - Move Constructor
  - Copy Assignment Operator
  - Move Assignment Operator
- Use the `delete` keyword when a function should not be used
- All functions should be const unless they modify the object (exception when wrapping external libraries)
- Use the `explicit` keyword when a constructor should not be used for implicit conversions
- All non trivial functions should be documented with Doxygen comments (`/** */`)

## Performance
- Avoid dynamic memory allocation when possible
- Use the stack when the size of the object is known at compile time
- Use move semantics when possible
- Use the `inline` keyword when a function should be inlined
- Use the `constexpr` keyword when a function should be evaluated at compile time
- Use the `noexcept` keyword when a function should not throw exceptions
- Use `NEBULA_ALWAYS_INLINE` for functions that should always be inlined
- Use `[[likely]]` and `[[unlikely]]` for branch prediction
- Always add the profiling macros to functions that are performance-critical (`NEBULA_PROFILE_FUNCTION`, `NEBULA_PROFILE_SCOPE')

## Code Style
- Use the `clang-format` tool to format the code
- Use the `clang-tidy` tool to check for errors
- Always build with `-Wall -Wextra -Werror -Wpedantic`

## STL containers
- All STL containers that allow a custom allocator must have a explicit allocator (`Nebula::Allocator`)

---

By following these guidelines, we can ensure that the codebase is maintainable, and easy to work with. This will also help us avoid common pitfalls, and make the codebase more robust.

