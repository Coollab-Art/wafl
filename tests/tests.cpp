#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <mylib/mylib.hpp>

// Check out doctest's documentation: https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

auto search_bar(std::string first, std::string second) -> bool
{
    return true;
}

TEST_CASE("testing the search bar function")
{
    CHECK(search_bar("test", "test2") == true)
    CHECK(search_bar("test", "test2") == false)
}