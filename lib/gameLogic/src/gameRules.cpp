//
// Created by mcgir on 10/14/2022.
//
using namespace std;

//-------------------------Control-Structures-------------------------//
void foreach(vector<T> &list, void (*func)(T)) {
    for (auto &item: list) {
        func(item);
    }
}

void loop(vector<T> &list, void (*rule)(T), void (*condition)(T)) {
    while (condition(list)) {
        rule(list);
    }
}

void inparallel(){}

void parallelfor(){}


void switch(vector<T> &list, T value, void (*rule)(T)) {
    for (auto &item: list) {
        if (item == value) {
            rule(item);
        }
    }
}

void when(){}

//-----------List-Operators-----------//
void extend(vector<T> &list, vector<T> &list2) {
    list.insert(list.end(), list2.begin(), list2.end());
}

void reverse(vector<T> &list) {
    reverse(list.begin(), list.end());
}

void shuffle(vector<T> &list) {
    random_shuffle(list.begin(), list.end());
}

void sort(vector<T> list) {
    std::sort(list.begin(), list.end());
}

void deal(vector<T> &deck, vector<T> &hand, int numCards) {
    for (int i = 0; i < numCards; i++) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

void discard(vector<T> list, T value){
    for (int i = 0; i < list.length; i++) {
        if (list[i] == value) {
            list.erase(list.begin() + i);
            return;
        }
    }
}

//-----------Arithmetic-Operators-----------//

void add(int &a, int b) {
    a = a + b;
}

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


