#include "../../../include/npc/types/druid.h"
#include "../../../include/combat/visitor/visitor.h"


Druid::Druid(const std::string& name, const Point& position) : NPC(name, position) {}

std::string Druid::get_type() const { return "Друид"; }

void Druid::accept(Visitor& visitor) { visitor.visit(*this); }

std::optional<std::string> Druid::vs(const NPC& target) const { return std::nullopt; }
