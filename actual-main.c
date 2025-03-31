#include <stdio.h>
#include "helper.h"

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
    // Initialize the system
    initializeSystem();
    
    // Game loop
    while (!over) {
        displayGrid();
        displayGameState();
        
        printf("Enter position (x,y) to place: ");
        Position pos = inputPosition();
        
        if (nextPlayerMove(pos)) {
            printf("Move accepted!\n");
        } else {
            printf("Invalid move! Try again.\n");
        }
        
        over = isGameOver();
    }
    
    // Display final state and winner
    displayGrid();
    determineWinner();
    
    return 0;
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
    
    // Initialize set F to contain all positions in P
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
    
    // C[1]: Main diagonal {(1,1),(2,2),(3,3),(4,4)}
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
    
    // Initialize set T (identity relation)
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
    // Case 1: Uno's turn to add a position
    if (turn && go && isInSet(pos, F)) {
        addToSet(pos, &Uno);
        removeFromSet(pos, &F);
        turn = !turn;
        go = !go;
        return true;
    }
    // Case 2: Remove a position from Uno or Tres
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
    // Game is over if Uno or Tres has a winning configuration or F is empty
    return isWinningConfiguration(Uno) || isWinningConfiguration(Tres) || F.count == 0;
}

// Check if a set contains a winning configuration
bool isWinningConfiguration(PositionSet set) {
    // Check if the set contains any of the winning configurations from C - T
    for (int i = 0; i < 4; i++) {
        bool isWinning = true;
        
        // Skip if this is the identity relation (T)
        if (i == 1) continue; // Since C[1] is the main diagonal which is T
        
        // Check if all positions in this configuration are in the set
        for (int j = 0; j < C[i].count; j++) {
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
    
    if (isWinningConfiguration(Uno)) {
        printf("Uno Wins!\n");
    } else if (F.count == 0) {
        printf("Dos Wins!\n");
    } else if (isWinningConfiguration(Tres)) {
        printf("Tres Wins!\n");
    }
}

// Display the current state of the grid
void displayGrid() {
    printf("\nCurrent Grid:\n");
    printf("  1 2 3 4\n");
    
    for (int i = 1; i <= MAX_A; i++) {
        printf("%d ", i);
        
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j};
            
            if (isInSet(pos, Uno)) {
                printf("U ");
            } else if (isInSet(pos, Tres)) {
                printf("T ");
            } else {
                printf(". ");
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
        if (go) {
            printf("Uno's turn to place\n");
        } else {
            printf("Tres's turn to place\n");
        }
    } else {
        printf("Time to remove a position\n");
    }
}

// Get position input from the user
Position inputPosition() {
    Position pos;
    scanf("%d,%d", &pos.x, &pos.y);
    
    // Validate input
    while (pos.x < 1 || pos.x > MAX_A || pos.y < 1 || pos.y > MAX_A) {
        printf("Invalid position! Enter values between 1 and 4: ");
        scanf("%d,%d", &pos.x, &pos.y);
    }
    
    return pos;
}

