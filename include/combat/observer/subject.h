#pragma once
#include "observer.h"
#include <vector>
#include <memory>

class Subject {
private:
    std::vector<Observer*> observers;

public:
    void add_observer(Observer* observer);
    void notify(const BattleEvent& event) const;
};
