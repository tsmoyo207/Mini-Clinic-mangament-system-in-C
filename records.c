#include "records.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "constants.h"
#include "observations.h"

void savehistory(patoutcome patout){

    FILE* f = fopen("patienthistory.csv", "a");
    if (!f) {
        printf("Error opening patient history file\n");
    }else{
        char outcome[50];
        patout.releasedate = time(NULL);
        fprintf(f, "%s, %s, %d, %d, %d, %d, %c, \"%s\", %d ,\"%s\", %ld, %ld\n",
                patout.data.firstname,
                patout.data.lastname,
                patout.data.id,
                patout.data.dob.day,
                patout.data.dob.month,
                patout.data.dob.year,
                patout.data.gender,
                patout.diagnosis,
                patout.outcome,
                patout.treatment,
                (long)patout.releasedate,
                (long)patout.admissiondate

                );
    }
    fclose(f);
}

void viewhistory(int num){
    FILE* f = fopen("patienthistory.csv", "r");
    int id, day, month, year;
    char gender;
    bool empty = true, found = false;
    char firstname[100], lastname[100], diagnosis[100], treatment[100];
    int outcome;
    time_t releasedate, admissiondate;



        if (!f) {
        printf(RED"Error opening file\n"RESET);
    }else{
         if (num==0){
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
        if(empty){
            printf(GRN_BR_BOLD"-------------------------------------------PATIENT HISTORY RECORDS--------------------------------------------------\n"RESET);
        }
        char *horloge1 = ctime(&releasedate);
        horloge1[24] = '\0';

        if(outcome == 2){
            char *horloge2 = ctime(&admissiondate);
            horloge2[24] = '\0';
            double seconds = difftime(releasedate, admissiondate);
            char *observed = duration(seconds);

            printf("--------------------------------------------------------------------------------------------------------------------\n");
            printf(MGT"     Full Name: "RESET"%-12s %-12s                "MGT"ID:"RESET" %-6d                "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis );
            printf(MGT"     Outcome:"RESET" Placed Under Observation for %s\n", observed );
            printf(MGT"     Date of Admission:"RESET" %s "MGT"        Date of Release: "RESET"%s\n", horloge2, horloge1 );
            }else{
                if(outcome == 1){
                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                    printf(MGT"     Full Name: "RESET"%-12s %-12s            "MGT"  ID:"RESET" %-6d             "MGT"  DOB:"RESET" %d/%d/%d\n", firstname, lastname, id, day, month, year);
                    printf(MGT"     Diagnosis:"RESET" %s\n",diagnosis);
                    printf(MGT"     Outcome:"RESET"   Discharged with the following prescription: %s\n", treatment);
                    printf(MGT"     Date of Consulation:"RESET" %s\n", horloge1);
                    }else{
                        if(outcome == 3){
                        printf("--------------------------------------------------------------------------------------------------------------------\n");
                        printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-6d               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                        printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                        printf(MGT"     Outcome:"RESET"   Referred to %s\n", treatment);
                        printf(MGT"     Date of Consulation:"RESET" %s\n", horloge1);
                        }else{
                            printf("--------------------------------------------------------------------------------------------------------------------\n");
                            printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-6d               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                            printf(MGT"     Outcome:"RESET"   Died Under Observation\n");
                            printf(MGT"     Date of Death:"RESET" %s\n", horloge1);
                        }
                    }

                }
                empty = false;
            }
             if(empty){
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

                    if(id == num){
                          if(!found){
                                printf(GRN_BR_BOLD"-------------------------------------------PATIENT HISTORY RECORDS--------------------------------------------------\n"RESET);
                          }

                        char *horloge1 = ctime(&releasedate);
                        horloge1[24] = '\0';

                        if(outcome == 2){
                            char *horloge2 = ctime(&admissiondate);
                            horloge2[24] = '\0';
                            double seconds = difftime(releasedate, admissiondate);
                            char *observed = duration(seconds);

                            printf("--------------------------------------------------------------------------------------------------------------------\n");
                            printf(MGT"     Full Name: "RESET"%-12s %-12s                "MGT"ID:"RESET" %-6d                "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                            printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis );
                            printf(MGT"     Outcome:"RESET" Placed Under Observation for %s\n", observed );
                            printf(MGT"     Date of Admission:"RESET" %s "MGT"        Date of Release: "RESET"%s\n", horloge2, horloge1 );
                            free(observed);

                            }else{
                                if(outcome == 1){
                                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                                    printf(MGT"     Full Name: "RESET"%-12s %-12s            "MGT"  ID:"RESET" %-6d             "MGT"  DOB:"RESET" %d/%d/%d\n", firstname, lastname, id, day, month, year);
                                    printf(MGT"     Diagnosis:"RESET" %s\n",diagnosis);
                                    printf(MGT"     Outcome:"RESET"   Discharged with the following prescription: %s\n", treatment);
                                    printf(MGT"     Date of Consulation:"RESET" %s\n", horloge1);
                                }else{
                                    printf("--------------------------------------------------------------------------------------------------------------------\n");
                                    printf(MGT"     Full Name:"RESET" %-12s %-12s              "MGT"ID:"RESET" %-6d               "MGT"DOB: "RESET"%d/%d/%d\n", firstname, lastname, id, day, month, year);
                                    printf(MGT"     Diagnosis:"RESET" %s\n", diagnosis);
                                    printf(MGT"     Outcome:"RESET"   Referred to %s\n", treatment);
                                    printf(MGT"     Date of Consulation:"RESET" %s\n", horloge1);
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
void statistics(Node *start, observ_arr arr){
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

    int count=0, admitted =0, presc =0, obs = 0, refer = 0, died = 0;
    int prescribed=0, observationz=0, referals=0,i, diedtoday=0;


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

                    count++;
                    if(outcome == 1){
                        prescribed++;
                    }else{
                        if(outcome ==2){
                            observationz++;
                        }else{
                            if(outcome ==3){
                                referals++;
                        }else{
                            died++;
                        }
                      }
                    }


                    if(releasedate >= midnight){
                        admitted++;
                        if(outcome == 1){
                            presc++;
                        }else{
                            if(outcome ==2){
                                obs++;
                            }else{
                                if(outcome == 3){
                                    refer++;
                                }else{
                                    diedtoday++;
                                }

                            }

                        }

                    }
                }
    }
    fclose(f);

    double maxz = 0,minz = 2000000000.0;
    int occ = bedsoccupied(arr);

    for(i=0;i<maxbeds;i++){
        if(arr.beds[i].occupied == 1){
            double x = difftime(now, arr.beds[i].date);
            if(x<minz){
                minz=x;
            }
            if(x>maxz){
                maxz=x;
            }
        }
    }

    char *shorty, *longy;

    shorty = duration(minz);
    longy =  duration(maxz);

    int high=0, low=0, med =0;
    int que = 0;
    while(start != NULL){
        que++;
        if (start->data.priority==1){
            low++;
        }else{
            if (start->data.priority==2){
                med++;
            }else{
                high++;
            }
        }
        start = start->next;
    }
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                        HOSPITAL STATISTICS                           |\n"RESET);
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                              TODAY                                   |\n"RESET);
    printf(BR_YEL"|     Patients Admitted: "RESET"  %-5d                                       |\n", admitted);
    printf(BR_YEL"|     Patients Discharged: "RESET"%-5d                                       |\n", obs);
    printf(BR_YEL"|     Patients Referred:  "RESET" %-5d                                       |\n", refer);
    printf(BR_YEL"|     Patients Died:      "RESET" %-5d                                       |\n", diedtoday);
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                              RIGHT NOW                               |\n"RESET);
    printf(BR_YEL"|     Waiting in Queue:  "RESET"  %-5d                                       |\n", que);
    printf(BR_YEL"|     High Priority:    "RESET"   %-5d                                       |\n", high);
    printf(BR_YEL"|     Medium Priority:  "RESET"   %-5d                                       |\n", med);
    printf(BR_YEL"|     Low Priority:     "RESET"   %-5d                                       |\n", low);
    printf(BR_YEL"|     Beds Occupied:   "RESET"    %-5d                                       |\n", occ);
    printf(BR_YEL"|     Longest stay:    "RESET"    %-43s |\n", longy);
    printf(BR_YEL"|     Shortest stay:   "RESET"    %-43s |\n", shorty);
    printf("+----------------------------------------------------------------------+\n");
    printf(YEL"|                              ALL TIME                                |\n"RESET);
    printf(BR_YEL"|     Total Records in History:      "RESET"              %-5d               |\n",count);
    printf(BR_YEL"|     Total patients released with prescription: "RESET"  %-5d               |\n", prescribed);
    printf(BR_YEL"|     Total patients released after observation: "RESET"  %-5d               |\n",observationz);
    printf(BR_YEL"|     Total patients referred to other depatments: "RESET"%-5d               |\n", referals);
    printf(BR_YEL"|     Total patients Died:                         "RESET"%-5d               |\n", died);
    printf("+----------------------------------------------------------------------+\n");
    free(shorty);
    free(longy);
    }

char *duration(double seconds){

    char *abc = malloc(70*sizeof(char));
    if (seconds < 60){
        sprintf(abc, "%d seconds", (int)seconds);
        }else{
            if(seconds < 3600){
                int x = seconds/60;
                int y =  seconds - x*60;
                sprintf(abc, "%d minutes and %d seconds", x, y);
                }else{
                    if(seconds < 86400){
                        int z = seconds/3600;
                        int v = (seconds - z*3600)/60;
                        int x = (seconds - z*3600) - v*60;
                        sprintf(abc, "%d hours %d minutes and %d seconds",z, v, x);
                    }else{
                        int y = seconds/86400;
                        int z = (seconds - y*86400)/3600;
                        int u = ((seconds - y*86400) - z*3600)/60;
                        int v = ((seconds - y*86400) - z*3600) - u*60;
                        sprintf(abc, "%d days %d hours %d minutes and %d seconds",y, z, u, v);
                    }
                }
        }
        return abc;
}

