/*  Erik Safford
 *  Recursively Traverse a Filepath and List All Readable Files
 *  Spring 2019                                                 */

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#define PATH_LENGTH 1000 //Max filepath length

int listFiles(char *directory) {
	if(access(directory,F_OK) != 0) { //Check access to see if the directory/file exists (F_OK)
		                          //0 = exists, 1 = doesnt exist
		fprintf(stderr,"error = %d : %s, %s doesn't exist\n",errno,strerror(errno), directory);
    		return(0);
	}

	if(access(directory,R_OK) != 0) { //Check access to see if the directory/file is readable (R_OK)
		                          //0 = readable, 1 = not readable
		fprintf(stderr,"error = %d : %s, %s isn't readable\n",errno,strerror(errno), directory);
                return(0);
        }
	
	printf("%s\n",directory); //Print the current working directory

	DIR *dir;                   //Directory pointer
        struct dirent *entry;       //Structure with info referring to a directory entry
        char filePath[PATH_LENGTH]; //Buffer to hold filepaths

	if( (dir = opendir(directory)) == NULL ) { //Attempt to open directory
        	fprintf(stderr,"error = %d : %s, couldn't read %s\n",errno,strerror(errno), directory);
		return(0);
        }
	
	while( (entry = readdir(dir)) != NULL ) { //Attempt to read file(s) in directory

		//If current read directory is current working directory/parent directory
		if( strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0 ) {
            		//fprintf(stderr,"error = Don't want to read '.'\n");
			continue;  //Skip through the remaining part of the while loop
        	}                  //for a single iteration, continue while with next file value
		
		//Append next file in directory to end of filepath
		//snprintf() so we don't have 'accidental' buffer overwrite
		snprintf(filePath,sizeof(filePath) - 1,"%s/%s",directory,entry->d_name);
		//sprintf(filePath,"%s/%s",directory,entry->d_name); <- not as safe

		//Check dirent flags of current directory/file
		if(entry->d_type == DT_REG) { //If the file is a regular file
			if(access(filePath,R_OK) == 0) {  //And the file is readable
				//Print the filepath we have/the file we checked
				printf("%s\n",filePath);
				//printf("%s/%s\n",directory,entry->d_name); <- could have done this way
			}
			else {
				fprintf(stderr, "error: file %s isn't readable\n",filePath);
			}
		}
		else if(entry->d_type == DT_DIR) { //Else if the file is a directory
			listFiles(filePath); //Recursivly list the files in the directory
		}
	}
	if( closedir(dir) == -1 ) { //Close the directory we were working with
		fprintf(stderr,"error = %d : %s\n",errno,strerror(errno));
	}	
}

int main(int argc,char **argv) {

	if(argc == 1) { //If no pathname is present, pathname of present working directory assumed
		
		char directory[PATH_LENGTH];
		if( getcwd(directory,sizeof(directory)) == NULL ) {  //Store cwd in directory[]
			fprintf(stderr,"error = %d : %s\n",errno,strerror(errno));
			exit(1);
		}
		printf("Current directory being read is : %s\n",directory);
		listFiles(directory); //List readable files in directory

    	}
	else if(argc == 2) { //Else if pathname given as a command line argument
		
		char directory[PATH_LENGTH];
		if( chdir(argv[1]) == -1 ) { //Attempt to change to specified directory
			fprintf(stderr,"error = %d : %s\n",errno,strerror(errno));
                        exit(1);
		}
		if( getcwd(directory,sizeof(directory)) == NULL ) { //Store directory changed to
			fprintf(stderr,"error = %d : %s\n",errno,strerror(errno));
                        exit(1);
		}
		printf("Current directory being read is : %s\n",directory);
		listFiles(directory);  //List readable files in directory

	}
	else {  //Else there is too many command line arguments
		
		fprintf(stderr,"error = Too many command line arguments\n");
	
	}
    	return(0);
}
