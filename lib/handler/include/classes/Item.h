#pragma once

#include <string>
#include <StrongAlias.h>
using Type = StrongAlias<std::string, struct TypeTag>;
using Name = StrongAlias<std::string, struct NameTag>;
using Beat = StrongAlias<std::string, struct BeatTag>;

// Description: Generic class to define Constant's item for the game such as weapons
// E.g: Item { Type("Weapon"), Name("Rock"), Beat("Scissor") },
class Item{
    public:
        Item(Type type, Name name, Beat beat); 
        Type getType();
        Name getName();
        Beat getBeat();
    private: 
        Type type;
        Name name;
        Beat beat;
};