#ifndef PATIENT_H_INCLUDED
#define PATIENT_H_INCLUDED
#include <time.h>

typedef struct{
    int day;
    int month;
    int year;
}date;

typedef struct{
    int id;
    int ticketnum;
    char firstname[100];
    char lastname[100];
    date dob;
    char gender;
    int priority;
    char issue[100];
}patient;

typedef struct Node{
    patient data;
    struct Node *next;
}Node;

typedef struct{
    patient data;
    int outcome;
    char diagnosis[200];
    char treatment[200];
    time_t admissiondate;
    time_t releasedate;
}patoutcome;

void viewqueue(Node *temp);
Node* addpatient(Node *start, patient pat);
Node* loadqueue();
void savequeue(Node *start);
int queuelength(Node *start);


#endif // PATIENT_H_INCLUDED
