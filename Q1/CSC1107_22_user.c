#include <stdio.h>
#include <string.h>
#include <stdlib.h>      // memory allocation functions
#include <time.h>        // datetime
#include <errno.h>       // error handling
#include <fcntl.h>       // file control
#include <ctype.h>       // character handling
#include <unistd.h>      // file descriptor operations
#include <stdint.h>      // fixed-width integer types
#include <openssl/evp.h> // hashing function

#define BUF_SIZE 1024    // size of the text buffer
#define DEVICE_PATH "/dev/CSC1107_22"

// function prototypes
typedef struct DateTime dt; // give struct alias dt
static struct DateTime generate_datetime();
void format_sentence(char *sentence, dt dt);

// supported has types
typedef enum hash
{
    MD5 = 0,
    SHA512,
    SHA384,
    SHA256,
    SHA1
} hash_t;

// store date and time
typedef struct DateTime
{
    char date[40], time[40];
} dt;

// store sentence, hashing algorithm and generated hash from user space
typedef struct userspace
{
    uint8_t plaintext[BUF_SIZE]; // array of unsigned 8-bit integer
    hash_t algo;
    uint8_t hashtext[BUF_SIZE];
} userspace_t;


// get date and time
static dt generate_datetime()
{
    dt dt;
    time_t now; // variable to store current time
    time(&now); // store time in address of now

    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);
    strftime(dt.date, 40, "%dth %B %Y", local);
    strftime(dt.time, 40, "%H:%M:%S", local);

    return dt;
}

// format sentence with date and time
void format_sentence(char *sentence, dt dt)
{
    sprintf(sentence, "It is %s on %s now.", dt.time, dt.date);
}

// hashing functions using the OpenSSL library - EVP
// EVP_MD structure represents a digest algo (cryptographic hash function)
// this structure is used for the context of a hash computation,
// maintains the state initialize the hash context for a specific algo
// update the hash computation with input data -> sentence
// finalizes the hash computation and obtain resulting hash value
void hash(const char *sentence, uint8_t *hashed_sentence, const EVP_MD *hash_algo)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, hash_algo, NULL);
    EVP_DigestUpdate(ctx, sentence, strlen(sentence));
    EVP_DigestFinal_ex(ctx, (unsigned char *)hashed_sentence, NULL);
    EVP_MD_CTX_free(ctx);
}

int main(void)
{

    int dev;                 // file descriptor of the device file
    int ret;                 // return value
    char digest[BUF_SIZE];   // to store computed hash digest
    userspace_t us;          // to store required fields and pass to kernel space

    char sentence[100];      // to store generated sentence
    dt dt;                   // to store generated system datetime

    int choice;              // selection of hashing algorithm
    char *algo_str;          // selected type of hashing algorithm
    const EVP_MD *hash_algo; // hashing algorithm function
    size_t bytes_to_print;   // to store number of bytes to print in hash digest
                             // based on the algorithm

    // open the character device in read/write mode
    dev = open(DEVICE_PATH, O_RDWR);
    if (dev < 0)            // error handling
    {
        perror("Failed to open device");
        return errno;
    }

    // generate datetime and store to sentence
    dt = generate_datetime();
    format_sentence(sentence, dt);
    // convert to bytes and store in us.plaintext for byte-by-byte accessibility
    strncpy((char *)us.plaintext, sentence, BUF_SIZE);
    
    do {
        // get user input for hashing algorithm
        printf("Select a Hashing Algorithm (1-5) from below: \
            \n1. MD5 \
            \n2. SHA-512 \
            \n3. SHA-382 \
            \n4. SHA-256 \
            \n5. SHA-1 \
            \n\nChoice: ",
            stdout);
        scanf("%d", &choice);
    } while(choice < 0 || choice > 5);
    
    // provide hashing algorithm based on user input
    if (choice == 1)
    {
        us.algo = MD5;         // to store in us struct
        algo_str = "MD5";      // type of hashing algorithm
        hash_algo = EVP_md5(); // hashing algorithm in OpenSSL
        bytes_to_print = 16;   // MD5 hash digest = 128 bits = 16 bytes
    }
    else if (choice == 2)
    {
        us.algo = SHA512;
        algo_str = "SHA512";
        hash_algo = EVP_sha512();
        bytes_to_print = 64;   // SHA-512 hash digest = 512 bits = 64 bytes
    }
    else if (choice == 3)
    {
        us.algo = SHA384;
        algo_str = "SHA384";
        hash_algo = EVP_sha384();
        bytes_to_print = 48;   // SHA-384 hash digest = 384 bits = 48 bytes
    }
    else if (choice == 4)
    {
        us.algo = SHA256;
        algo_str = "SHA256";
        hash_algo = EVP_sha256();
        bytes_to_print = 32;   // SHA-256 hash digest = 256 bits = 32 bytes
    }
    else if (choice == 5)
    {
        us.algo = SHA1;
        algo_str = "SHA1";
        hash_algo = EVP_sha1();
        bytes_to_print = 20;   // SHA-1 hash digest = 160 bits = 20 bytes
    }

    // generate hashed_sentence in userspace
    char hashed_sentence[bytes_to_print];
    hash(sentence, hashed_sentence, hash_algo);

    // convert hashed_sentence to hex
    char hash_sentence_hex[2 * bytes_to_print + 1];
    for (size_t i = 0; i < bytes_to_print; i++)
        snprintf(&hash_sentence_hex[2 * i],
                 3,
                 "%02x",
                 (unsigned char)hashed_sentence[i]);

    // convert to bytes and store in us.hashtext for byte-by-byte accessibility
    strncpy((char *)us.hashtext, hash_sentence_hex, BUF_SIZE);

    // write to device
    ret = write(dev, &us, sizeof(userspace_t));
    if (ret < 0)
    {
        perror("Error while sending data to kernel space");
        return errno;
    }

    // read from device
    ret = read(dev, digest, BUF_SIZE);
    if (ret < 0)
    {
        perror("Error while reading data to kernel space");
        return errno;
    }

    // convert received digest bytes (hash text) from kernel to hexadecimal
    char digest_hex[2 * bytes_to_print + 1];
    for (size_t i = 0; i < bytes_to_print; i++)
        snprintf(&digest_hex[2 * i], 3, "%02x", (unsigned char)digest[i]);

    // print output
    printf("\nOriginal Sentence                 : %s", us.plaintext);
    printf("\nHashing Algorithm                 : %s", algo_str);
    printf("\nHashed Sentence from user space   : %s", hash_sentence_hex);
    printf("\nHashed Sentence from kernel space : %s", digest_hex);
    if (strcmp(hash_sentence_hex, digest_hex) == 0)
    {
        printf("\nComparison Results                : Match");
    }
    else
    {
        printf("\nComparison Results                : Invalid");
    }
    printf("\n\n");

    // close the device file
    close(dev);

    return 0;
}
