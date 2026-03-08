#include "Nilib/Logger/Log.hpp"
#include "Nilib/Structures/Circular.hpp"
#include "Nilib/Structures/StackVector.hpp"

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

int main()
{
    using namespace Nilib;
    {

        CircularStack<int, 5> buffer(-1);

        LOG_DEBUG("Printing buffer", &buffer, "capacity:", buffer.capacity(), ':', buffer);
        buffer.push_back(0);
        buffer.push_back(1);
        buffer.push_back(2);
        buffer.push_back(3);
        buffer.push_back(4);
        buffer.push_back(5);
        LOG_DEBUG("Printing buffer", &buffer, "capacity:", buffer.capacity(), ':', buffer);
        auto node3 = buffer.pop_back();
        CORE_ASSERT(node3 == 5);
        buffer.push_back(50);
        LOG_DEBUG("Printing buffer", &buffer, "capacity:", buffer.capacity(), ':', buffer, node3);
        auto node50 = buffer.pop_back();
        CORE_ASSERT(node50 == 50);
        buffer.push_back(4);
        buffer.push_back(5);
        buffer.push_back(6);
        buffer.push_back(7);
        auto node7 = buffer.pop_back();
        CORE_ASSERT(node7 == 7);

        LOG_DEBUG("Printing buffer", &buffer, "capacity:", buffer.capacity(), ':', buffer);
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

        LOG_DEBUG("Printing buffer", &buffer, "capacity:", buffer.capacity(), ':', buffer);
    }

    // Stackvector.
    {
        // Sizeof(vec) = 4 * size_t + sizeof(array)
        StackVector<char, 8> vec;

        vec.push_back(3);
        vec.push_back(4);
        vec.push_back(5);
        vec.push_back(6);
        vec.push_back(7);
        vec.push_back(8);

        vec.pop_back();

        vec.clear();

        LOG_DEBUG("Printing Stackvector", &vec, "size:", vec.size(), "elements,", sizeof(vec), "bytes", "vector:", vec);
    }

    LOG_SUCCESS("Structure Example completed!");
    return EXIT_SUCCESS;
}