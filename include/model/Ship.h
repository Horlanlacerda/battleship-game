#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <utility>
#include <string>

/** @brief Representa os possíveis tipos de navio da frota. */
enum class ShipType {
    SMALL,  ///< Navio pequeno, ocupa 2 células.
    MEDIUM, ///< Navio médio, ocupa 3 células.
    LARGE   ///< Navio grande, ocupa 5 células.
};

/**
 * @brief Gerencia o estado e as peças de um único navio.
 * @details A classe Ship mantém o controle de quantas células o navio ocupa
 * e quantas dessas células já foram atingidas pelo adversário.
 * Nota de arquitetura: o Ship conhece sua própria posição inicial (para
 * calcular as células que ocupa), mas não valida limites do tabuleiro
 * nem colisão com outros navios — essa responsabilidade é do Board.
 * @author Gabi
 */
class Ship {
public:
    /**
     * @brief Constrói um novo navio em uma posição e orientação definidas.
     * @details O tamanho do navio é derivado automaticamente do ShipType
     * através de sizeForType(), evitando inconsistência entre tipo e tamanho.
     * @param type Tipo do navio (SMALL, MEDIUM ou LARGE).
     * @param startRow Linha inicial (posição da proa) no tabuleiro.
     * @param startCol Coluna inicial (posição da proa) no tabuleiro.
     * @param horizontal Verdadeiro para orientação horizontal, falso para vertical.
     */
    Ship(ShipType type, int startRow, int startCol, bool horizontal);

    /**
     * @brief Retorna o tipo do navio.
     * @return O ShipType associado a este navio.
     */
    ShipType getType() const;

    /**
     * @brief Retorna o tamanho do navio em número de células.
     * @return Quantidade de células ocupadas pelo navio.
     */
    int getSize() const;

    /**
     * @brief Indica a orientação do navio.
     * @return true se horizontal, false se vertical.
     */
    bool isHorizontal() const;

    /**
     * @brief Retorna a linha inicial (proa) do navio.
     * @return Índice da linha inicial.
     */
    int getStartRow() const;

    /**
     * @brief Retorna a coluna inicial (proa) do navio.
     * @return Índice da coluna inicial.
     */
    int getStartCol() const;

    /**
     * @brief Calcula todas as células ocupadas pelo navio no tabuleiro.
     * @details Percorre a partir de (startRow, startCol) somando o deslocamento
     * de acordo com a orientação horizontal ou vertical.
     * @return Vetor de pares (linha, coluna) representando cada célula ocupada.
     */
    std::vector<std::pair<int, int>> getOccupiedCells() const;

    /**
     * @brief Verifica se uma célula específica pertence a este navio.
     * @param row Linha a ser verificada.
     * @param col Coluna a ser verificada.
     * @return true se a célula (row, col) faz parte do navio; false caso contrário.
     */
    bool occupiesCell(int row, int col) const;

    /**
     * @brief Registra um tiro que acertou este navio.
     * @details Incrementa o contador de acertos até o limite do tamanho do navio.
     * Chamadas além do necessário são ignoradas com segurança.
     */
    void hit();

    /**
     * @brief Verifica se o navio foi completamente afundado.
     * @return true se todas as células do navio já foram atingidas.
     */
    bool isSunk() const;

    /**
     * @brief Retorna quantos segmentos do navio ainda estão intactos.
     * @return Número de células ainda não atingidas.
     */
    int getRemainingSegments() const;

    /**
     * @brief Retorna o nome legível do tipo do navio, em português.
     * @return String com o nome do tipo ("Pequeno", "Medio" ou "Grande").
     */
    std::string getTypeName() const;

    /**
     * @brief Converte um ShipType para o tamanho correspondente em células.
     * @param type O tipo do navio a ser convertido.
     * @return Tamanho em número de células (2, 3 ou 5).
     * @warning Retorna 0 caso um ShipType inválido seja informado.
     */
    static int sizeForType(ShipType type);

private:
    ShipType type;
    int startRow;
    int startCol;
    int size;
    bool horizontal;
    int hitsReceived;
};

#endif