#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "constants.h"
#include <string.h>
#include "utility.h"


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

Node* loadqueue(){
    FILE* f = fopen("patientqueue.csv", "r");
    patient pat;
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
    }
    fclose(f);
    return start;
}

Node* addpatient(Node *start, patient pat){
    Node *current = NULL, *newNode= NULL;
    Node *prev = NULL, *size;
    newNode = malloc(sizeof(Node));
    newNode->data = pat;
    newNode->next= NULL;
    date dob;
    dob = pat.dob;
    int oldness = age(dob);
    if(start == NULL){
        newNode->data.ticketnum = 1000;
        start = newNode;
        current = start;
    }else{
        current = start;
        size = current;

        if (oldness > 60 || oldness < 10){
            while(current->next != NULL && current->data.priority > pat.priority){
            prev = current;
            current = current->next;
            }
        }else{
            while(current->next != NULL && current->data.priority >= pat.priority){
            prev = current;
            current = current->next;
        }
        }
        int x = size->data.ticketnum;
        while(size != NULL){
            if(x < size->data.ticketnum){
                x = size->data.ticketnum;
            }
            size = size->next;
        }
        x++;
        newNode->data.ticketnum = x;
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
    }
    fclose(f);
}

int queuelength(Node *start){
    int count = 0;
    while(start != NULL){
        count++;
        start = start->next;
    }
    return count;
}

patient registerpatient(Node *start){
    patient pat, rec, rey;
    bool inqueue = false, inrecords = false;
    int valid = 0;


        do{
            inqueue = false;
            printf("Enter ID Number: ");
            while((scanf("%d", &pat.id))!= 1){
                printf(RED"Invalid input!" RESET "Please Enter a number: ");
                while(getchar() != '\n');
                }

            do{
                if(pat.id == start->data.id){
                    inqueue = true;
                }else{
                    start = start->next;
                }
            }while(start != NULL && inqueue == false);

            if(inqueue){
                printf(RED"Error " RESET "Patient Already in Queue: \n");
            }

            }while(inqueue == true);


    FILE* f = fopen("patienthistory.csv", "r");
    int id, day, month, year;
    char gender;
    bool empty = true, found = false;
    char firstname[100], lastname[100], diagnosis[100], treatment[100];
    int outcome;
    time_t releasedate, admissiondate;

        if (!f) {
            printf("");
        }else{
            while((fscanf(f, " %99[^,] , %99[^,] , %d , %d , %d , %d , %c , \"%99[^\"]\" , %d , \"%99[^\"]\" , %ld, %ld",
                      rec.firstname,
                      rec.lastname,
                      &rec.id,
                      &rec.dob.day,
                      &rec.dob.month,
                      &rec.dob.year,
                      &rec.gender,
                      rec.issue,
                      &outcome,
                      treatment,
                      &releasedate,
                      &admissiondate)) == 12 && inrecords == false){

                rey = rec;
                if(pat.id == rec.id){
                    inrecords = true;
                }
                      }
        fclose(f);
        char option, option2;
        if (inrecords){
            printf("We Found a records with the same ID under the name "CYAN"%s %s.\n"RESET, rey.firstname, rey.lastname);
            printf("Is the patient back for a review? (Y/n): ");
            scanf(" %c", &option2);
            if (option2 == 'Y' || option2 == 'y'){
                    pat = rey;
                    pat.priority = 1;
            }else{
                printf("Should we autofill Firstname and Lastname, Date of Birth and Gender (Y/n)?:  ");
                scanf(" %c", &option);
                if (option == 'Y' || option == 'y'){
                    strcpy(pat.firstname, rey.firstname);
                    strcpy(pat.lastname, rey.lastname);
                    pat.dob = rey.dob;
                    pat.gender = rey.gender;

                }else{
                    printf("Enter Firstname and Surname: ");
                    scanf("%s %s", pat.firstname, pat.lastname);
                    printf("Enter Date of Birth (DD MM YYYY): ");
                    while((scanf("%d %d %d", &pat.dob.day, &pat.dob.month, &pat.dob.year )!= 3)){
                            printf(RED"Invalid input!" RESET "Please Enter numeric digits: ");
                            while(getchar() != '\n');
                            }
                    printf("Enter Gender(M/F): ");
                    scanf(" %c", &pat.gender);
                }
                printf("Enter Issue/ Complaint: ");
                scanf(" %[^\n]", pat.issue);
                printf("Enter Priority {1,2,3}: ");
                scanf(" %d", &pat.priority);
                valid = verify(pat);
            }
        }else{
                printf("Enter Firstname and Surname: ");
                scanf("%s %s", pat.firstname, pat.lastname);
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
                valid = verify(pat);
        }


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
            return pat;

}

Node* removepatient(Node *start, int ticketnumber){
    patient pat;
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
    pat = current->data;

    if(prev == NULL){
        start = current->next;
    }else{
        prev->next = current->next;
    }

    free(current);

    printf(GRN"ticketnumber: %d | %s %s"RESET,
          pat.ticketnum,
          pat.firstname,
          pat.lastname
          );
          printf("\n");
          printf("\n");

          return start;
}

void editqueue(Node *start, int id){

    patient pat;
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
            patient pat = registerpatient(start);
            pat.ticketnum = current->data.ticketnum;
            current->data = pat;
            printf(GRN"record edited successfully\n"RESET);
        }
    }
    printf("\n");
    printf("\n");

}
