#include "menu.h"
#include "displayNumbers.h"
#include "controllerEndpoints.h"
#include "snake.h"
#include "galaxia.h"
#include "display.h"
#include "tetris.h"

void menu_init()
{
    int gameId = 0;
    int gameCount = 3;

    // Initialize menu items
    drawCenteredTwoDigitNumber(gameId, CRGB::White);

    while (true)
    {
        // Update menu based on user input
        if (commandFlags[CMD_UP])
        {
            gameId = (gameId - 1 + gameCount) % gameCount;
            commandFlags[CMD_UP] = false; // Reset command flag
            clearFramebuffer();
            drawCenteredTwoDigitNumber(gameId, CRGB::Red);
        }
        if (commandFlags[CMD_DOWN])
        {
            gameId = (gameId + 1) % gameCount;
            commandFlags[CMD_DOWN] = false; // Reset command flag
            clearFramebuffer();
            drawCenteredTwoDigitNumber(gameId, CRGB::Red);
        }
        if (commandFlags[CMD_SELECT])
        {
            // Start the selected game
            commandFlags[CMD_SELECT] = false; // Reset command flag

            switch (gameId)
            {
            case 0:
                // Start Snake Game
                setupSnakeGame();
                break;
            case 1:
                setupGalaxiaGame();
                break;
            case 2:
                // Start Tetris Game
                setupTetrisGame();
                break;
            default:
                break;
            }
            break;
        }
    }
}