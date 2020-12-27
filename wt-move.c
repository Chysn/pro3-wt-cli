/* Move Sysex to New Wavetable Number
 *
 * Accept Pro 3 wavetable sysex at standard input, and a wavetable number
 *   (33-64) as a command line argument
 * Send a sysex file with the new wavetable number to standard output
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sequential_lib/sequential_packing.h"

int main(int argc, char *argv[])
{
    /* The wavetable number is expected as a command-line argument */
    if (argc < 2) {
        printf("\nusage: cat wt-old.syx | %s wavetable-number > wt-new.syx", argv[0]);
        return -1;
    }

    int wavetable_number = atoi(argv[1]);
    if (wavetable_number > 64 || wavetable_number < 33) {
        printf("\nwavetable number out of range (33-64)\n\n");
        return -1;
    }

    /* Read everything up to the wavetable number */
    int i;
    for (int i = 0; i < 7; i++)
    {
        int c = getchar();
        if (c == EOF) return -1;
        putchar((char) c);
    }
    wavetable_number--; /* Because wavetable numbers are zero-indexed to the Pro3 */
    putchar((char) wavetable_number);

    /* Now pass the rest of the file through */
    getchar(); /* Burn off the previous wavetable number */
    for (;;)
    {
        int c = getchar();
        if (c == EOF) break;
        putchar((char) c);
    }

    return 0;
}

/*
 * Copyright (c) 2020 The Beige Maze Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
