#include "../include/wafl/wafl.hpp"
#include <cctype>
#include <cstring>

auto to_lower(std::string_view str) -> std::string
{
    std::string res = "";
    std::for_each(str.begin(), str.end(), [&res](char c) {
        res += static_cast<char>(std::tolower(c));
    });
    return res;
}

void delete_punctuation(std::string& str)
{
    std::erase_if(str, std::ispunct);
}

void delete_spaces(std::string& str)
{
    std::erase_if(str, [](const char c) { return c == ' '; });
}

auto similarity(search_params p) -> float
{
    std::string input     = to_lower(p.input);
    std::string reference = to_lower(p.reference);
    delete_punctuation(input);
    delete_punctuation(reference);
    delete_spaces(input);
    delete_spaces(reference);

    float proximity_coef = 0.f;
    int   j_used         = -1;
    // int   number_of_perfect_match = 0;
    for (int i = 0; i < (int)input.size(); i++)
    {
        float proximity_coef_tmp = 0.f;
        for (int j = 0; j < (int)reference.size(); j++)
        {
            if (input[i] == reference[j])
            {
                if (i == j)
                {
                    proximity_coef_tmp = 1.f;
                    j_used             = j;
                    // number_of_perfect_match++;
                    break;
                }
                else if (proximity_coef_tmp < (1 / std::abs(float(j - i))) && j != j_used)
                {
                    proximity_coef_tmp = 1.f / std::abs(float(j - i)) / 2.f;
                }
            }
        }
        proximity_coef += proximity_coef_tmp;
    }

    // size_t min_word_size = std::min(input.size(), reference.size());
    // proximity_coef *= number_of_perfect_match / min_word_size;
    float word_size = 1.f;
    if (input.size() < reference.size())
    {
        word_size = input.size() * 0.95f + reference.size() * 0.05f;
    }
    else
    {
        word_size = input.size() * 0.1f + reference.size() * 0.9f;
    }

    return proximity_coef / word_size;
}

auto similarity_match(search_params p) -> Matches
{
    float proximity_coef = similarity(p);
    if (proximity_coef > .75f)
    {
        return Matches::Strongly;
    }
    else if (proximity_coef >= .5f)
    {
        return Matches::Weakly;
    }
    else
    {
        return Matches::NotAtAll;
    }
}

auto comparator_between_two_values(float first, float second) -> bool
{
    return first >= second;
}

auto remove_NotAtAll_from_vector(float value) -> bool
{
    return value < 0.5f;
}