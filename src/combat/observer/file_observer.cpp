#include "../../../include/combat/observer/file_observer.h"

FileObserver::FileObserver(const std::string& file) : filename(file) {}

void FileObserver::notify(const BattleEvent& event) const {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << "[File] Action: " << event.action << std::endl;
    }
}