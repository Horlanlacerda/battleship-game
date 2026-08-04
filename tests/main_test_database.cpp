#include <iostream>
#include "db/Database.h"

int main() {
    std::cout << "=== Teste manual da classe Database ===" << std::endl;

    Database db("test_ranking.db");

    // Insere algumas pontuacoes de teste
    db.saveResult({"Gabi", 950, 120, "POCA"});
    db.saveResult({"Georis", 800, 95, "POCA"});
    db.saveResult({"Horlan", 950, 100, "POCA"});
    db.saveResult({"Suelle", 950, 95, "POCA"});// mesmo score, tempo menor -> deve vir antes de Gabi

    std::cout << std::endl << "--- Top scores do mapa POCA ---" << std::endl;
    auto results = db.getTopScores("POCA", 10);

    for (const auto& entry : results) {
        std::cout << entry.playerName << " | Score: " << entry.score
                   << " | Tempo: " << entry.elapsedSeconds << "s" << std::endl;
    }

    return 0;
}