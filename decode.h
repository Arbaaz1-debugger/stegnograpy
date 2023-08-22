#include<stdio.h>
#ifndef DECODE_H
#define DECODE_H
#include"types.h"

//
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

//
typedef struct _DecodeInfo
{
    /* Source Image info */

    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* output File Info */

    char *output_fname;
    FILE *fptr_output;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */

    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;

/* read and validate decode args from argv from  command line argument i.e .bmp and optional*/

Status read_and_validate_decode_args(char * agrv[], DecodeInfo *decInfo);

/* perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* get file pointer for i/p and o/p files */
Status decode_open_files(DecodeInfo *decInfo);
/* decoding magic string */
Status decodemagicstring(DecodeInfo *decInfo);
/* decode_size_of_secr_file_ext */
Status decode_size_of_secr_file_ext(DecodeInfo *decInfo);
/* decode__secr_file_ext */
Status decode_secr_file_ext(DecodeInfo *decInfo);

/* decode_size_of_secr_file */
Status decode_size_of_secr_file(DecodeInfo *decInfo);
/* decode__secr_file_data */
Status decode_secr_file_data(DecodeInfo *decInfo);
#endif
