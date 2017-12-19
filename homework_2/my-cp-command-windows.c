#include <windows.h>
#include <stdio.h>

void main(int argc, char* argv[])
{



  /*declare all variables*/

  HANDLE hFile;
  HANDLE hAppend;
  DWORD  dwBytesRead, dwBytesWritten, dwPos;
  BYTE   buff[1024];
  int totalBytesWritten = 0;



  /*check for appropriate number of arguments*/

  if (argc < 3 || argc > 3) 
  {
     printf("Incorrect syntax. To use the program, type 'mycp <filename> <filename>'.\n");
     return;
  }



  /*open the first file, if it exists*/

  hFile = CreateFile(TEXT(argv[1]), // open first file
            GENERIC_READ,             // open for reading
            0,                        // do not share
            NULL,                     // no security
            OPEN_EXISTING,            // existing file only
            FILE_ATTRIBUTE_NORMAL,    // normal file
            NULL);                    // no attr. template

  if (hFile == INVALID_HANDLE_VALUE)
  {
     printf("File ");
     printf(argv[1]);
     printf(" could not be opened!\n"); 
     return;
  }



  /*open the existing file*/
  /*if the file does not exist, create a new file*/

  hAppend = CreateFile(TEXT(argv[2]), // open second file
              FILE_APPEND_DATA,         // open for writing
              FILE_SHARE_READ,          // allow multiple readers
              NULL,                     // no security
              CREATE_NEW,              // open or create
              FILE_ATTRIBUTE_NORMAL,    // normal file
              NULL);                    // no attr. template

  if (hAppend == INVALID_HANDLE_VALUE)
  {
     printf("File ");
     printf(argv[2]);
     printf(" already exists!\n"); 
     return;
  }



while (1 < 2)
{


  if (!ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL)) 
  {
     printf("Source file not read from. Error %d", GetLastError());
     return;
  }


  // Check for EOF reached
  if (dwBytesRead == 0) 
  {  
     printf("copied %d bytes\n", totalBytesWritten);
     return;
  }


  // Write file, check for error
  if (!WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL)) 
  {
     printf("Target file not written to. Error %u", GetLastError());
     return;
  }


  totalBytesWritten += dwBytesWritten;
  
}


  /*close both files*/

  CloseHandle(hFile);
  CloseHandle(hAppend);



}

