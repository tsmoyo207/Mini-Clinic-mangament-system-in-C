#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "observations.h"
#include "utility.h"
#include "records.h"

int main(){
    Patient pat, currentPatientInRoom;
    currentPatientInRoom.ticketnum = 0;
    Node *start = loadqueue();
    pat.ticketnum = 0;
    int bednumber, menuchoice,ticketnumber, ticket;
    Ward ward = loadobservations();
    char opt;
    bool loop = true;

    do{
        printmenu(ward, start, currentPatientInRoom);
        printf("\n");
        printf("     "BB_WHITE" Enter Option Please: "RESET);
        while((scanf("%d", &menuchoice))!= 1){
                printf(RED"Invalid input!" RESET "Please Enter a number: ");
                while(getchar() != '\n');
                }
        printf("\n");
        printf("\n");
        switch(menuchoice){
        case 1:
            viewqueue(start);
            pause();
            break;
        case 2:
           pat  = registerpatient(start);
           start = enqueuePatient(start, pat);
           pause();
           break;
        case 3:
            printf("Enter the Patient's Ticket Number: ");
            while((scanf("%d", &ticket))!= 1){//Makes Sure user enters a number
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
            }
            editqueue(start, ticket);
            pause();
            break;
        case 4:
            printf("Enter ticketnumber to remove Patient from Queue: ");
            while((scanf("%d", &ticketnumber))!= 1){
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
            }
            start = dequeuePatient(start,ticketnumber);
            pause();
            break;
        case 5:
            if (start == NULL){
                printf("The waiting queue is empty. No patients to call.\n");
            }else{

                if (currentPatientInRoom.ticketnum == 0){//This means No one is seeing the Doctor at the moment
                    char priority[8];
                    if(start->data.priority == 1){
                        strcpy(priority, "LOW");
                    }else{
                        if(start->data.priority == 2){
                        strcpy(priority, "MEDIUM");
                        }else{
                            strcpy(priority, "HIGH");
                        }
                    }
                    printf("Calling to Consultation Room: "GRN"%s %s "RESET"|  Ticket: "YEL" %d "RESET" |  Priority: "YEL" %s"RESET,
                        start->data.firstname,
                        start->data.lastname,
                        start->data.ticketnum,
                        priority
                         );
                    currentPatientInRoom = start->data;
                }else{

                    Node *temp = start;
                    start = start->next;

                    if(start == NULL){
                         printf("No patients waiting\n");
                    }else{
                        char priority[8];
                        if(start->data.priority == 1){
                            strcpy(priority, "LOW");
                        }else{
                            if(start->data.priority == 2){
                            strcpy(priority, "MEDIUM");
                            }else{
                                strcpy(priority, "HIGH");
                            }
                        }
                       printf("Calling to Consultation Room: "GRN"%s %s "RESET"|  Ticket: "YEL" %d "RESET" |  Priority: "YEL" %s"RESET,
                        start->data.firstname,
                        start->data.lastname,
                        start->data.ticketnum,
                        priority
                         );
                    }

                    ConsulatationRecord record;
                    record.admissiondate = 0;
                    record.data = currentPatientInRoom;

                    printf("\n");
                    printf(GRN"--- Closing consultation for %s %s (Ticket: %d) ---\n"RESET,
                        currentPatientInRoom.firstname,
                        currentPatientInRoom.lastname,
                        currentPatientInRoom.ticketnum);

                    printf("Enter Doctor's Diagnosis for %s %s: ", currentPatientInRoom.firstname, currentPatientInRoom.lastname);
                    if(start == NULL){
                         currentPatientInRoom.ticketnum = 0;
                    }else{
                         currentPatientInRoom = start->data;
                    }
                    scanf(" %[^\n]", record.diagnosis);
                    printf("\n");
                    do{
                        printf(CYAN"     -----CONSULTATION OUTCOME-----\n"RESET);
                        printf(CYAN"     [1]"RESET" Discharge with prescription\n");
                        printf(CYAN"     [2]"RESET" Admit to observation Ward\n");
                        printf(CYAN"     [3]"RESET" Refer to Another Depatment\n");
                        printf(BB_WHITE"     select an option: ");
                        while((scanf("%d", &record.outcome))!= 1){
                        printf(RED"Invalid input!" RESET " Please Enter a number: ");
                        while(getchar() != '\n');
                        }
                        printf("\n");
                        printf("\n");

                        switch(record.outcome){
                            case 1:
                                printf("Enter the Prescription given: ");
                                scanf(" %[^\n]", record.treatment);
                                appendConsultationRecord(record);
                                break;
                            case 2:
                                ward = admitToBed(temp, ward, record.diagnosis);
                                break;
                            case 3:
                                printf("Enter Name of Department  referred to: ");
                                scanf(" %[^\n]", record.treatment);
                                appendConsultationRecord(record);
                                break;
                            default:
                                printf(RED"Invalid option!"RESET " Please enter a number between 1 and 3");
                                printf("\n");
                                printf("\n");
                                break;
                        }
                    }while(record.outcome != 3 && record.outcome != 2 && record.outcome != 1);
                    free(temp);
                }

                pause();
            }
            break;
        case 6:
                viewobservations(ward);
                pause();
                break;
        case 7:
                printf("Enter Bed Number to Discharge Patient: ");
                while((scanf("%d", &bednumber))!= 1){
                    printf(RED"Invalid input!" RESET " Please Enter a number: ");
                    while(getchar() != '\n');
                }
                printf("Did the patient pass away? (Y/n)");
                scanf(" %c", &opt);
                if (opt == 'Y' || opt == 'y'){
                    bednumber = bednumber * -1;
                }
                ward = dischargepatient(ward, bednumber);
                break;
        case 8:
                int num;
                printf("Enter Specific ID to search for record or 0 to show all records: ");
                while((scanf("%d", &num))!= 1){
                    printf(RED"Invalid input!" RESET " Please Enter a number: ");
                    while(getchar() != '\n');
                }
                viewhistory(num);
                pause();
                break;
        case 9:
                statistics(start, ward);
                pause();
                break;
        case 0:
                if(currentPatientInRoom.ticketnum != 0){
                    ConsulatationRecord record;
                    record.admissiondate = 0;
                    record.data = currentPatientInRoom;
                    printf("Enter The Consulation details of the patient in the consulation Room to exit\n");
                    printf("Enter Doctor's Diagnosis for %s %s: ", currentPatientInRoom.firstname, currentPatientInRoom.lastname);
                    scanf(" %[^\n]", record.diagnosis);
                    printf("\n");
                    do{
                        printf(CYAN"     -----CONSULTATION OUTCOME-----\n"RESET);
                        printf(CYAN"     [1]"RESET" Discharge with prescription\n");
                        printf(CYAN"     [2]"RESET" Admit to observation Ward\n");
                        printf(CYAN"     [3]"RESET" Refer to Another Depatment\n");
                        printf(BB_WHITE"     select an option: ");
                        while((scanf("%d", &record.outcome))!= 1){
                        printf(RED"Invalid input!" RESET " Please Enter a number: ");
                        while(getchar() != '\n');
                        }
                        printf("\n");
                        printf("\n");

                        switch(record.outcome){
                            case 1:
                                printf("Enter the Prescription given: ");
                                scanf(" %[^\n]", record.treatment);
                                appendConsultationRecord(record);
                                break;
                            case 2:
                                ward = admitToBed(start, ward, record.diagnosis);
                                break;
                            case 3:
                                printf("Enter Name of Department  referred to: ");
                                scanf(" %[^\n]", record.treatment);
                                appendConsultationRecord(record);
                                break;
                            default:
                                printf(RED"Invalid option!"RESET " Please enter a number between 1 and 3");
                                printf("\n");
                                printf("\n");
                                break;
                        }
                    }while(record.outcome != 3 && record.outcome != 2 && record.outcome != 1);
                }
                printf("Saving data and shutting down. Goodbye.\n");
                savequeue(start);
                saveobservations(ward);
                freequeue(start);
                loop = false;
                break;
        default:
                printf(RED"Invalid selection!"RESET " Please enter a number between 0 and 9 \n");
                pause();
                break;
            }
    }while(loop==true);
    return 0;
}

