#ifndef ENCRYPTION_H
#define ENCRYPTION_H

/* Size of a stored hash text: "salt$hash" + end character */
#define HASH_SIZE 64

int initialize_encryption();

int hash_text(const char *plain_text, char hash[HASH_SIZE]);

int verify_text(const char *plain_text, const char *stored_hash);

#endif
