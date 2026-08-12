#include <iostream>
#include "db/Database.h"

int main() {
    std::cout << "=== Teste manual da classe Database ===" << std::endl;

    Database db("test_ranking.db");

    // Insere algumas pontuacoes de teste
    db.saveResult({"Gabi", 950, 120, "ACUDE"});
    db.saveResult({"Georis", 800, 95, "ACUDE"});
    db.saveResult({"Horlan", 950, 100, "ACUDE"});
    db.saveResult({"Suelle", 950, 95, "ACUDE"});// mesmo score, tempo menor -> deve vir antes de Gabi

    std::cout << std::endl << "--- Top scores do mapa ACUDE ---" << std::endl;
    auto results = db.getTopScores("ACUDE", 10);

    for (const auto& entry : results) {
        std::cout << entry.playerName << " | Score: " << entry.score
                   << " | Tempo: " << entry.elapsedSeconds << "s" << std::endl;
    }

    return 0;
}