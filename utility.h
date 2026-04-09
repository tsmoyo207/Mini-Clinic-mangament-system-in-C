#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
#include <stdbool.h>
#include <time.h>
#include "patient.h"
#include "observations.h"

void pause();
int verify(patient pat);
void printmenu(observ_arr arr, Node *start, patient currentPatientInRoom);
void freequeue(Node *start);
int age(date birth);




#endif // UTILITY_H_INCLUDED
