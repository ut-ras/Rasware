
// For dumping printf-like output into a buffer
void SPrintf(char *str, const char *pcString, ...);

// We don't have strnlen, strcpy, or strcmp so we need to implement them here
// Use uppercase first letter to avoid naming collision
unsigned int Strnlen(char *str, unsigned int max_len);

// copies all of src's chars to dst, up to and not including the null-terminator
char* Strcpy (char *dst, const char *src);

// Returns an integral value indicating the relationship between the strings:
// A zero value indicates that both strings are equal.
// A value greater than zero indicates that the first character that does not match has a greater value in str1 than in str2; 
// And a value less than zero indicates the opposite.
// int strcmp ( const char * str1, const char * str2 );
int Strcmp(const char *str1, const char *str2);
