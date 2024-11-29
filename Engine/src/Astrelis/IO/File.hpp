#pragma once

#include "Astrelis/Core/Result.hpp"

#include <cereal/archives/binary.hpp>
#include <filesystem>
#include <fstream>
#include <type_traits>
#include <utility>
#include <vector>

namespace Astrelis {

    template<typename T>
    concept BinarySerializable = requires(T obj, cereal::BinaryOutputArchive archive) {
                                     { obj.template serialize(archive) };
                                 };

    template<typename T>
    concept BinaryDeserializable = requires(T obj, cereal::BinaryInputArchive archive) {
                                       { obj.template serialize(archive) };
                                   };

    class File {
    public:
        explicit File(const char* path) : m_Path(path) {
        }

        explicit File(std::string path) : m_Path(std::move(path)) {
        }

        explicit File(const std::wstring& path) : m_Path(path) {
        }

        explicit File(std::filesystem::path path) : m_Path(std::move(path)) {
        }

        bool Exists() const;
        bool IsDirectory() const;
        bool IsRegularFile() const;
        bool CanRead() const;
        bool CanWrite() const;
        bool CanExecute() const;

        const std::filesystem::path& GetPath() const;
        std::filesystem::path        GetAbsolutePath() const;
        std::filesystem::path        GetParentPath() const;
        std::filesystem::path        GetRelativePath(const File& other) const;

        std::string GetFilename() const;
        std::string GetStem() const;
        std::string GetExtension() const;

        /**
        * @brief Check if the file can be read from, by checking if it exists and is a regular file, and has permissions to read.
        * @return True if the file can be read from, false otherwise.
        */
        bool                                   CanReadFromFile() const;
        Result<std::string, std::string>       ReadText(bool checking = true) const;
        Result<std::vector<char>, std::string> ReadBinary(bool checking = true) const;

        template<BinarySerializable T>
        Result<EmptyType, std::string> WriteBinaryStructure(const T& data) const {
            std::ofstream file(GetPath(), std::ios::binary);
            if (!file.is_open()) {
                return "Failed to open file for writing";
            }

            cereal::BinaryOutputArchive archive(file);
            archive(data);

            return Result<EmptyType, std::string>::Ok();
        }

        // Deserializing
        template<BinaryDeserializable T> Result<T, std::string> ReadBinaryStructure() const {
            std::ifstream file(GetPath(), std::ios::binary);
            if (!file.is_open()) {
                return "Failed to open file for reading";
            }

            cereal::BinaryInputArchive archive(file);
            T                          data;
            archive(data);

            return Result<T, std::string>::Ok(std::move(data));
        }

        std::ofstream Open(std::ios_base::openmode mode) const {
            return std::ofstream(GetPath(), mode);
        }

        std::vector<File> ListFiles() const;
        File              operator/(const File& other) const;

        bool operator==(const File& other) const noexcept;
        bool operator!=(const File& other) const noexcept;
    private:
        std::filesystem::path m_Path;
    };
} // namespace Astrelis
