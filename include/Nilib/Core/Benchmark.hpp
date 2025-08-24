#ifndef _BENCHMARK_HPP
#define _BENCHMARK_HPP

#include "Nilib/Core/Profiler.hpp"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Logger/CSV.hpp"

#include <chrono>

namespace Nilib
{

    class BenchMarkEnv
    {
        size_t d_iterations = 1000;
        size_t d_bench_marks = 10;

        std::string d_filename = "";

    public:
        BenchMarkEnv(size_t iterations_per_bench, size_t bench)
            : BenchMarkEnv(iterations_per_bench, bench, "") {
              }

        BenchMarkEnv(size_t iterations_per_bench, size_t bench, std::string const &filename)
            : d_iterations(iterations_per_bench), d_bench_marks(bench), d_filename(filename) {
              }

        BenchMarkEnv(BenchMarkEnv const &other) = delete;

        // Individual benchmark run
        template <typename Func, typename... Args>
        void function(std::string const &name, Func &&func, Args... args)
        {
            size_t const iterations = d_iterations;
            for (size_t bench = 1; bench < d_bench_marks + 1; bench++)
            {
                auto start = std::chrono::high_resolution_clock::now();

                for (size_t i = 0; i < iterations; ++i)
                    func(std::forward<Args>(args)...);

                auto end = std::chrono::high_resolution_clock::now();
                auto total = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                double avg_ns = static_cast<double>(total) / iterations;

                if (d_filename != "")
                {
                    CSV(d_filename.c_str(),name,iterations,total,avg_ns,bench);
                }
                else
                {
                    LOG_DEBUG("Function:", name, "| Avg Time:", avg_ns, "ns");
                }
            }
        }
    };

} // namespace Nilib

#endif