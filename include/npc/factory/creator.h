#pragma once

#include "../npc.h"
#include <memory>

class Creator {
public:
    virtual ~Creator() = default;
    virtual std::unique_ptr<NPC> create_npc(const std::string& name, Point position) const = 0; 
};