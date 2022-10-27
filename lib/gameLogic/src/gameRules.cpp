//
// Created by mcgir on 10/14/2022.
//
using namespace std;
#include <boost/thread/thread.hpp>


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

void timer(int duration){ //this needs to be expanded and idk how to do it
    boost::this_thread::sleep_for(boost::chrono::seconds(duration));
}

//-----------Human-Input-----------//
void input-choice(Player &player, string_view prompt, vector<T> &choices, vector<T> &result) {
    message(player, prompt);
    for (int i = 0; i < choices.length; i++) {
        message(player, to_string(i) + ": " + choices[i]);
    }

    for (int i = 0; i < choices.length; i++) {
        if (player.getInput() == to_string(i)) {
            result.push_back(choices[i]);
            return;
        }
    }
}

void input-text(){}

void input-vote(){}

//-----------Output-----------//
void message(vector<Player> &players, string_view message) {
    for(auto &player: players){
        player.message(message);
    }
}

void global-message(string_view message){
    cout << message << endl;
}

void scores(vector<Player> &players, bool ascending) {
    if (ascending) {
        sort(players);
    }

    for (auto &player: players) {
        global-message(player.getName() + ": " + player.getCurrentScore());
    }
}


