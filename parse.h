#ifndef PARSE_H
#define PARSE_H

enum Instruction {
	NEW_DISEASE_ENTER_DESCRIPTION,
	NEW_DISEASE_COPY_DESCRIPTION,
	PRINT_DESCRIPTION,
	CHANGE_DESCRIPTION,
	DELETE_PATIENT_DATA
};

extern char* getDescription();
extern char* getName1();
extern char* getName2();
extern enum Instruction getInstruction();
extern int getNumber();
extern void clearCommand();
int getLine();
#endif /* PARSE_H */
