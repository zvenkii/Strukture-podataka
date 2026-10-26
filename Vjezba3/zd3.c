#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxSize 67

#define errorMalloc -1
#define errorEmpty -2
#define errorNotFound -3
#define errorOpening -4

typedef struct _Osoba {
    char Name[maxSize];
    char Surname[maxSize];
    int BirthYear;
    struct _Osoba *Next;
} Osoba;

Osoba *createPerson(const char *name, const char *surname, int birthYear);
int insertFirst(Osoba **head, const char *name, const char *surname, int birthYear);
int printList(Osoba *head);
int insertLast(Osoba **head, const char *name, const char *surname, int birthYear);
Osoba *findBySurname(Osoba *head, const char *surname);
int deletePerson(Osoba **head, const char *surname);
int freeList(Osoba **head);

int insertAfter(Osoba *head, const char *targetSurname, const char *name, const char *surname, int birthYear);
int insertBefore(Osoba **head, const char *targetSurname, const char *name, const char *surname, int birthYear);
int sortList(Osoba *head);
int writeToFile(Osoba *head, const char *fileName);
int readFromFile(Osoba **head, const char *fileName);

int main() {
    Osoba *head = NULL;

    insertLast(&head, "Marko", "Marulic", 1450);
    insertLast(&head, "Ana", "Horvat", 1998);
    insertLast(&head, "Luka", "Modric", 1985);

    insertAfter(head, "Horvat", "Ivan", "Mazuranic", 1814);
    insertBefore(&head, "Marulic", "Petar", "Zoranic", 1508);

    printList(head);

    sortList(head);
    printList(head);

    writeToFile(head, "osobe.txt");

    freeList(&head);

    readFromFile(&head, "osobe.txt");
    printList(head);

    freeList(&head);

    return 0;
}

Osoba *createPerson(const char *name, const char *surname, int birthYear) {
    Osoba *newPerson = (Osoba *)malloc(sizeof(Osoba));
    if (newPerson == NULL) {
        return NULL;
    }
    strcpy(newPerson->Name, name);
    strcpy(newPerson->Surname, surname);
    newPerson->BirthYear = birthYear;
    newPerson->Next = NULL;

    return newPerson;
}

int insertFirst(Osoba **head, const char *name, const char *surname, int birthYear) {
    Osoba *newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL) {
        return errorMalloc;
    }

    newPerson->Next = *head;
    *head = newPerson;

    return 0;
}

int printList(Osoba *head) {
    if (head == NULL) {
        return errorEmpty;
    }

    Osoba *temp = head;
    while (temp != NULL) {
        printf("%s %s %d\n", temp->Name, temp->Surname, temp->BirthYear);
        temp = temp->Next;
    }

    return 0;
}

int insertLast(Osoba **head, const char *name, const char *surname, int birthYear) {
    Osoba *newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL) {
        return errorMalloc;
    }

    if (*head == NULL) {
        *head = newPerson;
        return 0;
    }

    Osoba *temp = *head;
    while (temp->Next != NULL) {
        temp = temp->Next;
    }
    temp->Next = newPerson;

    return 0;
}

Osoba *findBySurname(Osoba *head, const char *surname) {
    Osoba *temp = head;

    while (temp != NULL) {
        if (strcmp(temp->Surname, surname) == 0) {
            return temp;
        }
        temp = temp->Next;
    }

    return NULL;
}

int deletePerson(Osoba **head, const char *surname) {
    if (*head == NULL) {
        return errorEmpty;
    }

    Osoba *temp = *head;
    Osoba *prev = NULL;

    if (strcmp(temp->Surname, surname) == 0) {
        *head = temp->Next;
        free(temp);
        return 0;
    }

    while (temp != NULL && strcmp(temp->Surname, surname) != 0) {
        prev = temp;
        temp = temp->Next;
    }

    if (temp == NULL) {
        return errorNotFound;
    }

    prev->Next = temp->Next;
    free(temp);

    return 0;
}

int freeList(Osoba **head) {
    Osoba *temp = NULL;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->Next;
        free(temp);
    }
    return 0;
}

int insertAfter(Osoba *head, const char *targetSurname, const char *name, const char *surname, int birthYear) {
    Osoba *target = findBySurname(head, targetSurname);
    if (target == NULL) {
        return errorNotFound;
    }

    Osoba *newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL) {
        return errorMalloc;
    }

    newPerson->Next = target->Next;
    target->Next = newPerson;

    return 0;
}

int insertBefore(Osoba **head, const char *targetSurname, const char *name, const char *surname, int birthYear) {
    if (*head == NULL) {
        return errorEmpty;
    }

    if (strcmp((*head)->Surname, targetSurname) == 0) {
        return insertFirst(head, name, surname, birthYear);
    }

    Osoba *temp = *head;
    while (temp->Next != NULL && strcmp(temp->Next->Surname, targetSurname) != 0) {
        temp = temp->Next;
    }

    if (temp->Next == NULL) {
        return errorNotFound;
    }

    Osoba *newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL) {
        return errorMalloc;
    }

    newPerson->Next = temp->Next;
    temp->Next = newPerson;

    return 0;
}

int sortList(Osoba *head) {
    if (head == NULL) {
        return errorEmpty;
    }

    int swapped = 0;
    Osoba *i = NULL;

    do {
        swapped = 0;
        i = head;

        while (i->Next != NULL) {
            if (strcmp(i->Surname, i->Next->Surname) > 0) {
                char tempName[maxSize];
                char tempSurname[maxSize];
                int tempYear;

                strcpy(tempName, i->Name);
                strcpy(tempSurname, i->Surname);
                tempYear = i->BirthYear;

                strcpy(i->Name, i->Next->Name);
                strcpy(i->Surname, i->Next->Surname);
                i->BirthYear = i->Next->BirthYear;

                strcpy(i->Next->Name, tempName);
                strcpy(i->Next->Surname, tempSurname);
                i->Next->BirthYear = tempYear;

                swapped = 1;
            }
            i = i->Next;
        }
    } while (swapped);

    return 0;
}

int writeToFile(Osoba *head, const char *fileName) {
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) {
        return errorOpening;
    }

    Osoba *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%s %s %d\n", temp->Name, temp->Surname, temp->BirthYear);
        temp = temp->Next;
    }

    fclose(fp);
    return 0;
}

int readFromFile(Osoba **head, const char *fileName) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        return errorOpening;
    }

    char name[maxSize];
    char surname[maxSize];
    int birthYear;

    while (fscanf(fp, "%s %s %d", name, surname, &birthYear) == 3) {
        insertLast(head, name, surname, birthYear);
    }

    fclose(fp);
    return 0;
}