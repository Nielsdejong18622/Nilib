#include "Nilib/Logger/CSV.hpp"

int main()
{
    int a = 5;
    int b, c;
    // First call will insert the header " a, b, c" exactly as typed on line 7, and the values of the variables.
    CSV("test.csv", a, b, c); 
    
    // Subsequent calls will dump the variables into the respective columns. 
    CSV("test.csv", a, b, c);
    CSV("test.csv", a, b, c);

    // Very handy when you want to do this:
    for (size_t iteration = 0; iteration < 10; ++iteration)
    {
        size_t iteration2 = iteration * iteration;
        CSV("Iteration.csv",iteration, iteration2);
    }
    

}