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
    patient pat;
    Node *start = loadqueue();
    pat.ticketnum = 0;
    int q, opt1,ticketnumber;
    observ_arr arr = loadobservations();
    bool loop = true;

    do{
        printmenu(arr, start);
        printf("\n");
        printf("     "BB_WHITE" Enter Option Please: "RESET);
        scanf("%d", &opt1);
        printf("\n");
        printf("\n");
        switch(opt1){
        case 1:
            viewqueue(start);
            pause();
            break;
        case 2:
           patient pat  = registerpatient();
           start = addpatient(start, pat);

            break;
        case 3:
            printf("Enter ticketnumber to remove patient from Queue: ");
            scanf("%d", &ticketnumber);
            start = removepatient(start,ticketnumber);
            break;
        case 4:
            if (start == NULL){
                printf("No patients waiting");
            }else{
                Node* tmp = start;
                patoutcome patout;
                patout.admissiondate = 0;
                patout.data = tmp->data;
                printf("Next Patient:"GRN" %s %s "RESET"           Ticket number:"RED_BR" %d\n"RESET,
                        start->data.firstname,
                         start->data.lastname,
                         start->data.ticketnum
                         );
                printf("\n");
                printf("What was the Doctors Diagnosis: ");
                scanf(" %[^\n]", patout.diagnosis);
                printf("\n");
                printf(CYAN"     CONSULTATION RESULT SUB MENU\n"RESET);
                printf(CYAN"     [1]"RESET" Discharge with prescription\n");
                printf(CYAN"     [2]"RESET" Place Under Observarion\n");
                printf(CYAN"     [3]"RESET" Refer to Another Depatment\n");
                printf(BB_WHITE"     select an option: ");
                scanf("%d", &patout.outcome);
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
                }
                start = start->next;
                free(tmp);
                }
                pause();
                break;
        case 5:
                viewobservations(arr);
                pause();
                break;
        case 6:
                arr = dischargepatient(arr);
                break;
        case 7:
                int num;
                printf("Enter Specific ID to search for record or 0 to show all records: ");
                scanf("%d", &num);
                viewhistory(num);
                pause();
                break;
        case 8:
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
            }
    }while(loop==true);
    return 0;
}
