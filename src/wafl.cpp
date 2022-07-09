#include "../include/wafl/wafl.hpp"

auto similarity(search_params p) -> float
{
    float proximity_coef = 0.f;
    for (int i = 0; i < (int)p.input.size(); i++)
    {
        float proximity_coef_tmp = 0.f;
        for (int j = 0; j < (int)p.reference.size(); j++)
        {
            if (p.input[i] == p.reference[j])
            {
                if (i == j)
                {
                    proximity_coef_tmp = 1.f;
                }
                else if (proximity_coef_tmp < (1 / std::abs(float(j - i))))
                {
                    proximity_coef_tmp = 1.f / std::abs(float(j - i)) / 2.f;
                }
            }
        }
        proximity_coef += proximity_coef_tmp;
    }
    size_t smallest_word_size = std::min(p.input.size(), p.input.size());
    return proximity_coef / smallest_word_size;
}

auto similarity_match(search_params p) -> Matches
{
    float proximity_coef = similarity(p);
    if (proximity_coef > .75f)
    {
        return Matches::Strongly;
    }
    else if (proximity_coef > .5f)
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
    if (first >= second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

auto remove_NotAtAll_from_vector(float value) -> bool
{
    return value <= 0.5f;
}