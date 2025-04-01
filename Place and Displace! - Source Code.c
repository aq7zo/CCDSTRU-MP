/* 
    SECTION: S15
    GROUP NUMBER: 11

    LASTNAME1, FIRSTNAME1: CAMPO, BENETTE ENZO
    LASTNAME1, FIRSTNAME2: HALLARE, ZACH BENEDICT
    LASTNAME3, FIRSTNAME3: MARTIN, KURT NEHEMIAH
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Constants for the system
#define MAX_A 4  // Maximum element in set A (1 to 4)
#define SIZE_P MAX_A * MAX_A

// Data structures to represent the system
typedef struct {
    int x; // Column position
    int y; // Row position
} Position;

typedef struct {
    Position positions[SIZE_P]; // There are 16 positions to choose from
    int count;
} PositionSet;

// Function prototypes
// Main menu functions
void displayMenu();
int getValidMenuChoice();
int getValidRestartChoice();
void clearScreen();

// Game flow functions
void playGame();
void initializeSystem();

// Position set operations
bool isInSet(Position pos, PositionSet set);
void addToSet(Position pos, PositionSet *set);
void removeFromSet(Position pos, PositionSet *set);
bool nextPlayerMove(Position pos);

// Game state functions
bool isGameOver();
bool isWinningConfiguration(PositionSet set);
void determineWinner();

// Display functions
void displayGrid();
void displayGameState();

// Input handling functions
Position inputPosition();
void clearInputBuffer();

// Game state
PositionSet Uno;
PositionSet Tres;
PositionSet F;    // Free positions
bool turn;        // true for Uno's turn, false for other
bool go;          // Controls alternation between Uno and Tres
bool over;        // true when game is over

// Set C containing winning configurations
PositionSet C[4];

// Set T (identity relation)
PositionSet T;

int main() {
    int choice;
    bool exitGame = false;
    
    while (!exitGame) {
        displayMenu();
        
        choice = getValidMenuChoice();
        
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                exitGame = true;
                printf("Thanks for playing! Goodbye.\n");
                break;
            default:
                clearScreen();
                printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }
    
    return 0;
}


void displayMenu() { // Display the main menu
    printf("\n  ___ _                            _   ___  _         _             _ \n");
    printf(" | _ \\ |__ _ __ ___   __ _ _ _  __| | |   \\(_)____ __| |__ _ __ ___| |\n");
    printf(" |  _/ / _` / _/ -_) / _` | ' \\/ _` | | |) | (_-< '_ \\ / _` / _/ -_)_|\n");
    printf(" |_| |_\\__,_\\__\\___| \\__,_|_||_\\__,_| |___/|_/__/ .__/_\\__,_\\__\\___(_)\n");
    printf("                                                |_|                   \n");
    printf("1. Play Game\n");
    printf("2. Quit\n");
    printf("=================================\n");
}

void restartMenu() { // Display the restart menu
    printf("\nGame ended. What would you like to do next?\n");
    printf("1. Play Again\n");
    printf("2. Return to Main Menu\n");
    printf("Enter your choice: ");
}

// Play the game
void playGame() {
    // clears the screen
    clearScreen();

    // Initialize the system
    initializeSystem();
    
    // Game loop
    while (!over) {
        displayGrid();
        displayGameState();
        
        printf("Enter position (x,y) to place: ");
        Position pos = inputPosition();
        
        if (!nextPlayerMove(pos)) {
            printf("Invalid move! Try again.\n"); // Displays if position is already taken.
        }
        
        over = isGameOver();
    }
    
    // Display final state and winner
    displayGrid();
    determineWinner();
    
    // Ask if player wants to play again
    restartMenu(); 
    int choice ;
    choice = getValidRestartChoice();
    
    if (choice == 1) {
        playGame();  // Restart the game
    }
    clearScreen();
}

// Initialize the system according to specifications
void initializeSystem() {
    // Initialize player sets to empty
    Uno.count = 0;
    Tres.count = 0;
    
    // Initialize boolean variables
    turn = true;
    go = false;
    over = false;
    
    // Initialize how many free positions are in the grid
    F.count = 0;
    for (int i = 1; i <= MAX_A; i++) {
        for (int j = 1; j <= MAX_A; j++) {
            F.positions[F.count].x = i;
            F.positions[F.count].y = j;
            F.count++;
        }
    }
    
    // Initialize set C with winning configurations
    // C[0]: First row {(1,1),(1,2),(1,3),(1,4)}
    C[0].count = 4;
    for (int i = 0; i < 4; i++) {
        C[0].positions[i].x = 1;
        C[0].positions[i].y = i + 1;
    }
    
    // C[1]: Main diagonal {(1,1),(2,2),(3,3),(4,4)}. Has similarities with T: identity relation
    C[1].count = 4;
    for (int i = 0; i < 4; i++) {
        C[1].positions[i].x = i + 1;
        C[1].positions[i].y = i + 1;
    }
    
    // C[2]: Anti-diagonal {(1,4),(2,3),(3,2),(4,1)}
    C[2].count = 4;
    for (int i = 0; i < 4; i++) {
        C[2].positions[i].x = i + 1;
        C[2].positions[i].y = 4 - i;
    }
    
    // C[3]: Last column {(4,1),(4,2),(4,3),(4,4)}
    C[3].count = 4;
    for (int i = 0; i < 4; i++) {
        C[3].positions[i].x = 4;
        C[3].positions[i].y = i + 1;
    }
    
    // Initialize set T (identity relation). Has similarities with C[1]: main diagonal
    T.count = 4;
    for (int i = 0; i < 4; i++) {
        T.positions[i].x = i + 1;
        T.positions[i].y = i + 1;
    }
}

// Check if a position is in a set
bool isInSet(Position pos, PositionSet set) {
    for (int i = 0; i < set.count; i++) {
        if (set.positions[i].x == pos.x && set.positions[i].y == pos.y) {
            return true;
        }
    }
    return false;
}

// Add a position to a set
void addToSet(Position pos, PositionSet *set) {
    if (!isInSet(pos, *set)) {
        set->positions[set->count] = pos;
        set->count++;
    }
}

// Remove a position from a set
void removeFromSet(Position pos, PositionSet *set) {
    for (int i = 0; i < set->count; i++) {
        if (set->positions[i].x == pos.x && set->positions[i].y == pos.y) {
            // Replace with the last element and decrease count
            set->positions[i] = set->positions[set->count - 1];
            set->count--;
            return;
        }
    }
}

// Process the next player move according to system rules
bool nextPlayerMove(Position pos) {
    clearScreen();

    // Case 1: Uno's turn to add a position
    if (turn && go && isInSet(pos, F)) {
        addToSet(pos, &Uno);
        removeFromSet(pos, &F);
        turn = !turn;
        go = !go;
        return true;
    }

    // Case 2: Dos' turn to remove a position from Uno or Tres
    else if (!turn && (isInSet(pos, Uno) || isInSet(pos, Tres))) {
        removeFromSet(pos, &Uno);
        removeFromSet(pos, &Tres);
        addToSet(pos, &F);
        turn = !turn;
        return true;
    }

    // Case 3: Tres's turn to add a position
    else if (turn && !go && isInSet(pos, F)) {
        addToSet(pos, &Tres);
        removeFromSet(pos, &F);
        go = !go;
        return true;
    }
    
    return false;
}

// Check if the game is over
bool isGameOver() {
    // Check if the game is over by checking if any player has a winning configuration or if there are no free positions left
    return isWinningConfiguration(Uno) || isWinningConfiguration(Tres) || F.count == 0;
}

// Check if a set contains a winning configuration
bool isWinningConfiguration(PositionSet set) {
    for (int i = 0; i < 4; i++) { // // Check if the set contains any of the winning configurations: W = C - T
        bool isWinning = true;
        if (i == 1) continue; // Does not check the main diagonal since it is crossed out in the game rules. C[1] - T == NULL SET
        
        for (int j = 0; j < C[i].count; j++) { // Check if all positions in this configuration are in the set
            if (!isInSet(C[i].positions[j], set)) {
                isWinning = false;
                break;
            }
        }
        
        if (isWinning) {
            return true;
        }
    }
    
    return false;
}

// Determine the winner of the game
void determineWinner() {
    printf("\nGame Over!\n");
    
    if (isWinningConfiguration(Uno)) { // Check if Uno has a winning configuration
        printf("Uno Wins!\n");
    } else if (F.count == 0) { // Check if there are no free positions left in the grid
        printf("Dos Wins!\n"); 
    } else if (isWinningConfiguration(Tres)) { // Check if Tres has a winning configuration
        printf("Tres Wins!\n");
    }
}

// Display the current state of the grid
void displayGrid() {
    printf("\nCurrent Grid:\n");
    printf("  1 2 3 4\n"); // Column numbers
    
    for (int i = 1; i <= MAX_A; i++) {
        printf("%d ", i); // Row number
        
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j}; // Create a position object
            
            if (isInSet(pos, Uno)) {
                printf("U "); // Uno's position
            } else if (isInSet(pos, Tres)) {
                printf("T "); // Tres's position
            } else {
                printf(". "); // Free position
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Display the current game state
void displayGameState() {
    printf("Current State:\n");
    
    if (turn) {
        if (go) { // If it's Uno's turn to place
            printf("Uno's turn to place\n");
        } else { // If it's Tres's turn to place
            printf("Tres's turn to place\n");
        }
    } else { // If it's Dos' turn to remove
        printf("Time to remove a position\n");
    }
}

// Get position input from the user
Position inputPosition() {
    Position pos = {0, 0};
    int result;
    bool validInput = false;
    char buffer[100];

    while (!validInput) {
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) { // Check for EOF or error in input
             // Handle EOF or error
            continue;
        }
        
        if (buffer[0] == '\n') { // Skip if input is just a newline
            clearScreen();
            displayGrid();
            displayGameState();
            printf("\rEnter position (x,y) to place: ");
            fflush(stdout);
            continue;
        }
        
        // Try to parse the input
        result = sscanf(buffer, "%d,%d", &pos.x, &pos.y);
        
        if (result != 2) { // Validate that the input is two integers
            clearScreen();
            printf("Invalid input! Please enter two numbers separated by a comma (example: 2,3)\n");
            displayGrid();
            displayGameState();
            printf("Enter position (x,y) to place: ");
            fflush(stdout);
            continue;
        }
        
        if (pos.x < 1 || pos.x > MAX_A || pos.y < 1 || pos.y > MAX_A) { // Validate that the input is within the valid range
            clearScreen();
            printf("Invalid position! Enter values between 1 and %d\n", MAX_A);
            displayGrid();
            displayGameState();
            printf("Enter position (x,y) to place: ");
            fflush(stdout);
            continue;
        }
        
        // If we made it here, the input is valid
        validInput = true;
    }
    
    return pos;
}

void clearInputBuffer() { // Clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidMenuChoice() {
    int input;
    int choice = 0;
    bool validInput = false;

    while (!validInput) {
        printf("Enter your choice: ");

        input = getchar();

        if (input == '\n') { // Skip if input is just a newline
            clearScreen();
            displayMenu();
        }
        
        // Check if the input is a valid digit (1 or 2)
        if (input >= '1' && input <= '2') {
            choice = input - '0';  // Convert char digit to integer
            
            int nextChar = getchar(); // Check if there are more characters in the buffer (excluding newline)
            if (nextChar != '\n') { // If there are more characters, it's invalid input
                ungetc(nextChar, stdin); // Put back the character we just read
                clearInputBuffer();
                clearScreen();
                printf("Invalid input. Please enter just a single digit (1 or 2)");
                displayMenu();
                continue;
            }
            
            validInput = true;
        } 
        
        else if (input == '\n') { // Skip if input is just a newline
            continue;
        }
        
        else { // Invalid input
            clearInputBuffer();
            clearScreen();
            printf("Invalid choice. Please enter 1 or 2");
            displayMenu();
        }
    }
    
    return choice;
}

int getValidRestartChoice() {
    int input;
    int choice = 0;
    bool validInput = false;

    while (!validInput) {
        input = getchar();
        
        if (input >= '1' && input <= '2') { // Check if the input is a valid digit (1 or 2)
            choice = input - '0';  // Convert char digit to integer
            
            int nextChar = getchar(); // Check if there are more characters in the buffer (excluding newline)

            if (nextChar != '\n') { // If there are more characters, it's invalid input
                ungetc(nextChar, stdin); // Put back the character we just read
                clearInputBuffer();
                clearScreen();
                printf("Invalid input. Please enter just a single digit (1 or 2)\n");
                displayGrid();
                determineWinner();
                restartMenu();
                continue;
            }
            
            validInput = true;
        } 

        else if (input == '\n') { // Skip if input is just a newline
            clearScreen();
            displayGrid();
            determineWinner();
            restartMenu();
            continue;
        }
        
        else { // Invalid input
            clearInputBuffer();
            clearScreen();
            printf("Invalid choice. Please enter 1 or 2\n");
            displayGrid();
            determineWinner();
            restartMenu();
        }
    }
    
    return choice;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls"); // For Windows
    #else
        system("clear"); // For Unix/Linux/MacOS
    #endif
}