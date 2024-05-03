/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 * Altered by Sophia-15 <SnakeGame>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

struct snakeBody
{
    char *body;
    int x;
    int y;
    struct snakeBody *next;
};

int enemyX = 40, enemyY = 5;
int snakeX = 34, snakeY = 12;

int incX = 1, incY = 1;

int linkedLen(struct snakeBody **head)
{
    int cont = 0;
    struct snakeBody *n = *head;

    while (n != NULL)
    {
        cont++;
        n = n->next;
    }

    return cont;
}

void addNode(struct snakeBody **head)
{
    struct snakeBody *n = *head;

    if (*head == NULL)
    {
        struct snakeBody *first = (struct snakeBody *)malloc(sizeof(struct snakeBody));
        first->body = "o";
        first->next = NULL;
        first->x = snakeX;
        first->y = snakeY;
        *head = first;
    }
    else
    {
        while (n->next != NULL)
        {
            n = n->next;
        }

        n->next = (struct snakeBody *)malloc(sizeof(struct snakeBody));
        n->next->body = "o";
        n->next->x = snakeX;
        n->next->y = snakeY;
        n->next->next = NULL;
    }
}

void printSnake(int nextX, int nextY, int collision, struct snakeBody **head)
{
    struct snakeBody *n = *head;
    int previousX = n->x, previousY = n->y;
    int tempX, tempY;
    if (collision)
    {
        addNode(head);
    }
    screenSetColor(CYAN, DARKGRAY);

    while (n != NULL)
    {
        if (n == *head)
        {
            n->x = snakeX;
            n->y = snakeY;
            screenGotoxy(n->x, n->y);
            printf(" ");
            n->x = nextX;
            n->y = nextY;
            screenGotoxy(n->x, n->y);
        }
        else
        {
            tempX = n->x;
            tempY = n->y;

            screenGotoxy(n->x, n->y);
            printf(" ");

            n->x = previousX;
            n->y = previousY;

            screenGotoxy(n->x, n->y);

            previousX = tempX;
            previousY = tempY;
        }
        printf("o");
        n = n->next;
    }

    snakeX = nextX;
    snakeY = nextY;
}

void printEnemy()
{
    srand(time(NULL));
    int randX = (rand() % 70) + 2, randY = (rand() % 2) + 3;
    screenSetColor(CYAN, DARKGRAY);
    enemyX = randX;
    enemyY = randY;
    screenGotoxy(enemyX, enemyY);
    printf("🍒");
}

void printKey(int ch)
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");

    screenGotoxy(34, 23);
    printf("            ");

    if (ch == 27)
        screenGotoxy(36, 23);
    else
        screenGotoxy(39, 23);

    printf("%d ", ch);
    while (keyhit())
    {
        printf("%d ", readch());
    }
}

int main()
{
    static int ch = 0;
    struct snakeBody *head = NULL;
    addNode(&head);

    screenInit(1);
    keyboardInit();
    timerInit(100);

    printSnake(snakeX, snakeY, 0, &head);
    printEnemy(enemyX, enemyY);
    screenUpdate();

    // even if the time doesn't exceed i want ch to be on the screen
    while (ch != 10) // enter
    {
        // Handle user input
        if (keyhit())
        {
            ch = readch();
            printKey(ch);
            screenUpdate();
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            // dealing with movement
            int newX, newY;
            if (ch == 97)
            {
                newX = snakeX - incX;

                if (!(newX <= MINX + 1))
                {
                    incX = 1;
                }
                if (newX <= MINX + 1)
                {

                    incX = 0;
                }

                printSnake(newX, snakeY, 0, &head);
            }

            if (ch == 100)
            {
                newX = snakeX + incX;

                if (newX <= (MAXX - strlen("o") - 1))
                {

                    incX = 1;
                }
                if (newX >= (MAXX - strlen("o") - 1))
                {

                    incX = 0;
                }

                printSnake(newX, snakeY, 0, &head);
            }

            if (ch == 115)
            {
                newY = snakeY + incY;

                if (newY >= MAXY - 3)
                {
                    incY = 0;
                }

                if (newY <= MAXY - 3)
                {
                    incY = 1;
                }

                printSnake(snakeX, newY, 0, &head);
            }

            if (ch == 119)
            {
                newY = snakeY - incY;
                if (newY <= MINY + 3)
                {
                    incY = 0;
                }

                if (newY >= MINY + 3)
                {
                    incY = 1;
                }

                printSnake(snakeX, newY, 0, &head);
            }

            // dealing with collision
            if (newY == enemyY && (newX == enemyX || enemyX == (newX - 1)))
            {
                printEnemy();
                printSnake(snakeX, snakeY, 1, &head);
            }

            // Updating screen
            printKey(ch);
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}