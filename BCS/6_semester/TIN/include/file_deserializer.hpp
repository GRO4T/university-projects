/*
 * Project: TinyAd
 * Description: Class for creating files from MultimediaHeader and MultimediaData
 * Author: Rafal Kulus
 * Created: 26.05.2021
 */

#ifndef FILE_DESERIALIZER_HPP
#define FILE_DESERIALIZER_HPP

#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "message.hpp"

namespace TinyAd {

class FileDeserializer {
public:
    FileDeserializer(const MultimediaHeader& header, const std::string& dir_path);
    ~FileDeserializer();

    void addPart(const MultimediaData& data);

    bool isCompleted() const;

    std::string getFilePath() const;

    std::vector<uint> getRemainingParts() const;
    size_t getRemainingPartsCount() const;

    uint32_t getAllPartsCount() const;

private:
    std::string file_path_;
    std::ofstream file_;
    uintmax_t size_;

    uint32_t parts_total_;

    std::set<uint> remaining_parts_;
};

}  // namespace TinyAd

#endif