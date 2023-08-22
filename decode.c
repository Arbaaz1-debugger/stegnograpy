#include<stdio.h>
#include<string.h>
#include"decode.h"
#include"types.h"
#include"common.h"
//#include"encode.c"

// function definations

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
//    printf("length of magic string is %ld",strlen(MAGIC_STRING));

    if(strcmp(strstr(argv[2],"."),".bmp")==0)
    {

	decInfo->src_image_fname=argv[2]; 				// we can save in stego_image_fname
	printf("sorce image name = %s\n",decInfo->src_image_fname);
	
	if(argv[3]!=NULL)
	{

	    decInfo->output_fname=argv[3];
	}
	else
	{

	    decInfo->output_fname="output.txt";
	//    decInfo->fptr_output=output_fname;
	}
	    printf("output file name = %s\n",decInfo->output_fname);
	return e_success;
    }
    else
    {
	return e_failure;

    }
}
/* do decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    if(decode_open_files(decInfo)==e_success)
    {
	printf("sucessfully opened the file\n");
    }
    else
    {
	printf("failed to open the file\n");
    }
    if(decodemagicstring(decInfo)==e_success)
    {
	printf("decodeing magic string is completed\n");

	if(decode_size_of_secr_file_ext(decInfo)==e_success)
	{
	    printf("decodeing size of secreat file extension is completed\n");
	    
	    if(decode_secr_file_ext(decInfo)==e_success)
	    {
		printf("decodeing secreat file extension is completed\n");

		if(decode_size_of_secr_file(decInfo)==e_success)
		{
		    printf("decodeing size of secreat file is completed\n");
		   
		if(decode_secr_file_data(decInfo)==e_success)
		{
		    printf("decodeing of secreat file data is completed\n");
		    
		}
		else
		{
		    printf("failed to decode secreat file data\n");
		}
		}
		else
		{
		    printf("failed in decodeing size of secreat file\n");
		}

	    }
	    else
	    {
		printf("failed in decodeing secreat file extension\n");
	    }

	}
	else
	{
	    printf("failed to decode size of secreat file extension \n");
	}
    }
    else
    {
	printf("failed to decodeing magic string\n");
    }


}
/*   get file pointer for i/p and o/p
   i/p : DecodeInfo src_image_fname ,output_fname
   o/p : FILE pointer for the above files
   return value : e_success or e_failure, on file errors*/
Status decode_open_files(DecodeInfo *decInfo)
{
    // sorce image file open in read mode
    decInfo -> fptr_src_image = fopen(decInfo -> src_image_fname,"r");
    // do error handling
    if(decInfo->fptr_src_image==NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR: unable to open file %s\n",decInfo -> src_image_fname);
	return e_failure;
    }
    //output.txt file open in write mode
    decInfo -> fptr_output = fopen(decInfo ->output_fname,"w");
    // do error handling
    if(decInfo->fptr_output==NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR: unable to open file %s\n",decInfo -> output_fname);
	return e_failure;
    }
	return e_success;
}
Status decodemagicstring(DecodeInfo * decInfo)
{
    char arr1[2]={0};
    char data[16];
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
    fread(data,16,1,decInfo->fptr_src_image);
    for(int i=0;i<2;i++)
    {
	for(int j=0;j<8;j++)
	{
	    printf("data[%d] = %x\t",j,data[i*8+j]);
	    arr1[i] = (arr1[i]<<1) | (data[i*8+j]&0x01);
	    printf("arr1[%d] = %x\n",i,arr1[i]);
	}
	printf("\n");
    }
    printf("%s\n",arr1);
    if(strcmp(arr1,MAGIC_STRING)==0)
    {
    return e_success;
    }
}

Status decode_size_of_secr_file_ext(DecodeInfo *decInfo)
{   
   char data[32];
   int a=0; 
    fseek(decInfo->fptr_src_image,70,SEEK_SET);
    fread(data,32,1,decInfo->fptr_src_image);
    for(int i=0;i<32;i++)
    {
	printf("data[%d] = %x\t",i,data[i]);
	a=(a<<1)|(data[i]&0x01);
	    printf("a = %d\n",a);
    }
    printf("%d\n",a);
    return e_success;
}

Status decode_secr_file_ext(DecodeInfo *decInfo)
{   
   char data[32];
   char arr[4]={0};
    fseek(decInfo->fptr_src_image,102,SEEK_SET);
    fread(data,32,1,decInfo->fptr_src_image);
   int index;
    for(int i=0;i<4;i++)
    {
   	 for(int j=0;j<8;j++)
    	{
	    index=i*8+j;
		printf("data[%d] = %x\t",index,data[index]);
		arr[i]=(arr[i]<<1)|(data[index]&0x01);
		printf("arr[%d] = %x\n",i,arr[i]);
	}
		    printf("\narr[%d] = %c\n",i,arr[i]);
    }
   printf("decoded_secreat_file_extension = %s\n",arr);
   if(strcmp(arr,".txt"))
    return e_success;
}

Status decode_size_of_secr_file(DecodeInfo *decInfo)
{   
   char data[32];
   int a=0; 
    fseek(decInfo->fptr_src_image,134,SEEK_SET);
    fread(data,32,1,decInfo->fptr_src_image);
    for(int i=0;i<32;i++)
    {
	printf("data[%d] = %x\t",i,data[i]);
	a=(a<<1)|(data[i]&0x01);
	    printf("a = %d\n",a);
    }
    decInfo->size_secret_file=a;
    printf("\n size of secret file = %d\n",a);
    return e_success;
}

Status decode_secr_file_data(DecodeInfo *decInfo)
{  
   printf("\n\nsize of secreat file is = %ld\n\n",decInfo->size_secret_file); 
    long int len=(decInfo->size_secret_file*8);
   printf("\n\n len of secreat file is = %ld\n\n",len); 

    int size=len/8;
   char data[len];
   char arr[size];
    fseek(decInfo->fptr_src_image,166,SEEK_SET);
    fread(data,len,1,decInfo->fptr_src_image);
   int index;
    for(int i=0;i<size;i++)
    {
   	 for(int j=0;j<8;j++)
    	{
	    index=i*8+j;
		printf("data[%d] = %x\t",index,data[index]);
		arr[i]=(arr[i]<<1)|(data[index]&0x01);
		printf("arr[%d] = %x\n",i,arr[i]);
	}
		    printf("\narr[%d] = %c\n",i,arr[i]);
    }
  //  fopen(decInfo->fptr_output,"w");
   // fwrite(decInfo->fptr_output,1,arr);
   // arr[size++]='\0';
   printf("decoded_secreat_file_data = %s\n",arr);
  // char ch;
//  while(fread(ch,1,1,arr[i])!='\0')
//  {
    decInfo->fptr_output=fopen(decInfo->output_fname,"w");
  // fread(
    fwrite(arr,size,1,decInfo->fptr_output);
//  }
//   if(strcmp(arr,".txt"))
    return e_success;
}
