#pragma once // uma instrução enviada diretamente ao compilador que diz pra ele usar só uma versão dele

#include <vector>

/**
 * @brief Representa os possíveis estados de uma célula no tabuleiro.
 * @details Cada célula do tabuleiro pode estar em um destes estados,
 * determinando se há navio, se foi atingida, ou se o terreno impõe
 * restrições de posicionamento (Funcionalidades do Sprint 4).
 * @author Georis
 * @date 10/07/2026
 */
enum class CellState {
    EMPTY,    ///< Célula vazia, ainda não atacada
    SHIP,     ///< Célula ocupada por um segmento de navio intacto
    HIT,      ///< Célula onde um tiro acertou um navio (dano confirmado)
    MISS,     ///< Célula onde um tiro caiu na água (erro)
    BLOCKED,  ///< Terreno bloqueado, navio não pode ser posicionado nesta célula
    SHALLOW   ///< Terreno raso, apenas navios de tamanho pequeno podem ocupar esta célula
};

/**
 * @brief Gerencia o tabuleiro do jogo de Batalha Naval.
 * @details A classe Board é responsável por manter o estado de todas as células
 * do tabuleiro, validar posicionamento de navios, processar tiros e verificar
 * condições de fim de jogo.
 *
 * Arquitetura: O Board é a única entidade que manipula diretamente a matriz
 * de células, garantindo encapsulamento e integridade dos dados. O GameController
 * não pode acessar a grid diretamente.
 *
 * Suporte a terrenos especiais (BLOCKED e SHALLOW)
 *
 * @author Georis
 * @date 10/07/2026
 */
class Board {
public:
    /**
     * @brief Constructor de um novo tabuleiro com as dimensões especificadas.
     * @details Aloca a matriz bidimensional e inicializa todas as células
     * com o estado CellState::EMPTY.
     *
     * @param rows Número de linhas do tabuleiro.
     * @param cols Número de colunas do tabuleiro.
     *
     * @warning Valores menores ou iguais a zero para rows ou cols podem
     * causar comportamento indefinido ou alocação inválida de memória.
     *
     * @author Georis
     * @date 10/07/2026
     */
    Board(int rows, int cols);


    // --- Posicionamento ------------------------>

    /**
     * @brief Verifica se um navio pode ser posicionado na posição escolhida.
     * @details Valida as regras de colisão, limites do tabuleiro e restrições
     * de terreno (BLOCKED e SHALLOW) antes de permitir o posicionamento.
     *
     * @param row Linha inicial onde a proa do navio será posicionada.
     * @param col Coluna inicial onde a proa do navio será posicionada.
     * @param size Tamanho do navio em número de células.
     * @param horizontal true para orientação horizontal, false para vertical.
     *
     * @return true se o navio pode ser posicionado na posição solicitada ou
     *         false se houver colisão com outro navio, sair dos limites do
     *         tabuleiro, ou violar restrições de terreno.
     *
     * @author Georis
     * @date 10/07/2026
     */
    bool canPlace(int row, int col, int size, bool horizontal) const;

    /**
     * @brief Posiciona um navio no tabuleiro.
     * @details Marca as células correspondentes com CellState::SHIP.
     * A operação só é realizada se a validação de canPlace() retornar true.
     *
     * @param row Linha inicial onde a proa do navio será posicionada.
     * @param col Coluna inicial onde a proa do navio será posicionada.
     * @param size Tamanho do navio em número de células.
     * @param horizontal true para orientação horizontal, false para vertical
     *
     * @warning Terrenos BLOCKED e SHALLOW ainda não são validados nesta versão.
     *
     * @return true se o navio foi posicionado com sucesso;
     *         false se a posição for inválida (falha na validação).
     *
     * @see canPlace() para verificar as regras de validação antes de posicionar.
     *
     * @author Georis
     * @date 10/07/2026
     */
    bool placeShip(int row, int col, int size, bool horizontal);

    /**
     * @brief Remove um navio previamente posicionado no tabuleiro.
     * @details Restaura as células ocupadas pelo navio para o estado CellState::EMPTY.
     * Utilizado principalmente para desfazer operações de posicionamento
     * durante a fase de setup ou para algoritmos de backtracking.
     *
     * @param row Linha inicial onde a proa do navio está posicionada.
     * @param col Coluna inicial onde a proa do navio está posicionada.
     * @param size Tamanho do navio em número de células.
     * @param horizontal true se o navio está na orientação horizontal, false se vertical.
     *
     * @warning Certifique-se de que o navio realmente existe nesta posição,
     * pois o método não verifica se as células contêm CellState::SHIP.
     * Remover um navio inexistente pode corromper o estado de células adjacentes.
     *
     * @author Georis
     * @date 10/07/2026
     */
    void removeShip(int row, int col, int size, bool horizontal);


    // --- Tiro 🔫 ------------------------>

    /**
     * @brief Verifica se uma coordenada de tiro é válida.
     * @details Um tiro é considerado válido se a coordenada está dentro dos
     * limites do tabuleiro e a célula ainda não foi atacada (não está HIT nem MISS).
     *
     * @param row Linha da coordenada do tiro.
     * @param col Coluna da coordenada do tiro.
     *
     * @return true se o tiro pode ser realizado nesta coordenada e
     *         false se a coordenada estiver fora do tabuleiro ou se a célula
     *         já foi atingida anteriormente.
     *
     * @author Georis
     * @date 10/07/2026
     */
    bool isValidShot(int row, int col) const;

    /**
     * @brief Processa um tiro na coordenada especificada.
     * @details Atualiza o estado da célula atingida:
     * - CellState::SHIP passa para CellState::HIT (acerto)
     * - CellState::EMPTY, BLOCKED ou SHALLOW passam para CellState::MISS (erro)
     *
     * @param row Linha da coordenada do tiro.
     * @param col Coluna da coordenada do tiro.
     *
     * @return O NOVO estado da célula após o tiro (HIT ou MISS).
     * Se o tiro for inválido, retorna o estado atual sem modificar.
     *
     * @warning Este método não valida se o tiro é válido. Use isValidShot()
     * antes de chamar shoot() para garantir que o tiro não seja repetido.
     *
     * @see isValidShot() para verificar se o tiro é permitido.
     *
     * @author Georis
     * @date 10/07/2026
     */
    CellState shoot(int row, int col);


    // --- Getters ------------------------>

    /**
     * @brief Retorna o estado atual de uma célula específica.
     *
     * @param row Linha da célula a ser consultada.
     * @param col Coluna da célula a ser consultada.
     *
     * @return O CellState atual da célula na posição (row, col).
     *
     * @throws std::out_of_range se a coordenada estiver fora dos limites.
     *
     * @author Georis
     * @date 10/07/2026
     */
    CellState getCell(int row, int col) const;

    /**
     * @brief Retorna o número de linhas do tabuleiro.
     * @return Quantidade de linhas (dimensão vertical).
     *
     * @author Georis
     * @date 10/07/2026
     */
    int getRows() const;

    /**
     * @brief Retorna o número de colunas do tabuleiro.
     * @return Quantidade de colunas.
     *
     * @author Georis
     * @date 10/07/2026
     */
    int getCols() const;


    // --- Utilitários ------------------------>

    /**
     * @brief Verifica se ainda existe algum navio intacto no tabuleiro.
     * @details Percorre toda a matriz em busca de células com estado
     * CellState::SHIP. É utilizado pelo GameController para determinar
     * se o jogo terminou (todos os navios do jogador foram destruídos).
     *
     * @return true se houver pelo menos uma célula com CellState::SHIP;
     *         false se todos os navios foram destruídos.
     *
     * @author Georis
     * @date 10/07/2026
     */
    bool hasAnyShipRemaining() const;

    /**
     * @brief Reinicia completamente o tabuleiro.
     * @details Restaura todas as células para o estado CellState::EMPTY,
     * removendo todos os navios e marcas de tiros. É equivalente a criar
     * um novo tabuleiro preservando as dimensões originais.
     *
     * @warning Este método destrói todo o estado do jogo atual,
     * incluindo navios posicionados e histórico de tiros.
     *
     * @author Georis
     * @date 10/07/2026
     */
    void reset();

private:
    int rows;                               ///< Número de linhas do tabuleiro
    int cols;                               ///< Número de colunas do tabuleiro
    std::vector<std::vector<CellState>> grid; ///< Matriz bidimensional que armazena o estado de cada célula

    /**
     * @brief Verifica se uma coordenada está dentro dos limites do tabuleiro.
     * @param row Linha a ser verificada.
     * @param col Coluna a ser verificada.
     * @return true se a coordenada (row, col) é válida e false caso contrário.
     */
    bool inBounds(int row, int col) const;
};