#pragma once

#include <string>

// TOOD namespace wafl

enum class Matches {
    Strongly,
    Weakly,
    NotAtAll,
};

struct search_params {
    std::string_view input;
    std::string_view reference;
};

auto similarity(search_params) -> float;
auto similarity_match(search_params) -> Matches;