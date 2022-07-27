
//This program creates a char array of user defined size
//It consists of one repeated char that the user gets to pick


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define GRIDSIZE 20


struct unit {
  struct unitInfo *info;
  int x;
  int y;
};
struct unitInfo {
  char letter;
  char hp;
  char attack;
};
struct unitAmount {
  struct unitInfo *info;
  int amount;
};

int askInfo(struct unitInfo *info, struct unitAmount *quantity, int i) {
	char characterUsed;
	int numberUsed;
	//Ask the user which character they want to use to represent a unit and how many of each unit
	do {
		printf("What character should unit %d be represented by? X or x is prohibited. ", i);
		scanf(" %c", &characterUsed);
		//clean input buffer
		while(getchar() != '\n');
		
		info->letter = characterUsed;
	} while (characterUsed == 'X' || characterUsed == 'x');
	
	puts("");
	printf("How many units of %c? ",info->letter);
	scanf(" %d", &numberUsed);
	quantity->amount = numberUsed;
	puts("");
	
	info->hp = 10;
	info->attack = 5;
	
	return numberUsed;
}

struct unit *combineArrays(struct unit *warriors[], struct unitAmount quantity[], int totalUnits, int unitTypes) {
	
	struct unit *combinedArray = (struct unit *)malloc(sizeof(struct unit)*(totalUnits));
	int n = 0;
	
	for(int i = 0; i < unitTypes; i++) 
	{
		for(int j = 0; j < quantity[i].amount; j++) 
		{
//			printf("%c,(%d,%d)", warriors[i][j].info->letter, warriors[i][j].x, warriors[i][j].y);
			combinedArray[n] = warriors[i][j];
//			printf("%c,(%d,%d)", combinedArray[n].info->letter, combinedArray[n].x, combinedArray[n].y);
//			printf("\n");
			n++;
		}
	
	}
	
	return combinedArray;
}

int assignBegArrayPos(struct unit *warriors[], struct unitAmount quantity[], int unitTypes) {
	int row = 0; 
	//first loops through each unitType
	for(int i = 0; i < unitTypes; i++) 
	{
		int column = 0;
		
		//then loops through each individual unit, printing the desired number of chars
		for(int j = 0; j < quantity[i].amount; j++) 
		{
			//print newline if column limit is reached
			if(column % GRIDSIZE == 0 && column != 0)
			{
//				printf("\n");
				row++;
				column = 0;
			}
			
//			printf("%c", quantity[i].info->letter);
			warriors[i][j].x = column;
			warriors[i][j].y = row;
//			printf("(%d,%d)", warriors[i][j].x, warriors[i][j].y);
			column++;			
			
		}
 		//print newline at the end of every unit type unless is exactly a full row
		if(column != 0 || ((quantity[i].amount - column) % GRIDSIZE) == 0){
//			printf("\n");
			row++;
			column = 0;
		}
	}
	row--;
	return row;
	
}

void renumberArrayPos(struct unit array[], int totalUnits) {
	
	struct unit temp; //used to store the old xy coords
	
	//these are for checking if the row pos has changed according to the old xy values -- uses temp
	int rowBeg, rowEnd;
	rowBeg = rowEnd = array[0].y; //initialize to the y value of the first element
	
	//row and column keep track of the new, actual pos of the printed characters -- uses array
	int column, row;
	row = column = 0; 
//	printf("\n");
	
	for(int i = 0; i < totalUnits; i++) {
		
		temp = array[i]; 
		
		rowEnd = (temp.y != rowEnd) ? temp.y : rowEnd; //check if row should change
		if(rowEnd != rowBeg) 
		{
			rowBeg = rowEnd;
//			printf("\n");
			row++;
			column = 0;
		}
	
		array[i].x = column;
		array[i].y = row;
//		printf("%c(%d,%d)", array[i].info->letter, array[i].x, array[i].y);
		column++;			
	}
}
 
void reverseArmy(struct unit *reversedArray, struct unit *array, int totalUnits) {
	
	for(int i = 0; i < totalUnits; i++) {
		reversedArray[i] = array[totalUnits-i-1];
	}
	
	renumberArrayPos(reversedArray, totalUnits);
 }
 
void printArmy(struct unit array[], int totalUnits) {	
	int rowBeg, rowEnd;
	rowBeg = rowEnd = array[0].y;
	
	for(int i = 0; i < totalUnits; i++)
	{
		rowEnd = (array[i].y != rowEnd) ? array[i].y : rowEnd;
		if(rowEnd != rowBeg) 
		{
			printf("\n");
			rowBeg = rowEnd;
		}
		printf("%c", array[i].info->letter);
	}
}

void selectRegion(struct unit array[], int selectionIndex[], int *row, int totalUnits) {
	int counter = 0;
	for(int i = 0; i < totalUnits; i++)
	{
		//fills index with 1 if this element is still alive and is in the correct row
		selectionIndex[i] = counter = ((*row==array[i].y && array[i].info->hp!=0) ? 1 : 0);
		
	}
	if(counter = GRIDSIZE) {
		*row++;
	}
	printf("\n");
}

void printRegion(int selectionIndex[], struct unit array[], int totalUnits) {
	
	for(int i = 0; i < totalUnits; i++)
	{
		if(selectionIndex[i] == 1) {
			printf("%c", array[i].info->letter);
		}
	}
}

int allocateAndAssign(int i, struct unit *array[], struct unitAmount quantity[], struct unitInfo info[]) {
		int totalUnits = 0;
		totalUnits += askInfo(&info[i], &quantity[i], i+1);
		
		array[i] = (struct unit *)malloc(sizeof(struct unit) * quantity[i].amount);
		
		quantity[i].info = &info[i];
		
		//fill each individual warrior with its info member
		for(int j = 0; j < quantity[i].amount; j++) 
		{
			array[i][j].info = &info[i];
		}
		
		return totalUnits;
}

int allocateAndAssignEnemy(int i, int enemyAmount, struct unit *array[], struct unitAmount quantity[], struct unitInfo info[]) {
	
	int totalUnits = quantity[i].amount = enemyAmount;
	array[i] = (struct unit *)malloc(sizeof(struct unit) * quantity[i].amount);
	
	do {
		info[i].letter = ('!' + (rand() % 89)); //roll a random character as the letter

	} while (toupper(info[i].letter) == 'X');
	
	quantity[i].info = &info[i];
	
	//fill each individual warrior with its info member
	for(int j = 0; j < quantity[i].amount; j++) 
	{
		array[i][j].info = &info[i];
	}
	return totalUnits;
}

int selectFighter(int *selectionIndex, int totalUnits) {
	
	int j = 0;
	
	for(int i = 0; i < totalUnits; i++) {
		//determine the number of units that are selectable
		j += ((selectionIndex[i] == 1) ? 1 : 0);
		if(j == GRIDSIZE) {
			break;
		}
	}
	
	int selectedUnit = ((rand() % j) + 1); //select one of the eligible units and store it in j
	printf("Units in Region: %d\n", j);
	printf("Selected Unit: %d\n", selectedUnit);
	j = 0;
	//loop through the entire selectionIndex array
	for(int i = 0; i < totalUnits; i++) {
		
		if(selectionIndex[i] == 1) {
			j++;
			//set the nonselected elements in the selectionIndex equal to zero
			if(selectedUnit != j) {
				selectionIndex[i] = 0;
				printf("%d", selectionIndex[i]);
			}
			else { //print and return the selected fighter
				printf("%d", selectionIndex[i]);
				return selectionIndex[i];
			}
		}
	}
}

void printFighter(int fighterLocation, struct unit array[], int totalUnits, int reversed) {
	if(reversed == 1) {
	printf("\n\nSelected fighter: %c (%d,%d)\n\n", array[totalUnits-fighterLocation].info->letter,
	array[fighterLocation].x, array[fighterLocation].y);
	}
	else{
	printf("\n\nSelected fighter: %c (%d,%d)\n\n", array[fighterLocation].info->letter,
	array[fighterLocation].x, array[fighterLocation].y);
	}
}

void fight(int fighterLocation1, int fighterLocation2, struct unit array[]) {
	
}

void main() 
{
	time_t t;
	srand(time(&t));
	
	int totalUnitsArmy1, totalUnitsArmy2;
	totalUnitsArmy1 = totalUnitsArmy2 = 0;
	int unitTypes, enemyUnitTypes, enemyAmount, fighterLocation1, fighterLocation2, rowCount1, rowCount2;

	int selectedRegion1, selectedRegion2;
	//enemy army unit constants
	enemyUnitTypes = 3;
	enemyAmount = 30;
	
	
	printf("How many different unit types would you like in your army? ");
	scanf("%d", &unitTypes);
	printf("\n");
	
	struct unit *warriors[unitTypes]; struct unitInfo info[unitTypes]; struct unitAmount quantity[unitTypes];
	struct unit *enemyArray[enemyUnitTypes]; struct unitInfo enemyInfo[enemyUnitTypes]; struct unitAmount enemyQuantity[enemyUnitTypes];

	//Ask for info on units, allocate memory, and assign member values
	
	for(int i = 0; i < unitTypes; i++) {
		
		totalUnitsArmy1 += allocateAndAssign(i, warriors, quantity, info);
	}

	//Enemy army allocations and assignments	
	for(int i = 0; i < enemyUnitTypes; i++) { 
		enemyInfo[i].hp = 10;
		enemyInfo[i].attack = 10;
		totalUnitsArmy2 += allocateAndAssignEnemy(i, enemyAmount, enemyArray, enemyQuantity, enemyInfo);
	}
	//number of rows in each array
	rowCount1 = assignBegArrayPos(warriors, quantity, unitTypes);
	rowCount2 = assignBegArrayPos(enemyArray, enemyQuantity, enemyUnitTypes);
	
	//combine 2d warrior array into a 1d combined array
	struct unit *consolidatedArray1, *consolidatedArray2;
	consolidatedArray1 = combineArrays(warriors, quantity, totalUnitsArmy1, unitTypes);
	consolidatedArray2 = combineArrays(enemyArray, enemyQuantity, totalUnitsArmy2, enemyUnitTypes);

	printf("\nTotal units in your army is: %d\n", totalUnitsArmy1);
	printf("\nTotal units in their army is: %d\n", totalUnitsArmy2);
 	
	// Initialize the reversed struct that will be passed in to the function
	struct unit reversedArray[totalUnitsArmy2];
	
	printf("Your Army\n");
	printArmy(consolidatedArray1, totalUnitsArmy1);
	
	printf("\n\nEnemy Army\n");
//	reverseArmy(reversedArray, consolidatedArray2, totalUnitsArmy2); printf("\n");
	printArmy(consolidatedArray2, totalUnitsArmy2);

 	int selectionIndex[GRIDSIZE];
	//set the selectedRegion to the first last row in the first array and the first row in the second array
	//this makes it so the combined selection is next to one another
	selectedRegion1 = rowCount1;
	selectedRegion2 = 0;
	
	printf("\n\nSelected Region 1");
	selectRegion(consolidatedArray1, selectionIndex, &selectedRegion1, totalUnitsArmy1);
	printRegion(selectionIndex, consolidatedArray1, totalUnitsArmy1);
	printf("\n");
	fighterLocation1 = selectFighter(selectionIndex, totalUnitsArmy1);
	
	selectRegion(consolidatedArray2, selectionIndex, &selectedRegion2, totalUnitsArmy2);
	printf("\nSelected Region 2\n");
	printRegion(selectionIndex, consolidatedArray2, totalUnitsArmy2);
	printf("\n");
	fighterLocation2 = selectFighter(selectionIndex, totalUnitsArmy2);
	
	printf("\n");
	printFighter(fighterLocation1, consolidatedArray1, totalUnitsArmy1, 1);
	printFighter(fighterLocation2, consolidatedArray2, totalUnitsArmy2, 0);
	

		
	//free allocated memory
	free(consolidatedArray1); free(consolidatedArray2);
	for(int i = 0; i < unitTypes; i++) {
		free(warriors[i]);
	}
	for(int i = 0; i < enemyUnitTypes; i++) {
		free(enemyArray[i]);
	}
	

}

