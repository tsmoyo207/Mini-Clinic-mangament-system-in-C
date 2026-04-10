#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include <string.h>
#include <ctype.h>
#define CN  "+"
#define HZ  "-"
#define VN  "|"


void pause() {
    printf(GREY "\nPress Enter to continue..." RESET);
    while(getchar() != '\n');
    getchar();
    system("cls");
}

int validatePatient(Patient pat){
    int i,j;

    if (pat.dob.day < 1 || pat.dob.day > 31){
        return 1;
    }
    if (pat.dob.month < 1 || pat.dob.month > 12 ){
        return 1;
    }
    if(pat.dob.year< 1900 || pat.dob.year > 2026){
        return 1;
    }
    if(pat.id< 100000 || pat.id > 999999){
        return 2;
    }
    if(pat.gender != 'M'  && pat.gender != 'm' && pat.gender != 'F'  && pat.gender != 'f'){
        return 6;
    }
    if(pat.priority < 1 || pat.priority > 3){
        return 7;
    }

    for(i=0;i<strlen(pat.firstname);i++){
            if(!(isalpha(pat.firstname[i]))){
                return 3;
            }
        }
    for(j=0; j<strlen(pat.lastname);j++){
            if(!(isalpha(pat.lastname[j]))){
               return 4;
            }

    }
    return 0;
}

void printmenu(Ward ward, Node* start, Patient currentPatientInRoom){
    int x = queuelength(start);
    int y = bedsoccupied(ward);

    printf("     "CYAN "+-------------------------------------------------------------------------+\n" RESET);
    printf("     |                  HOSPITAL MANAGEMENT SYSTEM V1.0                        |\n" );
    printf("     "CYAN "+-------------------------------------------------------------------------+\n" RESET);
    printf("     |                    QUEUE: "CYAN"%-3d "RESET "Patients Beds: "CYAN"%-2d/%-2d "RESET"                     |\n", x, y, maxbeds);
    printf("     "CYAN "+-------------------------------------------------------------------------+\n" RESET);
    if (currentPatientInRoom.ticketnum == 0){
        printf("     |                  Doctor Is Not Seeing Anyone at the moment              |\n");
    }else{
        printf("     |    Doctor Is Now Seeing: "CYAN"%-10s %-10s    "RESET"ticketNumber:"CYAN" %-4d    "RESET"| \n" RESET,
               currentPatientInRoom.firstname,
               currentPatientInRoom.lastname,
               currentPatientInRoom.ticketnum );
    }

    printf("     "CYAN "+-------------------------------------------------------------------------+\n" RESET);
    printf("\n");
    printf("     "CYAN" [1]"RESET" View Patient Queue.\n");
    printf("     "CYAN" [2]"RESET" Add Patient to Queue.\n");
    printf("     "CYAN" [3]"RESET" Edit Patient in Queue.\n");
    printf("     "CYAN" [4]"RESET" Remove Patient from Queue.\n");
    printf("     "CYAN" [5]"RESET" Call the Next Patient to see the Doctor.\n");
    printf("     "CYAN" [6]"RESET" View Observation Ward.\n");
    printf("     "CYAN" [7]"RESET" Discharge Patient from Observation Ward.\n");
    printf("     "CYAN" [8]"RESET" Search History or View All Patient History Records.\n");
    printf("     "CYAN" [9]"RESET" View Hospital Statistics.\n");
    printf("     "CYAN" [0]"RESET" Save and Exit.\n");



}

void freequeue(Node *start){
    Node *tmp;
    while(start != NULL){
        tmp = start;
        start = start->next;
        free(tmp);
    }
}

int age(date birth) {
    time_t timestamp = time(NULL);
    struct tm *t = localtime(&timestamp);
    int age = (t->tm_year + 1900) - birth.year;
    if ((t->tm_mon + 1 < birth.month) ||
        ((t->tm_mon + 1 == birth.month) && (t->tm_mday < birth.day))) {
        age--;
    }
    return age;
}
