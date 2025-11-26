#include "../include/dungeon/dungeon.h"
#include "../include/geometry/point.h"
#include <gtest/gtest.h>
#include <fstream>

class DungeonEditorFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("log.txt");
        std::remove("test_save.txt");
    }
    
    void TearDown() override {
        std::remove("log.txt");
        std::remove("test_save.txt");
    }
    
    DungeonEditor editor;
};

TEST_F(DungeonEditorFileTest, SaveAndLoad) {
    // Добавляем NPC
    editor.add_npc("Друид", "Мерлин", 10, 20);
    editor.add_npc("Орк", "Гром'Гор", 30, 40);
    
    // Сохраняем
    testing::internal::CaptureStdout();  // Успешные операции в stdout
    editor.save_to_file("test_save.txt");
    std::string save_output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(save_output.find("Данные сохранены") != std::string::npos);
    
    // Проверяем что файл создан
    std::ifstream file("test_save.txt");
    EXPECT_TRUE(file.is_open());
    file.close();
    
    // Создаем новый редактор и загружаем
    DungeonEditor new_editor;
    testing::internal::CaptureStdout();  // Успешные операции в stdout
    new_editor.load_from_file("test_save.txt");
    std::string load_output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(load_output.find("Данные загружены") != std::string::npos);
    
    // Проверяем что загрузилось правильно
    EXPECT_EQ(new_editor.get_alive_count(), 2);
}

TEST_F(DungeonEditorFileTest, SaveToInvalidPath) {
    testing::internal::CaptureStderr();  // Ошибки в stderr
    
    editor.save_to_file("/invalid/path/file.txt");
    std::string output = testing::internal::GetCapturedStderr();
    
    EXPECT_TRUE(output.find("Ошибка при сохранении") != std::string::npos)
        << "Expected to find 'Ошибка при сохранении' in: " << output;
}

TEST_F(DungeonEditorFileTest, LoadFromNonExistentFile) {
    testing::internal::CaptureStderr();  // Ошибки в stderr
    
    editor.load_from_file("non_existent_file.txt");
    std::string output = testing::internal::GetCapturedStderr();
    
    EXPECT_TRUE(output.find("Ошибка при загрузке") != std::string::npos)
        << "Expected to find 'Ошибка при загрузке' in: " << output;
}