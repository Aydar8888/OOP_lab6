#pragma once

#include "creator.h"

class SlaveTraderCreator : public Creator {
public:
    std::unique_ptr<NPC> create_npc(const std::string& name, Point position) const override;
};