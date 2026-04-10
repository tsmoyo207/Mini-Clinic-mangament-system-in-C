#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "observations.h"
#include "utility.h"
#include "records.h"


Ward loadobservations(){
    FILE* f = fopen("patientobservations.csv", "r");
    Ward ward;
     for(int i = 0; i < maxbeds; i++){
        ward.beds[i].occupied = 0;
        ward.beds[i].info.id = 0;
        ward.beds[i].info.dob.day = 0;
        ward.beds[i].info.dob.month = 0;
        ward.beds[i].info.dob.year = 0;
        ward.beds[i].info.gender = '0';
        strcpy(ward.beds[i].info.issue , "0");
        ward.beds[i].date = 0;
        strcpy(ward.beds[i].info.firstname, "0");
        strcpy(ward.beds[i].info.lastname, "0");
        strcpy(ward.beds[i].diagnosis, "0");
    }
    if (!f) {
        printf(RED "Error opening oberservations file\n" RESET);
    }else{
        for(int i = 0; i<maxbeds; i++){
            int items = fscanf(f, " %d , %99[^,] , %99[^,\n\r] , %d , %d , %d , %d , %c , %99[^,] , %99[^,] , %ld",
                   &ward.beds[i].occupied,
                   ward.beds[i].info.firstname,
                   ward.beds[i].info.lastname,
                   &ward.beds[i].info.id,
                   &ward.beds[i].info.dob.day,
                   &ward.beds[i].info.dob.month,
                   &ward.beds[i].info.dob.year,
                   &ward.beds[i].info.gender,
                   ward.beds[i].info.issue,
                   ward.beds[i].diagnosis,
                   &ward.beds[i].date

                   );
            if(items != 11){
                printf("Error reading line %d\n", i+1);
            }

            }

    }
    fclose(f);
    return ward;
}


void viewobservations(Ward ward){
    printf(GRN_BR_UND"                                                                                                                       \n"RESET);
    printf(GRN_BR_UND"Bed | # | Firstname    | Surname      | ID       | D.O.B      | Sex |  Date of Admission       |      Diagnosis       |\n"RESET);
    //printf("\n");
    for(int i = 0; i<maxbeds; i++){

        if(ward.beds[i].occupied == 0){
            //printf(YEL"Bed |%-2d"RED_BR" | EMPTY \n"RESET, i+1);
            printf(YEL"Bed |%-2d "RESET"| "RED_BR"EMPTY     "RESET"   |      -       |    -     |      -     |  -  |            -             |          -           \n", i+1);
        }else{
            char *horloge =    ctime(&ward.beds[i].date);
            horloge[24] = '\0';
            printf(YEL"Bed |%-2d"RESET" | %-12s |"BR_YEL" %-12s "RESET"|%-8d  |"BR_YEL" %-2d/%-2d/%d "RESET"|  %c  |"BR_YEL" %-23s "RESET "| %s\n",
                   i+1,
                   ward.beds[i].info.firstname,
                   ward.beds[i].info.lastname,
                   ward.beds[i].info.id,
                   ward.beds[i].info.dob.day,
                   ward.beds[i].info.dob.month,
                   ward.beds[i].info.dob.year,
                   ward.beds[i].info.gender,
                   horloge,
                   ward.beds[i].diagnosis
                   );

        }
    }
}


Ward admitToBed(Node *tempo, Ward ward, char diagnosis[200]){
    int i=0;
    while(i<maxbeds && ward.beds[i].occupied == 1){
        i++;
    }
    if(i>=maxbeds){
        printf(RED"Observation Ward at Maximum Capicity\n"RESET);
    }else{
        ward.beds[i].info.dob.day = tempo->data.dob.day;
        ward.beds[i].info.dob.month = tempo->data.dob.month;
        ward.beds[i].info.dob.year = tempo->data.dob.year;
        ward.beds[i].info.id = tempo->data.id;
        ward.beds[i].info.gender = tempo->data.gender;
        strcpy(ward.beds[i].info.issue, tempo->data.issue);
        ward.beds[i].occupied = 1;
        ward.beds[i].date= time(NULL);
        strcpy(ward.beds[i].info.firstname, tempo->data.firstname);
        strcpy(ward.beds[i].info.lastname, tempo->data.lastname);
        strcpy(ward.beds[i].diagnosis, diagnosis);
        printf(GRN"Patient has been Allocated to Bed No. %d\n"RESET, i+1);
    }
    return ward;
}


void saveobservations(Ward ward){
 FILE* f = fopen("patientobservations.csv", "w");
    if (!f) {
        printf(RED "Error opening oberservations file\n" RESET);
    }else{
        for(int i = 0; i<maxbeds; i++){
            fprintf(f, "%d, %s, %s, %d, %d, %d, %d, %c, %s, %s, %ld\n",
                   ward.beds[i].occupied,
                   ward.beds[i].info.firstname,
                   ward.beds[i].info.lastname,
                   ward.beds[i].info.id,
                   ward.beds[i].info.dob.day,
                   ward.beds[i].info.dob.month,
                   ward.beds[i].info.dob.year,
                   ward.beds[i].info.gender,
                   ward.beds[i].info.issue,
                   ward.beds[i].diagnosis,
                   ward.beds[i].date

                   );
            }
    }
    fclose(f);
}

int bedsoccupied(Ward ward){
    int count =0;
    for(int i=0; i<maxbeds; i++){
        if(ward.beds[i].occupied == 1){
            count++;
        }
    }
    return count;
}
Ward dischargepatient(Ward ward, int bednumber){
     bool died = false;

     while(bednumber == 0 || bednumber < -maxbeds || bednumber > maxbeds){
            printf(RED"Invalid Bed Number, Please try again: "RESET);
            while((scanf("%d", &bednumber))!= 1){
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
                }
    }
    if(bednumber<1){
        died = true;
        bednumber = bednumber * -1;
    }

    if(ward.beds[bednumber-1].occupied == 0){
        printf(RED"This bed is already empty\n"RESET);
    }else{
        ward.beds[bednumber-1].occupied = 0;
        printf(GRN"%s %s has been discharged successfully\n"RESET,
                ward.beds[bednumber-1].info.firstname,
                ward.beds[bednumber-1].info.lastname
                );
                ConsulatationRecord dischargePatient;
                dischargePatient.data = ward.beds[bednumber-1].info;
                dischargePatient.admissiondate = ward.beds[bednumber-1].date;

                strcpy(dischargePatient.diagnosis , ward.beds[bednumber-1].diagnosis);
                strcpy(dischargePatient.treatment , "Medical Observation");
                if(died){
                    dischargePatient.outcome = 4;
                }else{
                    dischargePatient.outcome = 2;
                }
                appendConsultationRecord(dischargePatient);
            }
            pause();
            return ward;
}
