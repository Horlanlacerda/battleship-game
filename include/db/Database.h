#pragma once

#include <string>
#include <vector>

/**
 * @brief Representa uma entrada individual do ranking de pontuação.
 * @details Agrupa os dados de uma partida finalizada: nome do jogador,
 * pontuação obtida, tempo decorrido (usado como critério de desempate)
 * e o tipo de mapa jogado (para segmentar rankings por mapa).
 */
struct RankingEntry {
    std::string playerName;     ///< Nome do jogador que registrou a pontuação.
    int         score;          ///< Pontuação final da partida.
    int         elapsedSeconds; ///< Tempo total da partida, em segundos (critério de desempate).
    std::string mapType;        ///< Tipo de mapa jogado ("ACUDE", "LAGO" ou "OCEANO").
};

/**
 * @brief Gerencia a persistência do ranking de pontuações via SQLite.
 * @details A classe Database encapsula toda a comunicação com o banco
 * SQLite, escondendo os detalhes da API C do sqlite3 (por isso o ponteiro
 * do banco é armazenado como void*, evitando expor <sqlite3.h> no header).
 * Cria automaticamente a tabela de ranking, caso não exista, ao ser construída.
 * @author Gabi
 * @version 1.0
 * @date 30/07/2026 
 */
class Database {
public:
    /**
     * @brief Abre (ou cria) o arquivo de banco de dados SQLite.
     * @param filepath Caminho do arquivo .db. Padrão: "ranking.db".
     * @throws std::runtime_error se o banco não puder ser aberto.
     */
    Database(const std::string& filepath = "ranking.db");

    /**
     * @brief Fecha a conexão com o banco de dados.
     */
    ~Database();

    /**
     * @brief Salva uma nova entrada de ranking no banco.
     * @param entry Dados da partida a serem persistidos.
     */
    void saveResult(const RankingEntry& entry);

    /**
     * @brief Retorna as melhores pontuações de um mapa específico.
     * @details Os resultados são ordenados por pontuação decrescente,
     * usando o menor tempo como critério de desempate.
     * @param mapType Tipo de mapa a filtrar ("ACUDE", "LAGO" ou "OCEANO").
     * @param limit Número máximo de entradas retornadas (padrão: 10).
     * @return Vetor de RankingEntry, do melhor para o pior colocado.
     */
    std::vector<RankingEntry> getTopScores(const std::string& mapType, int limit = 10);

private:
    void* db; ///< Ponteiro opaco para sqlite3* — evita expor sqlite3.h no header.

    /**
     * @brief Cria a tabela de ranking, caso ainda não exista.
     */
    void createTableIfNeeded();
};