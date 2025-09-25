#ifndef __LIB_CTYPE_H
#define __LIB_CTYPE_H

static inline int isdigit(char c) {
    return (c >= '0' && c <= '9');
}

static inline int isspace(char c) {
    return (c == ' ');
}

static inline int islower(char c) {
    return ((c >= 'a') && (c <= 'z'));
}

static inline int isupper(char c) {
    return ((c >= 'A') && (c <= 'Z'));
}

static inline int isalpha(char c) {
    return (islower(c) || isupper(c));
}

static inline int isgraph(char c) {
    return ((c >= 32) && (c <= 126));
}

#endif
