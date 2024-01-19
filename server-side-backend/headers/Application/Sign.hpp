#pragma once
#include <string>

enum Sign{
    None = 0,
    X = 1,
    O = 2
};

inline std::string sign_to_string(Sign s)
{
    switch(s)
    {
        case Sign::X:
            return "X";
        case Sign::O:
            return "O";
        default:
            return "-"; 
    } 
}
