#include "../../../include/npc/factory/druid_creator.h"
#include "../../../include/npc/types/druid.h"

#include <memory>

std::unique_ptr<NPC> DruidCreator::create_npc(const std::string& name, Point position) const {
    return std::make_unique<Druid>(name, position);
}