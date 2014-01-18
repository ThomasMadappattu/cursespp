/*
    Program  name : Command Line based presentor for linux  
    Description   : A new based presentor for linux
    Written by    : Sandeep Mathew
    License       : All rights waived , Public Domain
*/
#include <stdio.h>
#include <stdlib.h>
#include <newt.h>
#include <sys/stat.h> 
#include <string.h>
#include <ctype.h>

#define DELTA  2 
#define LINE_MAX 1024

/*

  Function: exit_error 
  Descirption : exit with an error 

*/

void exit_error( char *msg)
{
    fprintf(stderr,"%s",msg);
    exit(EXIT_FAILURE); 
}


/*
   Function     :  trimwhitespace
   Description  :  strips off white space from the string
*/

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

/*
    function : get_file_size 
    Description: clibrary way to get size of a file  
*/
unsigned long get_file_size(char *filename)
{
    unsigned int file_size  ; 
    FILE *fp = fopen(filename,"r");
    fseek(fp,0L,SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    return file_size;  
}


/*
    function : load_file 
    description : load the contents of a file into a buffer


*/
void load_file( char *buffer , char *filename)
{
    FILE *fp = NULL  ;
    unsigned  long file_size; 
    int counter = 0 ; 
    fp = fopen(filename,"r"); 
    if ( fp == NULL ) 
    {
         exit_error("Unable to open the file !");        
    }
    file_size = get_file_size(filename);   
    while ( counter <  (file_size))
    {
        buffer[counter++] = fgetc(fp); 
    }
    buffer[counter]='\0'; 
    fclose(fp); 
     

}

/*

  display usage information 

*/

void display_usage()
{
    fprintf(stderr,"%s\n" , "Usage : cursespp  <metafilename>" ); 
}

/*
   Main program :  display the power point text 
*/
int main ( int argc , char *argv[])
{
    


     /*
          Initialize variables 
     */
    unsigned int rows , cols;  
    char *buffer; 
    FILE *metafile = NULL;
    char line[LINE_MAX];  
    newtComponent form; 
    newtComponent displayText; 
    if ( argc < 2 ) 
    {
        display_usage(); 
        exit(EXIT_FAILURE);    
    }
    newtInit();
    newtCls();
    newtGetScreenSize(&rows , &cols);
    newtPushHelpLine(NULL);
    metafile = fopen(argv[1] , "r");
    if ( metafile == NULL )
    {
       exit_error("Unable to open meta file"); 
    }     
   
           

 
    newtOpenWindow(DELTA,DELTA , rows-3 * DELTA ,cols-3 * DELTA, argv[1]);  
   
    form = newtForm(NULL,NULL,0);
    displayText =  newtTextbox( DELTA +1 , DELTA+1 , rows-5 * DELTA-2  ,cols-5 * DELTA-2 , NEWT_FLAG_SCROLL);        
    newtFormAddComponent(form,displayText);
    
    /*

         read line by line 
         open file in each line    
         dump it to srceen 
         wait for key press 

    */
      
    fgets(line,LINE_MAX-2 ,metafile);

    while(!feof(metafile))
    { 
       

       if ( strlen(line) > 2 )
       { 
           buffer = malloc( get_file_size(trimwhitespace(line)) + 1); 
	   load_file((char *)buffer,line);
           newtTextboxSetText(displayText,(const char *)buffer);
           newtRefresh();  
           newtRunForm(form);
           free(buffer);
       } 
      fgets(line,LINE_MAX-2 ,metafile);
    }

    newtPopWindow(); 
    newtWaitForKey();
    newtFinished();
    return 0;   
}


