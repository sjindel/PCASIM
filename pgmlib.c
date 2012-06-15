/* sample PGM reader/writer for 24-354 General Robotics Lab 2

   fall 2000: amperez
   * original version)
   spring 2002: bstolt
   * updated file reader
   * removed hard-coded filenames (segfaults!)
   * handle comments in headers
   * added some basic error checking
   * misc clean-up
   fall 2003: spieper
   * made it simpler to do all three operations in one shot.
   * improved names on output files.
   */  
     
/* Dear reader, instead of reading in characters, read in ints. Your
   life will be a lot better. If you insist on reading in chars, then
   read in unsigned chars. Note that ints are bigger than chars
*/ 
    
#include "pgmlib.h"
    
/*
   This LoadPGM function is an updated version that will deal with
   comments in file headers and do at least some basic checking if the
   file can be opened or is the correct format. It does assume the file
   is actually a valid length once the header has been read.

   You are of course free to use your own file reading functions.
*/ 
void LoadPGM(char *filename, pgm * pPgm)
{
    FILE * ifp;
    int word;
    int nRead = 0;
    char readChars[256];
     
        //open the file, check if successful
        ifp = fopen(filename, "r");
    if (!ifp) {
        printf("Error: Unable to open file %s.\n\n", filename);
        exit(1);
    }
     
        //read headers from file
        printf("Reading PGM file: %s...\n", filename);
    fscanf(ifp, "%s", readChars);
     if (strcmp(readChars, "P2") == 0) {
        
            //valid file type
            //get a word from the file
            fscanf(ifp, "%s", readChars);
        while (readChars[0] == '#') {
            
                //if a comment, get the rest of the line and a new word
                fgets(readChars, 255, ifp);
            fscanf(ifp, "%s", readChars);
        }
         
            //ok, comments are gone
            //get width, height, color depth
            sscanf(readChars, "%d", &pPgm->w);
        fscanf(ifp, "%d", &pPgm->h);
        fscanf(ifp, "%d", &pPgm->max);
         
            // allocate some memory, note that on the HandyBoard you want to
            // use constant memory and NOT use calloc/malloc
            pPgm->pData = (int *)malloc(sizeof(int) * pPgm->w * pPgm->h);
         
            // now read in the image data itself
            for (nRead = 0; nRead < pPgm->w * pPgm->h; nRead++) {
            fscanf(ifp, "%d", &word);
            pPgm->pData[nRead] = word;
            
                // printf("nRead = %d %d\n",nRead,pPgm->pData[nRead]);
        }
         printf("Loaded PGM. Size: %dx%d, Greyscale: %d \n", pPgm->w,
                  pPgm->h, pPgm->max + 1);
    }
    
    else {
        printf("Error: Read file type %s. Format unsupported.\n\n", readChars);
        exit(1);
    }
    fclose(ifp);
}

 
/* Simply saves the PGM file to a file whose name is in filename */ 
void WritePGM(char *filename, pgm * pPgm)
{
    FILE * ofp;
    int w, h, max;
    int nWrite = 0;
    int i, j;
     ofp = fopen(filename, "w");
    if (!ofp) {
        printf("Error: Unable to open file %s.\n\n", filename);
        exit(1);
    }
     
//    printf ("Writing ouput PGM: %s\n\n", filename);
        
        //write the header
        fprintf(ofp, "P3\n");
    fprintf(ofp, "%d %d\n", pPgm->w, pPgm->h);
    fprintf(ofp, "%d\n", pPgm->max);
     
        //write the image data
        for (i = 0; i < pPgm->h; i++) {
        for (j = 0; j < pPgm->w; j++)
             {
            fprintf(ofp, "%d ", *(pPgm->pData + i * pPgm->w + j) & 255);
            fprintf(ofp, "%d ", (*(pPgm->pData + i * pPgm->w + j) >> 8) & 255);
            fprintf(ofp, "%d ",
                     (*(pPgm->pData + i * pPgm->w + j) >> 16) & 255);
            fprintf(ofp, "\n");
            }
    }
    fclose(ofp);
}


