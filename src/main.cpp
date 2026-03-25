#include <iostream>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "services/dice_service.h"
#include "adapters/event_bus_impl.h"
#include "domain/dice.h"

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

} // anonymous namespace

int main() {
    try {
        json input;
        std::cin >> input;

        const std::string command = input.at("command").get<std::string>();
        if (command != "dice.roll") {
            throw std::invalid_argument("unknown command: " + command);
        }

        erebus::domain::DiceType type = parseDiceType(input.at("diceType").get<std::string>());
        int count = input.at("count").get<int>();

        erebus::adapters::EventBusImpl bus;
        erebus::services::DiceService service(bus);
        erebus::domain::DiceRoll result = service.roll(type, count);

        json output;
        output["results"] = result.results;
        output["total"]   = result.total();
        std::cout << output.dump() << std::endl;

    } catch (const std::exception& e) {
        json err;
        err["error"] = e.what();
        std::cerr << err.dump() << std::endl;
        return 1;
    }

    return 0;
}
