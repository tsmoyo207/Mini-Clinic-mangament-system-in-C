#ifndef RECORDS_H_INCLUDED
#define RECORDS_H_INCLUDED
#include "patient.h"
#include "observations.h"

void appendConsultationRecord(ConsulatationRecord record);
void viewhistory(int num);
void statistics(Node *start, Ward ward);
char* duration(double seconds);


#endif // RECORDS_H_INCLUDED
