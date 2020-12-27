/* Pro 3 Morph Controller Generator
 *
 * Usage:
 * 
 * ./morph-controller wavetable-number name level1 ... level16 > sysex-file.syx
 * 
 * wavetable-number is a number between 33 and 64
 * if wavetable-number is 0, pro3-lfo outputs raw PCM instead of sysex
 * name is a string of up to eight characters
 * 16 levels between -32768 and 32767 must be provided
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "sequential_lib/pro3_wavetable.h"
 
int main(int argc, char *argv[]) 
{
    /* The wavetable number is expected as a command-line argument */
    if (argc < 4) {
        printf("\nusage: %s name wavetable-number level1 ... level16", argv[0]);
        return -1;
    }

    int wavetable_number = atoi(argv[2]);
    if (wavetable_number && (wavetable_number > 64 || wavetable_number < 33)) {
        printf("\nwavetable number out of range (33-64)\n\n");
        return -1;
    }
    wavetable_number--; /* Because wavetable numbers are zero-indexed to the Pro3 */
    Wavetable table = new_Wavetable();
    int ref_num = 0;
    int argnum;
    for(argnum = 3; argnum < argc; argnum++) 
    {
        signed int lfo[16];
        pcm_sample_t level = atoi(argv[argnum]);
        int i;
        for (i = 0; i < 16; i++)
        {
            lfo[i] = -level;
        }
        PCMData pcm = new_PCMData();
        set_pcm_data(&pcm, 16, lfo);
        //printf("Num: %i Level: %i\n", ref_num, level);
        set_reference(&table, &pcm, ref_num++);
    }

    /* Output sysex if a wavetable number is specified. Otherwise output raw PCM */
    /* (16-bit big endian) */
    if (wavetable_number) {
        wavetable_sysex_dump(&table, wavetable_number, argv[1]);
    } else {
        wavetable_pcm_dump(&table);
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
