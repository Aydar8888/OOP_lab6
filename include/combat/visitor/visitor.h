#pragma once

class Druid;
class Orc;
class SlaveTrader;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Druid& druid) = 0;
    virtual void visit(Orc& orc) = 0;
    virtual void visit(SlaveTrader& slave_trader) = 0;
};
