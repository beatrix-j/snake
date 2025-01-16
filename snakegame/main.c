//
//  main.c
//  snakegame
//
//  Created by Beatrix Jones on 12/1/2025.
//  Updated by Stuart on 12/1/2025 - basic movement
//  Updated by Stuart on 16/1/2025 - add user input, length definable, sceen wrap, border
//

/*  Explanation of Key Components:
 *
 *  get_new_direction:
 *  Function checks for user input using getch() and updates the snake's direction based on the arrow keys.
 *  If no valid key is pressed, it returns the current direction.
 *
 *  update_snake:
 *  Function updates the snake's body positions by shifting each segment to the position of the previous segment.
 *  The head position is updated based on the current direction.
 *
 *  draw_border:
 *  This function draws a border around the play area using the # character.
 *  It uses mvprintw() to place characters at specific coordinates.
 *
 *  main:
 *  Initializes the snake's body positions and the ncurses environment.
 *  Runs the game loop, which handles input, updates the snake's position, and redraws the screen.
 *  Cleans up resources and exits when the user presses 'q'.
 */
 
#include <ncurses.h>   // Include the ncurses library for terminal handling
#include <unistd.h>    // Include for usleep() to introduce delays

// Define directions using an enum for better readability
typedef enum {
    DIR_UP,    // Represents the upward direction
    DIR_DOWN,  // Represents the downward direction
    DIR_LEFT,  // Represents the leftward direction
    DIR_RIGHT  // Represents the rightward direction
} Direction;

// Define the snake length as a macro (change this value to set the snake length)
#define SNAKE_LENGTH 10

// Define a structure to represent the snake
typedef struct {
    int x[SNAKE_LENGTH]; // Array to store the X coordinates of the snake's body segments
    int y[SNAKE_LENGTH]; // Array to store the Y coordinates of the snake's body segments
    int length;          // Current length of the snake
} Snake;

/**
 * Function: get_new_direction
 * --------------------------
 * Handles user input and updates the snake's direction.
 *
 * Parameters:
 *   - current_direction: The current direction of the snake.
 *
 * Returns:
 *   - The new direction based on user input, or the current direction if no valid key is pressed.
 */

Direction get_new_direction(Direction current_direction) {
    int ch = getch(); // Get a character from the keyboard input (non-blocking due to nodelay)
    switch (ch) {     // Check which key was pressed
        case KEY_UP:
            return DIR_UP;    // Return the upward direction
        case KEY_DOWN:
            return DIR_DOWN;  // Return the downward direction
        case KEY_LEFT:
            return DIR_LEFT;  // Return the leftward direction
        case KEY_RIGHT:
            return DIR_RIGHT; // Return the rightward direction
        default:
            return current_direction; // If no valid key is pressed, keep the current direction
    }
}

/**
 * Function: update_snake
 * ----------------------
 * Updates the snake's position based on the current direction.
 *
 * Parameters:
 *   - snake: A pointer to the Snake structure representing the snake.
 *   - direction: The current direction of the snake.
 */
void update_snake(Snake *snake, Direction direction) {
    // Shift the snake's body segments to make room for the new head
    for (int i = snake->length - 1; i > 0; i--) {
        snake->x[i] = snake->x[i - 1]; // Move each segment to the position of the previous segment
        snake->y[i] = snake->y[i - 1];
    }

    // Update the head position based on the direction
    switch (direction) {
        case DIR_UP:
            snake->y[0]--; // Move the head up (decrease Y coordinate)
            break;
        case DIR_DOWN:
            snake->y[0]++; // Move the head down (increase Y coordinate)
            break;
        case DIR_LEFT:
            snake->x[0]--; // Move the head left (decrease X coordinate)
            break;
        case DIR_RIGHT:
            snake->x[0]++; // Move the head right (increase X coordinate)
            break;
    }
}

/**
 * Function: draw_border
 * ---------------------
 * Draws a border around the play area using the '#' character.
 *
 * Parameters:
 *   - max_x: The maximum X coordinate of the terminal (width).
 *   - max_y: The maximum Y coordinate of the terminal (height).
 */
void draw_border(int max_x, int max_y) {
    // Draw the top and bottom borders
    for (int i = 0; i < max_x; i++) {
        mvprintw(0, i, "#");         // Draw the top border
        mvprintw(max_y - 1, i, "#"); // Draw the bottom border
    }
    // Draw the left and right borders
    for (int i = 0; i < max_y; i++) {
        mvprintw(i, 0, "#");         // Draw the left border
        mvprintw(i, max_x - 1, "#"); // Draw the right border
    }
}

/**
 * Function: main
 * --------------
 * The entry point of the program. Initializes the game, handles the game loop, and cleans up resources.
 */
int main(void) {
    Snake snake; // Declare a Snake structure to represent the snake
    Direction current_direction = DIR_RIGHT; // Initialize the snake's direction to right

    // Initialize the snake's body positions
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake.x[i] = 5 + i; // Set the X positions in a horizontal line
        snake.y[i] = 5;     // Set the Y positions to the same row
    }
    snake.length = SNAKE_LENGTH; // Set the snake's length

    // Initialize ncurses
    initscr();             // Start curses mode (initialize the screen)
    cbreak();              // Disable line buffering (input is processed character by character)
    noecho();              // Don't echo input (keeps user input from being displayed on the screen)
    curs_set(0);           // Hide the default cursor
    keypad(stdscr, TRUE);  // Enable special keys (like arrow keys)
    nodelay(stdscr, TRUE); // Make getch() non-blocking (allows the game loop to continue without waiting for input)

    // Get the terminal's maximum dimensions
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x); // Store the width in max_x and the height in max_y

    // Main game loop
    while (1) {
        // Check for user input (exit on 'q')
        int ch = getch(); // Get a character from the keyboard input
        if (ch == 'q') {
            break; // Exit the loop if 'q' is pressed
        }

        // Clear the previous snake positions
        for (int i = 0; i < snake.length; i++) {
            mvprintw(snake.y[i], snake.x[i], " "); // Clear each segment of the snake
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
            mvprintw(snake.y[i], snake.x[i], "O"); // Draw each segment of the snake
        }

        // Draw the border
        draw_border(max_x, max_y);

        // Refresh the screen to display changes
        refresh();

        // Slow down the movement
        usleep(100000); // Delay of 100ms between loops to set snake speed

        // Handle user input and update direction
        current_direction = get_new_direction(current_direction);
    }

    // Clean up and exit
    endwin(); // End curses mode and restore the terminal
    return 0; // Return 0 to indicate successful execution
}


