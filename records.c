#include "records.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "constants.h"
#include "observations.h"

void appendConsultationRecord(ConsulatationRecord record){

    FILE* f = fopen("patienthistory.csv", "a");
    if (!f) {
        printf("Error opening patient history file\n");
    }else{
        char outcome[50];
        record.releasedate = time(NULL);
        fprintf(f, "%s, %s, %d, %d, %d, %d, %c, \"%s\", %d ,\"%s\", %ld, %ld\n",
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
}

void viewhistory(int searchID){
    FILE* f = fopen("patienthistory.csv", "r");
    int id, day, month, year;
    char gender;
    bool notPrintedYet = true, found = false;
    char firstname[100], lastname[100], diagnosis[100], treatment[100];
    int outcome;
    time_t releasedate, admissiondate;



        if (!f) {
        printf(RED"Error opening file\n"RESET);
    }else{
         if (searchID==0){
               while((fscanf(f, " %99[^,] , %99[^,] , %d , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
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
        char *releaseDate = ctime(&releasedate);
        releaseDate[24] = '\0';

        if(outcome == 2){
            char *admissionDate = ctime(&admissiondate);
            admissionDate[24] = '\0';
            double observationDuration = difftime(releasedate, admissiondate);
            char *observed = duration(observationDuration);

            printf("--------------------------------------------------------------------------------------------------------------------\n");
            printf(MGT"     Full Name: "RESET"%-12s %-12s                "MGT"ID:"RESET" %-6d                "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis );
            printf(MGT"     Outcome:"RESET" Placed Under Observation for %s\n", observed );
            printf(MGT"     Date of Admission:"RESET" %s "MGT"        Date of Release: "RESET"%s\n", admissionDate, releaseDate );
            }else{
                if(outcome == 1){
                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                    printf(MGT"     Full Name: "RESET"%-12s %-12s            "MGT"  ID:"RESET" %-6d             "MGT"  DOB:"RESET" %d/%d/%d\n", firstname, lastname, id, day, month, year);
                    printf(MGT"     Diagnosis:"RESET" %s\n",diagnosis);
                    printf(MGT"     Outcome:"RESET"   Discharged with the following prescription: %s\n", treatment);
                    printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                    }else{
                        if(outcome == 3){
                        printf("--------------------------------------------------------------------------------------------------------------------\n");
                        printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-6d               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                        printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                        printf(MGT"     Outcome:"RESET"   Referred to %s\n", treatment);
                        printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                        }else{
                            printf("--------------------------------------------------------------------------------------------------------------------\n");
                            printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-6d               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                            printf(MGT"     Outcome:"RESET"   Died Under Observation\n");
                            printf(MGT"     Date of Death:"RESET" %s\n", releaseDate);
                        }
                    }

                }

            }
            notPrintedYet = false;
             if(notPrintedYet){
                printf(RED"No history Recorded"RESET);
            }
        }else{
            while((fscanf(f, " %99[^,] , %99[^,] , %d , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
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

                        char *releaseDate = ctime(&releasedate);
                        releaseDate[24] = '\0';

                        if(outcome == 2){
                            char *admissionDate = ctime(&admissiondate);
                            admissionDate[24] = '\0';
                            double observationDuration = difftime(releasedate, admissiondate);
                            char *observed = duration(observationDuration);

                            printf("--------------------------------------------------------------------------------------------------------------------\n");
                            printf(MGT"     Full Name: "RESET"%-12s %-12s                "MGT"ID:"RESET" %-6d                "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis );
                            printf(MGT"     Outcome:"RESET" Placed Under Observation for %s\n", observed );
                            printf(MGT"     Date of Admission:"RESET" %s "MGT"        Date of Release: "RESET"%s\n", admissionDate, releaseDate );
                            free(observed);

                            }else{
                                if(outcome == 1){
                                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                                    printf(MGT"     Full Name: "RESET"%-12s %-12s            "MGT"  ID:"RESET" %-6d             "MGT"  DOB:"RESET" %d/%d/%d\n", firstname, lastname, id, day, month, year);
                                    printf(MGT"     Diagnosis:"RESET" %s\n",diagnosis);
                                    printf(MGT"     Outcome:"RESET"   Discharged with the following prescription: %s\n", treatment);
                                    printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                                }else{
                                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                                    printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-6d               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                                    printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                                    printf(MGT"     Outcome:"RESET"   Referred to %s\n", treatment);
                                    printf(MGT"     Date of Consulation:"RESET" %s\n", releaseDate);
                                }
                            }
                            found = true;
                        }

                    }
                    if (!found){
                        printf(RED"Record not found\n"RESET);
                    }
            }
    }
}
void statistics(Node *start, Ward ward){
    FILE* f = fopen("patienthistory.csv", "r");
    int id, day, month, year;
    char gender;
    bool empty = true, found = false;
    char firstname[100], lastname[100], diagnosis[100], treatment[100];
    int outcome;
    time_t releasedate, admissiondate, now= time(NULL);
    struct tm *t = localtime(&now);
    struct tm today = *t;

    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;

    time_t midnight = mktime(&today);

    int totalRecords=0, admittedToday =0, prescribedToday =0, observedToday = 0, referredToday = 0, totalDeaths= 0;
    int totalPrescribed=0, totalObserved=0, totalReferals=0,i, diedToday=0;


    if (!f) {
        printf(RED"Error opening file\n"RESET);
    }else{

        while((fscanf(f, " %99[^,] , %99[^,] , %d , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
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

char *duration(double observationDuration){

    char *result = malloc(70*sizeof(char));
    if (observationDuration < 60){
        sprintf(result, "%d observationDuration", (int)observationDuration);
        }else{
            if(observationDuration < 3600){
                int totalMinutes = observationDuration/60;
                int remainSeconds =  observationDuration - totalMinutes*60;
                sprintf(result, "%d minutes and %d observationDuration", totalMinutes, remainSeconds);
                }else{
                    if(observationDuration < 86400){
                        int totalHours = observationDuration/3600;
                        int remainMinutes  = (observationDuration - totalHours*3600)/60;
                        int totalMinutes = (observationDuration - totalHours*3600) - remainMinutes *60;
                        sprintf(result, "%d hours %d minutes and %d observationDuration",totalHours, remainMinutes , totalMinutes);
                    }else{
                        int remainSeconds = observationDuration/86400;
                        int totalHours = (observationDuration - remainSeconds*86400)/3600;
                        int totalDays = ((observationDuration - remainSeconds*86400) - totalHours*3600)/60;
                        int remainMinutes  = ((observationDuration - remainSeconds*86400) - totalHours*3600) - totalDays*60;
                        sprintf(result, "%d days %d hours %d minutes and %d observationDuration",remainSeconds, totalHours, totalDays, remainMinutes );
                    }
                }
        }
        return result;
}

