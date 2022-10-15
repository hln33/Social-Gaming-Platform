//
// Created by mcgir on 10/14/2022.
//


//-------------------------Control-Structures-------------------------//
void foreach(<T> list, <T> function) {
    for (int i = 0; i < list.length; i++) {
        function(list[i]);
    }
}

void loop(){}

void inparallel(){}

void parallelfor(){}


void switch(<T> list, <T> value){
    for (int i = 0; i < list.length; i++) {
        if (list[i] == value) {
            //do the meat of the operation
            return i;
        }
    }
    return -1;
}

void when(){}

//-----------List-Operators-----------//
void extend(){}

void reverse(){}

void shuffle(){}

void sort(){}

void deal(){}

void discard(){}

//-----------Arithmetic-Operators-----------//

void add(){}

//-----------Timing-----------//

void timer(){}

//-----------Human-Input-----------//
void input-choice(){}

void input-text(){}

void input-vote(){}

//-----------Output-----------//
void message(){}

void global-message(){}

void scores(){}


