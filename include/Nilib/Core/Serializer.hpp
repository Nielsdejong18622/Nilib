#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

#include "Nilib/Core/Log.hpp"

class Serializer 
{
    std::ofstream file;
public:

    // Close the file to serialize to, freeing resources.
    void close()
    {
        if (file.is_open()) {
            file.flush();
            file.close();
        }
    }

    ~Serializer()
    {
        // Automatically close the file on destruction of the Serializer. 
        close();
    }

    explicit Serializer(std::string const &filename)
    {
        file.open(filename, std::ios::binary | std::ios::out);
        if (!file) {
            Log::error("Serialization Error: Could not open file ") << filename << " for writing!\n";
        }
    }
    

    // Write raw data to a binary file
    template <typename T>
    bool writeRaw(T &data) {
        static_assert(std::is_trivially_copyable<T>::value, "Object is not trivially copyable!");
        file.write(reinterpret_cast<char const *>(&data), sizeof(T));
        if (!file) {
           Log::error("Error: Failed to write raw data to file.\n");
        }
        return !file;
    }
    
    // Write custom object to a binary stream. 
    template <typename T>
    bool writeObject(T &data) {
        if constexpr (std::is_trivially_copyable<T>::value)
        {
            writeRaw(data);
        }
        else
        {
            data.serialize(*this);
        }
        if (!file) {
           Log::error("Error: Failed to write Object data to file.\n");
        }
        return !file;
    }


    // Specialization for matrix. 
    // template <typename T>
    // bool write(Matrix<T> const &data) {
    //     Log::debug("Writing Matrix!\n");
    //     size_t n = data.rows();
    //     size_t m = data.cols();
    //     file.write(reinterpret_cast<char const *>(&n), sizeof(n));
    //     file.write(reinterpret_cast<char const *>(&m), sizeof(m));
    //     // Use the conversion operator to write the vector data. 
    //     file.write(reinterpret_cast<char const *>(data.data()), n * m * sizeof(T));
    //     return true;
    // }

    // Specialization for std::string
    bool writeString(std::string const &data) {
        size_t length = data.size();
        file.write(reinterpret_cast<char const *>(&length), sizeof(length));
        file.write(data.c_str(), length);
        file.close();
        if (!file) {
           Log::error("Error: Failed to write String data to file.\n");
        }
        return !file;
    }

    // Specialization for std::vector<T>
    template <typename T>
    bool writeVector(std::vector<T> const &vec) {
        // Write the size of the vector first
        size_t size = vec.size();
        writeRaw(size);

        // If trivially copyable, write the entire data block at once
        if constexpr (std::is_trivially_copyable<T>::value) {
            file.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(T));
        } else {
            // For non-trivially copyable types, serialize each element
            for (const auto& element : vec) {
                writeObject(element);
            }
        }
        if (!file) {
           Log::error("Error: Failed to write Vector data to file.\n");
        }
        return !file;
    }
    // Static methods (for one shot encoding)
    /*
    // Write raw data to a binary file
    template <typename T>
    static bool write(T const &data, std::string const &filename) {
        static_assert(std::is_trivially_copyable<T>::value, "Data type must be trivially copyable!");
        
        std::ofstream file(filename, std::ios::binary | std::ios::out);
        if (!file) {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            return false;
        }

        file.write(reinterpret_cast<char const *>(&data), sizeof(T));
        file.close();
        return true;
    }

    // Specialization for std::string
    static bool write(std::string const &data, std::string const &filename) {
        std::ofstream file(filename, std::ios::binary | std::ios::out);
        if (!file) {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            return false;
        }

        size_t length = data.size();
        file.write(reinterpret_cast<char const *>(&length), sizeof(length));
        file.write(data.c_str(), length);
        file.close();
        return true;
    }
    */
};

#endif