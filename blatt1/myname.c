#include <stdio.h>

int
main (void)
{
    char* name  = "thetek-thi";
    int   day   = 4,
          month = 11,
          year  = 2021;

    printf ("my name is %s\n", name);
    printf ("my birthday is on %02d.%02d.%04d\n", day, month, year);
}

