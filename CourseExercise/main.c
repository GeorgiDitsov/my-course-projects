#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Names{
    char firstName[20];
    char surname[20];
    char lastName[20];
}Name;

typedef struct Students{
    Name studentName;
    char facNumber[10];
    char faculty[20];
    char program[20];
    int group;
    char course[10];
}Student;

typedef struct Node{
    Student student;
    struct Node *next;
}node;

node *head = NULL;
node *current = NULL;

void loadFromFile();
void saveInFile();
void addStudent();
void searchForStudentByLastName();
void deleteStudentByFacNumber();
void printStudentsAlphabeticallyByGroup();

int main()
{
    loadFromFile();
    int choice;
    while(1){
        printf("MENU:\n1.Add new student\n2.Search for student by last name\n3.Delete student by faculty number\n4.Print all students from group\n5.Exit program\n\n\nEnter your choice...");
        scanf("%d", &choice);
        fflush(stdin);
        switch(choice){
            case 1:
                addStudent();
                saveInFile();
                break;
            case 2:
                searchForStudentByLastName();
                break;
            case 3:
                deleteStudentByFacNumber();
                saveInFile();
                break;
            case 4:
                printStudentsAlphabeticallyByGroup();
                break;
            case 5:
                return 0;
                break;
            default:
                printf("Wrong choice entered!\n");
        }
    }
    return 0;
}

void loadFromFile(){
    FILE *fp = fopen("students.bin", "rb");
    if(!fp){
        printf("Error in reading file!");
        exit(1);
    }
    Student newStudent;
    while(1){
        if(fread(&newStudent, sizeof(Student), 1, fp) != 1){
            break;
        }
        node *newNode = (node*)malloc(sizeof(node));
        newNode->student = newStudent;
        if(head == NULL){
            head = newNode;
            newNode->next = NULL;
        }
        else{
            newNode->student = newStudent;
            newNode->next = head;
            head = newNode;
        }
    }
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
}

void saveInFile(){
    FILE *fp = fopen("students.bin", "wb");
    if(!fp){
        printf("Error in writing in file!");
        exit(1);
    }
    current = head;
    while(current!= NULL){
        if(fwrite(&current->student, sizeof(Student), 1, fp) != 1){
            printf("Error in writing in file!");
            exit(2);
        }
        current = current->next;
    }
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
}

void addStudent(){
    Student newStudent;

    printf("Enter student's first name:\n");
    fgets(newStudent.studentName.firstName, 20, stdin);
    printf("Enter students's surname:\n");
    fgets(newStudent.studentName.surname, 20, stdin);
    printf("Enter student's last name:\n");
    fgets(newStudent.studentName.lastName, 20, stdin);
    printf("Enter the faculty number of the student:\n");
    fgets(newStudent.facNumber, 10, stdin);
    fflush(stdin);
    printf("Enter the name of the faculty:\n");
    fgets(newStudent.faculty, 20, stdin);
    printf("Enter the student's program:\n");
    fgets(newStudent.program, 20, stdin);
    printf("Enter the student's group:\n");
    scanf("%d", &newStudent.group);
    fflush(stdin);
    printf("Enter the student's course:\n");
    fgets(newStudent.course, 10, stdin);

    newStudent.studentName.firstName[strlen(newStudent.studentName.firstName)-1] = '\0';
    newStudent.studentName.surname[strlen(newStudent.studentName.surname)-1] = '\0';
    newStudent.studentName.lastName[strlen(newStudent.studentName.lastName)-1] = '\0';
    newStudent.faculty[strlen(newStudent.faculty)-1] = '\0';
    newStudent.program[strlen(newStudent.program)-1] = '\0';
    newStudent.course[strlen(newStudent.course)-1] = '\0';

    node *newNode = (node*)malloc(sizeof(node));
    newNode->student = newStudent;

    if(head == NULL){
        head = newNode;
        newNode->next = NULL;
    }
    else{
        newNode->next = head;
        head = newNode;
    }
}

void searchForStudentByLastName(){
    char enteredLastName[20];

    printf("Enter the last name of the student:\n");
    fgets(enteredLastName, 20, stdin);
    enteredLastName[strlen(enteredLastName)-1] = '\0';

    current = head;
    while(current != NULL){
        if(strcmp(current->student.studentName.lastName, enteredLastName) == 0){
            printf("Names: %s %s %s\nFaculty Number: %s\nFaculty: %s\nProgram: %s\nGroup: %d\nCourse: %s\n", current->student.studentName.firstName, current->student.studentName.surname, current->student.studentName.lastName, current->student.facNumber, current->student.faculty, current->student.program, current->student.group, current->student.course);
        }
            current = current->next;
    }
    free(current);
    current = NULL;
}

void deleteStudentByFacNumber(){
    char enteredFacultyNumber[10];
    node *temp = NULL;

    printf("Enter the faculty number of the student:\n");
    fgets(enteredFacultyNumber, 10, stdin);
    //enteredFacultyNumber[strlen(enteredFacultyNumber)-1] = '\0';

    current = head;
    while(current != NULL){
        if(strcmp(current->next->student.facNumber, enteredFacultyNumber) == 0){
            temp = current->next;
            current->next = temp->next;
            free(temp);
        }
            current = current->next;
    }
}

void printStudentsAlphabeticallyByGroup(){
    int enteredGroup, count = 0;
    char firstName[30][20], surname[30][20], lastName[30][20], tempName[20];
    printf("Enter group:\n");
    scanf("%d", &enteredGroup);
    fflush(stdin);

    current = head;
    while(current != NULL){
        if(current->student.group == enteredGroup){
            strcpy(firstName[count], current->student.studentName.firstName);
            strcpy(surname[count], current->student.studentName.surname);
            strcpy(lastName[count], current->student.studentName.lastName);
            count++;
        }
        current = current->next;
    }
    for(int i = 0; i < count; i++){
        for(int j = i+1; j < count; j++){
            if(strcmp(firstName[i], firstName[j]) > 0){
                strcpy(tempName, firstName[i]);
                strcpy(firstName[i], firstName[j]);
                strcpy(firstName[j], tempName);
                strcpy(tempName, surname[i]);
                strcpy(surname[i], surname[j]);
                strcpy(surname[j], tempName);
                strcpy(tempName, lastName[i]);
                strcpy(lastName[i], lastName[j]);
                strcpy(lastName[j], tempName);
            }
            if(strcmp(firstName[i], firstName[j]) == 0){
                if(strcmp(lastName[i], lastName[j]) > 0){
                    strcpy(tempName, firstName[i]);
                    strcpy(firstName[i], firstName[j]);
                    strcpy(firstName[j], tempName);
                    strcpy(tempName, surname[i]);
                    strcpy(surname[i], surname[j]);
                    strcpy(surname[j], tempName);
                    strcpy(tempName, lastName[i]);
                    strcpy(lastName[i], lastName[j]);
                    strcpy(lastName[j], tempName);
                }
            }
        }
    }
    printf("Students in group %d:\n", enteredGroup);
    for(int k = 0; k < count; k++){
        printf("%s %s %s\n", firstName[k], surname[k], lastName[k]);
    }
}
