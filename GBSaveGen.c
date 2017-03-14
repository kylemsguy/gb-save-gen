#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "GBSaveGen.h"


char rom_data[ROM_HEADER_SIZE];


void usage(){
    printf("./BennVennSaveGen.py -r <ROM filename> [-n <output_filename>] [-h]\n");
    printf("\t-r: The ROM's filename (required)\n");
    printf("\t-n: (optional) The desired save file's name. If this is not provided,\n");
    printf("\t    the ROM's name is either read from the header, or the ROM's filename\n");
    printf("\t    (default)\n");
    printf("\t-h: Read the ROM's filename from the ROM header (ignored if -n provided)\n");
    exit(EXIT_FAILURE);
}

/**
 * Returns SRAM size in bytes
 */
int get_sram_size(char cart_type, char ram_type){
    if(cart_type == 0x5 || cart_type == 0x6){
        return 256;
    }

    switch(ram_type){
        case 0:
            return 0;
        case 1:
            return 2048;
        case 2:
            return 8192;
        case 3:
            return 32768;
        case 4:
            return 128 * 1024;
        case 5:
            return 64 * 1024;
        default:
            return -1;
    }
}

int main(int argc, char **argv){
    char *in_filename = NULL;
    char *out_filename = NULL;
    bool read_rom_name = false;
    bool is_gbc = true; // TODO set this flag and use it
    char rom_title_name[17];
    FILE *infile;

    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "r:n:h")) != -1){
        switch (c){
            case 'r':
                in_filename = optarg;
                break;
            case 'n':
                out_filename = optarg;
                break;
            case 'h':
                read_rom_name = true;
                break;
            case '?':
                if(optopt == 'r' || optopt == 'n'){
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }else if (isprint(optopt)){
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }else{
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                return EXIT_FAILURE;
            default:
                exit(EXIT_FAILURE);
        }
    }

    // if we want to support non-option args
    // for (index = optind; index < argc; index++){
    //     printf ("Non-option argument %s\n", argv[index]);
    // }

    if(in_filename == NULL){
        usage();
    }

    if((infile = fopen(in_filename, "rb")) == NULL){
        fprintf(stderr, "Failed to open input file (%s)\n", in_filename);
        exit(EXIT_FAILURE);
    }

    int read_bytes = fread(rom_data, 1, ROM_HEADER_SIZE, infile);

    if(read_bytes < ROM_HEADER_SIZE){
        fprintf(stderr, "Input file size too small (%d bytes < %d bytes expected)", 
            read_bytes, ROM_HEADER_SIZE);
        exit(EXIT_FAILURE);
    }

    fclose(infile);

    struct GBRomHeaderv2 *header = (struct GBRomHeaderv2 *) (rom_data + 0x100);

    // the magic begins
    if(read_rom_name){
        memcpy(rom_title_name, header->title, 16); // 16 for gb, 15 or 11 for cgb or whatever
        rom_title_name[17] = '\0';
        out_filename = rom_title_name;
    }

    int sram_size = get_sram_size(header->cart_type, header->sram_size);

    printf("SRAM size detected: %d\n", sram_size);

    return EXIT_SUCCESS;
}