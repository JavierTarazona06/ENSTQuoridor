# Creating Tests for ENSTQuoridor

This guide explains how to create new unit tests for the ENSTQuoridor project using the Catch2 testing framework.

## Prerequisites

- Catch2 is already integrated into the project via vcpkg
- All test files are located in the `tests/` directory
- Tests are compiled into a single executable: `test_all`

## Step 1: Create a New Test File

Create a new `.cpp` file in the `tests/` directory following the naming convention `test_<component>.cpp`.

**Example:** `tests/test_mycomponent.cpp`

```cpp
#include <catch2/catch_test_macros.hpp>
#include "model/MyComponent.hpp"

using namespace Quoridor;

TEST_CASE("Component initialization", "[mycomponent]") {
    MyComponent component;
    
    SECTION("Default values are set correctly") {
        REQUIRE(component.getValue() == 0);
        REQUIRE(component.isValid() == true);
    }
    
    SECTION("Component can be modified") {
        component.setValue(42);
        REQUIRE(component.getValue() == 42);
    }
}

TEST_CASE("Component validation", "[mycomponent][validation]") {
    MyComponent component;
    
    SECTION("Invalid input throws exception") {
        REQUIRE_THROWS_AS(component.setValue(-1), std::invalid_argument);
    }
    
    SECTION("Valid input is accepted") {
        REQUIRE_NOTHROW(component.setValue(100));
    }
}
```

## Step 2: Add Test File to CMakeLists.txt

Open `tests/CMakeLists.txt` and add your new test file to the `add_executable` command.

**Before:**
```cmake
add_executable(test_all
    test_board.cpp
    test_state.cpp
    test_rules.cpp
)
```

**After:**
```cmake
add_executable(test_all
    test_board.cpp
    test_state.cpp
    test_rules.cpp
    test_mycomponent.cpp
)
```

## Step 3: Rebuild the Project

After adding your test file, rebuild the project:

Follow instructions at installation/game.md

And execute what you need.

## Catch2 Test Macros

### Test Structure
- **`TEST_CASE("description", "[tags]")`** - Defines a test case
- **`SECTION("description")`** - Divides a test case into sections (each section runs independently)

### Assertions
- **`REQUIRE(expression)`** - Fails and aborts test if expression is false
- **`CHECK(expression)`** - Fails but continues test execution
- **`REQUIRE_FALSE(expression)`** - Requires expression to be false

### Exception Testing
- **`REQUIRE_THROWS(expression)`** - Requires expression to throw any exception
- **`REQUIRE_THROWS_AS(expression, exception_type)`** - Requires specific exception type
- **`REQUIRE_NOTHROW(expression)`** - Requires expression not to throw

### Comparison
- **`REQUIRE(a == b)`** - Equality check with detailed output
- **`REQUIRE(a != b)`** - Inequality check
- **`REQUIRE(a < b)`**, **`REQUIRE(a > b)`** - Comparison checks

## Best Practices

1. **Use descriptive test names** - Make it clear what is being tested
2. **Use tags** - Organize tests with tags like `[board]`, `[rules]`, `[integration]`
3. **One concept per TEST_CASE** - Keep tests focused
4. **Use SECTION for variations** - Test different scenarios within the same setup
5. **Test edge cases** - Include boundary conditions and error cases
6. **Keep tests independent** - Each test should be able to run in isolation

## Example: Complete Test File

```cpp
#include <catch2/catch_test_macros.hpp>
#include "model/Wall.hpp"

using namespace Quoridor;

TEST_CASE("Wall placement validation", "[wall][validation]") {
    SECTION("Valid horizontal wall") {
        Wall wall = {{3, 3}, Orientation::Horizontal};
        REQUIRE(wall.position.x == 3);
        REQUIRE(wall.position.y == 3);
        REQUIRE(wall.orientation == Orientation::Horizontal);
    }
    
    SECTION("Valid vertical wall") {
        Wall wall = {{0, 0}, Orientation::Vertical};
        REQUIRE(wall.position.x == 0);
        REQUIRE(wall.position.y == 0);
        REQUIRE(wall.orientation == Orientation::Vertical);
    }
}

TEST_CASE("Wall collision detection", "[wall][collision]") {
    Wall wall1 = {{3, 3}, Orientation::Horizontal};
    Wall wall2 = {{3, 3}, Orientation::Horizontal};
    
    SECTION("Identical walls overlap") {
        REQUIRE(wall1.overlaps(wall2) == true);
    }
    
    SECTION("Different positions don't overlap") {
        wall2.position = {4, 4};
        REQUIRE(wall1.overlaps(wall2) == false);
    }
}
```

## Additional Resources

- [Catch2 Documentation](https://github.com/catchorg/Catch2/tree/devel/docs)
- [Catch2 Tutorial](https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md)
- Existing test files in `tests/` directory for reference

## Troubleshooting

**Tests not discovered by CTest:**
- Ensure CMakeLists.txt includes your test file
- Run `cmake --preset <preset>` to reconfigure
- Rebuild the project

**Compilation errors:**
- Check that all necessary headers are included
- Verify namespace usage (`using namespace Quoridor;`)
- Ensure the component being tested is part of `quoridor_model` library

**Test failures:**
- Use `--output-on-failure` flag with ctest
- Run individual test: `./test_all "Test name"`
- Use `SECTION` to isolate failing scenarios
