  This C program is targeted at the Linux platfrom and take one or zero command line 
  arguments. The argument, if present, should be a file pathname (relative/absolute).
  If no argument is present, the file pathname of the present working directory is
  assumed (getcwd).

  After reading in a file pathname, the program recursively traverses the directories
  and their files, starting with the pathname, and prints every filepath that is readable
  by the current process. No order is defined for the paths that are output.

  Notable functions used include: chdir() to change to a specified directory, getcwd() to get
  the current working directory, access() to test the file pathname accessibility 
  (exists/reading), opendir(), readdir(), and closedir(), along with the dirent structure,
  to access the readable files/names/types within a file pathname. 

  A makefile is included with the following commands:
     -all   : Compiles everything together and produces an executable
     -run   : Command for running executable with specified pathname,
              i.e. make run fp=/remotehomes/erik.safford/Documents  
                   (print filepaths of all directories/files in Documents),
                   ** make run fp=_  (blank) does not work
                   ** for zero command line arguments simply use: ./readable
                   ** can also just do: ./readable /remotehomes/erik.safford/Documents
     -test  : Test on big file system (/), all fprintf errors to stderr (2) go to a created
              file error.txt
     -clean : Removes any object/temp/error files that were created with all/test

  In recursive function listFiles() : 

  access() : checks the file pathname for accessibility according to a bit pattern.
    - i.e. access(filePath,F_OK) - test to see if file exists
           access(filePath,R_OK) - test to see if file is readable

  dirent : stores information about a read file/directory
    struct dirent {
      ino_t          d_ino;       /* inode number */
      off_t          d_off;       /* offset to the next dirent */
      unsigned short d_reclen;    /* length of this record */
      unsigned char  d_type;      /* type of file; not supported
                                     by all file system types */
      char           d_name[256]; /* filename */
    };

  When a directory is opened (opendir) it is set to a directory pointer DIR*. In order
  to read files from the directory it must be read with readdir, which is set to a
  struct dirent* entry that contains information about the given directory/file. While the
  dirent entry isnt NULL, we still have files to be read from the opened directory. Each
  time a file is read from the directory, it is appended to the end of the current directory
  we are reading from with a /, i.e. sprintf(filePath,"%s/%s",directory,entry->d_name);,
  effectively creating the filepath for each individually read file/directory. For each file
  that is read, the dirent structure entry allows us to read the name of the file 
  (entry->d_name) and if the type of file is a regular file or a directory file 
  (entry->d_type == DT_REG/DT_DIR).

  If the newly appended filepath is a regular file, we again check if the file is readable
  with access(filePath,R_OK), and print the filePath if it is. However, if the filepath is
  a directory, we recursively call listFiles() to run the whole process again with the newly
  appended filePath/directory. 

  If . (current directory) or .. (parent directory) is ever read, in order to keep the
  recursion of listFiles(), a continue statement continues the function. 

  Error catching is implemented to catch and print any errors with the appropriate error 
  number and the message produced by that error to stderr (errno.h and string.h).
