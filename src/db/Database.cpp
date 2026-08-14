#include "Database.h"
#include <sqlite3.h>
#include <stdexcept>

Database::Database(const std::string& filepath) {
    if (sqlite3_open(filepath.c_str(), (sqlite3**)&db) != SQLITE_OK)
        throw std::runtime_error("Nao foi possivel abrir o banco de dados.");
    createTableIfNeeded();
}

// DESTRUTOR, QUE MASSA
// Serve para evitar vazamento de dados
Database::~Database() {
    sqlite3_close((sqlite3*)db);
}

void Database::createTableIfNeeded() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS ranking ("
        "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  player_name TEXT    NOT NULL,"
        "  score       INTEGER NOT NULL,"
        "  elapsed_sec INTEGER NOT NULL,"
        "  map_type    TEXT    NOT NULL"
        ");";

    char* errMsg = nullptr;
    if (sqlite3_exec((sqlite3*)db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error("Erro ao criar tabela: " + err);
    }
}

void Database::saveResult(const RankingEntry& entry) {
    const char* sql =
        "INSERT INTO ranking (player_name, score, elapsed_sec, map_type) "
        "VALUES (?, ?, ?, ?);";    // Prepared Statements

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, entry.playerName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 2, entry.score);
    sqlite3_bind_int (stmt, 3, entry.elapsedSeconds);
    sqlite3_bind_text(stmt, 4, entry.mapType.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<RankingEntry> Database::getTopScores(const std::string& mapType, int limit) {
    const char* sql =
        "SELECT player_name, score, elapsed_sec, map_type "
        "FROM ranking "
        "WHERE map_type = ? "
        "ORDER BY score DESC, elapsed_sec ASC "
        "LIMIT ?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, mapType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 2, limit);

    std::vector<RankingEntry> results;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RankingEntry e;
        e.playerName     = (const char*)sqlite3_column_text(stmt, 0);
        e.score          = sqlite3_column_int (stmt, 1);
        e.elapsedSeconds = sqlite3_column_int (stmt, 2);
        e.mapType        = (const char*)sqlite3_column_text(stmt, 3);
        results.push_back(e);
    }
    sqlite3_finalize(stmt);
    return results;
}