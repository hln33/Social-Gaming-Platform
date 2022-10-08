#include "Item.h"

Item::Item(Type type, Name name, Beat beat) 
        : 
        type{std::move(type)}, 
        name{std::move(name)}, 
        beat{std::move(beat)} 
    {} 

Type Item::getType(){
    return this->type;
}

Name Item::getName(){
    return this->name;
}

Beat Item::getBeat(){
    return this->beat;
}