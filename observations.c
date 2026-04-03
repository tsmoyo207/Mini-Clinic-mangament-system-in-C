#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "observations.h"


observ_arr loadobservations(){
    FILE* f = fopen("patientobservations.csv", "r");
    observ_arr arr;
     for(int i = 0; i < maxbeds; i++){
        arr.beds[i].occupied = 0;
        arr.beds[i].info.id = 0;
        arr.beds[i].info.dob.day = 0;
        arr.beds[i].info.dob.month = 0;
        arr.beds[i].info.dob.year = 0;
        arr.beds[i].info.gender = '0';
        strcpy(arr.beds[i].info.issue , "0");
        arr.beds[i].date = 0;
        strcpy(arr.beds[i].info.firstname, "0");
        strcpy(arr.beds[i].info.lastname, "0");
        strcpy(arr.beds[i].diagnosis, "0");
    }
    if (!f) {
        printf(RED "Error opening oberservations file\n" RESET);
    }else{
        for(int i = 0; i<maxbeds; i++){
            int items = fscanf(f, " %d , %99[^,] , %99[^,\n\r] , %d , %d , %d , %d , %c , %99[^,] , %99[^,] , %ld",
                   &arr.beds[i].occupied,
                   arr.beds[i].info.firstname,
                   arr.beds[i].info.lastname,
                   &arr.beds[i].info.id,
                   &arr.beds[i].info.dob.day,
                   &arr.beds[i].info.dob.month,
                   &arr.beds[i].info.dob.year,
                   &arr.beds[i].info.gender,
                   arr.beds[i].info.issue,
                   arr.beds[i].diagnosis,
                   &arr.beds[i].date

                   );
            if(items != 11){
                printf("Error reading line %d\n", i+1);
            }

            }

    }
    fclose(f);
    return arr;
}


void viewobservations(observ_arr arr){
    printf(GRN_BR_UND"                                                                                                                       \n"RESET);
    printf(GRN_BR_UND"Bed | # | Firstname    | Surname      | ID       | D.O.B      | Sex |  Date of Admission       |      Diagnosis       |\n"RESET);
    //printf("\n");
    for(int i = 0; i<maxbeds; i++){

        if(arr.beds[i].occupied == 0){
            //printf(YEL"Bed |%-2d"RED_BR" | EMPTY \n"RESET, i+1);
            printf(YEL"Bed |%-2d "RESET"| "RED_BR"EMPTY     "RESET"   |      -       |    -     |      -     |  -  |            -             |          -           \n", i+1);
        }else{
            char *horloge =    ctime(&arr.beds[i].date);
            horloge[24] = '\0';
            printf(YEL"Bed |%-2d"RESET" | %-12s |"BR_YEL" %-12s "RESET"|%-8d  |"BR_YEL" %-2d/%-2d/%d "RESET"|  %c  |"BR_YEL" %-23s "RESET "| %s\n",
                   i+1,
                   arr.beds[i].info.firstname,
                   arr.beds[i].info.lastname,
                   arr.beds[i].info.id,
                   arr.beds[i].info.dob.day,
                   arr.beds[i].info.dob.month,
                   arr.beds[i].info.dob.year,
                   arr.beds[i].info.gender,
                   horloge,
                   arr.beds[i].diagnosis
                   );

        }
    }
}


observ_arr placeobservation(Node *tempo, observ_arr arr, char diagnosis[200]){
    int i=0;
    while(i<maxbeds && arr.beds[i].occupied == 1){
        i++;
    }
    if(i>=maxbeds){
        printf(RED"Observation Ward at Maximum Capicity\n"RESET);
    }else{
        arr.beds[i].info.dob.day = tempo->data.dob.day;
        arr.beds[i].info.dob.month = tempo->data.dob.month;
        arr.beds[i].info.dob.year = tempo->data.dob.year;
        arr.beds[i].info.id = tempo->data.id;
        arr.beds[i].info.gender = tempo->data.gender;
        strcpy(arr.beds[i].info.issue, tempo->data.issue);
        arr.beds[i].occupied = 1;
        arr.beds[i].date= time(NULL);
        strcpy(arr.beds[i].info.firstname, tempo->data.firstname);
        strcpy(arr.beds[i].info.lastname, tempo->data.lastname);
        strcpy(arr.beds[i].diagnosis, diagnosis);
        printf(GRN"Patient has been Allocated to Bed No. %d\n"RESET, i+1);
    }
    return arr;
}


void saveobservations(observ_arr arr){
 FILE* f = fopen("patientobservations.csv", "w");
    if (!f) {
        printf(RED "Error opening oberservations file\n" RESET);
    }else{
        for(int i = 0; i<maxbeds; i++){
            fprintf(f, "%d, %s, %s, %d, %d, %d, %d, %c, %s, %s, %ld\n",
                   arr.beds[i].occupied,
                   arr.beds[i].info.firstname,
                   arr.beds[i].info.lastname,
                   arr.beds[i].info.id,
                   arr.beds[i].info.dob.day,
                   arr.beds[i].info.dob.month,
                   arr.beds[i].info.dob.year,
                   arr.beds[i].info.gender,
                   arr.beds[i].info.issue,
                   arr.beds[i].diagnosis,
                   arr.beds[i].date

                   );
            }
    }
    fclose(f);
}

int bedsoccupied(observ_arr arr){
    int count =0;
    for(int i=0; i<maxbeds; i++){
        if(arr.beds[i].occupied == 1){
            count++;
        }
    }
    return count;
}
