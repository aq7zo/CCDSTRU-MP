#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Constants for the system
#define MAX_A 4  // Maximum element in set A (1 to 4)
#define SIZE_P 16 // Size of set P (4x4 grid)

// Data structures to represent the system
typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position positions[SIZE_P];
    int count;
} PositionSet;

// Function prototypes
void initializeSystem();
void displayGrid();
void displayGameState();
bool isInSet(Position pos, PositionSet set);
void addToSet(Position pos, PositionSet *set);
void removeFromSet(Position pos, PositionSet *set);
bool nextPlayerMove(Position pos);
bool isGameOver();
void determineWinner();
bool isWinningConfiguration(PositionSet set);
Position inputPosition();