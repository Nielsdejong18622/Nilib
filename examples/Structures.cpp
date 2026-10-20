#include "Nilib/Logger/Log.hpp"
#include "Nilib/Structures/Circular.hpp"

struct SpecialData
{
    float reward;
    SpecialData() : reward(10.0f)
    {
    }
    // Friend allows access to private members (if needed)
    friend std::ostream &operator<<(std::ostream &os, SpecialData const &special)
    {
        return os << '[' << special.reward << ']';
    }
};

using namespace Nilib;
int main()
{
    {
        CircularStack<int, 5> buffer(-1);

        LOG_DEBUG("Printing buffer {} capacity: {}:{}", fmt_ptr(&buffer), buffer.capacity(), fmt_range(buffer));
        buffer.push_back(0);
        buffer.push_back(1);
        buffer.push_back(2);
        buffer.push_back(3);
        buffer.push_back(4);
        buffer.push_back(5);
        auto node3 = buffer.pop_back();
        CORE_ASSERT(node3 == 5);
        buffer.push_back(50);
        LOG_DEBUG("Printing buffer {} capacity: {}:{}", fmt_ptr(&buffer), buffer.capacity(), fmt_range(buffer));
        auto node50 = buffer.pop_back();
        CORE_ASSERT(node50 == 50);
        buffer.push_back(4);
        buffer.push_back(5);
        buffer.push_back(6);
        buffer.push_back(7);
        auto node7 = buffer.pop_back();
        CORE_ASSERT(node7 == 7);
        LOG_DEBUG("Printing buffer {} capacity: {}:{}", fmt_ptr(&buffer), buffer.capacity(), fmt_range(buffer));
    }

    {
        CircularStack<char, 4> buffer;
        buffer.pop_back();
        buffer.push_back('a');
        buffer.push_back('b');
        buffer.push_back('c');
        buffer.push_back('d');
        buffer.pop_back();
        buffer.push_back('e');

        LOG_DEBUG("Printing buffer {} capacity: {}:{}", fmt_ptr(&buffer), buffer.capacity(), fmt_range(buffer));
    }

    LOG_SUCCESS("Structure Example completed!");
    return EXIT_SUCCESS;
}