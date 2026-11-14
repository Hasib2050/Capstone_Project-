#include "encryption.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Simple salted hash (no external library needed).
   Stored format:  8 hex salt  $  16 hex hash
   Good enough for a university project. */

#define ROUNDS 5000

int initialize_encryption()
{
    srand((unsigned int)time(NULL) ^ (unsigned int)clock());
    return 0;   /* 0 = success */
}

/* Mixes salt + text many times and gives two 32-bit numbers */
static void make_hash(const char *salt, const char *text,
                      unsigned long *high, unsigned long *low)
{
    unsigned long h1 = 2166136261UL;
    unsigned long h2 = 5381UL;
    int r;
    size_t i;

    for (r = 0; r < ROUNDS; r++)
    {
        for (i = 0; salt[i] != '\0'; i++)
        {
            h1 = ((h1 ^ (unsigned char)salt[i]) * 16777619UL) & 0xFFFFFFFFUL;
            h2 = ((h2 * 33UL) + (unsigned char)salt[i]) & 0xFFFFFFFFUL;
        }
        for (i = 0; text[i] != '\0'; i++)
        {
            h1 = ((h1 ^ (unsigned char)text[i]) * 16777619UL) & 0xFFFFFFFFUL;
            h2 = ((h2 * 33UL) + (unsigned char)text[i]) & 0xFFFFFFFFUL;
        }
        h1 = (h1 ^ (h2 >> 7)) & 0xFFFFFFFFUL;
        h2 = (h2 ^ (h1 << 5)) & 0xFFFFFFFFUL;
    }

    *high = h1;
    *low  = h2;
}

int hash_text(const char *plain_text, char hash[HASH_SIZE])
{
    char salt[9];
    unsigned long high, low;
    unsigned long s;

    s = ((unsigned long)rand() << 16) ^ (unsigned long)rand() ^ (unsigned long)time(NULL);
    sprintf(salt, "%08lX", s & 0xFFFFFFFFUL);

    make_hash(salt, plain_text, &high, &low);

    sprintf(hash, "%s$%08lX%08lX", salt, high, low);
    return 1;   /* success */
}

int verify_text(const char *plain_text, const char *stored_hash)
{
    char salt[9];
    char again[HASH_SIZE];
    unsigned long high, low;

    if (strlen(stored_hash) < 9 || stored_hash[8] != '$')
    {
        return 0;
    }

    memcpy(salt, stored_hash, 8);
    salt[8] = '\0';

    make_hash(salt, plain_text, &high, &low);
    sprintf(again, "%s$%08lX%08lX", salt, high, low);

    return strcmp(again, stored_hash) == 0;
}
