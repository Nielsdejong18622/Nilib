#include "Nilib/Structures/Persistent.hpp"
#include "Nilib/Logger/Log.hpp"
using namespace Nilib;

int main()
{

    PersistentIndexVector<char> pvec(10);

    LOG_DEBUG(pvec.size());
    PersistentIndexVector<char>::index a = pvec.add('a');
    PersistentIndexVector<char>::index b = pvec.add('b');
    PersistentIndexVector<char>::index c = pvec.add('c');
    PersistentIndexVector<char>::index d = pvec.add('d');
    pvec.remove('c');
    pvec.remove(a);
    PersistentIndexVector<char>::index e = pvec.add('e');

    CORE_ASSERT(pvec[e] == 'e');

    
    LOG_DEBUG(pvec);
    std::fill(pvec.begin(), pvec.end(), '1');
    pvec.shrink_to_fit();
    LOG_DEBUG(pvec);


    LOG_SUCCESS("Completed Persistent Example!");
}