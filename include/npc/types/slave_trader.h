#pragma once

#include "../npc.h"

class SlaveTrader : public NPC {
public:
    SlaveTrader(const std::string& name, const Point& position);

    std::string get_type() const override;

    void accept(Visitor& visit) override;

    std::optional<std::string> vs(const NPC& target) const override;
};