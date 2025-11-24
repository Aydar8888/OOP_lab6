#include "../../../include/npc/factory/slave_trader_creator.h"
#include "../../../include/npc/types/slave_trader.h"

#include <memory>

std::unique_ptr<NPC> SlaveTraderCreator::create_npc(const std::string& name, Point position) const {
    return std::make_unique<SlaveTrader>(name, position);
}
