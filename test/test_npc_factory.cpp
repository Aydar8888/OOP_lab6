#include "../include/npc/factory/npc_factory.h"
#include "../include/npc/factory/druid_creator.h"
#include "../include/npc/factory/orc_creator.h"
#include "../include/npc/factory/slave_trader_creator.h"
#include "../include/npc/types/druid.h"
#include "../include/npc/types/orc.h"
#include "../include/npc/types/slave_trader.h"
#include "../include/geometry/point.h"
#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include <filesystem>

TEST(DruidCreatorTest, CreateDruid) {
    DruidCreator creator;
    Point position(10, 20);
    
    auto npc = creator.create_npc("Айдар", position);
    
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Айдар");
    EXPECT_EQ(npc->get_type(), "Друид");
    EXPECT_EQ(npc->get_position().get_x(), 10);
    EXPECT_EQ(npc->get_position().get_y(), 20);
    EXPECT_TRUE(npc->is_alive());
}

TEST(OrcCreatorTest, CreateOrc) {
    OrcCreator creator;
    Point position(5, 15);
    
    auto npc = creator.create_npc("Максим", position);
    
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Максим");
    EXPECT_EQ(npc->get_type(), "Орк");
    EXPECT_EQ(npc->get_position().get_x(), 5);
    EXPECT_EQ(npc->get_position().get_y(), 15);
    EXPECT_TRUE(npc->is_alive());
}

TEST(SlaveTraderCreatorTest, CreateSlaveTrader) {
    SlaveTraderCreator creator;
    Point position(30, 40);
    
    auto npc = creator.create_npc("Ваня", position);
    
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Ваня");
    EXPECT_EQ(npc->get_type(), "Работорговец");
    EXPECT_EQ(npc->get_position().get_x(), 30);
    EXPECT_EQ(npc->get_position().get_y(), 40);
    EXPECT_TRUE(npc->is_alive());
}

TEST(NPCFactoryTest, CreateByTypeDruid) {
    NPCFactory factory;
    Point position(7, 8);
    
    auto npc = factory.create_by_type("Друид", "Гера", position);
    
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Гера");
    EXPECT_EQ(npc->get_type(), "Друид");
    EXPECT_EQ(npc->get_position().get_x(), 7);
    EXPECT_EQ(npc->get_position().get_y(), 8);
    EXPECT_TRUE(npc->is_alive());
}

TEST(NPCFactoryTest, CreateByTypeOrc) {
    NPCFactory factory;
    Point position(12, 34);
    
    auto npc = factory.create_by_type("Орк", "Леха", position);
    
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Леха");
    EXPECT_EQ(npc->get_type(), "Орк");
    EXPECT_EQ(npc->get_position().get_x(), 12);
    EXPECT_EQ(npc->get_position().get_y(), 34);
    EXPECT_TRUE(npc->is_alive());
}

TEST(NPCFactoryTest, CreateByTypeSlaveTrader) {
    NPCFactory factory;
    Point position(56, 78);
    
    auto npc = factory.create_by_type("Работорговец", "Раб", position);
    
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Раб");
    EXPECT_EQ(npc->get_type(), "Работорговец");
    EXPECT_EQ(npc->get_position().get_x(), 56);
    EXPECT_EQ(npc->get_position().get_y(), 78);
    EXPECT_TRUE(npc->is_alive());
}

TEST(NPCFactoryTest, CreateByTypeInvalidType) {
    NPCFactory factory;
    Point position(0, 0);
    
    EXPECT_THROW({
        factory.create_by_type("Король", "Артур", position);
    }, std::invalid_argument);
    
    EXPECT_THROW({
        factory.create_by_type("", "Безымянный", position);
    }, std::invalid_argument);
    
    EXPECT_THROW({
        factory.create_by_type("Волшебник", "Гендальф", position);
    }, std::invalid_argument);
}

class NPCFactoryFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        createTestFile("valid_npcs.txt", {
            "Гермиона Друид 10 20",
            "ГарриПоттер Орк 30 40", 
            "Рон Работорговец 50 60",
            "Хагрид Друид 70 80"
        });
        
        createTestFile("mixed_npcs.txt", {
            "Малфой Друид 10 20",
            "Артур Рыцарь 30 40", // тут тип не допустимый
            "Торговец Работорговец 50 60"
        });
        
        createTestFile("invalid_format.txt", {
            "Северус Друид 10",
            "Дамблдор Орк",
            "Только_имя",
            "Друид 10 20" 
        });
        
        createTestFile("empty.txt", {});
        
        createTestFile("special_names.txt", {
            "Имя_с_подчерками Друид 1 1",
            "Волан-де-Морт Орк 2 2", 
            "ИмяБезПробелов Работорговец 3 3"
        });
    }
    
    void TearDown() override {
        cleanupFile("valid_npcs.txt");
        cleanupFile("mixed_npcs.txt");
        cleanupFile("invalid_format.txt");
        cleanupFile("empty.txt");
        cleanupFile("special_names.txt");
        cleanupFile("output.txt");
    }
    
    void createTestFile(const std::string& filename, const std::vector<std::string>& lines) {
        std::ofstream file(filename);
        for (const auto& line : lines) {
            file << line << "\n";
        }
    }
    
    void cleanupFile(const std::string& filename) {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }
    
    NPCFactory factory;
};

TEST_F(NPCFactoryFileTest, CreateByFileValid) {
    auto npcs = factory.create_by_file("valid_npcs.txt");
    
    EXPECT_EQ(npcs.size(), 4);
    
    // Проверяем первого NPC
    EXPECT_EQ(npcs[0]->get_name(), "Гермиона");
    EXPECT_EQ(npcs[0]->get_type(), "Друид");
    EXPECT_EQ(npcs[0]->get_position().get_x(), 10);
    EXPECT_EQ(npcs[0]->get_position().get_y(), 20);
    
    // Проверяем второго NPC
    EXPECT_EQ(npcs[1]->get_name(), "ГарриПоттер");
    EXPECT_EQ(npcs[1]->get_type(), "Орк");
    EXPECT_EQ(npcs[1]->get_position().get_x(), 30);
    EXPECT_EQ(npcs[1]->get_position().get_y(), 40);
    
    // Проверяем третьего NPC
    EXPECT_EQ(npcs[2]->get_name(), "Рон");
    EXPECT_EQ(npcs[2]->get_type(), "Работорговец");
    EXPECT_EQ(npcs[2]->get_position().get_x(), 50);
    EXPECT_EQ(npcs[2]->get_position().get_y(), 60);
}

TEST_F(NPCFactoryFileTest, CreateByFileMixedValidInvalid) {
    testing::internal::CaptureStderr();
    
    auto npcs = factory.create_by_file("mixed_npcs.txt");
    
    std::string output = testing::internal::GetCapturedStderr();
    
    // Должны создать только валидные NPC (2 из 3)
    EXPECT_EQ(npcs.size(), 2);
    EXPECT_TRUE(output.find("Ошибка создания NPC") != std::string::npos);
    
    // Проверяем что создались правильные NPC
    EXPECT_EQ(npcs[0]->get_name(), "Малфой");
    EXPECT_EQ(npcs[0]->get_type(), "Друид");
    EXPECT_EQ(npcs[1]->get_name(), "Торговец");
    EXPECT_EQ(npcs[1]->get_type(), "Работорговец");
}

TEST_F(NPCFactoryFileTest, CreateByFileEmpty) {
    auto npcs = factory.create_by_file("empty.txt");
    EXPECT_EQ(npcs.size(), 0);
}

TEST_F(NPCFactoryFileTest, CreateByFileInvalidFormat) {
    testing::internal::CaptureStderr();
    
    auto npcs = factory.create_by_file("invalid_format.txt");
    
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(npcs.size(), 0);
}

TEST_F(NPCFactoryFileTest, CreateByFileSpecialNames) {
    auto npcs = factory.create_by_file("special_names.txt");
    
    EXPECT_EQ(npcs.size(), 3);
    
    EXPECT_EQ(npcs[0]->get_name(), "Имя_с_подчерками");
    EXPECT_EQ(npcs[0]->get_type(), "Друид");
    EXPECT_EQ(npcs[0]->get_position().get_x(), 1);
    EXPECT_EQ(npcs[0]->get_position().get_y(), 1);
    
    EXPECT_EQ(npcs[1]->get_name(), "Волан-де-Морт");
    EXPECT_EQ(npcs[1]->get_type(), "Орк");
    EXPECT_EQ(npcs[1]->get_position().get_x(), 2);
    EXPECT_EQ(npcs[1]->get_position().get_y(), 2);
    
    EXPECT_EQ(npcs[2]->get_name(), "ИмяБезПробелов");
    EXPECT_EQ(npcs[2]->get_type(), "Работорговец");
    EXPECT_EQ(npcs[2]->get_position().get_x(), 3);
    EXPECT_EQ(npcs[2]->get_position().get_y(), 3);
}

TEST_F(NPCFactoryFileTest, CreateByFileNonExistent) {
    EXPECT_THROW({
        factory.create_by_file("non_existent_file.txt");
    }, std::runtime_error);
}

TEST_F(NPCFactoryFileTest, SaveToFile) {
    // Создаем несколько NPC
    std::vector<std::unique_ptr<NPC>> npcs;
    npcs.push_back(factory.create_by_type("Друид", "Мерлин", Point(10, 20)));
    npcs.push_back(factory.create_by_type("Орк", "Гром'Гор", Point(30, 40)));
    npcs.push_back(factory.create_by_type("Работорговец", "Торговец", Point(50, 60)));
    
    // Убиваем одного NPC
    npcs[1]->kill();
    
    // Сохраняем в файл через экземпляр фабрики
    factory.save_to_file("output.txt", npcs);
    
    // Проверяем содержимое файла
    std::ifstream file("output.txt");
    ASSERT_TRUE(file.is_open());
    
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    // Должны сохраниться только живые NPC (2 из 3)
    EXPECT_EQ(lines.size(), 2);
    
    // Проверяем содержимое
    bool found_druid = false;
    bool found_trader = false;
    
    for (const auto& saved_line : lines) {
        if (saved_line.find("Мерлин Друид 10 20") != std::string::npos) {
            found_druid = true;
        }
        if (saved_line.find("Торговец Работорговец 50 60") != std::string::npos) {
            found_trader = true;
        }
    }
    
    EXPECT_TRUE(found_druid);
    EXPECT_TRUE(found_trader);
}

TEST_F(NPCFactoryFileTest, SaveToFileAllDead) {
    // Создаем NPC и всех убиваем
    std::vector<std::unique_ptr<NPC>> npcs;
    npcs.push_back(factory.create_by_type("Друид", "Мерлин", Point(10, 20)));
    npcs.push_back(factory.create_by_type("Орк", "Гром'Гор", Point(30, 40)));
    
    for (auto& npc : npcs) {
        npc->kill();
    }
    
    // Сохраняем в файл через экземпляр фабрики
    factory.save_to_file("output.txt", npcs);
    
    // Файл должен быть пустым
    std::ifstream file("output.txt");
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            line_count++;
        }
    }
    
    EXPECT_EQ(line_count, 0);
}

TEST_F(NPCFactoryFileTest, SaveToFileEmptyVector) {
    std::vector<std::unique_ptr<NPC>> npcs;
    
    // Сохраняем в файл через экземпляр фабрики
    factory.save_to_file("output.txt", npcs);
    
    // Файл должен существовать но быть пустым
    std::ifstream file("output.txt");
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            line_count++;
        }
    }
    
    EXPECT_EQ(line_count, 0);
}

TEST_F(NPCFactoryFileTest, SaveToFileInvalidPath) {
    std::vector<std::unique_ptr<NPC>> npcs;
    npcs.push_back(factory.create_by_type("Друид", "Мерлин", Point(10, 20)));
    
    EXPECT_THROW({
        factory.save_to_file("/invalid/path/npcs.txt", npcs);
    }, std::runtime_error);
}

TEST_F(NPCFactoryFileTest, FullCycle) {
    // Создаем NPC
    std::vector<std::unique_ptr<NPC>> original_npcs;
    original_npcs.push_back(factory.create_by_type("Друид", "Мерлин", Point(10, 20)));
    original_npcs.push_back(factory.create_by_type("Орк", "Гром'Гор", Point(30, 40)));
    original_npcs.push_back(factory.create_by_type("Работорговец", "Торговец", Point(50, 60)));
    
    // Сохраняем через экземпляр фабрики
    factory.save_to_file("output.txt", original_npcs);
    
    // Загружаем обратно
    auto loaded_npcs = factory.create_by_file("output.txt");
    
    // Проверяем что загрузилось столько же NPC
    EXPECT_EQ(loaded_npcs.size(), original_npcs.size());
    
    // Проверяем каждого NPC
    for (size_t i = 0; i < loaded_npcs.size(); ++i) {
        EXPECT_EQ(loaded_npcs[i]->get_name(), original_npcs[i]->get_name());
        EXPECT_EQ(loaded_npcs[i]->get_type(), original_npcs[i]->get_type());
        EXPECT_EQ(loaded_npcs[i]->get_position().get_x(), original_npcs[i]->get_position().get_x());
        EXPECT_EQ(loaded_npcs[i]->get_position().get_y(), original_npcs[i]->get_position().get_y());
        EXPECT_TRUE(loaded_npcs[i]->is_alive());
    }
}

TEST(NPCFactoryEdgeCasesTest, SpecialNamesAndPositions) {
    NPCFactory factory;
    
    // Пустое имя
    auto npc1 = factory.create_by_type("Друид", "", Point(0, 0));
    EXPECT_EQ(npc1->get_name(), "");
    EXPECT_EQ(npc1->get_type(), "Друид");
    
    // Имя с подчеркиваниями и дефисами
    auto npc2 = factory.create_by_type("Орк", "Громовой-Молот", Point(1, 1));
    EXPECT_EQ(npc2->get_name(), "Громовой-Молот");
    EXPECT_EQ(npc2->get_type(), "Орк");
    
    // Отрицательные координаты
    auto npc3 = factory.create_by_type("Работорговец", "Торговец", Point(-10, -20));
    EXPECT_EQ(npc3->get_position().get_x(), -10);
    EXPECT_EQ(npc3->get_position().get_y(), -20);
    
    // Большие координаты
    auto npc4 = factory.create_by_type("Друид", "Друид", Point(1000, 2000));
    EXPECT_EQ(npc4->get_position().get_x(), 1000);
    EXPECT_EQ(npc4->get_position().get_y(), 2000);
    
    // Нулевые координаты
    auto npc5 = factory.create_by_type("Орк", "Орк", Point(0, 0));
    EXPECT_EQ(npc5->get_position().get_x(), 0);
    EXPECT_EQ(npc5->get_position().get_y(), 0);
}

TEST(NPCFactoryTest, CreateMultipleNPCs) {
    NPCFactory factory;
    
    auto druid = factory.create_by_type("Друид", "Друид1", Point(1, 1));
    auto orc = factory.create_by_type("Орк", "Орк1", Point(2, 2));
    auto trader = factory.create_by_type("Работорговец", "Торговец1", Point(3, 3));
    
    EXPECT_EQ(druid->get_type(), "Друид");
    EXPECT_EQ(orc->get_type(), "Орк");
    EXPECT_EQ(trader->get_type(), "Работорговец");
}