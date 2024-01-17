#pragma once
#include <string>

enum Sign{
    None = -1,
    X = 0,
    O = 1
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
            return " "; 
    } 
}
