#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <wafl/wafl.hpp>

// Check out doctest's documentation: https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

TEST_CASE("testing the search bar function")
{
    // true
    CHECK(similarity_match({.input = "test", .reference = "test2"}) == Matches::Strongly);
    CHECK(similarity_match({.input = "test", .reference = "tsey"}) == Matches::Weakly);
    CHECK(similarity_match({.input = "ceci est un test", .reference = "cei erst un tset"}) == Matches::Strongly);

    // false (on the limit)
    CHECK(similarity_match({.input = "test", .reference = "etts"}) == Matches::NotAtAll);

    // false
    CHECK(similarity_match({.input = "tentative", .reference = "tntatvei"}) == Matches::NotAtAll);
    CHECK(similarity_match({.input = "rouge", .reference = "eguor"}) == Matches::NotAtAll);
}

TEST_CASE("Matches the beginning of a sentence")
{
    CHECK(similarity_match({.input = "test", .reference = "test mais beaucoup plus long"}) == Matches::Strongly);
    CHECK(similarity_match({.input = "test mais beaucoup plus long", .reference = "test"}) == Matches::Weakly);
    CHECK(similarity_match({.input = "test", .reference = "test"}) == Matches::Strongly);
    CHECK(similarity_match({.input = "test", .reference = "test     "}) == Matches::Strongly);
    CHECK(similarity_match({.input = "test", .reference = "testa"}) == Matches::Strongly);
}

TEST_CASE("Two tests exactly similar return 1")
{
    CHECK(similarity({.input = "test", .reference = "test"}) == 1);
}

struct MyData {
    std::string name;
    int         value;

    friend auto operator==(const MyData&, const MyData&) -> bool = default;
};

using Container = std::vector<MyData>;

TEST_CASE("Search filter")
{
    const auto res = search_results("hello", std::vector<MyData>{
                                                 {"Hi", 0},
                                                 {"Hello World", 1},
                                                 {"Something", 9},
                                                 {"Hello", 2},
                                                 {"Helo", 3},
                                             },
                                    [](const MyData& my_data) -> std::string_view {
                                        return my_data.name;
                                    });

    CHECK(res == std::vector<MyData>{
                     {"Hello", 2},
                     {"Hello World", 1},
                     {"Helo", 3},
                 });
}