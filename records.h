#ifndef RECORDS_H_INCLUDED
#define RECORDS_H_INCLUDED
#include "patient.h"
#include "observations.h"

void savehistory(patoutcome patout);
void viewhistory(int num);
void statistics(Node *start, observ_arr arr);
char* duration(double seconds);


#endif // RECORDS_H_INCLUDED
