#ifndef OBSERVATIONS_H_INCLUDED
#define OBSERVATIONS_H_INCLUDED
#include "patient.h"
#include <time.h>
#include "constants.h"

typedef struct{
    patient info;
    time_t date;
    char diagnosis[200];
    int occupied;

}observ;

typedef struct{
    observ beds[maxbeds];
}observ_arr;

observ_arr loadobservations();
void viewobservations(observ_arr arr);
observ_arr placeobservation(Node *tempo, observ_arr arr, char diagnosis[200]);
observ_arr dischargepatient(observ_arr arr);
void saveobservations(observ_arr arr);
int bedsoccupied(observ_arr arr);

#endif // OBSERVATIONS_H_INCLUDED
