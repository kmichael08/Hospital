// Michal Kuzba 371148 - zadanie hospital
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "parse.h"

int main(int arg, char *argv[]) {
	
	// poprawnosc parametrow
	if (arg > 2 || (arg == 2 && strcmp(argv[1], "-v") != 0)) {
		printf("ERROR\n");
		return 1;
	}
	
	int isParameter = (arg == 2);
	
	while(getLine()) {
	
		switch (getInstruction()) {
			case NEW_DISEASE_ENTER_DESCRIPTION :
			{
				newDiseaseEnterDescription(getName1(), getDescription());			
			}
			break;
			
			case NEW_DISEASE_COPY_DESCRIPTION :
			{
				newDiseaseCopyDescription(getName1(), getName2());
			}
			break;
			
			case PRINT_DESCRIPTION :
			{
				printDescription(getName1(), getNumber());
			}
			break;
			
			case CHANGE_DESCRIPTION :
			{
				changeDescription(getName1(), getNumber(), getDescription());
			}
			break;
			
			case DELETE_PATIENT_DATA :
			{
				deletePatientData(getName1());
			}
			break;
			
		}
		
		// zwalniamy pamiec polecenia
		clearCommand();
		
		// jesli wywolane z parametrem -v to wypisujemy na wyjscie diagnostyczne
		if (isParameter) 
			fprintf(stderr, "DESCRIPTIONS: %d\n", globalDiseaseNumber());
		
	}
	
	cleanMemory();
	
	return 0;
}
