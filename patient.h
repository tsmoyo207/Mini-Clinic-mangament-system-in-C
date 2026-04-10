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
}Patient;

typedef struct Node{
    Patient data;
    struct Node *next;
}Node;

typedef struct{
    Patient data;
    int outcome;
    char diagnosis[200];
    char treatment[200];
    time_t admissiondate;
    time_t releasedate;
}ConsulatationRecord;

void viewqueue(Node *temp);
Node* enqueuePatient(Node *start, Patient pat);
Node* loadqueue();
Node* dequeuePatient(Node *start, int ticketnumber);
void savequeue(Node *start);
void editqueue(Node *start, int id);
int queuelength(Node *start);
Patient registerpatient(Node *start);



#endif // PATIENT_H_INCLUDED
