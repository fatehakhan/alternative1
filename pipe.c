#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>

// Erzeugung von Prozessen
//int running = 1; // Speichert den Status einer Endlosschleife
// Signal-Handler-Funktion für "SIGINT", die die Endlosschleife beendet:
/*void sigint_handler(int sig)
{
    printf("Signal SIGINT erhalten. Beende das Programm.\n");
    running = 0;
}*/
int main(int argc, char *argv[])
{
// Registrieren des Signal-Handlers für SIGINT
   /* if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        printf("Fehler beim Registrieren des Signal-Handlers für SIGINT.\n");
        return 1;
    }
while (running)
    {
*/
    // Anlegen der Pipe, fd=file descriptor
    int fd1[2]; // Erste Pipe

    if (pipe(fd1) == -1)
    {
        printf("Fehler beim Erstellen der ersten Pipe\n");
        return 1;
    }
    int fd2[2]; // Zweite Pipe
    if (pipe(fd2) == -1)
    {
        printf("Fehler beim Erstellen der zweiten Pipe\n");
        return 2;
    }
    int fd3[2]; // Dritte Pipe
    if (pipe(fd3) == -1)
    {
        printf("Fehler beim Erstellen der dritten Pipe\n");
        return 3;
    }
    
    // Prozesse erzeugen
    int childproc_conv = fork();

    // Hier ist der Kindprozess conv! 
    if (childproc_conv == -1)
    {
        printf("Fehler beim Fork");
        return 4;
    }
    if (childproc_conv == 0)
    {
        printf("Conv: Prozess erzeugt Zufallszahlen \n");
        printf("Conv: PID: %i\n", getpid());

        // 10 Zufallszahlen zw. 0 und 99
        int i;
        srand(time(NULL));
        printf("Zufallszahlen: ");
        int zufallszahlen[10];
        for (i = 0; i < 10; i++)
        {
            printf("%d ", zufallszahlen[i] = rand() % 100);
        }
        printf("\n");

        if (write(fd1[1], zufallszahlen, sizeof(int) * 10) == -1)
        {
            printf("Fehler beim Schreiben erster Pipe");
            return 5;
        }
        if (write(fd2[1], zufallszahlen, sizeof(int) * 10) == -1)
        {
            printf("Fehler beim Schreiben zweiter Pipe");
            return 6;
        }

        close(fd1[1]);
        close(fd2[1]);

        int childproc_stat = fork();
        // Hier ist der Kindprozess stat! 
        if (childproc_stat == -1)
        {
            printf("Fehler beim fork");
            return 7;
        }
        if (childproc_stat == 0)
        {
            printf("Stat: liest Messwerte von Conv aus und berechnet statistische Daten\n");
            printf("Stat: PID: %i\n", getpid());
            // Summe der Zufallszahlen
            int i, zufallszahlen[i];
            float summe = 0, mittelwert;
            close(fd1[1]);
            close(fd2[1]);

            if (read(fd2[0], zufallszahlen, sizeof(int) * 10) == -1) 
            {
                printf("1.Fehler beim Lesen zweiter Pipe");
                return 8;
            }
            close(fd2[0]);

            for (i = 0; i < 10; i++)
            {
                summe += zufallszahlen[i];
            }
            // Mittelwert der Zufallszahlen
            mittelwert = summe / 10;

            close(fd2[0]);                                  
            if (write(fd3[1], &summe, sizeof(float)) == -1) 
            {
                printf("Fehler beim Schreiben dritter Pipe\n");
                return 9;
            }

            if (write(fd3[1], &mittelwert, sizeof(float)) == -1)
            {
                printf("Fehler beim Schreiben dritter Pipe\n");
                return 10;
            }
            close(fd3[1]);

            int childproc_log = fork();
            // Hier ist der Kindprozess log!
            if (childproc_log == -1)
            {
                printf("Fehler beim Fork");
                return 11;
            }
            if (childproc_log == 0)
            {
                close(fd3[1]);
                printf("Log: liest Messwerte aus conv und schreibt sie in einer lokalen Datei\n");
                printf("Log: PID: %i\n", getpid());
                int i, zufallszahlen[i];

                if (read(fd1[0], zufallszahlen, sizeof(int) * 10) == -1)
                {
                    printf("2. Fehler beim Lesen zweiter Pipe\n");
                    return 12;
                }
                close(fd1[0]);
                // Ergebnisse in einer lokalen Datei einschreiben
                FILE *lokal_file;
                lokal_file = fopen("log", "w");
                if (lokal_file == 0)
                { // Prüft, ob Datei erfolgreich geöffnet wurde
                    printf("Fehler beim Öffnen der Datei\n");
                    exit(1);
                }
                for (i = 0; i < 10; i++)
                {
                    fprintf(lokal_file, " %d\n", zufallszahlen[i]); // Schreibt in Datei ein
                }
                fclose(lokal_file); // Datei schließen

            // Hier ist der Kindprozess report!
            int childproc_report = fork();
            if (childproc_report == -1)
            {
                printf("Fehler beim Fork");
                return 13;
            }
            if (childproc_report == 0)
            {
                close(fd1[0]);
                printf("Report: Prozess greift auf Ergebnisse von Stat zu\n");
                printf("Report: PID: %i\n", getpid());

                if (read(fd3[0], &summe, sizeof(float)) == -1)
                {
                    printf("Fehler beim Lesen dritter Pipe\n");
                    return 14;
                }
                if (read(fd3[0], &mittelwert, sizeof(float)) == -1)
                {
                    printf("Fehler beim Lesen dritter Pipe\n");
                    return 15;
                }
                close(fd3[0]);

                // Ausgabe Shell:
                printf("\n");
                printf("Summe der Zufallszahlen: %.2f\n", summe);
                printf("Mittelwert der Zufallszahlen: %.2f\n", mittelwert);
            }
            }

           // printf("\n");
            return 0;
        }
       
    }

   // sleep(1);
  }
   // return 16;
//}
/* char c1;

        while (scanf("%c", c1) != 'c')
        {

            printf("%d ", zufallszahlen[i] = rand() % 100);
        }
        printf("\n");*/