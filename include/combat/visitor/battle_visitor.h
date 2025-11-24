#pragma once
#include "visitor.h"
#include "../../npc/npc.h"
#include <vector>
#include <memory>
#include "../observer/subject.h"
#include "../observer/battle_event.h"
#include "../observer/observer.h"

class BattleObserver;

class BattleVisitor : public Visitor {
private:
    NPC* current_attacker;
    double battle_radius;
    Subject subject; 

public:
    BattleVisitor(double radius);
    ~BattleVisitor() = default;

    void set_attacker(NPC* attacker);
    double get_radius() const;

    void battle(NPC& target);

    void visit(Druid& druid) override;
    void visit(Orc& orc) override;
    void visit(SlaveTrader& slave_trader) override;

    void add_observer(Observer* observer);
};
