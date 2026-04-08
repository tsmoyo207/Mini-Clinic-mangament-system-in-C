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
        }else{
            while(current->next != NULL && current->data.priority >= pat.priority){
            prev = current;
            current = current->next;
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
