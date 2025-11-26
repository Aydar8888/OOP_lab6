#pragma once

#include <optional>

#include "../geometry/point.h"

class Visitor;

class NPC {
protected:
    std::string name;
    Point position;
    bool alive;

public:
    virtual ~NPC() = default;

    NPC(const std::string& name, const Point& position);

    virtual std::string get_type() const = 0;

    virtual void accept(Visitor& visit) = 0;

    virtual std::optional<std::string> vs(const NPC& target) const = 0;

    virtual std::string get_name() const;

    virtual Point get_position() const;

    virtual bool is_alive() const;

    void kill();
};
