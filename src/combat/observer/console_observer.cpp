#include "../../../include/combat/observer/console_observer.h"

void ConsoleObserver::notify(const BattleEvent& event) const {
    std::cout << "[Console] Action: " << event.action << std::endl;
}