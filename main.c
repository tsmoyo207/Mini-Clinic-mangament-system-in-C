#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "observations.h"
#include "utility.h"
#include "records.h"

//What if we ask the outcome of the preveous patient instead of the current cause it seems like you'd have to wait till he's finished
//you need an age calulator function
//you need to prioritise children and old ladies a little bit more than regaulr patients
//you need to separate case 2, 3 and 5 from your main menu into separate functions. Call them handleaddpatient, handle.....
//you need to check the national ID when user is entering details to see if user has ever been here before then autofill the rest
//you need to allow editing of the queue, that is the a patient being removed from the queue or moving up or down the queue or editing the record on the queeu


int main(){
    patient pat, currentPatientInRoom;
    currentPatientInRoom.ticketnum = 0;
    Node *start = loadqueue();
    pat.ticketnum = 0;
    int q, opt1,ticketnumber, id;
    observ_arr arr = loadobservations();
    bool loop = true;

    do{
        printmenu(arr, start, currentPatientInRoom);
        printf("\n");
        printf("     "BB_WHITE" Enter Option Please: "RESET);
        while((scanf("%d", &opt1))!= 1){
                printf(RED"Invalid input!" RESET "Please Enter a number: ");
                while(getchar() != '\n');
                }
        printf("\n");
        printf("\n");
        switch(opt1){
        case 1:
            viewqueue(start);
            pause();
            break;
        case 2:
           patient pat  = registerpatient(start);
           start = addpatient(start, pat);
           pause();
           break;
        case 3:
            printf("Enter the patient's id: ");
            while((scanf("%d", &id))!= 1){
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
            }
            editqueue(start, id);
            pause();
            break;
        case 4:
            printf("Enter ticketnumber to remove patient from Queue: ");
            while((scanf("%d", &ticketnumber))!= 1){
                printf(RED"Invalid input!" RESET " Please Enter a number: ");
                while(getchar() != '\n');
            }
            start = removepatient(start,ticketnumber);
            pause();
            break;
        case 5:
            if (start == NULL){
                printf("No patients waiting");
            }else{
                if (currentPatientInRoom.ticketnum == 0){
                    printf("Next Patient:"GRN" %s %s "RESET"          Ticket number:"RED_BR" %d\n"RESET,
                        start->data.firstname,
                        start->data.lastname,
                        start->data.ticketnum
                         );
                    currentPatientInRoom = start->data;
                }else{

                    Node *temp = start;
                    start = start->next;
                    free(temp);
                    patoutcome patout;
                    patout.admissiondate = 0;
                    patout.data = currentPatientInRoom;
                    printf("Next Patient:"GRN" %s %s "RESET"           Ticket number:"RED_BR" %d\n"RESET,
                            start->data.firstname,
                            start->data.lastname,
                            start->data.ticketnum
                             );
                    printf("\n");
                    printf("What was the Doctors Diagnosis for %s %s: ", currentPatientInRoom.firstname, currentPatientInRoom.lastname);
                    currentPatientInRoom = start->data;
                    scanf(" %[^\n]", patout.diagnosis);
                    printf("\n");
                    do{
                        printf(CYAN"     CONSULTATION RESULT SUB MENU\n"RESET);
                        printf(CYAN"     [1]"RESET" Discharge with prescription\n");
                        printf(CYAN"     [2]"RESET" Place Under Observarion\n");
                        printf(CYAN"     [3]"RESET" Refer to Another Depatment\n");
                        printf(BB_WHITE"     select an option: ");
                        while((scanf("%d", &patout.outcome))!= 1){
                        printf(RED"Invalid input!" RESET " Please Enter a number: ");
                        while(getchar() != '\n');
                        }
                        printf("\n");
                        printf("\n");

                        switch(patout.outcome){
                            case 1:
                                printf("Enter the Prescription given: ");
                                scanf(" %[^\n]", patout.treatment);
                                savehistory(patout);
                                break;
                            case 2:
                                arr = placeobservation(start, arr, patout.diagnosis);
                                break;
                            case 3:
                                printf("Enter Name of Department  reffered to: ");
                                scanf(" %[^\n]", patout.treatment);
                                savehistory(patout);
                                break;
                            default:
                                printf(RED"Invalid option!"RESET " Please enter a number between 1 and 3");
                                printf("\n");
                                printf("\n");
                                break;
                        }
                    }while(patout.outcome != 3 && patout.outcome != 2 && patout.outcome != 1);
                }
                pause();
            }
            break;
        case 6:
                viewobservations(arr);
                pause();
                break;
        case 7:
                arr = dischargepatient(arr);
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
                statistics(start, arr);
                pause();
                break;
        case 0:
                printf("System Shutting Down.............");

                savequeue(start);
                saveobservations(arr);
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

