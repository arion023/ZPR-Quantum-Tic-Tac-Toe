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

inline Sign string_to_sign(std::string s)
{
    if(s.length() == 1)
    {
        switch((int) s[0])
        {
            case 88:
                return Sign::X;
            case 79:
                return Sign::O;
        }
    }
    return Sign::None; 

}

