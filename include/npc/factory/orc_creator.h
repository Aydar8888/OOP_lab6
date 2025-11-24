#pragma once

#include "creator.h"
#include <memory>

class OrcCreator : public Creator {
public:
    std::unique_ptr<NPC> create_npc(const std::string& name, Point position) const override;
};