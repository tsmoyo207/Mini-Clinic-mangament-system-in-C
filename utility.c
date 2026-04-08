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

int verify(patient pat){
    int i,j;

    if (pat.dob.day < 0 || pat.dob.day > 31){
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

void printmenu(observ_arr arr, Node* start){
    int x = queuelength(start);
    int y = bedsoccupied(arr);
    printf("     "CYAN CN HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ CN"\n" RESET);
    printf("     "VN"     HOSPITAL MANAGEMENT SYSTEM V1.0     "VN"\n" );
    printf("     "CYAN CN HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ  HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ CN"\n" RESET);
    printf( "     "VN "       QUEUE: "CYAN"%-3d "RESET "patients Beds: "CYAN"%-2d/%-2d "RESET"  "  VN"\n", x, y, maxbeds);
    printf("     "CYAN CN HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ HZ CN"\n" RESET);
    printf("\n");
    printf("     "CYAN" [1]"RESET" View Patient Queue.\n");
    printf("     "CYAN" [2]"RESET" Add Patient to Queue.\n");
    printf("     "CYAN" [3]"RESET" Call the Next Patient to see the Doctor.\n");
    printf("     "CYAN" [4]"RESET" View Observation Ward.\n");
    printf("     "CYAN" [5]"RESET" Discharge Patient from Observation Ward.\n");
    printf("     "CYAN" [6]"RESET" Search History or View All patient History Records.\n");
    printf("     "CYAN" [7]"RESET" View Hospital Statistics.\n");
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
