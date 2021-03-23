typedef struct mineField_struct* MineField;

struct mineField_struct {
	int* field;
	int numberOfMines;
	int width; int height;
	short int mineChance; // chance of having a mine in a cell
	int status; // 0: paused / 1: playing / -1: lost
	int revealedEmptyCells;
};

void play( MineField mineField );
int changeSettings( MineField minefield, int width, int height, int mineChance );
void startGame( MineField minefield );

MineField createField();

void clearField( MineField mineField );
void fillField( MineField mineField, int startedCell);
void showField( MineField mineField );
void showLayer( MineField mineField, int layer);
