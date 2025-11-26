#include "../include/npc/npc.h"
#include "../include/npc/types/druid.h"
#include "../include/npc/types/orc.h"
#include "../include/npc/types/slave_trader.h"
#include "../include/geometry/point.h"
#include <gtest/gtest.h>
#include <memory>


TEST(DruidTest, ConstructorAndType) {
    Point position(5, 5);
    Druid druid("Лабуба", position);
    
    EXPECT_EQ(druid.get_name(), "Лабуба");
    EXPECT_EQ(druid.get_type(), "Друид");
    EXPECT_TRUE(druid.is_alive());
    EXPECT_EQ(druid.get_position().get_x(), 5);
    EXPECT_EQ(druid.get_position().get_y(), 5);
}

TEST(DruidTest, KillMethod) {
    Druid druid("Друид", Point(0, 0));
    EXPECT_TRUE(druid.is_alive());
    
    druid.kill();
    EXPECT_FALSE(druid.is_alive());
}

TEST(DruidTest, VSMethodNeverAttacks) {
    Druid druid1("Друид1", Point(0, 0));
    Druid druid2("Друид2", Point(1, 1));
    Orc orc("Орк", Point(2, 2));
    SlaveTrader trader("Торговец", Point(3, 3));
    
    EXPECT_FALSE(druid1.vs(druid2).has_value());
    EXPECT_FALSE(druid1.vs(orc).has_value());
    EXPECT_FALSE(druid1.vs(trader).has_value());
}

TEST(DruidTest, VSMethodWhenDead) {
    Druid druid("Друид", Point(0, 0));
    Orc orc("Орк", Point(1, 1));
    
    druid.kill();
    
    EXPECT_FALSE(druid.vs(orc).has_value());
}


TEST(OrcTest, ConstructorAndType) {
    Point position(15, 25);
    Orc orc("Баобаб", position);
    
    EXPECT_EQ(orc.get_name(), "Баобаб");
    EXPECT_EQ(orc.get_type(), "Орк");
    EXPECT_TRUE(orc.is_alive());
}

TEST(OrcTest, VSMethodAgainstOrc) {
    Orc orc1("Орк1", Point(0, 0));
    Orc orc2("Орк2", Point(1, 1));
    
    auto result = orc1.vs(orc2);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "Орк каннибал съел сородича!");
}

TEST(OrcTest, VSMethodAgainstDruid) {
    Orc orc("Орк", Point(0, 0));
    Druid druid("Друид", Point(1, 1));
    
    auto result = orc.vs(druid);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "Орк разорвал бедолагу Друида!");
}

TEST(OrcTest, VSMethodAgainstSlaveTrader) {
    Orc orc("Орк", Point(0, 0));
    SlaveTrader trader("Торговец", Point(1, 1));
    
    auto result = orc.vs(trader);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "Рабство — это плохо! Орк уничтожил Работорговца!");
}

TEST(OrcTest, VSMethodWhenDead) {
    Orc orc1("Орк1", Point(0, 0));
    Orc orc2("Орк2", Point(1, 1));
    Druid druid("Друид", Point(2, 2));
    
    orc1.kill();
    
    EXPECT_FALSE(orc1.vs(orc2).has_value());
    EXPECT_FALSE(orc1.vs(druid).has_value());
}

TEST(OrcTest, VSMethodAgainstDeadTarget) {
    Orc orc("Орк", Point(0, 0));
    Druid druid("Друид", Point(1, 1));
    SlaveTrader trader("Торговец", Point(2, 2));
    
    druid.kill();
    trader.kill();
    
    EXPECT_FALSE(orc.vs(druid).has_value());
    EXPECT_FALSE(orc.vs(trader).has_value());
}

TEST(SlaveTraderTest, ConstructorAndType) {
    Point position(30, 40);
    SlaveTrader trader("Леха", position);
    
    EXPECT_EQ(trader.get_name(), "Леха");
    EXPECT_EQ(trader.get_type(), "Работорговец");
    EXPECT_TRUE(trader.is_alive());
}

TEST(SlaveTraderTest, VSMethodAgainstDruid) {
    SlaveTrader trader("Торговец", Point(0, 0));
    Druid druid("Друид", Point(1, 1));
    
    auto result = trader.vs(druid);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "Работорговец убил Друида!");
}

TEST(SlaveTraderTest, VSMethodAgainstOrc) {
    SlaveTrader trader("Торговец", Point(0, 0));
    Orc orc("Орк", Point(1, 1));
    
    EXPECT_FALSE(trader.vs(orc).has_value());
}

TEST(SlaveTraderTest, VSMethodAgainstSlaveTrader) {
    SlaveTrader trader1("Торговец1", Point(0, 0));
    SlaveTrader trader2("Торговец2", Point(1, 1));
    
    // Работорговцы не атакуют друг друга
    EXPECT_FALSE(trader1.vs(trader2).has_value());
}

TEST(SlaveTraderTest, VSMethodWhenDead) {
    SlaveTrader trader("Торговец", Point(0, 0));
    Druid druid("Друид", Point(1, 1));
    
    trader.kill();
    
    EXPECT_FALSE(trader.vs(druid).has_value());
}

TEST(SlaveTraderTest, VSMethodAgainstDeadDruid) {
    SlaveTrader trader("Торговец", Point(0, 0));
    Druid druid("Друид", Point(1, 1));
    
    druid.kill();

    EXPECT_FALSE(trader.vs(druid).has_value());
}

TEST(PolymorphismTest, GetTypePolymorphism) {
    std::unique_ptr<NPC> druid = std::make_unique<Druid>("Друид", Point(0, 0));
    std::unique_ptr<NPC> orc = std::make_unique<Orc>("Орк", Point(1, 1));
    std::unique_ptr<NPC> trader = std::make_unique<SlaveTrader>("Торговец", Point(2, 2));
    
    EXPECT_EQ(druid->get_type(), "Друид");
    EXPECT_EQ(orc->get_type(), "Орк");
    EXPECT_EQ(trader->get_type(), "Работорговец");
}

TEST(PolymorphismTest, BaseClassMethods) {
    std::unique_ptr<NPC> druid = std::make_unique<Druid>("Друид", Point(10, 20));
    
    EXPECT_EQ(druid->get_name(), "Друид");
    EXPECT_EQ(druid->get_position().get_x(), 10);
    EXPECT_EQ(druid->get_position().get_y(), 20);
    EXPECT_TRUE(druid->is_alive());
    
    druid->kill();
    EXPECT_FALSE(druid->is_alive());
}

TEST(InteractionTest, AllVSCombinationsAliveNPCs) {
    Druid druid("Друид", Point(0, 0));
    Orc orc("Орк", Point(1, 1));
    SlaveTrader trader("Торговец", Point(2, 2));
    

    EXPECT_TRUE(druid.is_alive());
    EXPECT_TRUE(orc.is_alive());
    EXPECT_TRUE(trader.is_alive());
    
    EXPECT_FALSE(druid.vs(orc).has_value());
    EXPECT_FALSE(druid.vs(trader).has_value());
    EXPECT_FALSE(druid.vs(druid).has_value());
    
    EXPECT_TRUE(orc.vs(druid).has_value());
    EXPECT_TRUE(orc.vs(trader).has_value());
    EXPECT_TRUE(orc.vs(orc).has_value());
    
    EXPECT_TRUE(trader.vs(druid).has_value());
    EXPECT_FALSE(trader.vs(orc).has_value());
    EXPECT_FALSE(trader.vs(trader).has_value());
}

TEST(InteractionTest, NoInteractionsWhenDead) {
    Druid druid("Друид", Point(0, 0));
    Orc orc("Орк", Point(1, 1));
    SlaveTrader trader("Торговец", Point(2, 2));
    
    druid.kill();
    orc.kill();
    trader.kill();
    
    EXPECT_FALSE(druid.vs(orc).has_value());
    EXPECT_FALSE(druid.vs(trader).has_value());
    EXPECT_FALSE(druid.vs(druid).has_value());
    
    EXPECT_FALSE(orc.vs(druid).has_value());
    EXPECT_FALSE(orc.vs(trader).has_value());
    EXPECT_FALSE(orc.vs(orc).has_value());
    
    EXPECT_FALSE(trader.vs(druid).has_value());
    EXPECT_FALSE(trader.vs(orc).has_value());
    EXPECT_FALSE(trader.vs(trader).has_value());
}


TEST(InteractionTest, Asymmetry) {
    Druid druid("Друид", Point(0, 0));
    Orc orc("Орк", Point(1, 1));
    SlaveTrader trader("Торговец", Point(2, 2));

    EXPECT_FALSE(druid.vs(orc).has_value());
    EXPECT_TRUE(orc.vs(druid).has_value());
    
    EXPECT_FALSE(trader.vs(orc).has_value());
    EXPECT_TRUE(orc.vs(trader).has_value());
    
    EXPECT_TRUE(trader.vs(druid).has_value());
    EXPECT_FALSE(druid.vs(trader).has_value()); 
}

TEST(PositionTest, DifferentPositions) {
    Druid druid("Друид", Point(-10, -10));
    Orc orc("Орк", Point(100, 50));
    SlaveTrader trader("Торговец", Point(0, -5));
    
    EXPECT_EQ(druid.get_position().get_x(), -10);
    EXPECT_EQ(druid.get_position().get_y(), -10);
    
    EXPECT_EQ(orc.get_position().get_x(), 100);
    EXPECT_EQ(orc.get_position().get_y(), 50);
    
    EXPECT_EQ(trader.get_position().get_x(), 0);
    EXPECT_EQ(trader.get_position().get_y(), -5);
}


TEST(StateTest, ImmutabilityAfterVS) {
    Druid druid("Друид", Point(0, 0));
    Orc orc("Орк", Point(1, 1));
    
    std::string originalDruidName = druid.get_name();
    Point originalDruidPos = druid.get_position();
    bool originalDruidAlive = druid.is_alive();

    auto result = orc.vs(druid);

    EXPECT_EQ(druid.get_name(), originalDruidName);
    EXPECT_EQ(druid.get_position().get_x(), originalDruidPos.get_x());
    EXPECT_EQ(druid.get_position().get_y(), originalDruidPos.get_y());
    EXPECT_EQ(druid.is_alive(), originalDruidAlive);
    EXPECT_TRUE(result.has_value());
}

TEST(LifeCycleTest, AliveStateManagement) {
    Orc orc("Орк", Point(0, 0));
    Druid druid("Друид", Point(1, 1));
    
    EXPECT_TRUE(orc.is_alive());
    EXPECT_TRUE(druid.is_alive());
    
    EXPECT_TRUE(orc.vs(druid).has_value());
    druid.kill();
    EXPECT_FALSE(druid.is_alive());
    
    EXPECT_FALSE(orc.vs(druid).has_value());
    
    orc.kill();
    EXPECT_FALSE(orc.is_alive());
    
    EXPECT_FALSE(orc.vs(druid).has_value());
}

TEST(EdgeCasesTest, EmptyName) {
    Druid druid("", Point(0, 0));
    EXPECT_EQ(druid.get_name(), "");
    EXPECT_EQ(druid.get_type(), "Друид");
}

TEST(EdgeCasesTest, SamePosition) {
    Point samePos(7, 7);
    Druid druid("Друид", samePos);
    Orc orc("Орк", samePos);
    
    EXPECT_EQ(druid.get_position().get_x(), 7);
    EXPECT_EQ(druid.get_position().get_y(), 7);
    EXPECT_EQ(orc.get_position().get_x(), 7);
    EXPECT_EQ(orc.get_position().get_y(), 7);
    
    auto result = orc.vs(druid);
    EXPECT_TRUE(result.has_value());
}