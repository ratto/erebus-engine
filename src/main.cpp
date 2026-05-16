#include <iostream>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "services/dice_service.h"
#include "handlers/combat_skill_handler.h"
#include "handlers/capacity_handler.hpp"
#include "adapters/event_bus_impl.h"
#include "domain/dice.h"
#include "domain/combat_skill.h"

using json = nlohmann::json;

namespace {

erebus::domain::DiceType parseDiceType(const std::string& s) {
    if (s == "d3")   return erebus::domain::DiceType::d3;
    if (s == "d4")   return erebus::domain::DiceType::d4;
    if (s == "d6")   return erebus::domain::DiceType::d6;
    if (s == "d8")   return erebus::domain::DiceType::d8;
    if (s == "d10")  return erebus::domain::DiceType::d10;
    if (s == "d12")  return erebus::domain::DiceType::d12;
    if (s == "d100") return erebus::domain::DiceType::d100;
    throw std::invalid_argument("unknown diceType: " + s);
}

erebus::domain::CombatSkillType parseCombatSkillType(const std::string& s) {
    if (s == "melee")  return erebus::domain::CombatSkillType::Melee;
    if (s == "ranged") return erebus::domain::CombatSkillType::Ranged;
    if (s == "shield") return erebus::domain::CombatSkillType::Shield;
    throw std::invalid_argument("unknown skillType: " + s);
}

json handleDiceRoll(const json& input) {
    erebus::domain::DiceType type = parseDiceType(input.at("diceType").get<std::string>());
    int count = input.at("count").get<int>();

    erebus::adapters::EventBusImpl bus;
    erebus::services::DiceService service(bus);
    erebus::domain::DiceRoll result = service.roll(type, count);

    json output;
    output["results"] = result.results;
    output["total"]   = result.total();
    return output;
}

json handleCombatSkillValidate(const json& input) {
    erebus::domain::CombatSkillAllocation alloc;
    alloc.type           = parseCombatSkillType(input.at("skillType").get<std::string>());
    alloc.attackBase     = input.at("attackBase").get<int>();
    alloc.defenseBase    = input.at("defenseBase").get<int>();
    alloc.pointsAllocated = input.at("pointsAllocated").get<int>();
    alloc.attackPoints   = input.at("attackPoints").get<int>();
    alloc.defensePoints  = input.at("defensePoints").get<int>();
    alloc.isDefault      = input.value("isDefault", false);

    erebus::handlers::CombatSkillHandler handler;
    erebus::domain::CombatSkillResult result = handler.validate(alloc);

    json output;
    output["valid"]        = result.valid;
    output["attackValue"]  = result.attackValue;
    output["defenseValue"] = result.defenseValue;
    output["errors"]       = result.errors;
    return output;
}

json handleCapacityCalculateY(const json& input) {
    double k        = input.at("k").get<double>();
    int    attribute = input.at("attribute").get<int>();

    erebus::handlers::CapacityHandler handler;
    auto result = handler.calculateY(k, attribute);

    json output;
    output["y"] = result.y;
    return output;
}

json handleCapacityCalculateK(const json& input) {
    double y = input.at("y").get<double>();

    erebus::handlers::CapacityHandler handler;
    auto result = handler.calculateK(y);

    json output;
    output["k"] = result.k;
    return output;
}

json handleCapacityDamageBonus(const json& input) {
    int fr = input.at("fr").get<int>();

    erebus::handlers::CapacityHandler handler;
    auto result = handler.calculateDamageBonus(fr);

    json output;
    output["damageBonus"] = result.damageBonus;
    return output;
}

} // anonymous namespace

int main() {
    try {
        json input;
        std::cin >> input;

        const std::string command = input.at("command").get<std::string>();

        if (command == "dice.roll") {
            std::cout << handleDiceRoll(input).dump() << std::endl;
        } else if (command == "combat_skill.validate") {
            std::cout << handleCombatSkillValidate(input).dump() << std::endl;
        } else if (command == "capacity.calculate_y") {
            std::cout << handleCapacityCalculateY(input).dump() << std::endl;
        } else if (command == "capacity.calculate_k") {
            std::cout << handleCapacityCalculateK(input).dump() << std::endl;
        } else if (command == "capacity.damage_bonus") {
            std::cout << handleCapacityDamageBonus(input).dump() << std::endl;
        } else {
            throw std::invalid_argument("unknown command: " + command);
        }

    } catch (const std::exception& e) {
        json err;
        err["error"] = e.what();
        std::cerr << err.dump() << std::endl;
        return 1;
    }

    return 0;
}
