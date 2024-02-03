/*
 * Project: TinyAd
 * Description: Class for creating files from MultimediaHeader and MultimediaData
 * Author: Rafal Kulus
 * Created: 26.05.2021
 */

#include "file_deserializer.hpp"

#include <math.h>

#include <algorithm>
#include <filesystem>

#include "exceptions.hpp"

namespace TinyAd {

FileDeserializer::FileDeserializer(const MultimediaHeader& header, const std::string& dir_path) {
    file_path_ = dir_path + std::filesystem::path(header.filename).filename().string();
    size_ = header.filesize;
    parts_total_ = header.num_chunks;

    uint32_t expected_parts = ceil(size_ / (double)MULTIMEDIA_DATA_SIZE);

    if (parts_total_ != expected_parts) {
        throw std::runtime_error("Improper header data!");
    }

    for (uint i = 0; i < header.num_chunks; ++i) {
        remaining_parts_.insert(remaining_parts_.end(), i);
    }

    file_.open(file_path_);

    if (!file_ || !file_.is_open()) {
        throw FileOpenException(file_path_);
    }

    std::filesystem::resize_file(std::filesystem::path(file_path_), size_);
}

FileDeserializer::~FileDeserializer() { file_.close(); }

void FileDeserializer::addPart(const MultimediaData& data) {
    if (isCompleted()) {
        return;
    }

    if (data.chunk_num < parts_total_) {
        file_.seekp(MULTIMEDIA_DATA_SIZE * data.chunk_num, file_.beg);
        file_ << data.data;

        remaining_parts_.erase(data.chunk_num);

        if (getRemainingPartsCount() == 0) {
            file_.close();
        }
    }
}

bool FileDeserializer::isCompleted() const { return remaining_parts_.empty() && !file_.is_open(); }

std::string FileDeserializer::getFilePath() const { return file_path_; }

std::vector<uint> FileDeserializer::getRemainingParts() const {
    return std::vector<uint>(remaining_parts_.begin(), remaining_parts_.end());
}

size_t FileDeserializer::getRemainingPartsCount() const { return remaining_parts_.size(); }

uint32_t FileDeserializer::getAllPartsCount() const { return parts_total_; }

}  // namespace TinyAd
