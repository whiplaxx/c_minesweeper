#include "minesweeper.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int DEFAULT_WIDTH = 10;
int DEFAULT_HEIGHT = 10;
int DEFAULT_MINECHANCE = 15;

int MAX_WIDTH = 26;
int MAX_HEIGHT = 30;
int COORD_LENGTH = 3;

char ALP_CAPT[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char ALP_LOW[] = "abcdefghijklmnopqrstuvwxyz";

int getNumOfMinesInCell( MineField mineField, int cell);
int openView( MineField mineField, int cell);
int getCell();
int getColumn( char x_char );

void play( MineField mineField ){

	showField( mineField );
	int cell = getCell(mineField);
	fillField( mineField, cell);
	
	int numOfCells = mineField->width * mineField->height;

	mineField->status = 1;
	while( mineField->status != -1 && mineField->revealedEmptyCells < numOfCells - mineField->numberOfMines ){
		showField( mineField );
		cell = getCell(mineField);
		if( openView( mineField, cell ) == -1){
			mineField->status = -1;
		}
	}
	showField( mineField );

	if( mineField->status == -1){ printf("You lose!\n"); }
	else{ printf("You won!\n"); }
}

int getCell( MineField mineField ){
	char x_char; int x = -1;
	int y = MAX_HEIGHT;
	
	int cell;
	int layerAdder = mineField->width * mineField->height;
	do{
		char coord[3];
		scanf("%s", coord);
		sscanf(coord, "%d%c", &y, &x_char);
		x = getColumn(x_char);
		cell = (y * mineField->width) + x; 
	}while( *(mineField->field + cell + layerAdder)== 1 );
	return cell;
}

MineField createField(){
	
	MineField mineField = (MineField) malloc( sizeof(struct mineField_struct) );

	mineField->field = (int*) malloc( sizeof(int)*(DEFAULT_WIDTH*DEFAULT_HEIGHT)*2 );
	if( mineField->field == NULL )
		return NULL;
	mineField->width = DEFAULT_WIDTH; mineField->height = DEFAULT_HEIGHT;
	mineField->mineChance = DEFAULT_MINECHANCE;
	mineField->status = 0;
	mineField->revealedEmptyCells = 0;

	return mineField;
}

int changeSettings( MineField mineField, int width, int height, int mineChance ){
 
	if( (width < 1 || width > MAX_WIDTH) || (height < 1 || height > MAX_HEIGHT) || mineChance < 0 || mineChance > 100 )
		return 0;

 	int* temp_alloc = (int*) malloc( sizeof(int)*(width*height)*2 );
	if( temp_alloc == NULL )
		return 0;

	free(mineField->field);
	mineField->field = temp_alloc;
	mineField->width = width; mineField->height = height;
	mineField->mineChance = mineChance;
	return 1;
}

void clearField( MineField mineField ){
	
	free(mineField->field);
	mineField->numberOfMines = 0;

}

void fillField(MineField mineField, int startedCell){
	// startedCell: cell clicked initialy by the player

	srand(time(NULL));
	
	int layerAdd = (mineField->width * mineField->height);

	// Filling layer 0 (putting the mines)
	mineField->numberOfMines = 0;
	for(int cell=0; cell < layerAdd; cell++){
		int randNumber = rand()%101;
		if( randNumber <= mineField->mineChance && cell != startedCell ){
			*(mineField->field + cell) = -1;
			mineField->numberOfMines++;
		}else{ *(mineField->field + cell) = 0; }

		*(mineField->field + cell + layerAdd) = 0;
	}

	// Filling layer 1 (putting the numbers)
	for(int cell=0; cell < layerAdd; cell++){
		if( *(mineField->field + cell) == 0 )
			*(mineField->field + cell) = getNumOfMinesInCell( mineField, cell );
	}

	openView( mineField, startedCell);
}

int getNumOfMinesInCell( MineField mineField, int cell){

	int numOfMines = 0;
	int vertAdders[] = { -mineField->width, 0, mineField->width }; // vertical adder
	int layerAdd = mineField->width * mineField->height;

	for( int vert = 0; vert < 3; vert++){
		int verfCell = cell + vertAdders[ vert ];
		if(verfCell >= 0 && verfCell < layerAdd ){
			for( int horiz = -1; horiz <= 1; horiz++){ // horizontal adder
				if( (verfCell%mineField->width)+horiz >= 0 && (verfCell%mineField->width)+horiz < mineField->width ){
					if( *(mineField->field + verfCell + horiz) == -1 )
						numOfMines++;
				}
			}
		}
	}
	// IDEIA: AO INVES DE CONTAR O NUMERO DE BOMBAS AO REDOR DE CADA CÉLULA, PERCORRER TODAS AS CELULAR COM MINAS E INCREMENTAR 1 EM TODAS AS CELULAR AO REDOR DE CADA UMA
	return numOfMines;
}

int openView( MineField mineField, int cell){
	
	int layerAdd = mineField->width * mineField->height;
	if( *(mineField->field + cell) == -1 ){
		*(mineField->field + cell + layerAdd) = 1;
		return -1;
	}

	int vertAdders[] = { -mineField->width, 0, mineField->width }; // vertical adder
	
	int cellValue = *(mineField->field + cell);
	if(	*(mineField->field + cell + layerAdd) == 0){
		*(mineField->field + cell + layerAdd) = 1;
		mineField->revealedEmptyCells += 1;
	}

	for( int vert = 0; vert < 3; vert++){ // vertical iteration
		int verfCell = cell + vertAdders[ vert ];
		if(verfCell >= 0 || verfCell < layerAdd ){ // if is not beyond the vertical scope
			for( int horiz = -1; horiz <= 1; horiz++){ // horizontal iteration
				if( (verfCell%mineField->width)+horiz >= 0 && (verfCell%mineField->width)+horiz < mineField->width ){ // if is not beyond the horizontal scope

					if( cellValue == 0 ){
						if( *(mineField->field + verfCell + horiz) > 0 && *(mineField->field + verfCell + horiz + layerAdd) == 0 ){ // 
							*(mineField->field + verfCell + horiz + layerAdd) = 1;
							mineField->revealedEmptyCells += 1;
						}
					}
					if( *(mineField->field + verfCell + horiz) == 0 && *(mineField->field + verfCell + horiz + layerAdd) == 0)
						openView( mineField, verfCell+horiz );
						
				}
			}
		}
	}
	return 1;
}

void showField( MineField mineField ){
	
	printf("Field | %d Mines | %dx%d\n", mineField->numberOfMines, mineField->width, mineField->height);
	
	printf("      ");
	for(int col = 0; col < mineField->width; col++)
		printf("%c ", ALP_CAPT[col]);

	int layerAdd = mineField->width * mineField->height;
	for(int cell = 0; cell < layerAdd; cell++){
		if( cell % mineField->width == 0 )
			printf("\n%3d | ", (cell)/mineField->width);
		int showCell = *(mineField->field + cell + layerAdd );

		if( showCell == 1 ){
			char c;
			if( *(mineField->field + cell) == -1 ){
				printf("@ ");
			}else{
				printf("%d ", *(mineField->field + cell));
			}
		}
		else{
			printf("# ");
		}
	}
	printf("\n");
}

void showLayer( MineField mineField, int layer){
	
	printf("Field | %d Mines | %dx%d\n", mineField->numberOfMines, mineField->width, mineField->height);

	int layerAdd = mineField->width * mineField->height;
	for(int cell = 0; cell < layerAdd; cell++){	
		int valueCell = *(mineField->field + cell + layer*layerAdd );
		if( valueCell == -1 ){
			printf("@ ");
		}
		else{
			printf("%d ", valueCell);
		}

		if( (cell+1) % mineField->width == 0 )
			printf("\n");
	}
}

int getColumn(char x_char){

	for(int i = 0; i < 26; i++){
		if( ALP_CAPT[i] == x_char )		
			return i;
	}
	for(int i = 0; i < 26; i++){
		if(ALP_LOW[i] == x_char)
			return i;
	}
	return -1;
}







