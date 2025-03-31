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

