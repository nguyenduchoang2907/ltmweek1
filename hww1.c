#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sllist.h"

void printMenu(){
    printf("USER MANAGEMENT PROGRAMM\n");
    printf("---------------------------------------------\n");
    printf("\t1. Register\n");
    printf("\t2. Sign in\n");
    printf("\t3. Search\n");
    printf("\t4. Sign out\n");
    printf("Your choice (1-4, other to quit): ");
}

int userLogin(LIST *userList, LIST *loginList, int *loginAttempts){
    char temporaryUsername[20];
    char temporaryPassword[20];
    LISTNODE* tempNode;

    do{
        printf("Username: ");
        scanf("%s", temporaryUsername);
        tempNode = searchList(userList, temporaryUsername);
        if(tempNode == NULL) printf("Cannot find account\n");
        else if(tempNode->inf.status == 0) printf("Account is blocked\n");
    }while(tempNode == NULL || tempNode->inf.status == 0);

    do{
        printf("Password: ");
        scanf("%s", temporaryPassword);
        if(strcmp(temporaryPassword, tempNode->inf.pass) != 0){
            printf("Password is incorrected\n");
            (*loginAttempts)++;
        }
        else {
            (*loginAttempts) = 0;
        }
    }while(strcasecmp(temporaryPassword, tempNode->inf.pass) != 0 && (*loginAttempts) < 3);

    if((*loginAttempts) >= 3) {
        tempNode->inf.status = 0;
        printf("Password is incorrected. Account is blocked\n");
        return 0;
    }
    else {
        insertAfterCurrent(loginList, tempNode->inf);
        printf("Hello %s\n", tempNode->inf.name);
    }
    return 1;
}

void userLogout(LIST *userList, LIST *loginList){
    char temporaryUsername[20];
    LISTNODE* tempNode;

    printf("Username: ");
    scanf("%s", temporaryUsername);
    tempNode = searchList(userList, temporaryUsername);
    if(tempNode == NULL) printf("Cannot find account\n");
    else{
        tempNode = searchList(loginList, temporaryUsername);
        if(tempNode == NULL) printf("Account is not signed in\n");
        else{
            printf("Goodbye %s\n", temporaryUsername);
            deleteNode(loginList, temporaryUsername);
        }
    }
}

void searchAccount(LIST *userList){
    char temporaryUsername[20];
    LISTNODE* tempNode;

    printf("Username: ");
    scanf("%s", temporaryUsername);
    tempNode = searchList(userList, temporaryUsername);
    if(tempNode == NULL) printf("Cannot find account\n");
    else if(tempNode->inf.status == 0) printf("Account is blocked\n");
    else printf("Account is active\n");
}

int readAccountFile(LIST *userList, char *file){
    FILE *fptr;
    if((fptr=fopen(file,"r"))==NULL){
        printf("File %s is not found!\n",file);
        return 0;
    }
    else{
        ListElementType temp;
        while(1){
            fscanf(fptr,"%s", temp.name);
            fscanf(fptr,"%s", temp.pass);
            fscanf(fptr,"%d", &temp.status);
            if(feof(fptr)) break;
            insertAfterCurrent(userList, temp);
        }
    }
    fclose(fptr);
    return 1;
}

void saveAccountFile(LIST *userList, char *file){
    FILE *fptr = fopen(file,"wb");
    if(userList->root == NULL) return;
    LISTNODE *p;
    for(p = userList->root; p != NULL; p = p->next){
        fprintf(fptr, "%s %s %d\n", p->inf.name, p->inf.pass, p->inf.status);
    }

    fclose(fptr);
}

int main(){
    int userChoice;

    LIST *userList = newList();
    LIST *loginList = newList();

    int loginAttempts = 0;
    int loginStatus = 0;

    if(readAccountFile(userList, "account.txt")){
        do{
            printMenu();
            scanf("%d", &userChoice);
            switch(userChoice){
                case 1:
                    insertAfterCurrent(userList, registerAcc(userList));
                    break;
                case 2:
                    loginStatus = userLogin(userList, loginList, &loginAttempts);
                    break;
                case 3:
                    if(loginList->root == NULL) loginStatus = 0;
                    if(loginStatus == 0) printf("Account is not signed in!\n");
                    else{
                        searchAccount(userList);
                    }
                    break;
                case 4:
                    if(loginList->root == NULL) loginStatus = 0;
                    if(loginStatus == 0) printf("Account is not signed in!\n");
                    else{
                        userLogout(userList, loginList);
                    }
                    break;
                default: break;
            }
        }while(userChoice == 1 || userChoice == 2 || userChoice == 3 || userChoice == 4);

        saveAccountFile(userList, "account.txt");
    }
    return 0;
}
