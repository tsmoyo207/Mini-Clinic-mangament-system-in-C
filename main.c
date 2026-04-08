#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "observations.h"
#include "utility.h"
#include "records.h"

//General Statistics
//What if we ask the outcome of the preveous patient instead of the current cause it seems like you'd have to wait till he's finished
//you need an age calulator function
//you need to prioritise children and old ladies a little bit more than regaulr patietns
//you neeed a veiw record function for viewhistory
//we want to add money by the patient and also add that to statistics
//you need to separate case 2, 3 and 5 from your main menu into separate functions. Call them handleaddpatient, handle.....
//you need to check the national ID when user is entering details to see if user has ever been here before then autofill the rest
//you need to allow editing of the queue, that is the a patient being removed from the queue or moving up or down the queue or editing the record on the queeu
//you can record consulation duration as well


int main(){
    patient pat;
    Node *start = loadqueue();
    pat.ticketnum = 0;
    int i,q;
    int opt1, opt2, opt3, opt4;
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
            int valid = 0;
            do{
                printf("Enter Firstname and Surname: ");
                scanf("%s %s", pat.firstname, pat.lastname);

                printf("Enter ID Number: ");
                while((scanf("%d", &pat.id))!= 1){
                    printf(RED"Invalid input!" RESET "Please Enter a number: ");
                    while(getchar() != '\n');
                }

                printf("Enter Date of Birth (DD MM YYYY): ");
                while((scanf("%d %d %d", &pat.dob.day, &pat.dob.month, &pat.dob.year )!= 3)){
                      printf(RED"Invalid input!" RESET "Please Enter numeric digits: ");
                      while(getchar() != '\n');
                      }

                printf("Enter Gender(M/F): ");
                scanf(" %c", &pat.gender);
                printf("Enter Issue/ Complaint: ");
                scanf(" %[^\n]", pat.issue);
                printf("Enter Priority {1,2,3}: ");
                scanf(" %d", &pat.priority);
                valid = verify(pat, start);
                switch(valid){
                case 1:
                    printf(RED"Invalid Date. Try Again\n"RESET);
                    break;
                case 2:
                    printf(RED"Invalid ID Number. Try Again\n"RESET);
                    break;
                case 3:
                    printf(RED"Invalid Firstname. Try Again\n"RESET);
                    break;
                case 4:
                    printf(RED"Invalid Lastname Age. Try Again\n"RESET);
                    break;
                case 5:
                    printf(RED"Patient is already in queue\n"RESET);
                    break;
                case 7:
                    printf(RED"Please Enter a number from 1 to 3 for priority\n"RESET);
                    break;
                case 6:
                    printf(RED"Please Enter  the letter M/m/F/f for gender"RESET);
                    break;
                }
                }while(valid != 0);
            pat.ticketnum = 0;
            start = addpatient(start, pat);
            pause();
            break;
        case 3:
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
        case 4:
                viewobservations(arr);
                pause();
                break;
        case 5:
                printf("Enter Bed Number: ");
                scanf("%d", &q);
                while(q<0 || q> 20 ){
                    printf(RED"Invalid Bed Number, Please try again: "RESET);
                    scanf("%d", &q);
                }
                if(arr.beds[q-1].occupied == 0){
                    printf(RED"This bed is already empty\n"RESET);
                }else{
                    arr.beds[q-1].occupied = 0;
                    printf(GRN"%s %s has been discharged successfully\n"RESET,
                            arr.beds[q-1].info.firstname,
                            arr.beds[q-1].info.lastname
                            );
                    patoutcome patty;
                    patty.data = arr.beds[q-1].info;
                    patty.admissiondate = arr.beds[q-1].date;
                    patty.outcome = 2;
                    strcpy(patty.diagnosis , arr.beds[q-1].diagnosis);
                    strcpy(patty.treatment , "Medical Observation");
                    savehistory(patty);
                        }
                pause();
                break;
        case 6:
                int num;
                printf("Enter Specific ID to search for record or 0 to show all records: ");
                scanf("%d", &num);
                viewhistory(num);
                pause();
                break;
        case 7:
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
