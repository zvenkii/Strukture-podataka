#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define errorMalloc -1
#define errorEmpty -2
#define errorOpening -3
#define errorInvalidExpression -4
#define errorZeroDivision -5

typedef struct _Element {
    double number;
    struct _Element *Next;
} Element;

Element *createElement(double number);
int push(Element *head, double number);
int pop(Element *head, double *result);
int freeStack(Element *head);
int calculatePostfix(Element *head, const char *fileName, double *finalResult);
int performOperation(double operand1, double operand2, char operation, double *result);

int main() {
    Element stack = { 0.0, NULL };
    double result = 0.0;

    int status = calculatePostfix(&stack, "postfiks.txt", &result);
    if (status == 0) {
        printf("Rezultat postfiks izraza: %.2f\n", result);
    } else {
        printf("Greska pri izracunavanju postfiks izraza (kod: %d).\n", status);
    }

    freeStack(&stack);

    return 0;
}

Element *createElement(double number) {
    Element *newElement = (Element *)malloc(sizeof(Element));
    if (newElement == NULL) {
        return NULL;
    }
    newElement->number = number;
    newElement->Next = NULL;

    return newElement;
}

int push(Element *head, double number) {
    Element *newElement = createElement(number);
    if (newElement == NULL) {
        return errorMalloc;
    }

    newElement->Next = head->Next;
    head->Next = newElement;

    return 0;
}

int pop(Element *head, double *result) {
    if (head->Next == NULL) {
        return errorEmpty;
    }

    Element *toDelete = head->Next;
    *result = toDelete->number;
    head->Next = toDelete->Next;
    free(toDelete);

    return 0;
}

int freeStack(Element *head) {
    Element *temp = NULL;
    while (head->Next != NULL) {
        temp = head->Next;
        head->Next = temp->Next;
        free(temp);
    }
    return 0;
}

int performOperation(double operand1, double operand2, char operation, double *result) {
    switch (operation) {
        case '+':
            *result = operand1 + operand2;
            break;
        case '-':
            *result = operand1 - operand2;
            break;
        case '*':
            *result = operand1 * operand2;
            break;
        case '/':
            if (operand2 == 0.0) {
                return errorZeroDivision;
            }
            *result = operand1 / operand2;
            break;
        default:
            return errorInvalidExpression;
    }
    return 0;
}

int calculatePostfix(Element *head, const char *fileName, double *finalResult) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        return errorOpening;
    }

    char buffer[MAX_LINE];
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fclose(fp);
        return errorEmpty;
    }
    fclose(fp);

    char *token = buffer;
    double number = 0.0;
    int numBytes = 0;

    while (*token != '\0' && *token != '\n' && *token != '\r') {
        if (sscanf(token, " %lf%n", &number, &numBytes) == 1) {
            if (push(head, number) != 0) {
                return errorMalloc;
            }
            token += numBytes;
        } else {
            char operation = ' ';
            if (sscanf(token, " %c%n", &operation, &numBytes) == 1) {
                double operand2 = 0.0;
                double operand1 = 0.0;
                double opResult = 0.0;

                if (pop(head, &operand2) != 0) {
                    return errorInvalidExpression;
                }
                if (pop(head, &operand1) != 0) {
                    return errorInvalidExpression;
                }

                if (performOperation(operand1, operand2, operation, &opResult) != 0) {
                    return errorInvalidExpression;
                }

                if (push(head, opResult) != 0) {
                    return errorMalloc;
                }
                token += numBytes;
            } else {
                token++;
            }
        }
    }

    if (pop(head, finalResult) != 0) {
        return errorInvalidExpression;
    }

    if (head->Next != NULL) {
        return errorInvalidExpression;
    }

    return 0;
}