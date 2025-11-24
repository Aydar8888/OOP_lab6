#include "include/dungeon/dungeon.h"
#include <iostream>
#include <limits>

void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void show_menu() {
    std::cout << "\n=== РЕДАКТОР ПОДЗЕМЕЛЬЯ BALAGUR FATE 3 ===\n";
    std::cout << "1. Добавить NPC\n";
    std::cout << "2. Сохранить в файл\n";
    std::cout << "3. Загрузить из файла\n";
    std::cout << "4. Показать всех NPC\n";
    std::cout << "5. Запустить боевой режим\n";
    std::cout << "6. Выход\n";
    std::cout << "Выберите действие: ";
}

int main() {
    DungeonEditor editor;
    int choice;
    
    do {
        show_menu();
        std::cin >> choice;
        clear_input();
        
        switch (choice) {
            case 1: {
                std::string type, name;
                int x, y;
                
                std::cout << "Доступные типы: Друид, Орк, Работорговец\n";
                std::cout << "Введите тип: ";
                std::getline(std::cin, type);
                std::cout << "Введите имя: ";
                std::getline(std::cin, name);
                std::cout << "Введите координату X (0-500): ";
                std::cin >> x;
                std::cout << "Введите координату Y (0-500): ";
                std::cin >> y;
                clear_input();
                
                if (x < 0 || x > 500 || y < 0 || y > 500) {
                    std::cout << "Ошибка: координаты должны быть в диапазоне 0-500!\n";
                    break;
                }
                
                editor.add_npc(type, name, x, y);
                break;
            }
            case 2: {
                std::string filename;
                std::cout << "Введите имя файла для сохранения: ";
                std::getline(std::cin, filename);
                editor.save_to_file(filename);
                break;
            }
            case 3: {
                std::string filename;
                std::cout << "Введите имя файла для загрузки: ";
                std::getline(std::cin, filename);
                editor.load_from_file(filename);
                break;
            }
            case 4:
                editor.print_all();
                break;
            case 5: {
                double radius;
                std::cout << "Введите радиус боя: ";
                std::cin >> radius;
                clear_input();
                editor.start_battle(radius);
                break;
            }
            case 6:
                std::cout << "Выход из программы...\n";
                break;
            default:
                std::cout << "Неверный выбор!\n";
        }
    } while (choice != 6);
    
    return 0;
}