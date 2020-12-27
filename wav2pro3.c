/* WAV to Pro 3 Sysex
 *
 * Usage:
 * 
 * ./wav2pro3 wavetable-number name wav-path1 ... wav-path16 > sysex-file.syx
 * 
 * wavetable-number is a number between 33 and 64
 * if wavetable-number is 0, wav2pro3 outputs raw PCM instead of sysex
 * name is a string of up to eight characters
 * at least one wavpath must be provided, and a maximum of 16
 * 
 * WAV files are placed into reference waves in order. To skip a reference wave,
 * use - in place of the wavpath.
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
        printf("\nusage: %s name wavetable-number wav-path1 wav-path2 ...\n\n", argv[0]);
        return -1;
    }

    int wavetable_number = atoi(argv[2]);
    if (wavetable_number && (wavetable_number > 64 || wavetable_number < 33)) {
        printf("\nwavetable number out of range (33-64)\n\n");
        return -1;
    }
    wavetable_number--; /* Because wavetable numbers are zero-indexed to the Pro3 */

    FILE *fp;
    int byte;
    int argnum;

    Wavetable table = new_Wavetable();
    int ref_num = 0;
    for(argnum = 3; argnum < argc; argnum++) 
    {
        if ((fp = fopen(argv[argnum], "r")) == NULL) {
            ref_num++;
        } else {
            int size = 0;
            signed int wav[PCM_PROC_MAX];
            while ((byte = getc(fp)) != EOF)
            {
                wav[size++] = byte;
            }
            PCMData pcm = wav_to_pcm(size, wav);
            pcm = pcm_from_channel(&pcm, PCM_PROC_CHANNEL_LEFT);
            pcm_change_resolution(&pcm, 16);
            pcm_change_size(&pcm, PRO3_SAMPLE_SIZE);
            set_reference(&table, &pcm, ref_num++);
            fclose(fp);
        }
    }

    wavetable_fill(&table);

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
