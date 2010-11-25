

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#define MAX_BUFFER 32
#define MAX_STRING 128
#define FALSE  -1
#define TRUE   1

void  prog_desc()
{
    printf("----------------------------------------------------------------------\n");
    printf("  CSV to HTML/WIKI Converter V.01\n");
    printf(" \n");
    printf("  Converts CSV file to Wiki/HTML format\n");
    printf("\n\n");
    printf("  http://www.dangerousprototypes.com\n");

    printf("----------------------------------------------------------------------\n");

}
void helpme(char *argv[])
{
         printf("Usage: - %s --i inputfile.csv  --wiki wikioutput.wiki --html htmloutput.html \n\n",argv[0]);
         exit(1);
 }


void removeQuotes( char cStr[])
{
    unsigned i, j; /* need to declare at top for 'C' compile ... */
    char Quotes='"';
    for( i = j = 0; cStr[i] != 0; ++i )
        if( cStr[i] == Quotes ) continue;
        else cStr[j++] = cStr[i] ;

    cStr[j] = '\0';
}
int main(int argc, char *argv[])
{

    FILE *in=NULL, *wikiout=NULL,*htmlout=NULL;
    char * ptrch;
    char header[MAX_BUFFER][MAX_STRING];
    char col[MAX_BUFFER][MAX_STRING];
    char buffer[4096];
    int c,index, option_index = 0;
    char *param_wiki_output=NULL;
    char *param_html_output=NULL;
    char *param_file_input=NULL;
    int  linecounter=0;
    static struct option long_options[] = {
            {"wiki", 1, 0, 0},
            {"html", 1, 0, 0},
            {"i", 1, 0, 0}
     };

    int j=0,i=0;
    char separator[5];
    separator[0]=',';
    separator[1]='"';
    separator[2]='\0';


    prog_desc(argv);  //program descripton and title
    while(1) {
        c = getopt_long (argc, argv,"",long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0:
                index=option_index;
                switch (index) {
                   case 0:  //wiki
                        param_wiki_output = optarg;
                        break;
                   case 1:  // html
                        param_html_output = optarg;
                        break;
                    case 2:  // input
                        param_file_input = optarg;
                        break;
                    default:
                        printf("Unknown Parameter: %i\n",index);
                        helpme(argv);

                }
                break;
            default:
                        printf("Invalid Parameter: %i\n",c);
                        helpme(argv);

        }
    }

    if (optind < argc) {
        printf (" Ignored no option parameter(s): ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }


    if ((param_wiki_output==NULL) &&( param_html_output==NULL)) {

         printf(" Error:  No output file specified:  use --wiki fname or --html fname\n\n");
         helpme(argv);

    }
    else {
		if (param_wiki_output !=NULL){
            wikiout=fopen(param_wiki_output,"w");
            if(wikiout==NULL ){
                printf("Can't create wiki output file %s\n",param_wiki_output);
                exit(1);
            }
		}
		if (param_html_output !=NULL) {
            htmlout=fopen(param_html_output,"w");
            if(htmlout==NULL ){
                printf("Can't create html output file %s\n",param_html_output);
                exit(1);
            }
        }
    }
    if (param_file_input ==NULL){
        printf("input text file required\n");
        helpme(argv);
    }
    else {
       in =fopen(param_file_input,"r");
       if(in==NULL ){
             printf("Can't open input file %s\n",param_file_input);
             exit(1);
       }
    }

	linecounter=0;
	while(!feof(in)) {
		if(fgets(buffer, MAX_STRING, in)) {
			//buflen=strlen(buffer);
			if (linecounter==0){
				// linecounter==0 means we are dealing with a header
				//remove double quotes("), replace comman (,) with !
				// do the column header first

				ptrch = strtok (buffer,",");  //tokens are delimited with pairs of quote comma
				i=0;
				while (ptrch != NULL) {
					   strcpy(header[i],ptrch);
					   removeQuotes(header[i]);
					   i++;
					   ptrch = strtok (NULL, ",");
				}

				if (htmlout!=NULL){
					 fprintf(htmlout,"<table class='wikitable' border='1'>\n");
					 fprintf(htmlout,"<tr>\n");
					 for (j=0; j < i;j++) {
						fprintf(htmlout,"<th scope='col'>%s</th>\n",header[j]);
					 }
					 fprintf(htmlout,"</tr>\n");
				}
				if (wikiout!=NULL){
					 fprintf(wikiout,"== Partlist ==\n\n");
					 fprintf(wikiout,"{| border='1' class='wikitable'\n");
					 fprintf(wikiout,"!%s",header[0]);
					 for(j=1;j < i;j++) {
						fprintf(wikiout,"!!%s",header[j]);
					 }
					// fprintf(wikiout,"\n");
				}



			}   // linecounter =0 only
            else {
                // tokens are separated by pair of quotes

				ptrch = strtok (buffer,"\"");  //tokens are delimited with quotes
				i=0;
				while (ptrch != NULL) {
				       if(*ptrch !=','){
                           strcpy(col[i],ptrch);
                           removeQuotes(col[i]);
                           i++;
                        }
					    ptrch = strtok (NULL, "\"");
				}


				 if (htmlout!=NULL){
					 fprintf(htmlout,"<tr>");
					 for (j=0;j < i-1;j++) {
						fprintf(htmlout,"<td>%s</td> ",col[j]);
					 }
					 fprintf(htmlout,"</tr>\n");

				 }
				  if (wikiout!=NULL){
					 fprintf(wikiout,"|-\n");
					 fprintf(wikiout,"|%s",col[0]);
					 for (j=1;j<i-1;j++) {
						fprintf(wikiout,"||%s",col[j]);
					 }
					  fprintf(wikiout,"\n");

				 }
            }

		}  //if (fgets(
         linecounter++;
	} //while !feof

	if (htmlout!=NULL){
	fprintf(htmlout,"</table>\n\n");
	printf("\n HTML FIle created: %s\n",param_html_output);
	}

   if (wikiout!=NULL){
        fprintf(wikiout,"|}\n\n");
        printf("\n Wiki FIle created: %s\n",param_wiki_output);
   }


   fclose(in);

   if(htmlout!=NULL ){
         fclose(htmlout);
   }
   if(wikiout!=NULL ) {
         fclose(wikiout);
   }
   return 0;
}
