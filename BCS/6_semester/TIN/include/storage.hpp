/*
 * Project: TinyAd
 * Description: ORM for SQLite
 * Author: Damian Kolaska
 * Created: 01.06.2021
 */

#ifndef TINYAD_STORAGE_HPP
#define TINYAD_STORAGE_HPP

#include <exception>

#include "common.hpp"
#include "message.hpp"
#include "sqlite3.h"

namespace TinyAd {

class Storage {
public:
    Storage() {
        int result = sqlite3_open(DB_NAME.c_str(), &db);
        if (result)
            throw new std::runtime_error(
                fmt::format("Can't open database: {}\n", sqlite3_errmsg(db)));
        LOG_S(INFO) << "Successfully opened database\n";
    }

    void createTables() {
        createTable(
            "CREATE TABLE FILE_HEADER("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "FILENAME           TEXT    NOT NULL,"
            "FILESIZE            INT     NOT NULL,"
            "CHUNKS            INT     NOT NULL );");

        createTable(
            "CREATE TABLE FILE_DATA("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "CHUNK_NUM           INT    NOT NULL,"
            "DATA            TEXT     NOT NULL,"
            "CRC            INT     NOT NULL );");
    }

    void createTable(const std::string& sql) {
        char* err_msg;
        int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
        if (result != SQLITE_OK) {
            LOG_S(ERROR) << fmt::format("SQL error: {}\n", err_msg);
            sqlite3_free(err_msg);
        } else {
            LOG_S(INFO) << "Table created successfully\n";
        }
    }

    template <class... ParamTypes>
    void insert(const std::string& sql, ParamTypes... sql_params) {
        sqlite3_stmt* statement;
        prepareStatement(&statement, sql, sql_params...);
        int result = sqlite3_step(statement);
        sqlite3_finalize(statement);
        if (result != SQLITE_DONE) {
            const std::string exc_msg = fmt::format("SQL error: {}\n", sqlite3_errmsg(db));
            throw std::runtime_error(exc_msg);
        } else {
            DLOG_S(INFO) << "Prepared statement executed successfully\n";
        }
    }

    template <class... ParamTypes>
    std::vector<std::vector<std::string>> select(const std::string& sql, ParamTypes... sql_params) {
        std::vector<std::vector<std::string>> query_result;
        sqlite3_stmt* statement;
        prepareStatement(&statement, sql, sql_params...);
        int result = 0;
        while (true) {
            result = sqlite3_step(statement);
            if (result == SQLITE_DONE) break;
            if (result == SQLITE_ERROR) {
                const std::string exc_msg = fmt::format("SQL error: {}\n", sqlite3_errmsg(db));
                sqlite3_finalize(statement);
                throw std::runtime_error(exc_msg);
            }
            std::vector<std::string> row;
            for (int i = 0; i < sqlite3_column_count(statement); ++i) {
                row.push_back(std::string((char*)sqlite3_column_text(statement, i)));
            }
            query_result.push_back(row);
        }
        sqlite3_finalize(statement);
        return query_result;
    }

    void insert(const MultimediaHeader& data) {
        std::string sql =
            "INSERT INTO FILE_HEADER (FILENAME, FILESIZE, CHUNKS) "
            "VALUES (?, ?, ?);";
        insert(sql, data.filename, data.filesize, data.num_chunks);
    }

    void insert(const MultimediaData& data) {
        std::string sql =
            "INSERT INTO FILE_DATA (CHUNK_NUM, DATA, CRC) "
            "VALUES (?, ?, ?);";
        insert(sql, data.chunk_num, data.data, data.crc);
    }

    void findByID(MultimediaHeader& multimediaHeader, int id) {
        const std::string sql = "SELECT * FROM FILE_HEADER WHERE ID = ?;";
        auto result = select(sql, id);
        for (auto row : result) {
            multimediaHeader.id = std::stoi(row[0]);
            multimediaHeader.filename = row[1];
            multimediaHeader.filesize = std::stoi(row[2]);
            multimediaHeader.num_chunks = std::stoi(row[3]);
        }
    }

    void findByID(MultimediaData& multimediaData, int id) {
        const std::string sql = "SELECT * FROM FILE_DATA WHERE ID = ?;";
        auto result = select(sql, id);
        for (auto row : result) {
            multimediaData.id = std::stoi(row[0]);
            multimediaData.chunk_num = std::stoi(row[1]);
            multimediaData.data = row[2];
            multimediaData.crc = std::stoi(row[3]);
        }
    }

    ~Storage() { sqlite3_close(db); }

private:
    const std::string DB_NAME = "tinyad.db";
    sqlite3* db;

    template <class... ParamTypes>
    void prepareStatement(sqlite3_stmt** statement, const std::string& sql,
                          ParamTypes... sql_params) {
        int result = sqlite3_prepare_v2(db, sql.c_str(), -1, statement, nullptr);
        if (result != SQLITE_OK) {
            const std::string exc_msg = fmt::format("SQL error: {}\n", sqlite3_errmsg(db));
            sqlite3_finalize(*statement);
            throw std::runtime_error(exc_msg);
        } else {
            DLOG_S(INFO) << "Prepared statement created successfully\n";
        }

        bindParams(*statement, 1, sql_params...);
        DLOG_S(INFO) << "Successfully bound query params\n";
    }

    void bindParam(sqlite3_stmt* statement, int index, const std::string& param) {
        sqlite3_bind_text(statement, index, param.c_str(), param.length(), SQLITE_TRANSIENT);
    }

    void bindParam(sqlite3_stmt* statement, int index, int param) {
        sqlite3_bind_int(statement, index, param);
    }

    void bindParams(sqlite3_stmt*, int) {}
    template <class T, class... ParamTypes>
    void bindParams(sqlite3_stmt* statement, int param_index, T param, ParamTypes... params) {
        bindParam(statement, param_index, param);
        LOG_S(INFO) << fmt::format("param index {} {}", param_index, param) << std::endl;
        bindParams(statement, param_index + 1, params...);
    }
};

}  // namespace TinyAd

#endif  // TINYAD_STORAGE_HPP
