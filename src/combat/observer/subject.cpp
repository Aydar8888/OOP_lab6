#include "../../../include/combat/observer/subject.h"

void Subject::add_observer(Observer* observer) {
    observers.push_back(observer);
}

void Subject::notify(const BattleEvent& event) const{
    for (auto obs : observers)
        obs->notify(event);
}