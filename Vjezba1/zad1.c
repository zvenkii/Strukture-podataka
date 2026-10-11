#include <stdio.h>
#include <stdlib.h>
#define maxSize 67
#define errorOpening -1
#define errorEmpty -2
#define errorMalloc -3
#define errorCode -4
typedef struct{
    char Name[maxSize];
    char Surname[maxSize];
    double Points;
}Student;
int countRows(const char *nameFile);
int main()
{
    const char *nameFile="studenti.txt";
    int counter=countRows(nameFile);
    if (counter<0)
    {
        return errorCode;
    }
    Student *s=(Student *)malloc(counter *sizeof(Student));
    if(s==NULL)
    {
        printf("Greska prio alokaciji\n");
        free(s);
        return errorMalloc;
    }
    FILE *fp=fopen(nameFile,"r");
    if(fp==NULL){
        printf("Greska pri otvaranju datoteke\n");
        return errorOpening;
    }
    double maxPoints=0.0;
    for (int i = 0; i < counter; i++) {
        fscanf(fp, "%s %s %lf", s[i].Name, s[i].Surname, &s[i].Points);
        if (s[i].Points > maxPoints) {
            maxPoints = s[i].Points;
        }
    }
    fclose(fp);
    printf("\n%s %s %s %s\n", "Ime", "Prezime", "Apsolutni", "Relativni (%)");
    for(int i=0;i<counter;i++)
    {
        double relativePoints=(s[i].Points/maxPoints)*100.0;
        printf("%s %s %.2f %.2f\n", s[i].Name, s[i].Surname, s[i].Points, relativePoints);
    }
    free(s);
    return 0;
}
int countRows(const char *nameFile)
{
    FILE *fp=fopen(nameFile,"r");
    if(fp==NULL){
        printf("Greska pri otvaranju datoteke\n");
        return errorOpening;
    }
    int counter=0;
    char tempName[maxSize],tempSurname[maxSize];
    double tempPoints;
    while(fscanf(fp,"%s %s %lf", tempName, tempSurname, &tempPoints)==3)
    {
        counter++;
    }
    if(counter==0)
    {
        printf("Datoteka je prazna\n");
        fclose(fp);
        return errorEmpty;
    }
    fclose(fp);
    return counter;
}