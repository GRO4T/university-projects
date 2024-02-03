/*
 * Project: TinyAd
 * Description: Example showing how to use storage class
 * Author: Damian Kolaska
 * Created: 1.06.2021
 */

#include <iostream>

#include "storage.hpp"

using namespace TinyAd;

int main(int argc, char** argv) {
    loggerInit(argc, argv, "database_client.log");
    try {
        Storage storage;
        storage.createTables();
        MultimediaHeader header(0, 123, 456, "test_insert_safe");
        storage.insert(header);
        MultimediaData data(0, 1, "test_data");
        storage.insert(data);
        MultimediaHeader multimediaHeader;
        storage.findByID(multimediaHeader, 1);
        DLOG_S(INFO) << multimediaHeader;
        MultimediaData multimediaData;
        storage.findByID(multimediaData, 1);
        DLOG_S(INFO) << multimediaData;
    } catch (std::runtime_error& e) {
        LOG_S(ERROR) << e.what();
    }
}