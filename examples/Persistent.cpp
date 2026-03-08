#include "Nilib/Structures/Persistent.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Math/RNG.hpp"

int main()
{
    using namespace Nilib;
    // PersistentIndexVector<char> pvec(10);

    // LOG_DEBUG(pvec.size());
    // uint16_t a = pvec.add('a');
    // uint16_t b = pvec.add('b');
    // uint16_t c = pvec.add('c');
    // uint16_t d = pvec.add('d');
    // pvec.remove('c');
    // pvec.remove(a);
    // uint16_t e = pvec.add('e');

    // CORE_ASSERT(pvec[e] == 'e');

    // LOG_DEBUG(pvec);
    // std::fill(pvec.begin(), pvec.end(), '1');
    // pvec.shrink_to_fit();
    // LOG_DEBUG(pvec);

    PIndexVector<char> pvec;

    pvec.reserve(5);
    pvec.add('a');  // ID: 0
    pvec.add('b');  // ID: 1
    pvec.add('c');  // ID: 2
    pvec.add('d');  // ID: 3
    pvec.add('e');  // ID: 4
    pvec.add('f');  // ID: 5
    pvec.add('g');  // ID: 6
    pvec.remove(1); // Remove 'b', which swaps g to the 1st position.

    CORE_ASSERT(pvec[6] == 'g')      // When we use ID: 6, we still want 'g'
    CORE_ASSERT(pvec.add('h') == 1); // Recycle index of 'b'
    CORE_ASSERT(pvec.add('i') == 7);

    pvec.remove(0); // Removes a
    pvec.remove(1); // Removes h
    pvec.remove(3); // Removes d
    pvec.remove(2); // Removes c
    CORE_ASSERT(pvec[7] == 'i');

    // LOG_DEBUG("pvec.data:", pvec.d_data);
    // LOG_DEBUG("pvec.head:", pvec.d_head);
    // LOG_DEBUG("pvec.tail:", pvec.d_tail);

    for (auto &&letter : pvec)
    {
        LOG_INFO(letter);
        letter = 'B';
    }
    // pvec[1] = pvec[2]; // Not smart!
    // std::sort(pvec.begin(), pvec.end()); // Not smart! Breaks the invariants.

    LOG_DEBUG("pvec:", pvec);
    RNG::seed();
    for (size_t id_tes = 0; id_tes < 10'000; id_tes++)
    {
        if (RNG::prob() > 0.5)
            pvec.add(RNG::rand() % 50 + 32);

        auto index = RNG::index(pvec.size());
        if (pvec.contains(index))
        {
            pvec.remove(index);
        }
    }
    LOG_DEBUG(pvec);
    LOG_SUCCESS("Completed Persistent Example!");

    return EXIT_SUCCESS;
}