#ifndef _DESERIALIZER_H
#define _DESERIALIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Math/Matrix.hpp"
#include "Nilib/ML/CNodes/CNode.h"

class Deserializer
{
    std::ifstream file;

public:
    explicit Deserializer(std::string const &filename)
        : file(filename, std::ios::binary)
    {
        if (!file.is_open())
        {
            LOG_ERROR("Deserialization Error: Could not open file", filename, "for reading!");
        }
        if (file.bad())
        {
            LOG_ERROR("File has gone bad!");
        }
    }

    void close()
    {
        if (file.is_open())
        {
            file.close();
        }
    }

    ~Deserializer()
    {
        close();
    }

    template <typename T>
    bool readRaw(T &data)
    {
        static_assert(std::is_trivially_copyable<T>::value, "Object is not trivially copyable!");
        file.read(reinterpret_cast<char *>(&data), sizeof(T));
        if (!file)
        {
            LOG_ERROR("Error: Failed to read", sizeof(T), "bytes of Raw data from file into", &data);
        }
        return !file;
    }

    template <typename T>
    bool readObject(T &data)
    {
        if constexpr (std::is_trivially_copyable<T>::value)
        {
            readRaw(data);
        }
        else
        {
            data.deserialize(*this);
        }
        if (!file)
        {
            LOG_ERROR("Error: Failed to read Object data from file.");
        }
        return !file;
    }

    // Read a string from stream.
    bool readString(std::string &data)
    {
        size_t length = 0;
        file.read(reinterpret_cast<char *>(&length), sizeof(length));
        data.resize(length);
        file.read(&data[0], length);
        if (!file)
        {
            LOG_ERROR("Error: Failed to read String data from file.");
        }
        return !file;
    }

    // Specialization for Matrix.
    template <typename T>
    bool readMatrix(Nilib::Matrix<T> &data)
    {
        size_t n, m;
        readRaw(n);
        readRaw(m);
        LOG_DEBUG("Reading Matrix", n, 'x', m);
        data = Nilib::Matrix<T>(n, m);
        // Use the conversion operator to write the vector data.
        for (size_t i = 0; i < n * m; i++)
        {
            readRaw(data(i));
        }
        return true;
    }

    // Specialization for CNode.
    bool readCNode(Nilib::CNode &data)
    {
        LOG_DEBUG("Reading CNode");
        readMatrix(data.value);
        return true;
    }

    // Specialization for std::vector<T>
    template <typename T>
    bool readVector(std::vector<T> &vec)
    {
        // Read the size of the vector
        size_t size;
        readRaw(size);
        LOG_DEBUG("Reading vector size", size);
        // Resize the vector to hold the data
        vec.resize(size);

        // If trivially copyable, read the entire data block at once
        if constexpr (std::is_trivially_copyable<T>::value)
        {
            file.read(reinterpret_cast<char *>(vec.data()), size * sizeof(T));
        }
        else
        {
            // For non-trivially copyable types, deserialize each element
            for (auto &element : vec)
            {
                readObject(element);
            }
        }
        if (!file)
        {
            LOG_ERROR("Error: Failed to read Vector data from file.");
        }
        return !file;
    }

    // Static methods for one shot encoding!
    /*
    // Read data from a binary file
    template <typename T>
    static bool read(T& data, std::string const &filename)
    {
        static_assert(std::is_trivially_copyable<T>::value, "Data type must be trivially copyable!");

        std::ifstream file(filename, std::ios::binary | std::ios::in);
        if (!file) {
            std::cerr << "Error: Could not open file for reading." << std::endl;
            return false;
        }

        file.read(reinterpret_cast<char*>(&data), sizeof(T));
        file.close();
        return true;
    }

    // Specialization for std::string
    static bool read(std::string& data, std::string const &filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::in);
        if (!file) {
            std::cerr << "Error: Could not open file for reading." << std::endl;
            return false;
        }

        size_t length = 0;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        data.resize(length);
        file.read(&data[0], length);
        file.close();
        return true;
    }
    */
};

#endif