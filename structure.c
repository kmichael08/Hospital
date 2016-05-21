// Michal Kuzba 371148 - zadanie hospital
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Patient* PatientList;
typedef struct PatientDisease* DiseasesList;

struct Disease {
	char *diseaseDescription;
	int referenceCounter;
};

struct PatientDisease {
	struct Disease* description;
	DiseasesList next;
	DiseasesList previous;
};

struct Patient {
	char *name;
	PatientList next;
	DiseasesList patientDiseasesList;
	int diseaseCounter;
};

// struktura danych
static PatientList globalPatientsList = NULL;
static int globalDiseasesCounter = 0;

static int isEmptyPatList(PatientList patList) {
	return patList == NULL;
}

static int isEmptyDiseaseList(DiseasesList disList) {
	return disList == NULL;
}

static void addPatient(PatientList *patList, char* name) {
	PatientList newPatient;
	newPatient = malloc(sizeof(struct Patient));
	newPatient->name = malloc(sizeof(char) * (strlen(name) + 1));
	newPatient->next = NULL;
	newPatient->patientDiseasesList = NULL;
	newPatient->diseaseCounter = 0;
	strcpy(newPatient->name, name);
	
    if(isEmptyPatList(*patList)) *patList = newPatient;
    else {
        newPatient->next = *patList;
        *patList = newPatient;
    }
}

// dodawanie choroby na konkretna liste chorob
static void addDisease(DiseasesList *disList, struct Disease* dis) {
	DiseasesList newDisease;
	newDisease = malloc(sizeof(struct PatientDisease));
	newDisease->description = dis;
	newDisease->next = NULL;
	newDisease->previous = NULL;	
	dis->referenceCounter++;
	
	if(isEmptyDiseaseList(*disList)) *disList = newDisease;
	else {
		newDisease->next = *disList;
		(*disList)->previous = newDisease;
		(*disList) = newDisease;
	}
}

static DiseasesList frontDisease(DiseasesList *disList) {
	return (*disList);
}

// wywolujemy tylko jesli n-ta choroba istnieje
static DiseasesList nthDisease(DiseasesList *disList, int n) {
	if(n == 1) return (*disList);
	return nthDisease(&(*disList)->next, n - 1);
}

// usuwamy chorobe z pamieci jesli nic juz na nia nie wskazuje
static void deleteDisease(DiseasesList disList) {
	free(disList->description->diseaseDescription);
	free(disList->description);
	globalDiseasesCounter--;
}

static void removeDiseasesList(DiseasesList disList) {
	if(isEmptyDiseaseList(disList)) return;
	
	disList->description->referenceCounter--;
	
	if(disList->description->referenceCounter == 0) 
		deleteDisease(disList);
		
	removeDiseasesList(disList->next);
	free(disList);
}

static int isPatient(PatientList *patList, char* name) {
	
	if(isEmptyPatList(*patList)) return 0;
	if(strcmp((*patList)->name, name) == 0) return 1;
	
	return isPatient(&(*patList)->next, name);
}

// wywolujemy tylko jesli pacjent istnieje
static PatientList findPatient(PatientList *patList, char *name) {
	if(strcmp((*patList)->name, name) == 0) return (*patList);
	return findPatient(&(*patList)->next, name);
}

static DiseasesList* listOfPatientDiseases(PatientList *patList, char *name) {
		return (&(findPatient(patList, name)->patientDiseasesList));
}

static void removeAllPatients(PatientList patList) {
	if(isEmptyPatList(patList)) return;
	
	removeDiseasesList(patList->patientDiseasesList);
	removeAllPatients(patList->next);
	
	free(patList->name);
	free(patList);	
}

void cleanMemory() {
	removeAllPatients(globalPatientsList);
}

int globalDiseaseNumber() {
	return globalDiseasesCounter;
}

/* moze tworzyc nowego czlowieka, 
 * tworzy nowa chorobe i dodaje jej przypadek na poczatek */
void newDiseaseEnterDescription(char *name, char *description) {
			
	if (!isPatient(&globalPatientsList, name)) 
		addPatient(&globalPatientsList, name);
		
	struct Disease* dis;
	dis = malloc(sizeof(struct Disease));
	dis->referenceCounter = 0;
	dis->diseaseDescription = malloc(sizeof(char) * strlen(description) + 2);
	strcpy(dis->diseaseDescription, description);
		
	DiseasesList* disListWsk;
	disListWsk = listOfPatientDiseases(&globalPatientsList, name);
	
	addDisease(disListWsk, dis);
	
	(findPatient(&globalPatientsList, name))->diseaseCounter++;
	
	// tworzymy nowa chorobe
	globalDiseasesCounter++;
	
	printf("OK\n");
}

// wypisuje na standardowe wyjscie opis n-tej choroby danego czlowieka
void printDescription(char *name, int n) {
	
	// pacjent nie istnieje
	if(!isPatient(&globalPatientsList, name)) {
		printf("IGNORED\n");
		return;
	}
	
	int diseaseAmount = findPatient(&globalPatientsList, name)->diseaseCounter;
	
	// pacjent ma mniej niz n chorob
	if (n > diseaseAmount) {
		printf("IGNORED\n");
		return;
	}	
	
	DiseasesList *disListWsk;
	disListWsk = listOfPatientDiseases(&globalPatientsList, name);
		
	// chcemy n-ta od momentu podawania na wejsciu
	DiseasesList ilness = nthDisease(disListWsk, diseaseAmount - n + 1);
	
	printf("%s\n", ilness->description->diseaseDescription);
	
}

// usuwa historie chorob danego pacjenta
void deletePatientData(char *name) {
	
	// pacjent nie istnieje
	if(!isPatient(&globalPatientsList, name)) {
		printf("IGNORED\n");
		return;
	}
	
	DiseasesList *disListWsk;
	disListWsk = listOfPatientDiseases(&globalPatientsList, name);
	removeDiseasesList(*disListWsk);
	
	(findPatient(&globalPatientsList, name))->diseaseCounter = 0;
	(findPatient(&globalPatientsList, name))->patientDiseasesList = NULL;
	
	printf("OK\n");	
}

// dodaje pacjentowi 1. ost. chorobe pacjenta 2. (moze tworzyc pacjenta 1.)
void newDiseaseCopyDescription(char *name1, char *name2) {	
	
	// pacjent 2. nie istnieje
	if(!isPatient(&globalPatientsList, name2)) {
		printf("IGNORED\n");
		return;
	}
		
	if (!isPatient(&globalPatientsList, name1))
		addPatient(&globalPatientsList, name1);
	
	DiseasesList* disListWsk1;
	DiseasesList* disListWsk2;
	
	disListWsk1 = listOfPatientDiseases(&globalPatientsList, name1);
	disListWsk2 = listOfPatientDiseases(&globalPatientsList, name2);
	
	// pacjent 2. nie ma ani jednej choroby
	if(isEmptyDiseaseList(*disListWsk2)) {
		printf("IGNORED\n");
		return;
	}
	
	DiseasesList ilness;
	ilness = frontDisease(disListWsk2);
	
	addDisease(disListWsk1, ilness->description); 
	
	(findPatient(&globalPatientsList, name1))->diseaseCounter++;

	printf("OK\n");
} 

// aktualizuje opis n-tej choroby pacjenta (dodaje jako nowa chorobe)
// tak jak wyzej, n-ta choroba moze nie istniec
void changeDescription(char *name, int n, char *description) {
	
	// pacjent nie istnieje	
	if(!isPatient(&globalPatientsList, name)) {
		printf("IGNORED\n");
		return;
	}
	
	int diseaseAmount = findPatient(&globalPatientsList, name)->diseaseCounter;

	// pacjent ma mniej niz n chorob
	if (n > diseaseAmount) {
		printf("IGNORED\n");
		return;
	}
	
	DiseasesList* disListWsk;
	disListWsk = listOfPatientDiseases(&globalPatientsList, name);

	struct Disease* dis;
	dis = malloc(sizeof(struct Disease));
	dis->diseaseDescription = malloc(sizeof(char) * strlen(description) + 2);
		
	dis->referenceCounter = 0;
	strcpy(dis->diseaseDescription, description);

	
	DiseasesList ilness;
	
	// chcemy n-ta od momentu podawania na wejsciu
	ilness = nthDisease(disListWsk, diseaseAmount - n + 1);
	
	ilness->description->referenceCounter--;
	
	// jesli usunelismy ostatni przypadek choroby
	if(ilness->description->referenceCounter == 0) 
		deleteDisease(ilness);
		
	ilness->description = dis;
	ilness->description->referenceCounter++;
	
	// dodajemy nowa chorobe
	globalDiseasesCounter++;
	
	printf("OK\n");
}


