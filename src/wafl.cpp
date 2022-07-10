#include "../include/wafl/wafl.hpp"

// si le mot à chercher est plus long que le mot d'entrée, ne pas diviser par la longueur du mot le plus court
// mais par un coefficient plus grand entre les deux longueurs.

auto similarity(search_params p) -> float
{
    float proximity_coef = 0.f;
    int   j_used         = -1;
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
    // size_t word_size = std::min(p.input.size(), p.reference.size());
    float word_size = 1.f;
    if (p.input.size() < p.reference.size())
    {
        word_size = p.input.size() * 0.95f + p.reference.size() * 0.05f;
    }
    else
    {
        word_size = p.input.size() * 0.1f + p.reference.size() * 0.9f;
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