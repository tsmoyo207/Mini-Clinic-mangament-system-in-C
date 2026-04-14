#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "observations.h"
#include "utility.h"
#include "records.h"

/**
*this function loads the entire observation ward from a CSV file.
*it initializes all the beds to zero/empty to ensure a clean state.
*/

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
        printf(RED "Warning: Could not open observations file. Starting with an empty Ward.\n" RESET);
    }else{
        for(int i = 0; i<maxbeds; i++){
            int items = fscanf(f, " %d , %99[^,] , %99[^,\n\r] , %lld , %d , %d , %d , %c , %99[^,] , %99[^,] , %ld",
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
            // ensures all 11 fields are successfully read from the file(validation)
            if(items != 11){
                printf("Error reading line %d\n", i+1);
            }

            }
            printf(GRN "       Observation Ward restored from last session.\n" RESET);
            fclose(f);
    }
    return ward;
}

/**
* This procedure displays a formatted or an neatly arranged table of beds in the ward.
* Shows 'EMPTY' for unoccupied beds and full details on occupied ones.
*/

void viewobservations(Ward ward){
    printf(GRN_BR_UND"                                                                                                                                          \n"RESET);
    printf(GRN_BR_UND"Bed | # | Firstname    | Surname      | ID                 | D.O.B      | Sex |  Date of Admission       |      Diagnosis                |\n"RESET);
    for(int i = 0; i<maxbeds; i++){
        if(ward.beds[i].occupied == 0){
            printf(YEL"Bed |%-2d "RESET"| "RED_BR"EMPTY     "RESET"   |      -       |    -     |      -     |  -  |            -             |          -           \n", i+1);
        }else{
            // Convert the numeric timestamp (time_t) into human readable string
            char *horloge =    ctime(&ward.beds[i].date);
            horloge[24] = '\0'; // Remove the default newline character added by ctime
            printf(YEL"Bed |%-2d"RESET" | %-12s |"BR_YEL" %-12s "RESET"|%-18lld  |"BR_YEL" %-2d/%-2d/%d "RESET"|  %c  |"BR_YEL" %-23s "RESET "| %s\n",
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

/**
*This function assigns a patient on the waiting list to the first available bed.
*/

Ward admitToBed(Node *tempo, Ward ward, char diagnosis[200]){
    int i=0;
    // Find the first index where the occupied bed is 0
    while(i<maxbeds && ward.beds[i].occupied == 1){
        i++;
    }
    if(i>=maxbeds){
       printf(RED "Observation Ward is at full capacity (%d/%d beds occupied)\n "
                  "Please discharge a patient before admitting a new one.\n" RESET, maxbeds, maxbeds);
    }else{
        // Transfers data from a queue to the ward array.
        ward.beds[i].info.dob.day = tempo->data.dob.day;
        ward.beds[i].info.dob.month = tempo->data.dob.month;
        ward.beds[i].info.dob.year = tempo->data.dob.year;
        ward.beds[i].info.id = tempo->data.id;
        ward.beds[i].info.gender = tempo->data.gender;
        strcpy(ward.beds[i].info.issue, tempo->data.issue);
        ward.beds[i].occupied = 1;
        ward.beds[i].date= time(NULL); // Captures the admission time.
        strcpy(ward.beds[i].info.firstname, tempo->data.firstname);
        strcpy(ward.beds[i].info.lastname, tempo->data.lastname);
        strcpy(ward.beds[i].diagnosis, diagnosis);
        printf(GRN"Patient has been Allocated to Bed No. %d\n"RESET, i+1);
    }
    return ward;
}

/**
* This procedure saves the current state of the ward to the CSV file.
* It doesn't append so it overrides the old file to keep the data in sync.
*/

void saveobservations(Ward ward){
 FILE* f = fopen("patientobservations.csv", "w");
    if (!f) {
        printf(RED "Error Saving Observations to file"RESET);
    }else{
        for(int i = 0; i<maxbeds; i++){
            fprintf(f, "%d, %s, %s, %lld, %d, %d, %d, %c, %s, %s, %ld\n",
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
    printf(GRN "Observation Ward saved successfully.\n" RESET);
}

/**
*This function counts the beds occupied inside the ward.
*/

int bedsoccupied(Ward ward){
    int count =0;
    for(int i=0; i<maxbeds; i++){
        if(ward.beds[i].occupied == 1){
            count++;
        }
    }
    return count;
}

/**
* This function handles patient's discharge and records the clinical outcome.
* Supports a special logic.If the bednumber is negative, the patient is marked as 'Deceased'.
*/

Ward dischargepatient(Ward ward, int bednumber, bool died){
     while(bednumber < 1 || bednumber > maxbeds){
            printf(RED"Invalid Bed Number, Please try again:\n"RESET);
            printf("Enter a Valid Bed Number to Discharge Patient ( 1 to %d): ", maxbeds);
            // This loop validates for the user to enter a number not a string or character
            while((scanf("%d", &bednumber))!= 1){
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
                }
    }
    if(ward.beds[bednumber-1].occupied == 0){
        printf(RED"Bed %d is already vacant. No action taken.\n"RESET, bednumber);
    }else{
        ward.beds[bednumber-1].occupied = 0;
        printf(GRN"Bed %d: %s %s has been successfully discharged\n"RESET,
                bednumber,
                ward.beds[bednumber-1].info.firstname,
                ward.beds[bednumber-1].info.lastname
                );
                ConsulatationRecord dischargePatient;
                dischargePatient.data = ward.beds[bednumber-1].info;
                dischargePatient.admissiondate = ward.beds[bednumber-1].date;
                strcpy(dischargePatient.diagnosis , ward.beds[bednumber-1].diagnosis);
                if(died){
                    strcpy(dischargePatient.treatment , "Died Under Medical Observation");
                    dischargePatient.outcome = 4;
                }else{
                    strcpy(dischargePatient.treatment , "Medical Observation");
                    dischargePatient.outcome = 2;
                }
                appendConsultationRecord(dischargePatient);
    }
    pause();
    return ward;
}
