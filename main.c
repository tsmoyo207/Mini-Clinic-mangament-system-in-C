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
    int bednumber, menuchoice,ticketnumber, id;
    Ward ward = loadobservations();
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
            printf("Enter the Patient's id: ");
            while((scanf("%d", &id))!= 1){//Makes Sure user enters a number
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
            }
            editqueue(start, id);
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
                printf("No patients waiting\n");
            }else{

                if (currentPatientInRoom.ticketnum == 0){//This means No one is seeing the Doctor at the moment
                    printf("Next Patient:"GRN" %s %s "RESET"          Ticket number:"RED_BR" %d\n"RESET,
                        start->data.firstname,
                        start->data.lastname,
                        start->data.ticketnum
                         );
                    currentPatientInRoom = start->data;
                }else{

                    Node *temp = start;
                    start = start->next;
                    if(start == NULL){
                         printf("No patients waiting\n");
                    }else{
                        printf("Next Patient:"GRN" %s %s "RESET"           Ticket number:"RED_BR" %d\n"RESET,
                            start->data.firstname,
                            start->data.lastname,
                            start->data.ticketnum
                             );
                    }
                    free(temp);
                    ConsulatationRecord record;
                    record.admissiondate = 0;
                    record.data = currentPatientInRoom;

                    printf("\n");
                    printf("What was the Doctors Diagnosis for %s %s: ", currentPatientInRoom.firstname, currentPatientInRoom.lastname);
                    if(start == NULL){
                         currentPatientInRoom.ticketnum = 0;
                    }else{
                         currentPatientInRoom = start->data;
                    }
                    scanf(" %[^\n]", record.diagnosis);
                    printf("\n");
                    do{
                        printf(CYAN"     CONSULTATION RESULT SUB MENU\n"RESET);
                        printf(CYAN"     [1]"RESET" Discharge with prescription\n");
                        printf(CYAN"     [2]"RESET" Place Under Observarion\n");
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
                                printf("Enter Name of Department  reffered to: ");
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
                pause();
            }
            break;
        case 6:
                viewobservations(ward);
                pause();
                break;
        case 7:
                printf("Enter Bed Number to Discharge. If Patient Died, Enter Negative number: ");
                while((scanf("%d", &bednumber))!= 1){
                    printf(RED"Invalid input!" RESET " Please Enter a number: ");
                    while(getchar() != '\n');
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
                printf("System Shutting Down.............");

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

