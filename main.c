#include <stdio.h>
#include <stdlib.h>
#include "minesweeper.h"

int main(){

	MineField mineField = createField();
	changeSettings( mineField, 5, 5, 15);
	
	fillField(mineField, 13);
	showLayer(mineField, 0);

	//play( mineField );

	clearField(mineField);

	return 0;
}
