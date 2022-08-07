#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4702)
#endif
#include "../lib/ZipIterator/ZipIterator.hpp"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace wafl {

enum class Matches {
    NotAtAll,
    Weakly,
    Strongly,
};

struct search_params {
    std::string_view input;
    std::string_view reference;
};

auto delete_punctuation(std::string& str) -> void;
auto to_lower(std::string_view str) -> std::string;

auto similarity(search_params) -> float;
auto similarity_match(search_params) -> Matches;

auto comparator_between_two_values(float first, float second) -> bool;
auto remove_NotAtAll_from_vector(float value) -> bool;

// Container == std::vector<std::string>
// StringGetter == [](const std::string& str) -> std::string_view { return str; }
template<typename Container, typename StringGetter> // assert StringGetter is a function from Container::value_type to std::string_view
auto search_results(std::string_view input, const Container& container, StringGetter&& get_string) -> Container
{
    auto               container_copy = container;
    std::vector<float> float_container;
    float_container.resize(container.size());
    std::transform(container.begin(), container.end(), float_container.begin(), [&](auto&& element) {
        return similarity({.input = input, .reference = get_string(element)});
    });

    auto zip = Zip(float_container, container_copy);

    auto it = std::remove_if(zip.begin(), zip.end(), [&](auto&& element) {
        return remove_NotAtAll_from_vector(std::get<0>(element.val()));
    });

    float_container.erase(std::get<0>(it.get_iterator()), float_container.end());
    container_copy.erase(std::get<1>(it.get_iterator()), container_copy.end());

    std::sort(zip.rbegin(), zip.rend());

    return container_copy;
}

} // namespace wafl