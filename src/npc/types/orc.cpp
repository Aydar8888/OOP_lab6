#include "../../../include/npc/types/orc.h"
#include "../../../include/combat/visitor/visitor.h"

Orc::Orc(const std::string& name, const Point& position) : NPC(name, position) {}

std::string Orc::get_type() const { return "Орк"; }

void Orc::accept(Visitor& visitor) { visitor.visit(*this); }

std::optional<std::string> Orc::vs(const NPC& target) const {
    if (!this->is_alive() || !target.is_alive()) {
        return std::nullopt; 
    }
    const std::string& type = target.get_type();

    if (type == "Орк") {
        return "Орк каннибал съел сородича!";
    } 
    else if (type == "Друид") {
        return "Орк разорвал бедолагу Друида!";
    } 
    else if (type == "Работорговец") {
        return "Рабство — это плохо! Орк уничтожил Работорговца!";
    }

    return std::nullopt;
}
