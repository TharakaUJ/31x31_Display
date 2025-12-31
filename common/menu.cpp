#include "menu.h"
#include "displayNumbers.h"
#include "controllerEndpoints.h"
#include "snake.h"
#include "galaxia.h"
#include "display.h"
#include "tetris.h"
#include "platform.h"

void menu_loop()
{
    int gameId = 0;
    int gameCount = 3;

    // Initialize menu items
    clearFramebuffer();
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
                setupSnakeGame();
                platformDeleteCurrentThread();
                break;
            case 1:
                setupGalaxiaGame();
                platformDeleteCurrentThread();
                break;
            case 2:
                setupTetrisGame();
                platformDeleteCurrentThread();
                break;
            default:
                break;
            }
            break;
        }
        platformDelay(100);
    }
}

void menu_init()
{
    createThread(
        "MenuTask",
        [](void *)
        {
            menu_loop();
        },
        nullptr,
        4096,  // Stack size
        1      // Priority
    );
}