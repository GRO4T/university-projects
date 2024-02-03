/*
 * Project: TinyAd
 * Description: Class for creating MultimediaHeader and MultimediaData from files
 * Author: Rafal Kulus
 * Created: 26.05.2021
 */

#include "file_serializer.hpp"

#include <filesystem>

#include "common.hpp"
#include "exceptions.hpp"

namespace TinyAd {

FileSerializer::FileSerializer(const std::string& file_path) {
    id_ = uuids::to_string(UuidMaker::Instance().GenUuid());
    filename_ = file_path.substr(file_path.find_last_of('/') + 1);

    file_.open(file_path);

    if (!file_ || !file_.is_open()) {
        throw FileNotFoundException(file_path);
    }

    size_ = std::filesystem::file_size(std::filesystem::path{file_path});
    parts_ = ceil(size_ / (double)MULTIMEDIA_DATA_SIZE);
    next_part_ = 0;
}

FileSerializer::~FileSerializer() { file_.close(); }

uintmax_t FileSerializer::getFileSize() const { return size_; }

uint FileSerializer::getPartCount() const { return parts_; }

uint FileSerializer::getPartsLeft() const { return parts_ - next_part_; }

MultimediaHeader FileSerializer::getFileHeader() const {
    return MultimediaHeader(id_, size_, parts_, filename_);
}

MultimediaData FileSerializer::getNextFileData() { return getFileData(next_part_, true); }

MultimediaData FileSerializer::getFileData(uint part, bool advance) {
    if (advance) {
        ++next_part_;
    }

    uintmax_t read = MULTIMEDIA_DATA_SIZE * part;
    uintmax_t remaining_bytes = size_ - read;

    size_t chunk_size =
        remaining_bytes > MULTIMEDIA_DATA_SIZE ? MULTIMEDIA_DATA_SIZE : remaining_bytes;

    std::string chunk(chunk_size, 'A');

    file_.seekg(read, file_.beg);
    file_.read((char*)chunk.data(), chunk_size);

    return MultimediaData(id_, part, chunk);
}

}  // namespace TinyAd
