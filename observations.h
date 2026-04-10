#ifndef OBSERVATIONS_H_INCLUDED
#define OBSERVATIONS_H_INCLUDED
#include "patient.h"
#include <time.h>
#include "constants.h"

typedef struct{
    Patient info;
    time_t date;
    char diagnosis[200];
    int occupied;

}HospitalBed;

typedef struct{
    HospitalBed beds[maxbeds];
}Ward;

Ward loadobservations();
void viewobservations(Ward ward);
Ward admitToBed(Node *tempo, Ward ward, char diagnosis[200]);
Ward dischargepatient(Ward ward, int bednumber);
void saveobservations(Ward ward);
int bedsoccupied(Ward ward);

#endif // OBSERVATIONS_H_INCLUDED
