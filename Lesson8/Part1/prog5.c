#include <detpic32.h>
#include "util.h"

int main(void)
{
    configUart(9600, 'N', 2);

    while (1)
    {
        putsUart("THIS IS A TEST\n");
        putsUart("Another test\n");
        delay(500);
    }
}