/*
 * Project: TinyAd
 * Description: Class for creating MultimediaHeader and MultimediaData from files
 * Author: Rafal Kulus
 * Created: 26.05.2021
 */

#ifndef FILE_SERIALIZER_HPP
#define FILE_SERIALIZER_HPP

#include <fstream>
#include <string>

#include "message.hpp"

namespace TinyAd {

class FileSerializer {
public:
    FileSerializer(const std::string& file_path);
    ~FileSerializer();

    uintmax_t getFileSize() const;
    uint getPartCount() const;
    uint getPartsLeft() const;

    MultimediaHeader getFileHeader() const;

    MultimediaData getNextFileData();
    MultimediaData getFileData(uint part, bool advance = false);

private:
    std::string id_;

    std::string filename_;
    std::ifstream file_;
    uintmax_t size_;

    uint parts_;
    uint next_part_;
};

}  // namespace TinyAd

#endif