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

int age(Date birth, time_t timestamp) {
    struct tm *t = localtime(&timestamp);

    int age = (t->tm_year + 1900) - birth.year;

    if ((t->tm_mon + 1 < birth.month) ||
        ((t->tm_mon + 1 == birth.month) && (t->tm_mday < birth.day))) {
        age--;
    }

    return age;
}

#endif // UTILITY_H_INCLUDED
