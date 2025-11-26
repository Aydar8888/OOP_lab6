#include "../../../include/npc/factory/npc_factory.h"

std::unique_ptr<Creator> NPCFactory::get_creator(const std::string& type) const {
    if (type == "Друид") return std::make_unique<DruidCreator>();
    if (type == "Орк") return std::make_unique<OrcCreator>();
    if (type == "Работорговец") return std::make_unique<SlaveTraderCreator>();
    throw std::invalid_argument("Неизвестный NPC: " + type);
}

std::unique_ptr<NPC> NPCFactory::create_by_type(const std::string& type, const std::string& name, const Point& position) const {
    auto creator = get_creator(type);
    return creator->create_npc(name, position);
}

std::vector<std::unique_ptr<NPC>> NPCFactory::create_by_file(const std::string& file_name) const {
    std::vector<std::unique_ptr<NPC>> npcs;
    std::ifstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл " + file_name);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);       
        std::string name, type;
        int x, y;

        if (iss >> name >> type >> x >> y) {
            try {
                Point position(x, y);
                auto npc = create_by_type(type, name, position);
                npcs.push_back(std::move(npc));
            } catch (const std::exception& e) {
                std::cerr << "Ошибка создания NPC по полученным из файла данным: " << e.what() << std::endl;
            }
        }
    }

    return npcs;
}

void NPCFactory::save_to_file(const std::string& file_name, const std::vector<std::unique_ptr<NPC>>& npcs) {
    std::ofstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл " + file_name);
    }

    for (const auto& npc : npcs) {
        if (npc->is_alive()) { 
            file << npc->get_name() << " " << npc->get_type() << " "  << npc->get_position().get_x() << " " << npc->get_position().get_y() << "\n"; 
        }
    }
}