#pragma once

#include <memory>
#include <vector>
#include "druid_creator.h"
#include "orc_creator.h"
#include "slave_trader_creator.h"
#include "creator.h"

class NPCFactory {
private:
    std::unique_ptr<Creator> get_creator(const std::string& type) const;
public:
    std::unique_ptr<NPC> create_by_type(const std::string& type, const std::string& name, const Point& position) const;
    std::vector<std::unique_ptr<NPC>> create_by_file(const std::string& file_name) const;
    void save_to_file(const std::string& fil_ename, const std::vector<std::unique_ptr<NPC>>& npcs);
};