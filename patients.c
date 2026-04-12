#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "constants.h"
#include <string.h>
#include "utility.h"


/**This Procedure displays the queue in a tabular format changing colours according to priority
*/

void viewqueue(Node *temp){
    if (temp == NULL){
        printf("The Queue is empty");
    }else{
        int count = queuelength(temp);
        printf(GRN_BR_BOLD "======================================= WAITING QUEUE  (%d patients) =======================================\n" RESET,count);
        printf(RED_BR"  RED - high priority              "BR_YEL" YELLOW - medium priority                "CYAN" BLUE - low priority\n"RESET);
        printf("\n");

        printf(GRN_BR_UND"                                                                                                             \n" RESET);
        printf(GRN_BR_UND"| QueueNo. | Firstname       | Surname         | ID                 | D.O.B      | Sex | Issue              |\n" RESET);
        while(temp!= NULL){
            if(temp->data.priority == 1){
                printf( CYAN "| %-8d " RESET "| %-15s |" CYAN  " %-15s " RESET "| %-18lld |" CYAN " %-2d/%-2d/%d "RESET"|  %c  |" CYAN " %s\n"RESET,
                   temp->data.ticketnum,
                   temp->data.firstname,
                   temp->data.lastname,
                   temp->data.id,
                   temp->data.dob.day,
                   temp->data.dob.month,
                   temp->data.dob.year,
                   temp->data.gender,
                   temp->data.issue
                   );
            }else{
                if(temp->data.priority == 2){
                   printf(BR_YEL "| %-8d " RESET "| %-15s |" BR_YEL   " %-15s " RESET "| %-18lld |" BR_YEL " %-2d/%-2d/%d "RESET"|  %c  |" BR_YEL " %s\n"RESET,
                   temp->data.ticketnum,
                   temp->data.firstname,
                   temp->data.lastname,
                   temp->data.id,
                   temp->data.dob.day,
                   temp->data.dob.month,
                   temp->data.dob.year,
                   temp->data.gender,
                   temp->data.issue

                   );
            }else{
                   printf(RED_BR "| %-8d " RESET "| %-15s |" RED_BR " %-15s " RESET "| %-18lld |" RED_BR " %-2d/%-2d/%d "RESET"|  %c  |" RED_BR " %s\n"RESET,
                   temp->data.ticketnum,
                   temp->data.firstname,
                   temp->data.lastname,
                   temp->data.id,
                   temp->data.dob.day,
                   temp->data.dob.month,
                   temp->data.dob.year,
                   temp->data.gender,
                   temp->data.issue
                   );
                    }
            }
            temp= temp->next;
        }
        printf(GRN_BR_BOLD "========================= END OF QUEUE — %d patient(s) waiting =========================\n" RESET, count);
    }

}

/**This Function reads the queue records from the file and connects the nodes creating a queue
*/
Node* loadqueue(){
    FILE* f = fopen("patientqueue.csv", "r");
    Patient pat;
    Node  *start = NULL;
    int count=0;
    if (!f) {
        printf("      Warning: Could not open queue file. Starting with an empty queue.\n");
    }else{
        Node *current = NULL, *newNode= NULL;
        while((fscanf(f, "%d , %99[^,] , %99[^,] , %lld , %d , %d , %d, %c, %99[^,] , %d",
                      &pat.ticketnum,
                      pat.firstname,
                      pat.lastname,
                      &pat.id,
                      &pat.dob.day,
                      &pat.dob.month,
                      &pat.dob.year,
                      &pat.gender,
                      pat.issue,
                      &pat.priority

                      )) == 10){
            count++;
            newNode = malloc(sizeof(Node));
            newNode->data = pat;
            newNode->next= NULL;
            if(start == NULL){
                start = newNode;
                current = start;
            }else{
                current->next = newNode;
                current = newNode;
            }
        }
        fclose(f);
        printf(GRN "       Queue loaded: %d patient(s) restored from last session.\n" RESET, count);
    }
    return start;
}

/**This Function adds a patient record to the queue according to priority and age
*/
Node* enqueuePatient(Node *start, Patient pat){
    Node *current = NULL, *newNode= NULL;
    Node *prev = NULL, *size;
    newNode = malloc(sizeof(Node));
    newNode->data = pat;
    newNode->next= NULL;
    date dob;
    dob = pat.dob;
    int patientAge = age(dob);
    int maxTicketNum = 1000;

    if(start == NULL){//this means queue is empty
        newNode->data.ticketnum = maxTicketNum;
        start = newNode;
        current = start;
    }else{
        current = start;
        size = current;
        //Allows Old people and young Children to be placed at the top of their Priority Group
        if (patientAge > 60 || patientAge < 10){
            while(current->next != NULL && current->data.priority > pat.priority){
            prev = current;
            current = current->next;
            }
        }else{
            //Places a patient record at the bottom of their priority group
            while(current->next != NULL && current->data.priority >= pat.priority){
            prev = current;
            current = current->next;
        }
        }

        //Looks for the largest TicketNum in the queue and increaments it before assigning to new patient record
        maxTicketNum = size->data.ticketnum;
        while(size != NULL){
            if(maxTicketNum < size->data.ticketnum){
                maxTicketNum = size->data.ticketnum;
            }
            size = size->next;
        }
        maxTicketNum++;
        newNode->data.ticketnum = maxTicketNum;

        //Connects Node to the Queue according to position
        if(current == start){
            newNode->next = current;
            start = newNode;
        }else{
            if(current->next == NULL){
                current->next = newNode;
            }else{
                prev->next = newNode;
                newNode->next = current;
            }
        }
    }
    printf(GRN"Record assigned ticketnum #%d and added to Queue successfully\n"RESET,maxTicketNum);
    return start;
}

/**This Procedure saves the queue records to a file
*/
void savequeue(Node *start){
    FILE* f = fopen("patientqueue.csv", "w");
    if (!f) {
        printf("Error opening patient history file\n");
    }else{
        while(start != NULL){
            fprintf(f, "%d, %s, %s, %lld, %d, %d, %d, %c, %s, %d\n",
                    start->data.ticketnum,
                    start->data.firstname,
                    start->data.lastname,
                    start->data.id,
                    start->data.dob.day,
                    start->data.dob.month,
                    start->data.dob.year,
                    start->data.gender,
                    start->data.issue,
                    start->data.priority
                    );
            start = start->next;
        }
    fclose(f);
    printf(GRN "Queue saved successfully.\n" RESET);
    }

}

/**This Function Calculates and returns the length of the queue
*/
int queuelength(Node *start){
    int count = 0;
    while(start != NULL){
        count++;
        start = start->next;
    }
    return count;
}

/**This Function handles the collection of patient information from the user,
    implementing autofill features by searching ID and Prompting the user to re-enter
    details until the validate function returns 0
*/
Patient registerpatient(Node *start){
    Patient newPatient, csvRecord, cachedRecord;
    bool inqueue = false, inrecords = false;
    int valid = 0;
    Node *temp = start;

        do{
            valid = 0;
            inqueue = false;
            printf("Enter Patient ID (18-digit number):");
            while((scanf("%lld", &newPatient.id))!= 1){
                printf(RED"Invalid input!" RESET "Please Enter a number: ");
                while(getchar() != '\n');
                }
            do{
                //Makes Sure User does not enter an ID that is already in the Queue
                temp = start;
                if(temp != NULL){
                    do{
                        if(newPatient.id == temp->data.id){
                            inqueue = true;
                        }else{
                            temp = temp->next;
                        }
                    }while(temp != NULL && inqueue == false);
                }
                if(inqueue){
                    printf(RED"Error " RESET "This patient (ID: %lld) is already waiting in the queue: \n", newPatient.id);
                }
            }while(inqueue == true);


            //Checks for patient ID in history records
            FILE* f = fopen("patienthistory.csv", "r");
            if (!f) {
                printf("");
            }else{
                while((fscanf(f, " %99[^,] , %99[^,] , %lld , %d , %d , %d , %c , \"%99[^\"]\" ",
                          csvRecord.firstname,
                          csvRecord.lastname,
                          &csvRecord.id,
                          &csvRecord.dob.day,
                          &csvRecord.dob.month,
                          &csvRecord.dob.year,
                          &csvRecord.gender,
                          csvRecord.issue
                          )) == 8){
                    if(newPatient.id == csvRecord.id){
                        inrecords = true;
                        cachedRecord = csvRecord;
                    }
                }
                fclose(f);
            }

        char option1, option2;
        if (inrecords){
            printf("A previous record was found for ID %lld under the name "CYAN" %s %s.\n"RESET, cachedRecord.id, cachedRecord.firstname, cachedRecord.lastname);
            printf("Is this a follow-up of the previous visit for "CYAN"%s %s? "RESET"(Y/n): ", cachedRecord.firstname, cachedRecord.lastname);
            scanf(" %c", &option1);
            //Autofill user information for all deatials and set priority to 1.


            //Autofill user information for all deatials and set priority to 1.

            if (option1 == 'Y' || option1 == 'y'){
                    newPatient = cachedRecord;
                    newPatient.priority = 1;
                    printf(GRN "Patient and Consulation information auto-filled from previous record.\n" RESET);
            }else{
                printf("Would you like to auto-fill personal details from their previous record? (Y/n)?:  ");
                scanf(" %c", &option2);
                if (option2 == 'Y' || option2 == 'y'){
                    strcpy(newPatient.firstname, cachedRecord.firstname);
                    strcpy(newPatient.lastname, cachedRecord.lastname);
                    newPatient.dob = cachedRecord.dob;
                    newPatient.gender = cachedRecord.gender;
                    printf(GRN "Personal details auto-filled from previous record.\n" RESET);
                }else{
                    printf("Enter Firstname and Surname: ");
                    scanf("%s %s", newPatient.firstname, newPatient.lastname);
                    printf("Enter Date of Birth (DD MM YYYY) e.g. 15 03 1990: ");
                    while((scanf("%d %d %d", &newPatient.dob.day, &newPatient.dob.month, &newPatient.dob.year )!= 3)){
                            printf(RED"Invalid input!" RESET "Please Enter numeric digits: ");
                            while(getchar() != '\n');
                            }
                    printf("Enter Gender(M/F): ");
                    scanf(" %c", &newPatient.gender);
                }
                printf("Enter Patient's Complaint or Reason for Visit: ");
                scanf(" %[^\n]", newPatient.issue);
                printf("Enter Priority Level:\n");
                printf(CYAN"    [1] Low    -routine checkup\n"RESET);
                printf(YEL "    [2] Medium  -needs attention soon\n"RESET);
                printf(RED "    [3] High    -urgent / emergency\n"RESET);
                printf("Select Priority: ");
                scanf(" %d", &newPatient.priority);
                valid = validatePatient(newPatient);
            }
        }else{
                printf("Enter Firstname and Surname: ");
                scanf("%s %s", newPatient.firstname, newPatient.lastname);
                printf("Enter Date of Birth (DD MM YYYY) e.g. 15 03 1990: ");
                while((scanf("%d %d %d", &newPatient.dob.day, &newPatient.dob.month, &newPatient.dob.year )!= 3)){
                        printf(RED"Invalid input!" RESET "Please Enter numeric digits: ");
                        while(getchar() != '\n');
                }
                printf("Enter Gender(M/F): ");
                scanf(" %c", &newPatient.gender);
                printf("Enter Patient's Complaint or Reason for Visit: ");
                scanf(" %[^\n]", newPatient.issue);
                 printf("Enter Priority Level:\n");
                printf(CYAN"    [1] Low    -routine checkup\n"RESET);
                printf(YEL "    [2] Medium -needs attention soon\n"RESET);
                printf(RED "    [3] High   -urgent / emergency\n"RESET);
                printf("Select Priority: ");
                scanf(" %d", &newPatient.priority);
                valid = validatePatient(newPatient);
        }

        //To make sure user enters the correct Details
        switch(valid){
            case 1:
                printf(RED"Invalid date of birth. Please check day (1-31), month (1-12) and year (1900-2026).\n"RESET);
                break;
            case 2:
                printf(RED"Invalid ID."RESET" ID must be a 18-digit National ID.\n");
                break;
            case 3:
                printf(RED"Invalid first name."RESET" Only letters are allowed, no numbers or symbols.\n");
                break;
            case 4:
                printf(RED"Invalid last name."RESET" Only letters are allowed, no numbers or symbols.\n");
                break;
            case 7:
                printf(RED"Invalid priority. "RESET"Please enter 1 (Low), 2 (Medium) or 3 (High).\n");
                break;
            case 6:
                printf(RED"Invalid gender."RESET" Please enter M for Male or F for Female."RESET);
                break;
            }
    }while(valid != 0);
    newPatient.ticketnum = 0;
    return newPatient;
}

/**This Function allows the removal of a patient record form the queue in accordance with ticknumber
*/
Node* dequeuePatient(Node *start, int ticketnumber){
    Patient newPatient;
    if(start == NULL){
        printf(RED"the queue is empty\n"RESET);
        return NULL;
    }

    Node *current = start;
    Node *prev = NULL;
    while(current != NULL && current->data.ticketnum != ticketnumber){
        prev = current;
        current = current->next;
    }
    if(current == NULL){
        printf(RED"No patient found with ticket number %d. No changes made.\n"RESET, ticketnumber);
        return start;
    }
    newPatient = current->data;
    if(prev == NULL){
        start = current->next;
    }else{
        prev->next = current->next;
    }
    free(current);

    printf(GRN"Patient removed from queue -Ticket: %d | Name: %s %s\n"RESET,
          newPatient.ticketnum,
          newPatient.firstname,
          newPatient.lastname
          );
          printf("\n");
          printf("\n");
    return start;
}

/**This Function allows the re-entering of patient details in case of incoreently entered details
*/
void editqueue(Node *start, int ticketnum){
    if(start == NULL){
        printf(RED"the queue is empty\n"RESET);
    }else{
        Node *current = start;
        while(current != NULL && current->data.ticketnum != ticketnum){
            current = current->next;
        }
        if(current == NULL){
            printf(RED"No patient with ticket %d found in the queue.\n"RESET, ticketnum);
        }else{
            printf("Editing record for: "CYAN"%s %s (TicketNum: %d)\n"RESET,
            current->data.firstname,
            current->data.lastname,
            current->data.ticketnum);
            current->data.id = 0;
            Patient pat = registerpatient(start);
            pat.ticketnum = ticketnum;
            current->data = pat;
            printf(GRN"Patient record updated successfully.\n"RESET);
        }
    }
    printf("\n");
    printf("\n");
}
