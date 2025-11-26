#include "../include/dungeon/dungeon.h"
#include "../include/geometry/point.h"
#include <gtest/gtest.h>
#include <fstream>

class DungeonEditorBattleTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("log.txt");
    }
    
    void TearDown() override {
        std::remove("log.txt");
    }
    
    DungeonEditor editor;
};

TEST_F(DungeonEditorBattleTest, BattleNoNPCs) {
    // Битва без NPC не должна падать
    EXPECT_NO_THROW({
        editor.start_battle(10.0);
    });
    EXPECT_EQ(editor.get_alive_count(), 0);
}

TEST_F(DungeonEditorBattleTest, BattleSingleNPC) {
    editor.add_npc("Друид", "Мерлин", 10, 20);
    
    // Битва с одним NPC не должна его убивать
    editor.start_battle(10.0);
    
    EXPECT_EQ(editor.get_alive_count(), 1);
}

TEST_F(DungeonEditorBattleTest, BattleOutOfRange) {
    // NPC далеко друг от друга
    editor.add_npc("Орк", "Орк1", 0, 0);
    editor.add_npc("Друид", "Друид1", 100, 100);
    
    editor.start_battle(10.0); // Маленький радиус
    
    // Оба должны остаться живы (слишком далеко)
    EXPECT_EQ(editor.get_alive_count(), 2);
}

TEST_F(DungeonEditorBattleTest, BattleOrcVsDruid) {
    // NPC близко друг к другу
    editor.add_npc("Орк", "Орк1", 0, 0);
    editor.add_npc("Друид", "Друид1", 1, 1);
    
    editor.start_battle(10.0);
    
    // Орк должен убить друида
    EXPECT_EQ(editor.get_alive_count(), 1);
}

TEST_F(DungeonEditorBattleTest, BattleOrcVsSlaveTrader) {
    editor.add_npc("Орк", "Орк1", 0, 0);
    editor.add_npc("Работорговец", "Торговец1", 1, 1);
    
    editor.start_battle(10.0);
    
    // Орк должен убить торговца
    EXPECT_EQ(editor.get_alive_count(), 1);
}

TEST_F(DungeonEditorBattleTest, BattleSlaveTraderVsDruid) {
    editor.add_npc("Работорговец", "Торговец1", 0, 0);
    editor.add_npc("Друид", "Друид1", 1, 1);
    
    editor.start_battle(10.0);
    
    // Торговец должен убить друида
    EXPECT_EQ(editor.get_alive_count(), 1);
}

TEST_F(DungeonEditorBattleTest, BattleDruidNeverAttacks) {
    editor.add_npc("Друид", "Друид1", 0, 0);
    editor.add_npc("Орк", "Орк1", 1, 1);
    
    editor.start_battle(10.0);
    
    // Друид не атакует, орк атакует друида
    EXPECT_EQ(editor.get_alive_count(), 1);
}

TEST_F(DungeonEditorBattleTest, BattleCreatesLogFile) {
    editor.add_npc("Орк", "Орк1", 0, 0);
    editor.add_npc("Друид", "Друид1", 1, 1);
    
    editor.start_battle(10.0);
    
    // Проверяем что лог файл создан
    std::ifstream log_file("log.txt");
    EXPECT_TRUE(log_file.is_open());
    log_file.close();
}

TEST_F(DungeonEditorBattleTest, BattleRemovesDeadNPCs) {
    editor.add_npc("Орк", "Орк1", 0, 0);
    editor.add_npc("Друид", "Друид1", 1, 1);
    editor.add_npc("Работорговец", "Торговец1", 2, 2);
    
    EXPECT_EQ(editor.get_alive_count(), 3);
    
    editor.start_battle(10.0);
    
    // После битвы должно остаться меньше NPC
    EXPECT_LT(editor.get_alive_count(), 3);
}

TEST_F(DungeonEditorBattleTest, BattleSamePosition) {
    // NPC в одинаковой позиции
    editor.add_npc("Орк", "Орк1", 5, 5);
    editor.add_npc("Друид", "Друид1", 5, 5);
    
    editor.start_battle(1.0); // Даже с маленьким радиусом
    
    // Должны сработать (расстояние 0)
    EXPECT_EQ(editor.get_alive_count(), 1);
}

TEST_F(DungeonEditorBattleTest, BattleZeroRadius) {
    editor.add_npc("Орк", "Орк1", 0, 0);
    editor.add_npc("Друид", "Друид1", 0, 0); // Та же позиция
    
    editor.start_battle(0.0); // Нулевой радиус
    
    // При нулевом радиусе и одинаковой позиции битва должна произойти
    EXPECT_EQ(editor.get_alive_count(), 1);
}