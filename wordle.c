#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define casuta_y 3
#define casuta_x 4

void titlu(int xmax)
{
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    attron(A_STANDOUT | A_BOLD | COLOR_PAIR(1));
    mvprintw(2, xmax / 2 - 9 / 2, " WORDLE ");
    attroff(A_STANDOUT | A_BOLD | COLOR_PAIR(1));
    refresh();
}

void chenare(int xmax)
{ // construiesc chenarele(casutele)
    int lungime = 5, lungime_x, lungime_add, cnt_coloana, cnt_linie = 6, lungime_y = 6;
    lungime_x = xmax / 2 - 3 * lungime;
    lungime_add = lungime + 1;
    cnt_coloana = lungime;
    WINDOW *casuta;
    move(lungime_y + 1, lungime_x + 1);
    refresh();
    // creez tabla de joc
    while (cnt_linie)
    {
        while (cnt_coloana)
        {
            // creez o casuta
            casuta = newwin(casuta_y, casuta_x, lungime_y, lungime_x);
            keypad(casuta, TRUE);
            box(casuta, 0, 0);
            wrefresh(casuta);
            refresh();
            lungime_x = lungime_x + lungime_add;
            cnt_coloana--;
        }
        cnt_coloana = lungime;
        lungime_x = xmax / 2 - 3 * lungime;
        cnt_linie--;
        lungime_y += 3;
    }
}

int main()
{
    initscr(); /* Incepe modul curses */
    cbreak();
    refresh();
    start_color();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(0, COLOR_BLACK, COLOR_WHITE);
    bkgd(COLOR_PAIR(1));

    // setez limitele
    int xmax = 100, ymax = 60;
    getmaxyx(stdscr, ymax, xmax);
    refresh();

    // Titlu
    titlu(xmax);

// in cazul unui nou joc
label_1:
    move(3, 0);
    clrtobot();

    int lungime = 5, lungime_x, lungime_add, cnt_coloana, cnt_linie = 6, lungime_y = 6;
    // construiesc chenarele(casutele)
    chenare(xmax);

    // incep citirea caracterelor
    lungime_x = xmax / 2 - 3 * lungime;
    lungime_add = lungime + 1;
    cnt_coloana = lungime;
    cnt_linie = 6;
    lungime_y = 6;
    move(lungime_y + 1, lungime_x + 1);
    int i;
    char cuv[7], raspuns[7];
    cuv[0] = '\0';

    // selectez un numar random
    char *p, dictionar[200];
    dictionar[0] = '\0';
    strcpy(dictionar, "arici, atent, baiat, ceata, debut, peste, fixat, hamac,harta, jalon, jucam, lacat, magie, nufar, oaste, perus,rigle, roman, sanie, scris, sonda, texte, tipar, titan, zebra, vapor, vatra");
    // selectez un numar random intre 0-26
    srand(time(0));
    i = rand() % 27;
    p = strtok(dictionar, ", ");
    while (p && i)
    {
        strcpy(raspuns, p);
        p = strtok(0, ", ");
        i--;
    }
    for (i = 0; i < strlen(raspuns); i++)
    {
        raspuns[i] = raspuns[i] - 32;
    }
    i = 0;
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);

    while (cnt_linie)
    {
        while (1)
        {
            // daca cuvantul este terminat
            if (i == 5)
            { // trece la urmatorul cuvant doar la tasta ENTER
                while (cuv[i] != '\n')
                {
                    cuv[i] = getch();
                    if (cuv[i] == 7)
                    {
                        lungime_x = lungime_x - lungime_add;
                        move(lungime_y + 1, lungime_x + 1);
                        printw(" ");
                        refresh();
                        cuv[i] = ' ';
                        i--;
                        break;
                    }
                }
                if (cuv[i] == '\n')
                    break;
            }

            // mut cursorul in casuta
            if (cuv[i] != 7)
                move(lungime_y + 1, lungime_x + 1);
            refresh();
            cuv[i] = getch();

            // sterg mesajul de la enter
            mvprintw(15, 0, "                                   ");
            move(lungime_y + 1, lungime_x + 1);
            refresh();

            if (cuv[i] >= 'a' && cuv[i] <= 'z')
                cuv[i] -= 32;

            if (cuv[i] >= 'A' && cuv[i] <= 'Z')
            { // afisez caracterul introdus
                move(lungime_y + 1, lungime_x + 1);
                printw("%c", cuv[i]);
                refresh();
                lungime_x = lungime_x + lungime_add;
                i++;
            }
            else
            { // implementez backspace
                if (cuv[i] == 7 && lungime_x > xmax / 2 - 3 * lungime)
                {
                    lungime_x = lungime_x - lungime_add;
                    move(lungime_y + 1, lungime_x + 1);
                    printw(" ");
                    refresh();
                    cuv[i] = ' ';
                    i--;
                }
                else
                {
                    if (cuv[i] == '\n')
                    {
                        mvprintw(15, 0, "Cuvantul trebuie sa aiba 5 litere !");
                        refresh();
                    }
                }
            }
        }

        // verific daca este raspunsul corect
        lungime_x = xmax / 2 - 3 * lungime;
        if (strncmp(cuv, raspuns, strlen(raspuns)) == 0)
        {
            break;
        }
        for (i = 0; i < strlen(raspuns); i++)
        {
            if (cuv[i] == raspuns[i])
            {
                attron(A_STANDOUT | COLOR_PAIR(2));
                mvprintw(lungime_y + 1, lungime_x + i * lungime_add + 1, "%c", cuv[i]);
                refresh();
                attroff(A_STANDOUT | COLOR_PAIR(2));
            }
            else
            {
                if (strchr(raspuns, cuv[i]) != 0)
                {
                    attron(A_STANDOUT | COLOR_PAIR(3));
                    mvprintw(lungime_y + 1, lungime_x + i * lungime_add + 1, "%c", cuv[i]);
                    refresh();
                    attroff(A_STANDOUT | COLOR_PAIR(3));
                }
                else
                {
                    attron(A_STANDOUT | COLOR_PAIR(4));
                    mvprintw(lungime_y + 1, lungime_x + i * lungime_add + 1, "%c", cuv[i]);
                    refresh();
                    attroff(A_STANDOUT | COLOR_PAIR(4));
                }
            }
        }

        i = 0;
        cuv[5] = '\0';
        lungime_x = xmax / 2 - 3 * lungime;
        cnt_linie--;
        lungime_y += 3;
    }

    // rezultatul jocului
    WINDOW *concluzie;
    init_pair(5, COLOR_RED, COLOR_BLACK);

    if (strncmp(cuv, raspuns, strlen(raspuns) - 1) == 0)
    { // daca jocul este castigat
        // cuvantul devine verde
        for (i = 0; i < strlen(raspuns); i++)
        {
            attron(A_STANDOUT | COLOR_PAIR(2));
            mvprintw(lungime_y + 1, lungime_x + i * lungime_add + 1, "%c", cuv[i]);
            refresh();
            attroff(A_STANDOUT | COLOR_PAIR(2));
        }
        // mesajul de final
        concluzie = newwin(20, 30, 6, xmax / 2 - 15);
        box(concluzie, 0, 0);
        wattron(concluzie, A_STANDOUT | COLOR_PAIR(1));
        mvwprintw(concluzie, 4, 9, "FELICITARI !");
        mvwprintw(concluzie, 6, 6, "PENTRU UN NOU JOC");
        mvwprintw(concluzie, 7, 5, "APASATI TASTA ENTER");
        wrefresh(concluzie);
        wattroff(concluzie, A_STANDOUT | COLOR_PAIR(1));
        if (getch() == '\n')
        {
            goto label_1;
        }
        wrefresh(concluzie);
    }
    else
    { // daca jocul este pierdut
        // mesajul de final
        concluzie = newwin(20, 30, 6, xmax / 2 - 15);
        box(concluzie, 0, 0);
        wattron(concluzie, A_STANDOUT | COLOR_PAIR(5));
        mvwprintw(concluzie, 4, 9, "JOC PIERDUT");
        mvwprintw(concluzie, 6, 7, "RASPUNS: %s", raspuns);
        mvwprintw(concluzie, 8, 6, "PENTRU UN NOU JOC");
        mvwprintw(concluzie, 9, 5, "APASATI TASTA ENTER");
        wrefresh(concluzie);
        wattroff(concluzie, A_STANDOUT | COLOR_PAIR(5));
        if (getch() == '\n')
        {
            goto label_1;
        }
        wrefresh(concluzie);
    }

    endwin(); /* Sfarsitul modului curses */

    return 0;
}
//  gcc -o wordle wordle.c -lncurses
//  ./wordle