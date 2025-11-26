#pragma once

#include "../npc/factory/npc_factory.h"
#include "../combat/visitor/battle_visitor.h"
#include "../combat/observer/console_observer.h"
#include "../combat/observer/file_observer.h"

class DungeonEditor {
private:
    std::vector<std::unique_ptr<NPC>> npcs;
    NPCFactory factory;
    ConsoleObserver console_observer;
    FileObserver file_observer;

public:
    DungeonEditor();

    void add_npc(const std::string& type, const std::string& name, int x, int y);
    
    void save_to_file(const std::string& filename);
    void load_from_file(const std::string& filename);
    
    void print_all() const;
    
    void start_battle(double radius);
    
    size_t get_alive_count() const;

    bool is_name_exists(const std::string& name) const;

    void remove_dead_npcs();
};