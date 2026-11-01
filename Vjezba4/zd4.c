#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define errorMalloc -1
#define errorEmpty -2
#define errorOpening -3

typedef struct _Element
{
    int coefficient;
    int exponent;
    struct _Element *Next;
} Element;

Element *createElement(int coeff, int exp);
int insertSorted(Element *head, int coeff, int exp);
int printPoly(Element *head);
int freeList(Element *head);
int readFromFile(Element *p1, Element *p2, const char *fileName);
int addPolynoms(Element *p1, Element *p2, Element *result);
int multiplyPolynoms(Element *p1, Element *p2, Element *result);

int main()
{
    Element poly1 = {0, 0, NULL};
    Element poly2 = {0, 0, NULL};
    Element polySum = {0, 0, NULL};
    Element polyMul = {0, 0, NULL};

    readFromFile(&poly1, &poly2, "polinomi.txt");

    printf("Polinom 1: ");
    printPoly(&poly1);

    printf("Polinom 2: ");
    printPoly(&poly2);

    addPolynoms(&poly1, &poly2, &polySum);
    printf("Zbroj:     ");
    printPoly(&polySum);

    multiplyPolynoms(&poly1, &poly2, &polyMul);
    printf("Umnozak:   ");
    printPoly(&polyMul);

    freeList(&poly1);
    freeList(&poly2);
    freeList(&polySum);
    freeList(&polyMul);

    return 0;
}

Element *createElement(int coeff, int exp)
{
    Element *newElement = (Element *)malloc(sizeof(Element));
    if (newElement == NULL)
    {
        return NULL;
    }
    newElement->coefficient = coeff;
    newElement->exponent = exp;
    newElement->Next = NULL;

    return newElement;
}

int insertSorted(Element *head, int coeff, int exp)
{
    if (coeff == 0)
    {
        return 0;
    }

    Element *temp = head;

    while (temp->Next != NULL && temp->Next->exponent > exp)
    {
        temp = temp->Next;
    }

    if (temp->Next != NULL && temp->Next->exponent == exp)
    {
        temp->Next->coefficient += coeff;
        if (temp->Next->coefficient == 0)
        {
            Element *toDelete = temp->Next;
            temp->Next = toDelete->Next;
            free(toDelete);
        }
    }
    else
    {
        Element *newElement = createElement(coeff, exp);
        if (newElement == NULL)
        {
            return errorMalloc;
        }
        newElement->Next = temp->Next;
        temp->Next = newElement;
    }

    return 0;
}

int printPoly(Element *head)
{
    if (head->Next == NULL)
    {
        printf("0\n");
        return errorEmpty;
    }

    Element *temp = head->Next;
    while (temp != NULL)
    {
        printf("%dx^%d ", temp->coefficient, temp->exponent);
        if (temp->Next != NULL && temp->Next->coefficient > 0)
        {
            printf("+ ");
        }
        temp = temp->Next;
    }
    printf("\n");

    return 0;
}

int freeList(Element *head)
{
    Element *temp = NULL;
    while (head->Next != NULL)
    {
        temp = head->Next;
        head->Next = temp->Next;
        free(temp);
    }
    return 0;
}

int readFromFile(Element *p1, Element *p2, const char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return errorOpening;
    }

    char line[MAX_LINE];

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        char *buffer = line;
        int coeff = 0, exp = 0, numBytes = 0;

        while (sscanf(buffer, "%d %d%n", &coeff, &exp, &numBytes) == 2)
        {
            insertSorted(p1, coeff, exp);
            buffer += numBytes;
        }
    }

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        char *buffer = line;
        int coeff = 0, exp = 0, numBytes = 0;

        while (sscanf(buffer, "%d %d%n", &coeff, &exp, &numBytes) == 2)
        {
            insertSorted(p2, coeff, exp);
            buffer += numBytes;
        }
    }

    fclose(fp);
    return 0;
}

int addPolynoms(Element *p1, Element *p2, Element *result)
{
    Element *i = p1->Next;
    Element *j = p2->Next;

    while (i != NULL && j != NULL)
    {
        if (i->exponent == j->exponent)
        {
            insertSorted(result, i->coefficient + j->coefficient, i->exponent);
            i = i->Next;
            j = j->Next;
        }
        else if (i->exponent > j->exponent)
        {
            insertSorted(result, i->coefficient, i->exponent);
            i = i->Next;
        }
        else
        {
            insertSorted(result, j->coefficient, j->exponent);
            j = j->Next;
        }
    }

    while (i != NULL)
    {
        insertSorted(result, i->coefficient, i->exponent);
        i = i->Next;
    }

    while (j != NULL)
    {
        insertSorted(result, j->coefficient, j->exponent);
        j = j->Next;
    }

    return 0;
}

int multiplyPolynoms(Element *p1, Element *p2, Element *result)
{
    for (Element *i = p1->Next; i != NULL; i = i->Next)
    {
        for (Element *j = p2->Next; j != NULL; j = j->Next)
        {
            int newCoeff = i->coefficient * j->coefficient;
            int newExp = i->exponent + j->exponent;
            insertSorted(result, newCoeff, newExp);
        }
    }

    return 0;
}