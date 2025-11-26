#include "../include/dungeon/dungeon.h"
#include "../include/geometry/point.h"
#include <gtest/gtest.h>
#include <memory>

class DungeonEditorTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("log.txt");
    }
    
    void TearDown() override {
        std::remove("log.txt");
    }
    
    DungeonEditor editor;
};

TEST_F(DungeonEditorTest, AddNPCValid) {
    // Просто проверяем что NPC добавляется и счетчик увеличивается
    editor.add_npc("Друид", "Мерлин", 10, 20);
    
    EXPECT_EQ(editor.get_alive_count(), 1);
    EXPECT_TRUE(editor.is_name_exists("Мерлин"));
}

TEST_F(DungeonEditorTest, AddNPCInvalidType) {
    // Простая проверка - невалидный тип не должен добавлять NPC
    editor.add_npc("Рыцарь", "Артур", 10, 20);
    
    EXPECT_EQ(editor.get_alive_count(), 0);
    EXPECT_FALSE(editor.is_name_exists("Артур"));
}

TEST_F(DungeonEditorTest, AddMultipleNPCs) {
    editor.add_npc("Друид", "Мерлин", 10, 20);
    editor.add_npc("Орк", "Гром'Гор", 30, 40);
    editor.add_npc("Работорговец", "Торговец", 50, 60);
    
    EXPECT_EQ(editor.get_alive_count(), 3);
}

TEST_F(DungeonEditorTest, IsNameExists) {
    editor.add_npc("Друид", "Мерлин", 10, 20);
    
    EXPECT_TRUE(editor.is_name_exists("Мерлин"));
    EXPECT_FALSE(editor.is_name_exists("Несуществующий"));
}

TEST_F(DungeonEditorTest, PrintAll) {
    editor.add_npc("Друид", "Мерлин", 10, 20);
    editor.add_npc("Орк", "Гром'Гор", 30, 40);
    
    // Просто проверяем что функция не падает
    EXPECT_NO_THROW({
        editor.print_all();
    });
}

TEST_F(DungeonEditorTest, GetAliveCount) {
    EXPECT_EQ(editor.get_alive_count(), 0);
    
    editor.add_npc("Друид", "Мерлин", 10, 20);
    EXPECT_EQ(editor.get_alive_count(), 1);
    
    editor.add_npc("Орк", "Орк", 20, 30);
    EXPECT_EQ(editor.get_alive_count(), 2);
}

TEST_F(DungeonEditorTest, AddDifferentTypes) {
    // Проверяем что все типы NPC могут быть добавлены
    editor.add_npc("Друид", "Друид1", 0, 0);
    editor.add_npc("Орк", "Орк1", 1, 1);
    editor.add_npc("Работорговец", "Торговец1", 2, 2);
    
    EXPECT_EQ(editor.get_alive_count(), 3);
}

TEST_F(DungeonEditorTest, AddNPCWithNegativeCoordinates) {
    editor.add_npc("Друид", "Мерлин", -10, -20);
    
    EXPECT_EQ(editor.get_alive_count(), 1);
    EXPECT_TRUE(editor.is_name_exists("Мерлин"));
}

TEST_F(DungeonEditorTest, AddNPCWithZeroCoordinates) {
    editor.add_npc("Орк", "Орк", 0, 0);
    
    EXPECT_EQ(editor.get_alive_count(), 1);
    EXPECT_TRUE(editor.is_name_exists("Орк"));
}

TEST_F(DungeonEditorTest, BattleNoNPCs) {
    // Битва без NPC не должна падать
    EXPECT_NO_THROW({
        editor.start_battle(10.0);
    });
    EXPECT_EQ(editor.get_alive_count(), 0);
}

TEST_F(DungeonEditorTest, BattleSingleNPC) {
    editor.add_npc("Друид", "Мерлин", 10, 20);
    
    // Битва с одним NPC не должна его убивать
    editor.start_battle(10.0);
    
    EXPECT_EQ(editor.get_alive_count(), 1);
}