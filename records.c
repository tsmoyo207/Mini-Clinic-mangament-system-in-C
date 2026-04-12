#include "records.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "constants.h"
#include "observations.h"

/**
* This procedure save the results of the consultation to a CSV file, it appends.
*/

void appendConsultationRecord(ConsulatationRecord record){

    FILE* f = fopen("patienthistory.csv", "a");
    if (!f) {
        // Validate if its a file or not for error reduction.
        printf("Error opening patient history file\n");
    }else{
        char outcome[50];
        record.releasedate = time(NULL);// This save the time at which the record is saved.
        fprintf(f, "%s, %s, %lld, %d, %d, %d, %c, \"%s\", %d ,\"%s\", %ld, %ld\n",
                record.data.firstname,
                record.data.lastname,
                record.data.id,
                record.data.dob.day,
                record.data.dob.month,
                record.data.dob.year,
                record.data.gender,
                record.diagnosis,
                record.outcome,
                record.treatment,
                (long)record.releasedate,
                (long)record.admissiondate

                );
    }
    fclose(f);
    printf(GRN "Record saved for %s %s.\n" RESET,
                        record.data.firstname,
                        record.data.lastname);

}

/**
* This procedure reads the patients's history from a CSV file.
* Allows to search for a patient using their id or enter a default option to view all patients.
*/

void viewhistory(long long searchID){
    FILE* f = fopen("patienthistory.csv", "r");
    int day, month, year;
    long long id;
    char gender;
    bool notPrintedYet = true, found = false;
    char firstname[100], lastname[100], diagnosis[100], treatment[100];
    int outcome;
    time_t releasedate, admissiondate;
    char *releaseDate, *admissionDate;



        if (!f) {
        // Validate if its a file or not to minimize errors.
        printf(RED"Error opening patient history file\n"RESET);
    }else{
         if (searchID==0){
               while((fscanf(f, " %99[^,] , %99[^,] , %lld , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
                      firstname,
                      lastname,
                      &id,
                      &day,
                      &month,
                      &year,
                      &gender,
                      diagnosis,
                      &outcome,
                      treatment,
                      &releasedate,
                      &admissiondate)) == 12){
        if(notPrintedYet){
            printf(GRN_BR_BOLD"-------------------------------------------PATIENT HISTORY RECORDS--------------------------------------------------\n"RESET);
        }
        releaseDate = ctime(&releasedate);//Convert numeric timestamps int human readable strings.
        releaseDate[24] = '\0';// Removes default new line added by ctime.

        if(outcome == 2){
            admissionDate = ctime(&admissiondate);//Convert numeric timestamps int human readable strings.
            admissionDate[24] = '\0';// Removes default new line added by ctime.

            double observationDuration = difftime(releasedate, admissiondate);//Calculates the length between 2 timestamps
            char *observed = duration(observationDuration);//Converts difftime into human readable string

            printf("--------------------------------------------------------------------------------------------------------------------\n");
            printf(MGT"     Full Name: "RESET"%-12s %-12s                "MGT"ID:"RESET" %-18lld                "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis );
            printf(MGT"     Outcome:"RESET" Placed Under Observation for %s\n", observed );
            printf(MGT"     Date of Admission:"RESET" %s "MGT"        Date of Release: "RESET"%s\n", admissionDate, releaseDate );
            }else{
                if(outcome == 1){
                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                    printf(MGT"     Full Name: "RESET"%-12s %-12s            "MGT"  ID:"RESET" %-18lld             "MGT"  DOB:"RESET" %d/%d/%d\n", firstname, lastname, id, day, month, year);
                    printf(MGT"     Diagnosis:"RESET" %s\n",diagnosis);
                    printf(MGT"     Outcome:"RESET"   Discharged with the following prescription: %s\n", treatment);
                    printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                    }else{
                        if(outcome == 3){
                        printf("--------------------------------------------------------------------------------------------------------------------\n");
                        printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-18lld               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                        printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                        printf(MGT"     Outcome:"RESET"   Referred to %s\n", treatment);
                        printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                        }else{
                            printf("--------------------------------------------------------------------------------------------------------------------\n");
                            printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-18lld               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                            printf(MGT"     Outcome:"RESET"   Died Under Observation\n");
                            printf(MGT"     Date of Death:"RESET" %s\n", releaseDate);
                        }
                    }

                }
                notPrintedYet = false;


            }

             if(notPrintedYet){
                printf(RED"No patient history records exist yet\n"RESET);
            }
        }else{
            while((fscanf(f, " %99[^,] , %99[^,] , %lld , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
                      firstname,
                      lastname,
                      &id,
                      &day,
                      &month,
                      &year,
                      &gender,
                      diagnosis,
                      &outcome,
                      treatment,
                      &releasedate,
                      &admissiondate)) == 12){

                    if(id == searchID){
                          if(!found){
                                printf(GRN_BR_BOLD"-------------------------------------------PATIENT HISTORY RECORDS--------------------------------------------------\n"RESET);
                          }

                        releaseDate = ctime(&releasedate);//Convert numeric timestamps int human readable strings
                        releaseDate[24] = '\0';// Removes default new line added by ctime.


                        if(outcome == 2){
                            char *admissionDate = ctime(&admissiondate);//Convert numeric timestamps int human readable strings
                            admissionDate[24] = '\0';// Removes default new line added by ctime.
                            double observationDuration = difftime(releasedate, admissiondate);//Calculates the length between 2 timestamps
                            char *observed = duration(observationDuration);//Converts difftime into human readable string

                            printf("--------------------------------------------------------------------------------------------------------------------\n");
                            printf(MGT"     Full Name: "RESET"%-12s %-12s                "MGT"ID:"RESET" %-18lld                "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis );
                            printf(MGT"     Outcome:"RESET" Placed Under Observation for %s\n", observed );
                            printf(MGT"     Date of Admission:"RESET" %s "MGT"        Date of Release: "RESET"%s\n", admissionDate, releaseDate );
                            free(observed);

                            }else{
                                if(outcome == 1){
                                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                                    printf(MGT"     Full Name: "RESET"%-12s %-12s            "MGT"  ID:"RESET" %-18lld             "MGT"  DOB:"RESET" %d/%d/%d\n", firstname, lastname, id, day, month, year);
                                    printf(MGT"     Diagnosis:"RESET" %s\n",diagnosis);
                                    printf(MGT"     Outcome:"RESET"   Discharged with the following prescription: %s\n", treatment);
                                    printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                                }else{
                                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                                    printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-18lld               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                                    printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                                    printf(MGT"     Outcome:"RESET"   Referred to %s\n", treatment);
                                    printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                                }
                            }
                            found = true;
                        }

                    }
                    if (!found){
                        // Validate if the Id is in the record or not
                        printf(RED"No history records found for ID %lld.\n"RESET, searchID);
                    }
                    free(releaseDate);
                    free(admissionDate);
            }
    }
}

/**
* This procedure reads the CSV and performs several operations and displays them.
*/
void statistics(Node *start, Ward ward){
    FILE* f = fopen("patienthistory.csv", "r");
    int day, month, year;
    long long id;
    char gender;
    bool empty = true, found = false;
    char firstname[100], lastname[100], diagnosis[100], treatment[100];
    int outcome;
    time_t releasedate, admissiondate, now= time(NULL);
    struct tm *t = localtime(&now);//timestamp for current time
    struct tm today = *t;

    today.tm_hour = 0;//intializing time in hours
    today.tm_min = 0;//intializing time in min
    today.tm_sec = 0;//intializing time in sec

    time_t midnight = mktime(&today);

    int totalRecords=0, admittedToday =0, prescribedToday =0, observedToday = 0, referredToday = 0, totalDeaths= 0;
    int totalPrescribed=0, totalObserved=0, totalReferals=0,i, diedToday=0;


    if (!f) {
        //Validates if its a file or not.
        printf(RED"Error opening file\n"RESET);
    }else{
        while((fscanf(f, " %99[^,] , %99[^,] , %lld , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
                firstname,
                lastname,
                &id,
                &day,
                &month,
                &year,
                &gender,
                diagnosis,
                &outcome,
                treatment,
                &releasedate,
                &admissiondate)) == 12){

                    totalRecords++;
                    if(outcome == 1){
                        totalPrescribed++;
                    }else{
                        if(outcome ==2){
                            totalObserved++;
                        }else{
                            if(outcome ==3){
                                totalReferals++;
                        }else{
                            totalDeaths++;
                        }
                      }
                    }


                    if(releasedate >= midnight){
                        admittedToday++;
                        if(outcome == 1){
                            prescribedToday++;
                        }else{
                            if(outcome ==2){
                                observedToday++;
                            }else{
                                if(outcome == 3){
                                    referredToday++;
                                }else{
                                    diedToday++;
                                }

                            }

                        }

                    }
                }
    }
    fclose(f);
    //this block of code calculates which patients in the ward have been there the longest or shortest.
    double max = 0,min = 2000000000.0;
    int occupiedBeds = bedsoccupied(ward);

    for(i=0;i<maxbeds;i++){
        if(ward.beds[i].occupied == 1){
            double x = difftime(now, ward.beds[i].date);
            if(x<min){
                min=x;
            }
            if(x>max){
                max=x;
            }
        }
    }
    char *shortestStay, *longestStay;
    shortestStay = duration(min);
    longestStay =  duration(max);
    // this block of code counts the number of patients in each priority group.
    int highPriorities=0, lowPriorities=0, medPriorities =0;
    int queueSize = 0;
    while(start != NULL){
        queueSize++;
        if (start->data.priority==1){
            lowPriorities++;
        }else{
            if (start->data.priority==2){
                medPriorities++;
            }else{
                highPriorities++;
            }
        }
        start = start->next;
    }
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                        HOSPITAL STATISTICS                           |\n"RESET);
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                              TODAY                                   |\n"RESET);
    printf(BR_YEL"|     Patients Admitted: "RESET"                      %-5d                   |\n", admittedToday);
    printf(BR_YEL"|     Patients Discharged with Prescription:   "RESET"%-5d                   |\n", prescribedToday);
    printf(BR_YEL"|     Patients Referred to Another Department:"RESET" %-5d                   |\n", referredToday);
    printf(BR_YEL"|     Patients Released from Observation:    "RESET"  %-5d                   |\n", observedToday);
    printf(BR_YEL"|     Patients Died:                          "RESET" %-5d                   |\n", diedToday);
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                              RIGHT NOW                               |\n"RESET);
    printf(BR_YEL"|     Waiting in Queue:  "RESET"  %-5d                                       |\n", queueSize);
    printf(BR_YEL"|     High Priority:    "RESET"   %-5d                                       |\n", highPriorities);
    printf(BR_YEL"|     Medium Priority:  "RESET"   %-5d                                       |\n", medPriorities);
    printf(BR_YEL"|     Low Priority:     "RESET"   %-5d                                       |\n", lowPriorities);
    printf(BR_YEL"|     Beds Occupied:   "RESET"    %-5d                                       |\n", occupiedBeds);
    printf(BR_YEL"|     Longest stay:    "RESET"    %-43s |\n", longestStay);
    printf(BR_YEL"|     Shortest stay:   "RESET"    %-43s |\n", shortestStay);
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                              ALL TIME                                |\n"RESET);
    printf(BR_YEL"|     Total Records in History:      "RESET"              %-5d               |\n",totalRecords);
    printf(BR_YEL"|     Total Patients released with prescription: "RESET"  %-5d               |\n", totalPrescribed);
    printf(BR_YEL"|     Total Patients released after observation: "RESET"  %-5d               |\n",totalObserved);
    printf(BR_YEL"|     Total Patients referred to other depatments: "RESET"%-5d               |\n", totalReferals);
    printf(BR_YEL"|     Total Patients Died:                         "RESET"%-5d               |\n", totalDeaths);
    printf("+----------------------------------------------------------------------+\n");
    free(shortestStay);
    free(longestStay);
}

/**
* This function converts number of seconds into a human readable string.
*/
char *duration(double observationDuration){

    char *result = malloc(70*sizeof(char));
    if (observationDuration < 60){
        sprintf(result, "%d seconds", (int)observationDuration);
        }else{
            if(observationDuration < 3600){
                int totalMinutes = observationDuration/60;
                int remainSeconds =  observationDuration - totalMinutes*60;
                sprintf(result, "%d minutes and %d seconds", totalMinutes, remainSeconds);
                }else{
                    if(observationDuration < 86400){
                        int totalHours = observationDuration/3600;
                        int remainMinutes  = (observationDuration - totalHours*3600)/60;
                        int totalMinutes = (observationDuration - totalHours*3600) - remainMinutes *60;
                        sprintf(result, "%d hours %d minutes and %d seconds",totalHours, remainMinutes , totalMinutes);
                    }else{
                        int totalDays = observationDuration/86400;
                        int totalHours = (observationDuration - totalDays*86400)/3600;
                        int remainMinutes= ((observationDuration - totalDays*86400) - totalHours*3600)/60;
                        int remainSeconds = ((observationDuration - totalDays*86400) - totalHours*3600) - remainMinutes*60;
                        sprintf(result, "%d days %d hours %d minutes and %d seconds", totalDays, totalHours, remainMinutes, remainSeconds );
                    }
                }
        }
        return result;
}

