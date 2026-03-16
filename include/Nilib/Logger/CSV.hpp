#ifndef _CSV_HPP
#define _CSV_HPP

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Core/Memory.hpp"

#include <filesystem>
#include <fstream>

namespace Nilib
{
    // Class that makes writing to a CSV file easier.
    class CSVWriter
    {

    public:
        // Base case for the recursion.
        void write_row() {}

        template <typename T, typename... Args>
        void write_row(T &&first, Args &&...args)
        {
            ASSERT(d_file.is_open(), "Open the CSVWriter file first! Use CSVWriter.open('filename.csv')!");
            d_header_inserted = true;

            // CSV'ing of custom types such as ranges:
            if constexpr (is_range<T>)
            {
                bool first_elem = true;
                for (auto &&elem : first)
                {
                    if (!first_elem)
                        d_file << ',';
                    d_file << elem;
                    first_elem = false;
                }
            }
            else
            {
                d_file << first;
            }
            if constexpr (sizeof...(args) > 0)
            {                                                  // Check if there are more arguments
                d_file << ',';                                 // Print separator
                return write_row(std::forward<Args>(args)...); // Template recurse
            }
            else
            {
                d_file << '\n';
            }
        }

    public:
        CSVWriter() = default;

        // Opens the file immediately.
        CSVWriter(std::filesystem::path const &name)
            : d_name(name.string()), d_file(name, std::ios_base::trunc), d_header_inserted(false) {

              };

        ~CSVWriter()
        {
            if (d_file.is_open())
            {
                close();
                uintmax_t const size = std::filesystem::file_size(d_name);
                LOG_PROGRESS("Closed CSVfile:", d_name, Nilib::format_bytes(size));
            }
        }

        void flush() noexcept
        {
            d_file.flush();
        }

        void close() noexcept
        {
            flush();
            d_file.close();
        }

        void open(std::filesystem::path const &name)
        {
            close();
            d_header_inserted = false;
            d_name = name.string();
            d_file.open(d_name, std::ios_base::trunc);
        }

        std::string const &file_name()
        {
            return d_name;
        }

        bool is_open() const
        {
            return d_file.is_open();
        }
        bool header_inserted() const
        {
            return d_header_inserted;
        }

    private:
        std::string d_name;
        std::ofstream d_file;
        bool d_header_inserted;
    };
}; // namespace Nilib

// MACRO for dumping variables to CSVfile. Automatically inserts header based on variable names.
#define CSV_STRINGIFY(...) #__VA_ARGS__
#define CSV(CSVwriter, ...)                              \
    if (!CSVwriter.header_inserted())                    \
    {                                                    \
        CSVwriter.write_row(CSV_STRINGIFY(__VA_ARGS__)); \
    }                                                    \
    CSVwriter.write_row(__VA_ARGS__);

#endif