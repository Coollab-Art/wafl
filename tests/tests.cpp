#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <wafl/wafl.hpp>

// Check out doctest's documentation: https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

namespace std {
template<typename T>
std::ostream& operator<<(std::ostream& output, std::vector<T> const& values)
{
    for (auto const& value : values)
    {
        output << value << ", ";
    }
    return output;
}
} // namespace std

TEST_CASE("testing the search bar function")
{
    // Strongly
    CHECK(similarity_match({.input = "test", .reference = "test2"}) == Matches::Strongly);
    CHECK(similarity_match({.input = "ceci est un test", .reference = "cei erst un tset"}) == Matches::Strongly);
    CHECK(similarity_match({.input = "hello", .reference = "helo"}) == Matches::Strongly);

    // Weakly
    CHECK(similarity_match({.input = "tentative", .reference = "tntatvei"}) == Matches::Weakly);
    CHECK(similarity_match({.input = "test", .reference = "tsey"}) == Matches::Weakly);
    CHECK(similarity_match({.input = "test", .reference = "etts"}) == Matches::Weakly);

    // false
    CHECK(similarity_match({.input = "hello", .reference = "hi"}) == Matches::NotAtAll);
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

TEST_CASE("Test similarity function")
{
    CHECK(similarity({.input = "hello", .reference = "hi"}) < 0.5f);
    CHECK(similarity({.input = "hello", .reference = "hello world"}) < 1.0f);
    CHECK(similarity({.input = "hello", .reference = "something"}) < 0.2f);
    CHECK(similarity({.input = "hello", .reference = "hello"}) == 1.0f);
    CHECK(similarity({.input = "hello", .reference = "helo"}) >= 0.8f);
}

TEST_CASE("Two tests exactly similar return 1")
{
    CHECK(similarity({.input = "test", .reference = "test"}) == 1);
    CHECK(similarity({.input = "test2", .reference = "test2"}) == 1);
}

TEST_CASE("The punctuation has no importance")
{
    CHECK(similarity({.input = "test", .reference = "te,st"}) == 1);
    CHECK(similarity({.input = "tes,t", .reference = "test"}) == 1);
    CHECK(similarity({.input = "tes,t", .reference = "te,st"}) == 1);
    CHECK(similarity_match({.input = "HelloWorld", .reference = "Hello, World"}) == Matches::Strongly);
    CHECK(similarity_match({.input = "Hello, World", .reference = "HelloWorld"}) == Matches::Strongly);
}

TEST_CASE("The case has no importance")
{
    CHECK(similarity({.input = "teSt", .reference = "Test"}) == 1);
}

// TODO ponctuation

struct MyData {
    std::string name;
    int         value;

    friend auto operator==(const MyData&, const MyData&) -> bool = default; // Only required for the tests to know if the vectors are equal
};

namespace std {
std::ostream& operator<<(std::ostream& output, const MyData& my_data)
{
    output << my_data.name;
    return output;
}
} // namespace std

using Container = std::vector<MyData>;

TEST_CASE("Search filter")
{
    const auto res = search_results("hello", std::vector<MyData>{
                                                 {"hi", 0},
                                                 {"hell World", 1},
                                                 {"something", 9},
                                                 {"hello", 2},
                                                 {"helo", 3},
                                             },
                                    [](const MyData& my_data) -> std::string_view {
                                        return my_data.name;
                                    });

    CHECK(res == std::vector<MyData>{
                     {"hello", 2},
                     {"helo", 3},
                     {"hell World", 1},
                 });
}