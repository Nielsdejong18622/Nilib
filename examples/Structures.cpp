#include "Nilib/Structures/Circular.hpp"
#include "Nilib/Structures/StackVector.hpp"
#include "Nilib/Logger/Log.hpp"

struct SpecialData
{
    float reward;
    SpecialData()
        : reward(10.0f)
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
        CircularStack<int, 5> buffer(0);

        LOG_DEBUG("Printing buffer", &buffer, "size:", buffer.size(), ':', buffer);
        CORE_ASSERT(buffer.head == 0);
        buffer.push_back(1);
        CORE_ASSERT(buffer.head == 1);

        buffer.push_back(2);
        CORE_ASSERT(buffer.head == 2);
        buffer.push_back(3);
        CORE_ASSERT(buffer.head == 3);
        auto node3 = buffer.pop_back();
        CORE_ASSERT(buffer.head == 2);
        CORE_ASSERT(node3 == 3);
        buffer.push_back(4);
        buffer.push_back(5);
        buffer.push_back(6);
        buffer.push_back(7);
        // CORE_ASSERT(buffer.head == );
        auto node7 = buffer.pop_back();
        CORE_ASSERT(node7 == 7);

        LOG_DEBUG("Printing buffer", &buffer, "size:", buffer.size(), ':', buffer);
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

        LOG_DEBUG("Printing buffer", &buffer, "size:", buffer.size(), ':', buffer);
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
}