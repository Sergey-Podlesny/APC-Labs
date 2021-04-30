#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <windows.h>

unsigned int delayTime = 0;

char date[6];

unsigned int registers[] = { 0x00, 0x02, 0x04, 0x07, 0x08, 0x09 };

void interrupt(*oldAlarm) (...);

void getTime();
void setTime();
void setAlarm();
void resetAlarm();
void inputTime();
int bcdToDec(int);
int decToBcd(int);


int bcdToDec(int bcd)
{
    return ((bcd / 16 * 10) + (bcd % 16));
}

int decToBcd(int dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}

void interrupt newAlarm(...)
{
    puts("Alarm");
    oldAlarm();
    resetAlarm();
}

int main()
{
    int delayMillisecond;
    while (1) {
        printf("1 - Current time\n");
        printf("2 - Set time\n");
        printf("3 - Set alarm\n");
        printf("0 - Exit\n");

        switch (getch()) {
        case '1':
            getTime();
            break;

        case '2':
            setTime();
            break;

        case '3':
            setAlarm();
            break;
        case '0':
            return 0;
        
        default:
            system("cls");
            break;
        }
    }
}

void getTime()
{
    char* monthToText[] =
    {
        "January", "Februart", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
    };

    system("cls");

    int i = 0;
    for (i = 0; i < 6; i++)
    {
        outp(0x70, registers[i]);
        date[i] = inp(0x71);
    }

    int decDate[6];
    for (i = 0; i < 6; i++)
    {
        decDate[i] = bcdToDec(date[i]);
    }

    printf(" %2d:%2d:%2d", decDate[2], decDate[1], decDate[0]);
    printf(" %s, %2d, 20%2d\n", monthToText[decDate[4] - 1], decDate[3], decDate[5]);
}

void setTime()
{
    inputTime();

    disable();

    unsigned int res;
    do
    {
        outp(0x70, 0xA); 
        res = inp(0x71) & 0x80; // 0x80 == 1000 0000
    } while (res);              //проверка на доступность значений для чтения и записи

    outp(0x70, 0xB);
    outp(0x71, inp(0x71) | 0x80); // отключение обновления часов

    for (int i = 0; i < 4; i++)
    {
        outp(0x70, registers[i]);
        outp(0x71, date[i]); //подаем нужные значения
    }

    outp(0x70, 0xB); // включаем обновление часов
    outp(0x71, inp(0x71) & 0x7F); // 0111 1111

    enable();
    system("cls");
}

void setAlarm()
{
    inputTime();

    disable();

    unsigned int res;
    do
    {
        outp(0x70, 0xA);
        res = inp(0x71) & 0x80; // 0x80 == 1000 0000
    } while (res);              //проверка на доступность значений для чтения и записи

    //подаем нужные значения
    outp(0x70, 0x05);
    outp(0x71, date[2]);

    outp(0x70, 0x03);
    outp(0x71, date[1]);

    outp(0x70, 0x01);
    outp(0x71, date[0]);

    outp(0x70, 0xB); // включаем обновление часов
    outp(0x71, (inp(0x71) | 0x20)); // 0111 1111

    oldAlarm = getvect(0x4A);
    setvect(0x4A, newAlarm); // прерывание будильника
    outp(0xA1, (inp(0xA0) & 0xFE)); // размаскировка линий сигнала запроса от часов 0xFE = 11111110 0ой бит в 0 тк IRQ8

    enable();
    printf("Alarm enabled\n");
}

void resetAlarm()
{
    if (oldAlarm == NULL)
    {
        return;
    }

    disable();

    setvect(0x4A, oldAlarm);
    outp(0xA1, (inp(0xA0) | 0x01)); 

    unsigned int res;
    do
    {
        outp(0x70, 0xA);
        res = inp(0x71) & 0x80;
    } while (res);

    outp(0x70, 0x05);
    outp(0x71, 0x00);

    outp(0x70, 0x03);
    outp(0x71, 0x00);

    outp(0x70, 0x01);
    outp(0x71, 0x00);

    outp(0x70, 0xB);
    outp(0x71, (inp(0x71) & 0xDF));

    enable();
}

void inputTime()
{
    int n;

    do {
        fflush(stdin);
        printf("Input hours: ");
        scanf("%i", &n);
    } while ((n > 23 || n < 0));
    date[2] = decToBcd(n);

    do {
        fflush(stdin);
        printf("Input minutes: ");
        scanf("%i", &n);
    } while (n > 59 || n < 0);
    date[1] = decToBcd(n);

    do {
        fflush(stdin);
        printf("Input seconds: ");
        scanf("%i", &n);
    } while (n > 59 || n < 0);
    date[0] = decToBcd(n);
}

