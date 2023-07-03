#include <stdio.h>
#include <stdlib.h> // memory allocation functions
#include <time.h>
#include <errno.h> //error handling 
#include <fcntl.h> // file control
#include <string.h> 
#include <ctype.h> // character handling
#include <unistd.h> // file descriptor operations
#include <stdint.h> // fixed-width integer types
#define BUF_SIZE 1024 // size of the plaintext buffer 
#define DEVICE_PATH "/dev/my_device"

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
    sprintf(sentence, "It is %s on %s now.", dt.time, dt.date);
}

// Secure Hashing Algorithm MD5, 512 (SHA-512), SHA-384, SHA-256 or SHA-1
// hash types
typedef enum hash {
    MD5 = 0,
    SHA512,
    SHA384,
    SHA256,
    SHA1
} hash_t;

// structure to hold sentence from user space and the hashing algorithm chosen by user
typedef struct usespace {
    uint8_t plaintext[BUF_SIZE]; // array of unsigned 8-bit integer with the pre-defined buffer size
    hash_t algo;
} userspace_t;

int main(void)
{
    // dev: file descriptor of the device file
    // ret: return value
    // digest: array of characters to store computed hash digest
    int dev, ret;
    char digest[BUF_SIZE];

    // open the character device
    dev = open(DEVICE_PATH, O_RDWR); // open device in read write mode
    if (dev < 0){
        perror("Failed to open device"); // error handling
        return errno;
    }

    char sentence[100];
    dt dt = generate_datetime();
    format_sentence(sentence, dt); // sentence for datetime

    // get user input for hashing algorithm
    int choice;
    printf("Select a Hashing Algorithm (1-5) from below:\n1. MD5\n2. SHA-512\n3. SHA-382\n4. SHA-256\n5. SHA-1\n\nChoice: ", stdout);
    scanf("%d", &choice);
    if (0 < choice || choice > 5 ) {
        puts("Please select an option from 1 to 5.");
        return -1;
    }

    userspace_t us; 
    strncpy((char*)us.plaintext, sentence, BUF_SIZE); // convert characters of the sentence to bytes and store in user.plaintext array to access data byte by byte if needed 
    // provide hashing algo based on user input
    if (choice == 1){
        us.algo = MD5;
    } else if (choice == 2){
        us.algo = SHA512;
    } else if (choice == 3){
        us.algo = SHA384;
    } else if (choice == 4){
        us.algo = SHA256;
    } else if (choice == 5){
        us.algo = SHA1;
    }

    // write to device
    ret = write(dev, &us, sizeof(userspace_t));
    if (ret < 0){
        perror("Error while sending data to kernel space");
        return errno;
    }

    // read from device
    ret = read(dev, digest, BUF_SIZE);
    if (ret < 0){
        perror("Error while reading data to kernel space");
        return errno;
    }

    // calculate number of bytes to print in hash digest based on the algo
    // MD5 hash digest = 128 bits = 16 bytes
    // SHA-512 hash digest = 512 bits = 64 bytes
    // SHA-384 hash digest = 384 bits = 48 bytes
    // SHA-256 hash digest = 256 bits = 32 bytes
    // SHA-1 hash digest = 160 bits = 20 bytes
    size_t bytes_to_print = (us.algo == MD5) ? 16 :
                            (us.algo == SHA512) ? 64 :
                            (us.algo == SHA384) ? 48 :
                            (us.algo == SHA256) ? 32 :
                            (us.algo == SHA1) ? 20 :
                            (puts("Algorithm not supported"), -1);
    
    // hashing algorithms to be implemented

    printf("Original Sentence: %s \nHashed Sentence: \"", us.plaintext);
    for (size_t i=0; i<bytes_to_print; i++)
        printf("%02x", (unsigned char)digest[i]);
    puts("\"");
    printf("\nHashing Algorithm: %s", us.algo);

    return 0;
}   