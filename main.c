#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>



void meniu();
void optiunea1();
void optiunea2();
void optiunea3();

#define FILENAME "rezultate.txt"


typedef struct
{
    char nume[50];
    char prenume[50];
    int castig;
    int infrangeri;
    float winrate;

} Jucator;

int scorJucator(Jucator jucatori[], int *nr, const char *nume, const char *prenume)
{
    for (int i = 0; i < *nr; i++)
    {
        if (strcmp(jucatori[i].nume, nume) == 0 && strcmp(jucatori[i].prenume, prenume) == 0)
            return i;
    }

    strcpy(jucatori[*nr].nume, nume);
    strcpy(jucatori[*nr].prenume, prenume);
    jucatori[*nr].castig = 0;
    jucatori[*nr].infrangeri = 0;
    jucatori[*nr].winrate = 0;
    (*nr)++;
    return *nr - 1;
}

void meniu()
{
    system("cls");
    int optiune;

    do
    {
        printf("\n=== Meniu Interactiv ===\n");
        printf("1. Introducerea Rezultatelor \n");
        printf("2. Vizualizare clasament \n");
        printf("3. Vizualizare istoric jucator\n");
        printf("0. Iesire\n");
        printf("Alege o optiune: ");
        scanf("%d", &optiune);
        getchar();

        switch(optiune)
        {
        case 1:
            optiunea1();
            break;
        case 2:
            optiunea2();
            break;
        case 3:
            optiunea3();
            break;
        case 0:
            printf("Iesire din program.\n");
            break;
        default:
            system("cls");
            printf("Alegere invalida. Alege din nou.\n");
        }
    }
    while(optiune != 0);
}

void optiunea1()
{
    system("cls");

    FILE *f = fopen(FILENAME, "a");
    if (!f)
    {
        printf("Eroare la deschiderea fisierului pentru scriere.\n");
        return;
    }

    char nume1[50], prenume1[100], nume2[50], prenume2[100];
    int scor1, scor2;

    printf("\n=== Introduceti datele meciului ===\n");
    printf("Scrie 'stop' la nume pentru a termina.\n");

    while (1)
    {
        printf("\nNume jucator 1: ");
        scanf("%s",nume1);
        if (strcmp(nume1,"stop") == 0) break;

        printf("Prenume jucator 1: ");
        getchar();
        fgets(prenume1, sizeof(prenume1), stdin);
        prenume1[strcspn(prenume1,"\n")] = 0;


        while (1)
        {
            printf("Scor jucator 1 (0-21): ");
            if (scanf("%d",&scor1) == 1 && scor1>= 0 && scor1<= 21) break;
            printf("Scor invalid. Introdu un numar intre 0 si 21.\n");
            while (getchar() != '\n');
        }

        printf("Nume jucator 2: ");
        scanf("%s", nume2);

        printf("Prenume jucator 2: ");
        getchar();
        fgets(prenume2, sizeof(prenume2), stdin);
        prenume2[strcspn(prenume2, "\n")] = 0;


        while (1)
        {
            printf("Scor jucator 2 (0-21): ");
            if (scanf("%d", &scor2) == 1 && scor2 >= 0 && scor2 <= 21) break;
            printf("Scor invalid. Introdu un numar intre 0 si 21.\n");
            while (getchar() != '\n');
        }


        fprintf(f, "%s\n%s\n%d\n%s\n%s\n%d\n", nume1, prenume1,scor1,nume2,prenume2, scor2);
        printf("Meci salvat cu succes.\n");
    }

    fclose(f);

    printf("\n1. Revenire pagina anterioara\n");
    int optiune2;
    printf("Alege o optiune: ");
    scanf("%d", &optiune2);
    getchar();
    if (optiune2 == 1) meniu();
}




   void optiunea2()
{
    system("cls");
    FILE *f = fopen(FILENAME, "r");
    if (!f)
    {
        printf("Eroare la deschiderea fisierului.\n");
        return;
    }

    Jucator jucatori[222];
    int nrJucatori = 0;

    char linie[200];
    char nume1[50], prenume1[100], nume2[50], prenume2[100];
    int scor1, scor2;

    while (
        fgets(nume1, sizeof(nume1), f) &&
        fgets(prenume1, sizeof(prenume1), f) &&
        fgets(linie, sizeof(linie), f) &&
        sscanf(linie, "%d", &scor1) == 1 &&
        fgets(nume2, sizeof(nume2), f) &&
        fgets(prenume2, sizeof(prenume2), f) &&
        fgets(linie, sizeof(linie), f) &&
        sscanf(linie, "%d", &scor2) == 1
    )
    {

        nume1[strcspn(nume1, "\n")] = '\0';
        prenume1[strcspn(prenume1, "\n")] = '\0';
        nume2[strcspn(nume2, "\n")] = '\0';
        prenume2[strcspn(prenume2, "\n")] = '\0';

        int i1 = scorJucator(jucatori, &nrJucatori, nume1,prenume1);
        int i2 = scorJucator(jucatori, &nrJucatori, nume2,prenume2);

        if (scor1 > scor2)
        {
            jucatori[i1].castig++;
            jucatori[i2].infrangeri++;
        }
        else
        {
            jucatori[i2].castig++;
            jucatori[i1].infrangeri++;
        }
    }

    fclose(f);

    for (int i= 0; i< nrJucatori;i++)
    {
        int total= jucatori[i].castig + jucatori[i].infrangeri;
        jucatori[i].winrate=(100.0f * jucatori[i].castig) / total;
    }

    for (int i = 0; i < nrJucatori - 1; i++)
    {
        for (int j=i + 1; j < nrJucatori; j++)
        {
            if (jucatori[j].winrate>jucatori[i].winrate)
            {
                Jucator aux=jucatori[i];
                jucatori[i]=jucatori[j];
                jucatori[j]=aux;
            }
        }
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
for (int i = 0; i < nrJucatori; i++)
{
    SetConsoleTextAttribute(hConsole,11);
    printf("%d. ",i + 1);

    SetConsoleTextAttribute(hConsole, 10);
    printf("%s %s",jucatori[i].nume,jucatori[i].prenume);

    SetConsoleTextAttribute(hConsole, 14);
    printf(" - %dW ",jucatori[i].castig);

    SetConsoleTextAttribute(hConsole,12);
    printf("%dL",jucatori[i].infrangeri);

    SetConsoleTextAttribute(hConsole,9);
    printf(" - %.2f%% WR\n",jucatori[i].winrate);

    SetConsoleTextAttribute(hConsole,7);
}


    printf("\n1. Revenire la meniu\n");
    int opt;
    scanf("%d",&opt);
    getchar();
    if (opt==1) meniu();
}





void optiunea3()
{
    system("cls");
    char numeCautat[50], prenumeCautat[100];
    printf("\n=== Introdu numele si prenumele jucatorului (ex: Popescu Vlad): ===\n");
    scanf("%s", numeCautat);
    getchar();
    fgets(prenumeCautat, sizeof(prenumeCautat), stdin);
    prenumeCautat[strcspn(prenumeCautat, "\n")] = 0;

    FILE *f = fopen(FILENAME, "r");
    if (!f)
    {
        printf("Eroare la deschiderea fisierului.\n");
        return;
    }

    char nume1[50], prenume1[100], scorStr1[10];
    char nume2[50], prenume2[100], scorStr2[10];

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("\n=== Meciuri pentru jucatorul %s %s ===\n\n", numeCautat, prenumeCautat);

    while (
        fgets(nume1, sizeof(nume1), f)    &&
        fgets(prenume1, sizeof(prenume1),f) &&
        fgets(scorStr1, sizeof(scorStr1), f) &&
        fgets(nume2, sizeof(nume2), f)&&
        fgets(prenume2, sizeof(prenume2), f)&&
        fgets(scorStr2, sizeof(scorStr2), f)
    )
    {

        nume1[strcspn(nume1, "\n")] =0;
        prenume1[strcspn(prenume1, "\n")] =0;
        nume2[strcspn(nume2, "\n")] = 0;
        prenume2[strcspn(prenume2, "\n")] = 0;

        int scor1 = atoi(scorStr1);
        int scor2 = atoi(scorStr2);

        int match1 =strcmp(nume1,numeCautat)== 0&&strcmp(prenume1,prenumeCautat)== 0;
        int match2 =strcmp(nume2,numeCautat)==0 && strcmp(prenume2, prenumeCautat)== 0;

        if (match1||match2)
        {
            SetConsoleTextAttribute(hConsole,10);
            printf("%s %s",nume1, prenume1);

            SetConsoleTextAttribute(hConsole,7);
            printf(" vs ");

            SetConsoleTextAttribute(hConsole,11);
            printf("%s %s", nume2, prenume2);

            SetConsoleTextAttribute(hConsole, 6);
            printf("(%d - %d)\n", scor1, scor2);

            SetConsoleTextAttribute(hConsole, 7);
        }
    }

    fclose(f);

    printf("\n1.Revenire pagina anterioara\n");
    int opt;
    printf("Alege o optiune:");
    scanf("%d",&opt);
    getchar();
    if (opt==1) meniu();
}

int main()
{
    meniu();
    return 0;
}
