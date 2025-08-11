#include "Nilib/Core/Enum.hpp"
#include "Nilib/Logger/Log.hpp"

#include <iostream>

using namespace Nilib;

ENUM(Color, RED = 1, GREEN = -1, YELLOW, FIVE = 5, BLACK, DEFAULT = 6);
int main()
{

    Color my_enum = Color::RED;

    LOG_DEBUG(static_cast<int>(my_enum)); // Okay.

    my_enum.print();
    LOG_DEBUG("Insert a color using a name:");
    std::string user_input1, user_input3;
    std::cin >> user_input1;
    Color selected1 = Color::fromName(user_input1);
    LOG_DEBUG("You selected:", std::string(selected1), int(selected1));

    LOG_DEBUG("Insert a color using an integer code:");
    int user_input2;
    std::cin >> user_input2;
    Color selected2 = Color::fromInt(user_input2);
    LOG_DEBUG("You selected:", std::string(selected2), int(selected2));

    LOG_DEBUG("Insert a color using an integer code or name:");
    // Color selected2 = Color::fromString(user_input2);
    std::cin >> user_input3;
    Color selected3 = Color::fromString(user_input3);
    LOG_DEBUG("You selected:", std::string(selected3), int(selected3));

    Color myenum = Color::GREEN;

    Color myenum2 = Color::FIVE;

    LOG_DEBUG(myenum.toString(), int(myenum));
    LOG_DEBUG(myenum);

    switch (myenum)
    {
    case Color::GREEN:
        LOG_SUCCESS("Green!");
        break;
    case Color::FIVE:
        LOG_SUCCESS(5);
        break;

    default:
        break;
    }
}