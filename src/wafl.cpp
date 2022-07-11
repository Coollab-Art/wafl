#include "../include/wafl/wafl.hpp"
#include <cctype>
#include <cstring>
#include <optional>

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
    std::erase_if(str, std::isspace);
}

auto find_next_word_position(
    std::string_view text,
    size_t           offset,
    std::string_view delimiters
) -> std::optional<std::pair<size_t, size_t>>
{
    const size_t idx1 = text.find_first_not_of(delimiters, offset);
    if (idx1 == std::string_view::npos)
    {
        return std::nullopt;
    }
    size_t idx2 = text.find_first_of(delimiters, idx1);
    if (idx2 == std::string_view::npos)
    {
        idx2 = text.size();
    }
    return std::make_pair(idx1, idx2);
}

auto split_into_words(
    std::string_view text,
    std::string_view delimiters
) -> std::vector<std::string>
{
    std::vector<std::string> res;
    auto                     word_pos = find_next_word_position(text, 0, delimiters);
    while (word_pos.has_value())
    {
        res.emplace_back(text.substr(word_pos->first, word_pos->second - word_pos->first));
        word_pos = find_next_word_position(text, word_pos->second, delimiters);
    }
    return res;
}

auto search_by_separated_words(
    std::vector<std::string> input_words,
    std::vector<std::string> reference_words
) -> float
{
    float proximity_coef = 0.f;

    for (int h = 0; h < (int)input_words.size(); h++)
    {
        float proximity_coef_by_word = 0.f;
        for (int k = 0; k < (int)reference_words.size(); k++)
        {
            float proximity_coef_by_letter = 0.f;
            int   j_used                   = -1; // devrait être un tableau de tous les j utilisés et pas que du dernier
            for (int i = 0; i < (int)input_words[h].size(); i++)
            {
                float proximity_coef_tmp = 0.f;
                for (int j = 0; j < (int)reference_words[k].size(); j++)
                {
                    if (input_words[h][i] == reference_words[k][j])
                    {
                        if (i == j)
                        {
                            proximity_coef_tmp = 1.f;
                            j_used             = j;
                            break;
                        }
                        else if (proximity_coef_tmp < (1 / std::abs(float(j - i))) && j != j_used)
                        {
                            proximity_coef_tmp = 1.f / std::abs(float(j - i)) / 2.f;
                        }
                    }
                }
                proximity_coef_by_letter += proximity_coef_tmp;
            }
            if (proximity_coef_by_word < proximity_coef_by_letter)
            {
                proximity_coef_by_word = proximity_coef_by_letter;
            }
        }
        if (proximity_coef < proximity_coef_by_word)
        {
            proximity_coef = proximity_coef_by_word;
        }
    }
    return proximity_coef;
}

auto search_by_string(
    std::string input,
    std::string reference
)
{
    float proximity_coef = 0.f;
    int   j_used         = -1;

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
    return proximity_coef;
}

auto similarity(search_params p) -> float
{
    std::string input     = to_lower(p.input);
    std::string reference = to_lower(p.reference);
    delete_punctuation(input);
    delete_punctuation(reference);
    std::vector<std::string> input_words     = split_into_words(input, " ");
    std::vector<std::string> reference_words = split_into_words(reference, " ");
    delete_spaces(input);
    delete_spaces(reference);

    float proximity_coef = std::max(search_by_separated_words(input_words, reference_words), search_by_string(input, reference));

    float word_size = 1.f; // quand les deux mots sont trop longs et qu'un des mot match parfaitement la division est trop importante et rend le match weak
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