#include "atospf.h"

int my_isdigit(char c) {
    return c >= '0' && c <= '9';
}

float atospf_aux(char *s)
{
    float a = 0.0;
    int e = 0;
    int c;
    while ((c = *s++) != '\0' && my_isdigit(c)) {
        a = a*10.0 + (c - '0');
    }
    if (c == '.') {
        while ((c = *s++) != '\0' && my_isdigit(c)) {
            a = a*10.0 + (c - '0');
            e = e-1;
        }
    }
    if (c == 'e' || c == 'E') {
        int sign = 1;
        int i = 0;
        c = *s++;
        if (c == '+')
            c = *s++;
        else if (c == '-') {
            c = *s++;
            sign = -1;
        }
        while (my_isdigit(c)) {
            i = i*10 + (c - '0');
            c = *s++;
        }
        e += i*sign;
    }
    while (e > 0) {
        a *= 10.0;
        e--;
    }
    while (e < 0) {
        a *= 0.1;
        e++;
    }
    return a;
}

float atospf(char* s) {
	if (s[0] == '-') {
		return -atospf_aux(++s);
	} else {
		return atospf_aux(s);
	}
}
