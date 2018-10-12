
/*
Created by: GREG WOO
Program: Work with semaphore-based synchronizations
*/

#include "A4_sort_helpers.h"

sem_t *semArray[27];

// Function: read_all()
// Provided to read an entire file, line by line.
// No need to change this one.
void read_all( char *filename ){

  FILE *fp = fopen( filename, "r" );
  int curr_line = 0;

  while( curr_line < MAX_NUMBER_LINES &&
    fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) )
    {
      curr_line++;
    }

    text_array[curr_line][0] = '\0';
    fclose(fp);
  }

  // Function: read_all()
  // Provided to read only the lines of a file staring with first_letter.
  // No need to change this one.
  void read_by_letter( char *filename, char first_letter ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
    text_array[curr_line][0] = '\0';

    while( fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) ){
      if( text_array[curr_line][0] == first_letter ){
        curr_line++;
      }

      if( curr_line == MAX_NUMBER_LINES ){
        sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
        write( 1, buf, strlen(buf) );
        break;
      }
    }

    text_array[curr_line][0] = '\0';
    fclose(fp);
  }

  // YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.
  void sort_words( ){

    for(int i = 0; i < MAX_NUMBER_LINES && text_array[i][0] != '\0'; i++) {
      for(int j = (i + 1); j < MAX_NUMBER_LINES && text_array[j][0] != '\0'; j++) {

        if(strcmp(text_array[i], text_array[j]) > 0) {

          char temp[MAX_LINE_LENGTH];
          strcpy(temp, text_array[i]);
          strcpy(text_array[i], text_array[j]);
          strcpy(text_array[j], temp);
        }
      }
    }
  }

  // YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2.
  int initialize( ){

    char semNames[27][50];

    for(int i = 0; i < 27; i++) {

      char a = (char) i;

      sprintf(semNames[i], "sem_%c", a);
      sem_unlink(semNames[i]);

      if (i == 0) {

        semArray[i] = sem_open(semNames[i], O_CREAT, 0666, 1);
      } else {

        semArray[i] = sem_open(semNames[i], O_CREAT, 0666, 0);
      }
    }

    return 0;
  }

  // YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.
  int process_by_letter( char* input_filename, char first_letter ){

    sem_wait(semArray[first_letter - 'a']);

    read_by_letter(input_filename, first_letter);
    sort_words();

    FILE* file = fopen("temp_file.txt","a");

    for (int i = 0; text_array[i][0] != '\0'; i++){

        fprintf(file, "%s", text_array[i]);
    }

    fclose(file);

    sem_post(semArray[first_letter - 'a' + 1]);

    return 0;
  }

  // YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
  int finalize( ){

    sem_wait(semArray[26]);

    FILE *file = fopen("temp_file.txt", "r");

    if(file == NULL) {

      return -1;
    }

    char stringBuffer[MAX_LINE_LENGTH];

    while(!feof(file)){

      strcpy(stringBuffer, "\0");
      fgets(stringBuffer, MAX_LINE_LENGTH, file);
      sprintf(buf, "%s",stringBuffer);
      write(1,buf,strlen(buf));
    }

    fclose(file);
    fopen("temp_file.txt", "w");
    fclose(file);

    sprintf( buf, "Sorting complete!\n" );
    write( 1, buf, strlen(buf) );

    return 0;
  }
