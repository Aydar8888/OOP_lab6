#include "../../include/dungeon/dungeon.h"
#include "../../include/geometry/point.h"
#include <iostream>
#include <cmath>
#include <algorithm>


DungeonEditor::DungeonEditor() 
    : file_observer("log.txt") {}

void DungeonEditor::add_npc(const std::string& type, const std::string& name, int x, int y) {
    try {
        Point position(x, y);
        auto npc = factory.create_by_type(type, name, position);
        npcs.push_back(std::move(npc));
        std::cout << "Добавлен " << type << " '" << name << "' в позиции (" << x << ", " << y << ")\n";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании NPC: " << e.what() << std::endl;
    }
}

void DungeonEditor::save_to_file(const std::string& filename) {
    try {
        factory.save_to_file(filename, npcs);
        std::cout << "Данные сохранены в файл: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении: " << e.what() << std::endl;
    }
}

void DungeonEditor::load_from_file(const std::string& filename) {
    try {
        auto loaded_npcs = factory.create_by_file(filename);
        npcs = std::move(loaded_npcs);
        std::cout << "Данные загружены из файла: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке: " << e.what() << std::endl;
    }
}

void DungeonEditor::print_all() const {
    std::cout << "\n=== СПИСОК NPC ===\n";
    for (const auto& npc : npcs) {
        if (npc && npc->is_alive()) {
            auto pos = npc->get_position();
            std::cout << "Имя: " << npc->get_name()
                      << " | Тип: " << npc->get_type()
                      << " | Позиция: (" << pos.get_x() << ", " << pos.get_y() << ")\n";
        }
    }
    std::cout << "Всего живых NPC: " << get_alive_count() << "\n\n";
}

void DungeonEditor::start_battle(double radius) {
    std::cout << "=== НАЧАЛО БИТВЫ (радиус: " << radius << ") ===\n";
    
    BattleVisitor battle_visitor(radius);
    battle_visitor.add_observer(&console_observer);
    battle_visitor.add_observer(&file_observer);
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!npcs[i] || !npcs[i]->is_alive()) continue;
        
        for (size_t j = 0; j < npcs.size(); ++j) {
            if (i == j || !npcs[j] || !npcs[j]->is_alive()) continue;
            
            auto pos1 = npcs[i]->get_position();
            auto pos2 = npcs[j]->get_position();
            double distance = pos1.distance_to(pos2);
            
            if (distance <= radius) {
                battle_visitor.set_attacker(npcs[i].get());
                npcs[j]->accept(battle_visitor);
            }
        }
    }
    remove_dead_npcs();
    
    std::cout << "=== БИТВА ЗАВЕРШЕНА ===\n";
    std::cout << "Осталось живых NPC: " << get_alive_count() << "\n\n";
}

size_t DungeonEditor::get_alive_count() const {
    size_t count = 0;
    for (const auto& npc : npcs) {
        if (npc != nullptr && npc->is_alive()) {
            count++;
        }
    }
    return count;
}

bool DungeonEditor::is_name_exists(const std::string& name) const {
    for (const auto& npc : npcs) {
        if (npc && npc->get_name() == name) {
            return true;
        }
    }
    return false;
}

void DungeonEditor::remove_dead_npcs() {
    for (auto it = npcs.begin(); it != npcs.end(); ) {
        if (!*it || !(*it)->is_alive()) {
            it = npcs.erase(it);
        } else {
            ++it;
        }
    }
}