#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
#include <stdbool.h>
#include <time.h>
#include "patient.h"
#include "observations.h"

void pause();
int verify(patient pat, Node *start);
void printmenu(observ_arr arr, Node *start);
void freequeue(Node *start);
int age(Date birth, time_t timestamp);




#endif // UTILITY_H_INCLUDED
