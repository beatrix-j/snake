//
//  main.c
//  snakegame
//
//  Created by Beatrix Jones on 12/1/2025.
//  Updated by Stuart Jones on 12/1/2025.
//

#include <ncurses.h>
#include <unistd.h> // For usleep()

// Define directions
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Snake structure
#define SNAKE_LENGTH 3 // Maximum length of the snake
typedef struct {
    int x[SNAKE_LENGTH]; // X coordinates of the snake's body
    int y[SNAKE_LENGTH]; // Y coordinates of the snake's body
    int length;          // Current length of the snake
} Snake;

// Function to handle user input and update direction
Direction get_new_direction(Direction current_direction) {
    int ch = getch(); // Get user input
    switch (ch) {
        case KEY_UP:
            return DIR_UP;
        case KEY_DOWN:
            return DIR_DOWN;
        case KEY_LEFT:
            return DIR_LEFT;
        case KEY_RIGHT:
            return DIR_RIGHT;
        default:
            return current_direction; // Keep the current direction if no valid key is pressed
    }
}

// Function to update the snake's position based on direction
void update_snake(Snake *snake, Direction direction) {
    // Shift the snake's body segments to make room for the new head
    for (int i = snake->length - 1; i > 0; i--) {
        snake->x[i] = snake->x[i - 1];
        snake->y[i] = snake->y[i - 1];
    }

    // Update the head position based on the direction
    switch (direction) {
        case DIR_UP:
            snake->y[0]--;
            break;
        case DIR_DOWN:
            snake->y[0]++;
            break;
        case DIR_LEFT:
            snake->x[0]--;
            break;
        case DIR_RIGHT:
            snake->x[0]++;
            break;
    }
}

// Function to draw a border around the play area
void draw_border(int max_x, int max_y) {
    for (int i = 0; i < max_x; i++) {
        mvprintw(0, i, "#");            // Top border
        mvprintw(max_y - 1, i, "#");    // Bottom border
    }
    for (int i = 0; i < max_y; i++) {
        mvprintw(i, 0, "#");            // Left border
        mvprintw(i, max_x - 1, "#");    // Right border
    }
}

// Main function
int main(void) {
    Snake snake = {{5, 5, 5}, {5, 6, 7}, SNAKE_LENGTH}; // Initialize the snake
    Direction current_direction = DIR_RIGHT; // Initial direction

    // Initialize ncurses
    initscr();             // Start curses mode
    cbreak();              // Disable line buffering
    noecho();              // Don't echo input
    curs_set(0);           // Hide the default cursor
    keypad(stdscr, TRUE);  // Enable special keys (including arrow keys)
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    // Get screen dimensions
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    // Main game loop
    while (1) {
        // Check for user input (exit on 'q')
        int ch = getch();
        if (ch == 'q') {
            break; // Exit the loop if 'q' is pressed
        }

        // Clear the previous snake positions
        for (int i = 0; i < snake.length; i++) {
            mvprintw(snake.y[i], snake.x[i], " ");
        }

        // Update the snake's position
        update_snake(&snake, current_direction);

        // Wrap the snake around the screen if it hits a border
        if (snake.x[0] <= 0) {
            snake.x[0] = max_x - 2; // Wrap to the right side
        } else if (snake.x[0] >= max_x - 1) {
            snake.x[0] = 1; // Wrap to the left side
        }
        if (snake.y[0] <= 0) {
            snake.y[0] = max_y - 2; // Wrap to the bottom
        } else if (snake.y[0] >= max_y - 1) {
            snake.y[0] = 1; // Wrap to the top
        }

        // Draw the snake
        for (int i = 0; i < snake.length; i++) {
            mvprintw(snake.y[i], snake.x[i], "O");
        }

        // Draw the border
        draw_border(max_x, max_y);

        // Refresh the screen
        refresh();

        // Slow down the movement
        usleep(200000); // 200ms delay

        // Handle user input and update direction
        current_direction = get_new_direction(current_direction);
    }

    // Clean up and exit
    endwin();
    return 0; // Standard return value for successful execution
}

