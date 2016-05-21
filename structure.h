#ifndef STRUCTURE_H
#define STRUCTURE_H

extern int globalDiseaseNumber();
extern void newDiseaseEnterDescription(char *name, char *description);
extern void printDescription(char *name, int n);
extern void deletePatientData(char *name);
extern void newDiseaseCopyDescription(char *name1, char *name2);
extern void changeDescription(char *name, int n, char *description);
extern void cleanMemory();

#endif /* STRUCTURE_H */
