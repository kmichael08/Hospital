// Michal Kuzba 371148 - zadanie hospital
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_LENGTH 10
#define MAX_LINE_SIZE 100001

enum Instruction {
	NEW_DISEASE_ENTER_DESCRIPTION,
	NEW_DISEASE_COPY_DESCRIPTION,
	PRINT_DESCRIPTION,
	CHANGE_DESCRIPTION,
	DELETE_PATIENT_DATA
};

struct Command {
	enum Instruction instruction;
	char *name1;
	char *name2;
	int number;
	char *description;	
};

typedef struct Command* Request;

Request actualCommand;
char line[MAX_LINE_SIZE];

static void readLine(char *line);

int getLine() {
	
	if(!fgets(line, MAX_LINE_SIZE, stdin)) 
		return 0;

	readLine(line);	
	
	return 1;
}

char* getDescription() {
	return actualCommand->description;
}

char* getName1() {
	return actualCommand->name1;
}

char* getName2() {
	return actualCommand->name2;
}

enum Instruction getInstruction() {
	return actualCommand->instruction;
}

int getNumber() {
	return actualCommand->number;
}

void clearCommand() {
	free(actualCommand->description);
	free(actualCommand->name1);
	free(actualCommand->name2);
	free(actualCommand);
}

static enum Instruction getCommand(char *pol) {
	if  (strcmp(pol, "NEW_DISEASE_ENTER_DESCRIPTION") == 0) 
		return NEW_DISEASE_ENTER_DESCRIPTION;
	if  (strcmp(pol, "NEW_DISEASE_COPY_DESCRIPTION") == 0) 
		return NEW_DISEASE_COPY_DESCRIPTION;
	if  (strcmp(pol, "PRINT_DESCRIPTION") == 0) 
		return PRINT_DESCRIPTION;
	if  (strcmp(pol, "CHANGE_DESCRIPTION") == 0) 
		return CHANGE_DESCRIPTION;
	if  (strcmp(pol, "DELETE_PATIENT_DATA") == 0) 
		return DELETE_PATIENT_DATA;
}

// indeks nastepnej spacji(znaku konca)
static int nextSpace(char *line, int begin) {
	int iter = begin;
	
	while(line[iter] != ' ' && line[iter] != '\n') {
		iter++;
	}
	
	return iter;
}

static int lastMark(char *line, int begin) {
	int iter = begin;
	
	while(line[iter] != '\n') {
		iter++;
	}
	
	return iter;
}

// do word zapisujemy fragment napisu line w zakresie [begin, end)
static void getWord(char* line, char *word, int begin, int end) {
	
	int i = 0;
	while (i < end - begin)
	{
		word[i] = line[i + begin];
		i++;
	}
	word[i] = '\0';
}


static void readLine(char *line) {
	
	actualCommand = malloc(sizeof(struct Command));
	actualCommand->number = 0;
	actualCommand->description = NULL;
	actualCommand->name2 = NULL;
		
	// wyluskujemy polecenie
	int end = nextSpace(line, 0);
	
	char *word = malloc(sizeof(char) * (end + 2)); 
	
	getWord(line, word, 0, end);
			
	actualCommand->instruction = getCommand(word);
	
	free(word);
	
	// wyluskujemy nazwisko
	int begin = end + 1;
	end = nextSpace(line, begin);
	
	actualCommand->name1 = malloc(sizeof(char) * (end - begin + 2));
	
	getWord(line, actualCommand->name1, begin, end);
	
	switch(actualCommand->instruction) {
		case NEW_DISEASE_COPY_DESCRIPTION :
		{
			begin = end + 1;
			end = lastMark(line, begin);
			
		    actualCommand->name2 = malloc(sizeof(char) * (end - begin + 2)); 

			getWord(line, actualCommand->name2, begin, end);			
		}
		break;		
		
		case PRINT_DESCRIPTION :
		{
			begin = end + 1;
			end = nextSpace(line, begin);
			
			char *numberString = malloc(sizeof(char) *  MAX_NUMBER_LENGTH);
			getWord(line, numberString, begin, end);
			
			// przepisujemy liczbe ze stringa na inta
			actualCommand->number = atoi(numberString);	
			
			free(numberString);	
		}
		break;
		
		case CHANGE_DESCRIPTION :
		{
			begin = end + 1;
			end = nextSpace(line, begin);
			
			char *numberString = malloc(sizeof(char) *  MAX_NUMBER_LENGTH);
			getWord(line, numberString, begin, end);
			
			// przepisujemy liczbe ze stringa na inta
			actualCommand->number = atoi(numberString);			
			
			free(numberString);
			
			begin = end + 1;
			end = lastMark(line, begin);
			
			actualCommand->description = malloc(sizeof(char) * (end - begin + 2));
			
			getWord(line, actualCommand->description, begin, end);
			
		}
		break;
		
		case NEW_DISEASE_ENTER_DESCRIPTION :
		{
			begin = end + 1;
			end = lastMark(line, begin);
			
			actualCommand->description = malloc(sizeof(char) * (end - begin + 2));
			
			getWord(line, actualCommand->description, begin, end);
						
		}
		
		case DELETE_PATIENT_DATA :
		break;
		
	}	
	
}
