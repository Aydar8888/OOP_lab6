#pragma once
#include "observer.h"
#include <fstream>
#include <string>

class FileObserver : public Observer {
private:
    std::string filename;

public:
    FileObserver(const std::string& file);

    void notify(const BattleEvent& event) const override;
};
