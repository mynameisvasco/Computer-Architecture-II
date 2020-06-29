#include <detpic32.h>
#include "util.h"

int main(void)
{
    configUart(115200, 'N', 1);

    while (1)
    {
        putcUart(getc());
    }
}