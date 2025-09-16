#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <type_traits>
#include <vector>

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Math/Matrix.hpp"
#include "Nilib/ML/CNodes/CNode.hpp"

class Serializer
{
    std::ofstream file;

public:
    // Close the file to serialize to, freeing resources.
    void close()
    {
        if (file.is_open())
        {
            file.flush();
            file.close();
        }
    }

    ~Serializer()
    {
        // Automatically close the file on destruction of the Serializer.
        close();
    }

    bool opened() const { return file.is_open() && !file.bad(); }

    explicit Serializer(std::filesystem::path const &filename)
        : file(filename, std::ios::binary | std::ios ::out)
    {
        if (!file)
        {
            LOG_ERROR() << "Serialization Error: Could not open file " << filename << " for writing!\n";
        }
    }

    // Write raw data to a binary file
    template <typename T>
    bool writeRaw(T const &data)
    {
        static_assert(std::is_trivially_copyable<T>::value, "Object is not trivially copyable!");
        file.write(reinterpret_cast<char const *>(&data), sizeof(T));
        if (!file)
        {
            LOG_ERROR() << "Error: Failed to write raw data to file.\n";
        }
        return !file;
    }

    // Write custom object to a binary stream.
    template <typename T>
    bool writeObject(T const &data)
    {
        if constexpr (std::is_trivially_copyable<T>::value)
        {
            writeRaw(data);
        }
        else
        {
            data.serialize(*this);
        }
        if (!file)
        {
            LOG_ERROR() << "Error: Failed to write Object data to file.\n";
        }
        close();
        return !file;
    }

    // Specialization for matrix.
    template <typename T>
    bool writeMatrix(Nilib::Matrix<T> const &data)
    {
        size_t n = data.rows();
        size_t m = data.cols();
        file.write(reinterpret_cast<char const *>(&n), sizeof(n));
        file.write(reinterpret_cast<char const *>(&m), sizeof(m));
        // Use the conversion operator to write the vector data.
        for (size_t i = 0; i < n * m; i++)
        {
            auto entry = data(i);
            file.write(reinterpret_cast<char const *>(&entry), sizeof(entry));
        }
        return true;
    }
    // Specialization for CNode.
    bool writeCNode(Nilib::CNode &data)
    {
        writeMatrix(data.value);
        return true;
    }

    // Specialization for std::string
    bool writeString(std::string const &data)
    {
        size_t length = data.size();
        file.write(reinterpret_cast<char const *>(&length), sizeof(length));
        file.write(data.c_str(), length);
        file.close();
        if (!file)
        {
            LOG_ERROR() << "Error: Failed to write String data to file.\n";
        }
        return !file;
    }

    // Specialization for std::vector<T>
    template <typename T>
    bool writeVector(std::vector<T> const &vec)
    {
        // Write the size of the vector first
        size_t size = vec.size();
        writeRaw(size);

        // If trivially copyable, write the entire data block at once
        if constexpr (std::is_trivially_copyable<T>::value)
        {
            file.write(reinterpret_cast<const char *>(vec.data()), size * sizeof(T));
        }
        else
        {
            // For non-trivially copyable types, serialize each element
            for (const auto &element : vec)
            {
                writeObject(element);
            }
        }
        if (!file)
        {
            LOG_ERROR() << "Error: Failed to write Vector data to file.\n";
        }
        return !file;
    }
};

#endif