#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sequential_lib/sequential_packing.h"

int main(int argc, char *argv[])
{
    /* Ignore sysex header bytes */
    for (int i=0; i < 17; i++) getchar();

    PackedData sysex;
    unsigned int data[SEQUENTIAL_DATA_MAX];
    unsigned long int size = 0;
    for (;;)
    {
        int c = getchar();
        if (c == 0xf7 || c == EOF || size == SEQUENTIAL_DATA_MAX) break;
       	data[size++] = c;
    }
    
    Seq_set(&sysex, size, data);
    
    UnpackedData params = Seq_unpack(sysex);
    Seq_dump(params);
    return 0;
}

