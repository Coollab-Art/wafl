#include "../include/wafl/wafl.hpp"

auto similarity(search_params p) -> float
{
    float proximity_coef = 0.f;
    for (int i = 0; i < p.input.size(); i++)
    {
        float proximity_coef_tmp = 0.f;
        for (int j = 0; j < p.reference.size(); j++)
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
    // float avg_size = (float)(p.input.size() + p.reference.size()) / 2;
    // return 1 < proximity_coef / p.input.size();
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