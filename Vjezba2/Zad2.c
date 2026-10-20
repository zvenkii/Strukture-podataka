#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxSize 69

#define errorMalloc -1
#define errorEmpty -2
#define errorNotFound -3

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

int main() {
    Osoba *head = NULL;

    insertFirst(&head, "Marko", "Livaja", 1992);
    insertFirst(&head, "Rokas", "Pukstas", 1998);

    insertLast(&head, "Ivan", "Mazuranic", 1823);
    insertLast(&head, "Luka", "Modric", 1984);

    printList(head);

    Osoba *found = findBySurname(head, "Mazuranic");
    if (found != NULL) {
        printf("%s %s %d\n", found->Name, found->Surname, found->BirthYear);
    } else {
        printf("Nije pronadjeno\n");
    }

    deletePerson(&head, "Horvat");

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