#pragma once
#include "observer.h"
#include <iostream>

class ConsoleObserver : public Observer {
public:
    void notify(const BattleEvent& event) const override;
};
