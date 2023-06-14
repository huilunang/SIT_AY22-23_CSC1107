#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// func prototype to be moved in header file
typedef struct DateTime dt; // give struct alias dt
static struct DateTime generate_datetime();
void format_sentence(char sentence[], dt dt);

typedef struct DateTime
{
    char date[40], time[40];
} dt;

// get date and time
static dt generate_datetime()
{
    dt dt;
    // vars to store datetime components
    int hours, minutes, seconds, day, year;
    char *month;

    // variable to store current time
    time_t now;
    // & means gets the address of where now variable exists
    // return time will be stored in that address of now
    time(&now);

    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    // // hours in 24-hr format
    // hours = local->tm_hour;
    // // minutes 0 - 59
    // minutes = local->tm_min;
    // // seconds 0 - 59
    // seconds = local->tm_sec;
    // // day 1 - 31
    // day = local->tm_mday;
    // // month 0 - 11
    // strftime(month, 20, "\%B", local);
    // // year since 1900
    // year = local->tm_year + 1900;

    // // set datetime
    // sprintf(dt.date, "%dth %s %d", day, month, year);
    // sprintf(dt.time, "%d:%d:%d", hours, minutes, seconds);

    strftime(dt.date, 40, "%dth %B %Y", local);
    strftime(dt.time, 40, "%H:%M:%S", local);

    return dt;
}

void format_sentence(char *sentence, dt dt)
{
    sprintf(sentence, "It is %s on %s now.\n", dt.time, dt.date);
}

// Secure Hashing Algorithm 512 (SHA-512), SHA-384, SHA-256, SHA-1, or MD5
//
//

int main()
{
    char sentence[100];
    dt dt = generate_datetime();
    format_sentence(sentence, dt);

    printf("%s", sentence);
}
