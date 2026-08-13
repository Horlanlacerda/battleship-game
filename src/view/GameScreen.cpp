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
// 190 (nao 150): sobra espaco para a barra de armas (ver kWeaponBarTop),
// entre o status e os rotulos "SUA FROTA"/"FROTA INIMIGA".
constexpr float kBoardTop = 190.f;

// Barra com os 3 botoes de arma (Basico/Missil/Torpedo), centralizada
// horizontalmente entre o status e os rotulos das frotas.
constexpr float kWeaponBarTop = 94.f;
constexpr float kWeaponButtonHeight = 28.f;
constexpr float kWeaponButtonWidth = 150.f;
constexpr float kWeaponButtonGap = 14.f;
constexpr WeaponType kWeaponButtonTypes[3] = {WeaponType::BASIC, WeaponType::MISSILE, WeaponType::TORPEDO};

sf::FloatRect weaponButtonBounds(int index) {
    const float totalWidth = 3.f * kWeaponButtonWidth + 2.f * kWeaponButtonGap;
    const float left = (static_cast<float>(ui::kWindowWidth) - totalWidth) / 2.f;
    const float x = left + index * (kWeaponButtonWidth + kWeaponButtonGap);
    return sf::FloatRect({x, kWeaponBarTop}, {kWeaponButtonWidth, kWeaponButtonHeight});
}

// Indice do botao clicado, ou -1 se o clique nao caiu em nenhum deles.
int weaponButtonAt(sf::Vector2f click) {
    for (int i = 0; i < 3; i++)
        if (weaponButtonBounds(i).contains(click)) return i;
    return -1;
}

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

    if (!AutoPlacer::place(gameState.getComputerBoard(), gameState.getComputerShips()))
        throw std::runtime_error("Nao foi possivel posicionar a frota do computador.");

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

        sf::Vector2f clickPos{(float)mousePressed->position.x, (float)mousePressed->position.y};

        // Clique num botao de arma: so troca a selecao, nao gasta o turno.
        const int buttonIndex = weaponButtonAt(clickPos);
        if (buttonIndex >= 0) {
            const WeaponType weapon = kWeaponButtonTypes[buttonIndex];
            if (weapon == WeaponType::BASIC ||
                (weapon == WeaponType::MISSILE && enemyController.getMissilesLeft() > 0) ||
                (weapon == WeaponType::TORPEDO && enemyController.getTorpedoesLeft() > 0)) {
                enemyController.selectWeapon(weapon);
            }
            continue;
        }

        int row, col;
        sf::Vector2i mousePos(mousePressed->position.x, mousePressed->position.y);
        if (!enemyController.screenToBoardCoord(mousePos, row, col)) continue;

        Board& enemyBoard = gameState.getComputerBoard();
        if (!enemyBoard.isValidShot(row, col)) continue;

        const GameController::FireResult result =
            enemyController.fire(enemyBoard, gameState.getComputerShips(), row, col);
        hits += result.hits;
        misses += result.misses;

        if (result.shipsSunk > 0) {
            shipsDestroyed += result.shipsSunk;

            // Um sorteio de arma por navio afundado neste disparo (o torpedo,
            // por atingir uma linha inteira, pode afundar mais de um de uma vez).
            std::string awardMsg;
            for (int i = 0; i < result.shipsSunk; i++) {
                const WeaponType awarded = enemyController.awardRandomWeapon();
                if (!awardMsg.empty()) awardMsg += " e ";
                awardMsg += "1 " + weaponName(awarded);
            }

            const std::string shipsText = (result.shipsSunk == 1)
                ? "um navio inimigo"
                : std::to_string(result.shipsSunk) + " navios inimigos";
            statusMessage = "Voce afundou " + shipsText + " em " + cellLabel(row, col) +
                             "! Voce ganhou " + awardMsg + "!";
        } else if (result.hits > 0) {
            statusMessage = (result.hits == 1)
                ? "Voce acertou um navio em " + cellLabel(row, col) + "!"
                : "Voce acertou " + std::to_string(result.hits) + " navios em " + cellLabel(row, col) + "!";
        } else {
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
    ui::drawChartGrid(window);

    const float centerX = static_cast<float>(window.getSize().x) / 2.f;
    ui::drawCenteredText(window, font, "BATALHA NAVAL", centerX, 30.f, 26, ui::kGold, 1.6f);

    const sf::Color statusColor = (gameState.getCurrentTurn() == Turno::PLAYER)
        ? ui::kInk
        : ui::withAlpha(ui::kInkSoft, 220.f);
    ui::drawCenteredText(window, font, statusMessage, centerX, 66.f, 14, statusColor, 1.1f);

    drawWeaponBar();

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

void GameScreen::drawWeaponBar() {
    const sf::Vector2i mouse = sf::Mouse::getPosition(window);
    const sf::Vector2f cursor{(float)mouse.x, (float)mouse.y};

    const WeaponType selected = enemyController.getSelectedWeapon();

    struct WeaponButton {
        WeaponType type;
        std::string label;
        int ammo;
        bool unlimited;
    };
    const WeaponButton buttons[3] = {
        {WeaponType::BASIC, "BASICO", 0, true},
        {WeaponType::MISSILE, "MISSIL", enemyController.getMissilesLeft(), false},
        {WeaponType::TORPEDO, "TORPEDO", enemyController.getTorpedoesLeft(), false},
    };

    for (int i = 0; i < 3; i++) {
        const WeaponButton& button = buttons[i];
        const sf::FloatRect area = weaponButtonBounds(i);
        const bool enabled = button.unlimited || button.ammo > 0;
        const bool selectedNow = selected == button.type;
        const bool hovered = enabled && area.contains(cursor);

        sf::RectangleShape plate(area.size);
        plate.setPosition(area.position);
        plate.setFillColor(ui::withAlpha(hovered ? ui::kPanelHot : ui::kPanel, enabled ? 226.f : 110.f));
        plate.setOutlineThickness(2.f);
        plate.setOutlineColor(selectedNow ? ui::kGold : ui::withAlpha(ui::kInkSoft, enabled ? 110.f : 60.f));
        window.draw(plate);

        std::string label = button.label;
        if (!button.unlimited) label += " (" + std::to_string(button.ammo) + ")";

        const sf::Color textColor = !enabled ? ui::withAlpha(ui::kInkSoft, 120.f)
                                    : (hovered ? sf::Color::White : ui::kInk);
        ui::drawCenteredText(window, font, label, area.position.x + area.size.x / 2.f,
                             area.position.y + 6.f, 13, textColor, 1.f);
    }
}

std::string GameScreen::weaponName(WeaponType weapon) {
    switch (weapon) {
        case WeaponType::MISSILE: return "missil";
        case WeaponType::TORPEDO: return "torpedo";
        default: return "";
    }
}
