#include "../../../include/npc/types/slave_trader.h"
#include "../../../include/combat/visitor/visitor.h"


SlaveTrader::SlaveTrader(const std::string& name, const Point& position) : NPC(name, position) {}

std::string SlaveTrader::get_type() const { return "Работорговец"; }

void SlaveTrader::accept(Visitor& visitor) { visitor.visit(*this); }

std::optional<std::string> SlaveTrader::vs(const NPC& target) const {
    const std::string& type = target.get_type();

    if (type == "Друид") {
        return "Работорговец убил Друида!";
    } 
    return std::nullopt;
}
