#pragma once

#include <vector>
#include <string>

enum Signal {
    COMPLETE,   // the rules are finished running
    INTERRUPT,  // need to pass control over to IO (networking)
    NOSIGNAL, 
};

struct Bundle {
    int         player;
    std::string message;
};

struct SignalData {
    Signal              signal;
    std::vector<Bundle> data;
};
