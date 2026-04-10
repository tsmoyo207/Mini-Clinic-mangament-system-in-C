#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "constants.h"
#include <string.h>
#include "utility.h"


/**This Procedure displayes the queue in a tabular format changing colours according to priority
*/

void viewqueue(Node *temp){
    if (temp == NULL){
        printf("The Queue is empty");
    }else{
        printf(GRN_BR_BOLD "-----------------------------------------PATIENT QUEUE--------------------------------------------\n" RESET);
        printf(RED_BR"  RED - high priority              "BR_YEL" YELLOW - medium priority                "CYAN" BLUE - low priority\n"RESET);
        printf("\n");

        printf(GRN_BR_UND"                                                                                                 \n" RESET);
        printf(GRN_BR_UND"| QueueNo. | Firstname       | Surname         | ID     | D.O.B      | Sex | Issue              |\n" RESET);
        while(temp!= NULL){
            if(temp->data.priority == 1){
                printf( CYAN "| %-8d " RESET "| %-15s |" CYAN  " %-15s " RESET "| %-6d |" CYAN " %-2d/%-2d/%d "RESET"|  %c  |" CYAN " %s\n"RESET,
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
                   printf(BR_YEL "| %-8d " RESET "| %-15s |" BR_YEL   " %-15s " RESET "| %-6d |" BR_YEL " %-2d/%-2d/%d "RESET"|  %c  |" BR_YEL " %s\n"RESET,
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
                   printf(RED_BR "| %-8d " RESET "| %-15s |" RED_BR " %-15s " RESET "| %-6d |" RED_BR " %-2d/%-2d/%d "RESET"|  %c  |" RED_BR " %s\n"RESET,
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
    }

}

/**This Function reads the queue records from the file and connects the nodes creating a queue
*/
Node* loadqueue(){
    FILE* f = fopen("patientqueue.csv", "r");
    Patient pat;
    Node  *start = NULL;
    if (!f) {
        printf("Error opening patient queue file\n");
    }else{
        Node *current = NULL, *newNode= NULL;
        while((fscanf(f, "%d , %99[^,] , %99[^,] , %d , %d , %d , %d, %c, %99[^,] , %d",
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

    if(start == NULL){//this means queue is empty
        newNode->data.ticketnum = 1000;
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
        int maxTicketNum = size->data.ticketnum;
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
    printf(GRN"Record Added to Queue successfully\n"RESET);
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
            fprintf(f, "%d, %s, %s, %d, %d, %d, %d, %c, %s, %d\n",
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
            printf("Enter ID Number: ");
            while((scanf("%d", &newPatient.id))!= 1){
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
                    printf(RED"Error " RESET "Patient Already in Queue: \n");
                }
            }while(inqueue == true);


            //Checks for patient ID in history records
            FILE* f = fopen("patienthistory.csv", "r");
            if (!f) {
                printf("");
            }else{
                while((fscanf(f, " %99[^,] , %99[^,] , %d , %d , %d , %d , %c , \"%99[^\"]\" ",
                          csvRecord.firstname,
                          csvRecord.lastname,
                          &csvRecord.id,
                          &csvRecord.dob.day,
                          &csvRecord.dob.month,
                          &csvRecord.dob.year,
                          &csvRecord.gender,
                          csvRecord.issue
                          )) == 8 && inrecords == false){

                    cachedRecord = csvRecord;
                    if(newPatient.id == csvRecord.id){
                        inrecords = true;
                    }
                }
                fclose(f);
            }

        char option1, option2;
        if (inrecords){
            printf("We Found a records with the same ID under the name "CYAN"%s %s.\n"RESET, cachedRecord.firstname, cachedRecord.lastname);
            printf("Is the Patient back for a review? (Y/n): ");
            scanf(" %c", &option1);
        ```//Autofill user information for all deatials and set priority to 1.
            if (option1 == 'Y' || option1 == 'y'){
                    newPatient = cachedRecord;
                    newPatient.priority = 1;
            }else{
                printf("Should we autofill Firstname and Lastname, Date of Birth and Gender (Y/n)?:  ");
                scanf(" %c", &option2);
                if (option2 == 'Y' || option2 == 'y'){
                    strcpy(newPatient.firstname, cachedRecord.firstname);
                    strcpy(newPatient.lastname, cachedRecord.lastname);
                    newPatient.dob = cachedRecord.dob;
                    newPatient.gender = cachedRecord.gender;

                }else{
                    printf("Enter Firstname and Surname: ");
                    scanf("%s %s", newPatient.firstname, newPatient.lastname);
                    printf("Enter Date of Birth (DD MM YYYY): ");
                    while((scanf("%d %d %d", &newPatient.dob.day, &newPatient.dob.month, &newPatient.dob.year )!= 3)){
                            printf(RED"Invalid input!" RESET "Please Enter numeric digits: ");
                            while(getchar() != '\n');
                            }
                    printf("Enter Gender(M/F): ");
                    scanf(" %c", &newPatient.gender);
                }
                printf("Enter Issue/ Complaint: ");
                scanf(" %[^\n]", newPatient.issue);
                printf("Enter Priority {1,2,3}: ");
                scanf(" %d", &newPatient.priority);
                valid = validatePatient(newPatient);
            }
        }else{
                printf("Enter Firstname and Surname: ");
                scanf("%s %s", newPatient.firstname, newPatient.lastname);
                printf("Enter Date of Birth (DD MM YYYY): ");
                while((scanf("%d %d %d", &newPatient.dob.day, &newPatient.dob.month, &newPatient.dob.year )!= 3)){
                        printf(RED"Invalid input!" RESET "Please Enter numeric digits: ");
                        while(getchar() != '\n');
                }
                printf("Enter Gender(M/F): ");
                scanf(" %c", &newPatient.gender);
                printf("Enter Issue/ Complaint: ");
                scanf(" %[^\n]", newPatient.issue);
                printf("Enter Priority {1,2,3}: ");
                scanf(" %d", &newPatient.priority);
                valid = validatePatient(newPatient);
        }

        //To make sure user enters the correct Details
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
        printf(RED"ticketnum #%d not found"RESET, ticketnumber);
        return start;
    }
    newPatient = current->data;
    if(prev == NULL){
        start = current->next;
    }else{
        prev->next = current->next;
    }
    free(current);

    printf(GRN"ticketnumber: %d | %s %s"RESET,
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
void editqueue(Node *start, int id){
    Patient newPatient;
    if(start == NULL){
        printf(RED"the queue is empty\n"RESET);
    }else{
        Node *current = start;
        while(current != NULL && current->data.id != id){
            current = current->next;
        }
        if(current == NULL){
            printf(RED"id #%d not found"RESET, id);
        }else{
            current->data.id = 0;
            Patient pat = registerpatient(start);
            pat.ticketnum = current->data.ticketnum;
            current->data = pat;
            printf(GRN"record edited successfully\n"RESET);
        }
    }
    printf("\n");
    printf("\n");
}
