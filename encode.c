#include<string.h>
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
/* Function Definitions */
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    char *add;
    add=strrchr(argv[2],'.');
    if(strcmp(add,".bmp")==0)
    {
	encInfo->src_image_fname=argv[2];
	char *secr;
	secr=strrchr(argv[3],'.');
	if(strcmp(secr,".txt")==0)
	{
	    encInfo->secret_fname=argv[3];
	}
	if(argv[4]!=NULL)
	{
	    encInfo->stego_image_fname=argv[4];
	}
	else
	{
	    encInfo->stego_image_fname="output.bmp";
	}

    return e_success;
    }

    else
    {
	return e_failure;
    }

}

/* do_encoding */

Status do_encoding(EncodeInfo *encInfo)
{
    // call the open file function

    if(open_files(encInfo)==e_success)
    {
	printf("opened the file sucessfully\n");
    }
    else
    {
	printf("failed to opened the file\n");
    }

    if (check_capacity(encInfo) == e_success)
    {
	printf("sucessfully checked the file size\n");
    }
    else
    {
	printf("failed in checking the file size\n");
    }

   if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
   {
       printf("bmp header copy sucess\n");
   }
   else
   {

       printf("bmp header copy fail\n");
   }

   if(encode_magic_string(MAGIC_STRING,encInfo)== e_success)
   {
       printf("sucessfully encode magic string\n");

   }
   else
   {
       printf("failed to  encode magic string\n");
   }

   if(encode_secret_file_extn_size(strlen(strstr(encInfo->secret_fname,".")),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
   {
       printf(" sucessfully secreat file enxtn size\n");
   }
   else
   {
       printf("failed to secreat file extension\n");
   }

   if(encode_secret_file_extn(encInfo->secret_fname,encInfo)==e_success)
   {
       printf("sucessfully encode_secret_file_extn\n");
   }
   else
   {
       printf("failed encode_secret_file_extn\n");
   }

   if(encode_secret_file_size(encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
   {
       printf("sucessfully  encode_secret_file_size \n");
   }
   else
   {
       printf("failed  encode_secret_file_size \n");
   }

   
   if(encode_secret_file_data(encInfo)==e_success)
   {
       printf("sucessfully encode_secret_file data\n");
   }
   else
   {
       printf("failed to  encode_secret_file data\n");
   }

   if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
   {
       printf("sucessfully copy_remaining_img_data\n");
    }
   else
   {
     printf("failed copy_remaining_img_data\n");
   }

}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo-> image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo-> size_secret_file=get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity>(16+32+32+32+(encInfo->size_secret_file*8)))
    {
	printf("it is working\n");
	return e_success;
    }
    else
    {
	printf("it is not working\n");
	return e_failure;
    }
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}


// copy bmp headder
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char str[54];
    rewind(fptr_src_image);
    fread(str,sizeof(char),54,fptr_src_image);
    //rewind(fptr_scr_image);

    fwrite(str,sizeof(char),54,fptr_dest_image);
    return e_success;
}

// stored magic string
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{

 encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image,(encInfo));
}

// encode data to image
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image,EncodeInfo *encInfo)
{
    for(int i=0;i<size;i++)
//encInfo

    {
	fread(encInfo->image_data,sizeof(char),8,fptr_src_image);
	 encode_byte_to_lsb(data[i],encInfo->image_data);
	fwrite(encInfo->image_data,sizeof(char),8,fptr_stego_image);
    }
    return e_success;
}    

// encode_bytr_to_lsb
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    uint mask=1<<7;
    for(int i=0;i<8;i++)
    {
	image_buffer[i]=(image_buffer[i]&0xFE)|((data & mask)>>7-i);
	mask=mask>>1;
    }
    return e_success;
}

//encode_secret_file_extn_size

Status encode_secret_file_extn_size(int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
  char str[32]; 
	fread(str,sizeof(char),32,fptr_src_image);
	 encode_size_to_lsb(str,size);
	fwrite(str,sizeof(char),32,fptr_stego_image);
    
    return e_success;
}


Status encode_size_to_lsb(char *buffer,int size)
{

    uint mask=1<<31;
    for(int i=0;i<32;i++)
    {
	buffer[i]=(buffer[i]&0xFE)|((size & mask)>>31-i);
	mask=mask>>1;
    }

}

// encode_secret_file_extnsion
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(strstr(encInfo->secret_fname,"."),strlen(strstr(encInfo->secret_fname,".")),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
    return e_success;
}

// encode_secret_file_size
Status encode_secret_file_size(long file_size, FILE *fptr_src_image,FILE *fptr_stego_image)
{
printf("size of the secreat file is = %ld\n",file_size);
 encode_secret_file_extn_size(file_size,fptr_src_image,fptr_stego_image);
 return e_success;
}

// encode_secret_file_data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
fseek(encInfo->fptr_secret,0,SEEK_SET);
     char str[encInfo->size_secret_file];
	fread(str,sizeof(char),encInfo->size_secret_file,encInfo->fptr_secret);
	encode_data_to_image(str, encInfo -> size_secret_file, encInfo->fptr_src_image, encInfo -> fptr_stego_image, encInfo);
    return e_success;

}


// copy_remaining_img_data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, sizeof(char), sizeof(char), fptr_src) > 0)
    {
	fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}






/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
