#include <stdio.h>						//preprocess directive standard input output headder library
#include <string.h>						// preprocess directive string headder file
#include "encode.h"						// including the encode.h file
#include "types.h"						// including the types.h file
#include "decode.h"

int main(int argc,char *argv[])					// start of the programe with command line arguments
{
    int operation=check_operation_type(argv);			//calling the check_operation_type() with the argv vector and return value is collected in the operation varable
    if(operation==e_encode)					// operation value is checked with enum e_encode
    {
	EncodeInfo encInfo;					// declering the type struct EncodeInfo  as encInfo varable

	printf("selected encoding\n");				//printing the message "selected encoding

	int sas;						// declaring the some integer varable as sas
	sas=read_and_validate_encode_args(argv,&encInfo);	// calling the read_and_validate _encode_args(argv,&encInfo) and return value is assigned to the sas varable
	if(sas==e_success)					// check (sas valiable is == e_sucess enum)
	{
	    printf("file type \"-e \" reading is sucess\n");		//
	    printf("file type is \"BMP\" \n");

	    do_encoding(&encInfo);

	}
	else if(sas==e_failure)
	{
	    printf("file type reading is failure\n");

	}
	else
	{
	    printf("somthing went wrong in file reading\n");
	}


    }
    else if(operation==e_decode)
    {
	DecodeInfo decInfo;

	printf("selected decoding \"-d \"\n");
	if(read_and_validate_decode_args(argv,&decInfo)==e_success)
	{
	    printf("file type reading is sucess\n");		//
//	    printf("file type is \"BMP\" \n");

	    do_decoding(&decInfo);
	}
	else if(read_and_validate_decode_args(argv,&decInfo)==e_failure)
	{
	    printf("file type reading or writing is failed");
	}
	else
	{
	    printf("somthing went wrong in file reading or writing\n");
	}

    }
    else
    {
	printf("invallied option\n");
    }

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
	return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}
	  

