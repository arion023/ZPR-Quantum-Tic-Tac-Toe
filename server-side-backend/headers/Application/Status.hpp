#pragma once
#include <string>

enum Status{
    False = -1,
    Off = -1,
    True = 0,
    Ongoing = 0,
    Cycle = 1,
    Stopped = 2,
    Finished = 3
};

inline std::string game_status_to_string(Status s)
{
    switch(s)
    {
        case Status::Off:
            return "off";
        case Status::Ongoing:
            return "ongoing";
        case Status::Cycle:
            return "cycle";
        case Status::Stopped:
            return "stopped";
        case Status::Finished:
            return "finished";
        default:
            return "error";
    }
}
