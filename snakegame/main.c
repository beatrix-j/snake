//
//  main.c
//  snakegame
//
//  Created by Beatrix Jones on 12/1/2025.
//  Updated by Stuart Jones on 12/1/2025.
//

#include <ncurses.h> // for cursor control in terminal
#include <unistd.h> // For usleep() to slow down cursor movement

// Main function with int return type
int main(void) {
    int x = 0, y = 0; // Initial position of the cursor
    int max_x = 0, max_y = 0; // Initialise screen dimensions

    // Initialize ncurses
    initscr();             // Start curses mode
    cbreak();              // Disable line buffering
    noecho();              // Don't echo input
    curs_set(0);           // Hide the default cursor
    keypad(stdscr, TRUE);  // Enable special keys
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    // Get screen dimensions
    getmaxyx(stdscr, max_y, max_x);

    // Main game loop
    while (1) {
        clear(); // Clear the screen

        // Draw the cursor (snake head)
        mvprintw(y, x, "-o>");

        // Refresh the screen
        refresh();

        // Move the cursor
        x++;
        if (x >= max_x) {
            x = 0;
            y++;
            if (y >= max_y) {
                y = 0;
            }
        }

        // Slow down the movement
        usleep(200000); // 200ms delay

        // Check for user input (exit on 'q')
        int ch = getch();
        if (ch == 'q') {
            break; // Exit the loop if 'q' is pressed
        }
    }

    // Clean up and exit
    endwin();
    return 0; // Standard return value for successful execution
}
