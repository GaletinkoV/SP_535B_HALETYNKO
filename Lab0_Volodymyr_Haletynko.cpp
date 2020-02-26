#include "stdafx.h"
#include <limits.h> 
#include <float.h> 


int main()
{
	struct Structura
	{
		char str[8];
		unsigned int number;
	};

	printf("|Type | Bytes | Min Value | Max Value |\n");
    printf("|struct | %d |\n", sizeof(struct Structura));
    printf("|char | %d | %d | %d |\n", sizeof(char), CHAR_MIN, CHAR_MAX);
	printf("|signed char | %d | %d | %d |\n", sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
	printf("|unsigned char | %d | %d | %d |\n", sizeof(unsigned char), 0, UCHAR_MAX);
	printf("|short | %d | %d | %d |\n", sizeof(short), SHRT_MIN, SHRT_MAX);
	printf("|unsigned short | %d | %d | %d |\n", sizeof(unsigned short), 0, USHRT_MAX);
	printf("|int | %d | %d | %d |\n", sizeof(int), INT_MIN, INT_MAX);
	printf("|unsigned int | %d | %u | %u |\n", sizeof(unsigned int), 0, UINT_MAX);
	printf("|long | %d | %li | %li |\n", sizeof(long), LONG_MIN, LONG_MAX);
	printf("|long long | %d | %lli | %lli |\n", sizeof(long long), LLONG_MIN, LLONG_MAX);
	printf("|unsigned long | %d | %lu | %lu |\n", sizeof(unsigned long), 0, ULONG_MAX);
	printf("|unsigned long long | %d | %llu | %llu |\n", sizeof(unsigned long long), 0, ULLONG_MAX);
	printf("|long double | %d | %Le | %Le |\n", sizeof(long double), LDBL_MIN, LDBL_MAX);
	printf("|double | %d  |%e | %e |\n", sizeof(double), DBL_MIN, DBL_MAX);
	printf("|float | %d  |%e | %e |\n", sizeof(float), FLT_MIN, FLT_MAX);
	printf("|bool | %d | %d | %d |\n", sizeof(bool), false, true);

    return 0;
}