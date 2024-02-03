/*
 * Project: TinyAd
 * Description: Sample app demonstrating
 *              config parsers usage
 * Author: Kamil Przybyła
 * Created: 30.04.2021
 */

#include <iostream>
#include <vector>

#include "message.hpp"
#include "message_deserializer.hpp"
#include "message_serializer.hpp"
#include "panel_config.hpp"
#include "schedule.hpp"
#include "time.hpp"

using namespace TinyAd;

int main() {
    const auto hour = TinyAd::Hour::now();
    const auto date = TinyAd::Date::now();
    const auto time = TinyAd::DateHour::now();

    std::cout << "Hour: " << hour << "\n";
    std::cout << "Date: " << date << "\n";
    std::cout << "Time: " << time << "\n";

    std::cout << "----------------------\n";

    const Schedule sched("data/example.sched");
    std::cout << sched.toString();

    std::cout << "----------------------\n";

    const PanelConfig conf = PanelConfig::fromFile("data/panel.conf");
    std::cout << conf.toString();

    return 0;
}
