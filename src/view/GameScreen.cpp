#include "view/GameScreen.h"
#include "utils/UiTheme.h"
#include "AutoPlacer.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <stdexcept>

namespace {

// Espaco entre os dois tabuleiros e margem minima usada so para limitar o
// tamanho da celula (o posicionamento real continua centralizado na janela).
constexpr float kBoardGap = 70.f;
constexpr float kMinSideMargin = 50.f;
constexpr float kBoardTop = 150.f;

// Altura reservada abaixo do tabuleiro para o painel de status das frotas
// (ver GameScreen::drawFleetPanel), para caber mesmo no mapa Oceano (10x10).
constexpr float kFleetPanelTop = 620.f;
constexpr float kFleetPanelHeight = 80.f;

float computeCellSize(int gridSize) {
    const float maxCellSize = 55.f;
    const float maxBoardWidth = (static_cast<float>(ui::kWindowWidth) - kBoardGap - 2.f * kMinSideMargin) / 2.f;
    const float widthLimit = maxBoardWidth / static_cast<float>(gridSize);
    const float heightLimit = (kFleetPanelTop - 10.f - kBoardTop) / static_cast<float>(gridSize);
    return std::min({maxCellSize, widthLimit, heightLimit});
}

}

GameScreen::GameScreen(sf::RenderWindow& window, GameState& gameState)
    : window(window),
      gameState(gameState),
      grid(gameState.getPlayerBoard().getCols()),
      cellSize(computeCellSize(grid)),
      leftOffsetX((static_cast<float>(ui::kWindowWidth) - (2.f * grid * cellSize + kBoardGap)) / 2.f),
      rightOffsetX(leftOffsetX + grid * cellSize + kBoardGap),
      boardOffsetY(kBoardTop),
      playerBoardRenderer(window, cellSize, leftOffsetX, boardOffsetY),
      enemyBoardRenderer(window, cellSize, rightOffsetX, boardOffsetY),
      enemyController(cellSize, rightOffsetX, boardOffsetY,
                      gameState.getComputerBoard().getRows(), gameState.getComputerBoard().getCols()),
      hits(0),
      misses(0),
      shipsDestroyed(0)
{
    if (!font.openFromFile("assets/fonts/Roboto-Regular.ttf"))
        throw std::runtime_error("Fonte nao encontrada: assets/fonts/Roboto-Regular.ttf");

    AutoPlacer::place(gameState.getComputerBoard(), gameState.getComputerShips());

    statusMessage = "Sua vez: clique no tabuleiro inimigo para atirar.";
}

GameScreen::Outcome GameScreen::run() {
    gameState.startTimer();

    while (window.isOpen() && gameState.checkResult() == GameResult::OCORRENDO) {
        processEvents();
        update();
        render();
    }

    const auto& playerShips = gameState.getPlayerShips();
    const int alliedSurvived = static_cast<int>(std::count_if(
        playerShips.begin(), playerShips.end(),
        [](const Ship& ship) { return !ship.isSunk(); }));

    return Outcome{
        gameState.checkResult(),
        hits,
        misses,
        shipsDestroyed,
        alliedSurvived,
        gameState.getElapsedSeconds()
    };
}

void GameScreen::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }

        if (gameState.getCurrentTurn() != Turno::PLAYER) continue;

        const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>();
        if (!mousePressed || mousePressed->button != sf::Mouse::Button::Left) continue;

        int row, col;
        sf::Vector2i mousePos(mousePressed->position.x, mousePressed->position.y);
        if (!enemyController.screenToBoardCoord(mousePos, row, col)) continue;

        Board& enemyBoard = gameState.getComputerBoard();
        if (!enemyBoard.isValidShot(row, col)) continue;

        const ShotOutcome outcome = resolveShot(enemyBoard, gameState.getComputerShips(), row, col);

        if (outcome.state == CellState::HIT) {
            hits++;
            if (outcome.shipSunk) {
                shipsDestroyed++;
                statusMessage = "Voce afundou um navio inimigo em " + cellLabel(row, col) + "!";
            } else {
                statusMessage = "Voce acertou um navio em " + cellLabel(row, col) + "!";
            }
        } else {
            misses++;
            statusMessage = "Voce atirou na agua em " + cellLabel(row, col) + ".";
        }

        if (gameState.checkResult() == GameResult::OCORRENDO) {
            gameState.switchTurn();
            aiDelayClock.restart();
        }
    }
}

void GameScreen::update() {
    if (gameState.getCurrentTurn() != Turno::COMPUTER) return;

    const float aiDelaySeconds = 1.0f;
    if (aiDelayClock.getElapsedTime().asSeconds() < aiDelaySeconds) return;

    Board& playerBoard = gameState.getPlayerBoard();
    const auto [row, col] = aiPlayer.makeShot(playerBoard);

    const ShotOutcome outcome = resolveShot(playerBoard, gameState.getPlayerShips(), row, col);

    aiPlayer.onShotResult(row, col, outcome.state);

    if (outcome.state == CellState::HIT) {
        if (outcome.shipSunk) {
            aiPlayer.onShipSunk();
            statusMessage = "O computador afundou um dos seus navios em " + cellLabel(row, col) + "!";
        } else {
            statusMessage = "O computador acertou um dos seus navios em " + cellLabel(row, col) + "!";
        }
    } else {
        statusMessage = "O computador atirou na agua em " + cellLabel(row, col) + ".";
    }

    if (gameState.checkResult() == GameResult::OCORRENDO) {
        gameState.switchTurn();
    }
}

void GameScreen::render() {
    window.clear(ui::kSkyTop);
    ui::drawSeascape(window);
    ui::drawChartGrid(window);

    const float centerX = static_cast<float>(window.getSize().x) / 2.f;
    ui::drawCenteredText(window, font, "BATALHA NAVAL", centerX, 30.f, 26, ui::kGold, 1.6f);

    const sf::Color statusColor = (gameState.getCurrentTurn() == Turno::PLAYER)
        ? ui::kInk
        : ui::withAlpha(ui::kInkSoft, 220.f);
    ui::drawCenteredText(window, font, statusMessage, centerX, 66.f, 14, statusColor, 1.1f);

    const float boardWidth = grid * cellSize;
    // -48 (nao -26): precisa sobrar espaco acima dos rotulos de coluna do
    // tabuleiro, que o BoardRenderer desenha em boardOffsetY - 20.
    ui::drawCenteredText(window, font, "SUA FROTA", leftOffsetX + boardWidth / 2.f, boardOffsetY - 48.f, 16, ui::kGold, 1.4f);
    ui::drawCenteredText(window, font, "FROTA INIMIGA", rightOffsetX + boardWidth / 2.f, boardOffsetY - 48.f, 16, ui::kGold, 1.4f);

    playerBoardRenderer.draw(gameState.getPlayerBoard(), false);
    enemyBoardRenderer.draw(gameState.getComputerBoard(), true);

    // Painel de status das frotas: quantos navios (e quantos segmentos de
    // cada um) ainda restam de cada lado.
    drawFleetPanel(leftOffsetX, boardWidth, kFleetPanelTop, "NOSSOS NAVIOS",
                   sf::Color(52, 78, 102), sf::Color(150, 195, 225), gameState.getPlayerShips());
    drawFleetPanel(rightOffsetX, boardWidth, kFleetPanelTop, "NAVIOS DO OPONENTE",
                   sf::Color(150, 88, 45), sf::Color(224, 158, 96), gameState.getComputerShips());

    ui::drawHudFrame(window);
    window.display();
}

GameScreen::ShotOutcome GameScreen::resolveShot(Board& board, std::vector<Ship>& ships, int row, int col) {
    ShotOutcome outcome{board.shoot(row, col), false};

    if (outcome.state == CellState::HIT) {
        for (Ship& ship : ships) {
            if (ship.occupiesCell(row, col)) {
                ship.hit();
                outcome.shipSunk = ship.isSunk();
                break;
            }
        }
    }

    return outcome;
}

std::string GameScreen::cellLabel(int row, int col) {
    return std::string(1, static_cast<char>('A' + col)) + std::to_string(row + 1);
}

void GameScreen::drawFleetPanel(float x, float panelWidth, float top, const std::string& title,
                                sf::Color headerColor, sf::Color aliveColor, const std::vector<Ship>& ships) {
    const float headerHeight = 20.f;
    const float bodyHeight = kFleetPanelHeight - headerHeight;
    const float squareSize = 8.f;
    const float squareGap = 2.f;
    const float shipGap = 10.f;

    // Cabecalho colorido com o nome da frota.
    sf::RectangleShape header({panelWidth, headerHeight});
    header.setPosition({x, top});
    header.setFillColor(headerColor);
    header.setOutlineThickness(1.f);
    header.setOutlineColor(ui::withAlpha(sf::Color::Black, 90.f));
    window.draw(header);
    ui::drawCenteredText(window, font, title, x + panelWidth / 2.f, top + 3.f, 12, sf::Color::White, 1.1f);

    // Fundo do corpo do painel, onde os navios ficam "em pe".
    sf::RectangleShape body({panelWidth, bodyHeight});
    body.setPosition({x, top + headerHeight});
    body.setFillColor(ui::withAlpha(ui::kShadow, 140.f));
    body.setOutlineThickness(1.f);
    body.setOutlineColor(ui::withAlpha(ui::kInkSoft, 90.f));
    window.draw(body);

    // Navios ordenados do menor para o maior, como na referencia visual.
    std::vector<std::size_t> order(ships.size());
    for (std::size_t i = 0; i < order.size(); i++) order[i] = i;
    std::sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) {
        return ships[a].getSize() < ships[b].getSize();
    });

    float totalWidth = 0.f;
    for (std::size_t idx : order) totalWidth += squareSize + shipGap;
    if (!order.empty()) totalWidth -= shipGap;

    float drawX = x + (panelWidth - totalWidth) / 2.f;
    const float bodyBottom = top + kFleetPanelHeight - 4.f;

    for (std::size_t idx : order) {
        const Ship& ship = ships[idx];
        const int size = ship.getSize();
        const int remaining = ship.getRemainingSegments();
        const bool sunk = ship.isSunk();

        // Cada segmento vira um quadrado empilhado de baixo para cima; os
        // segmentos ja atingidos aparecem no topo (a "vida" esvazia por cima).
        for (int seg = 0; seg < size; seg++) {
            sf::RectangleShape square({squareSize, squareSize});
            square.setPosition({drawX, bodyBottom - (seg + 1) * (squareSize + squareGap)});

            if (sunk) {
                // Navio afundado: so o contorno, sem preenchimento.
                square.setFillColor(sf::Color::Transparent);
                square.setOutlineThickness(1.f);
                square.setOutlineColor(ui::withAlpha(ui::kInkSoft, 100.f));
            } else if (seg < remaining) {
                square.setFillColor(aliveColor);
                square.setOutlineThickness(1.f);
                square.setOutlineColor(ui::withAlpha(sf::Color::Black, 90.f));
            } else {
                square.setFillColor(ui::withAlpha(sf::Color(200, 60, 60), 210.f));
                square.setOutlineThickness(1.f);
                square.setOutlineColor(ui::withAlpha(sf::Color::Black, 90.f));
            }

            window.draw(square);
        }

        drawX += squareSize + shipGap;
    }
}
