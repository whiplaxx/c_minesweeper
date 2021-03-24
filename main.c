#include <stdio.h>
#include <stdlib.h>
#include "minesweeper.h"

int main(){

	MineField mineField = createField();
	changeSettings( mineField, 14, 18, 15);
	
	//fillField(mineField, 13);

	play( mineField );

	clearField(mineField);

	return 0;
}
