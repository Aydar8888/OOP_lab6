#include "../../../include/npc/factory/orc_creator.h"
#include "../../../include/npc/types/orc.h"

std::unique_ptr<NPC> OrcCreator::create_npc(const std::string& name, Point position) const {
    return std::make_unique<Orc>(name, position);
}