#include<stdio.h>

// There's always a final Newline at the end of files that needs to be ignored
const int EOF_OFFSET = 2;

main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3)
  {
    printf("--------------------------------\n");
    printf("Welcome to the Read-File utility\n");
    printf("--------------------------------\n");
    printf("\n");
    printf("This is designed to watch lines get appended to a file\n");
    printf("\n");
    printf("Usage 1: Start from beginning\n");
    printf("         rf.exe <file-path>\n");
    printf("\n");
    printf("Usage 2: skip bytes then read\n");
    printf("         rf.exe <file-path> <bytes-to-skip>\n");
    printf("\n");
    printf("Usage 3: skip to end of file\n");
    printf("         rf.exe <file-path> -1\n");
    printf("\n");
    printf("!!When a file reduces in size the whole file is printed to the screen!!\n");
    return;
  }

  char *path = argv[1];
  FILE *f;
  int c;
  int b_nl = 0;
  long int len = 0;
  long int clen = 0;
  long skipBytes = 0;


  // set skipBytes
  if(argc == 3)
  {
    char *eptr;
    /* Convert the provided value to a decimal long */
    skipBytes = strtol(argv[2], &eptr, 10);
    if (skipBytes == 0 && (*eptr != '\0'))
    {
      printf("Conversion error occurred:%s\n", argv[2]);
      return;
    }

    if(skipBytes == -1 && (f = fopen(path,"r")))
    {
      fseek(f, 0L, SEEK_END);
      skipBytes = ftell(f) - EOF_OFFSET;
      fclose(f);
    }
  }

  // We're now watching this file
  while(1)
  {
    if(f = fopen(path,"r"))
    {
      // Get size of file
      fseek(f, 0L, SEEK_END);
      clen = ftell(f);

      // We only want to print file contents that are 'new'
      if(clen != len)
      {
        b_nl = 0;
        if(len > 0L)
          fseek(f, len - EOF_OFFSET, SEEK_SET);
        else 
          fseek(f, skipBytes, SEEK_SET);

        // Write remainder of file to screen
        while ((c = getc(f)) != EOF)
        {
          // avoiding printing the final newline 0xa
          if(b_nl)
            putchar(0xa);
          if(!(b_nl = (c == 0xa)))
            putchar(c);
        }

        // Update to the spot we've read to.
        if(len < clen)
          len = clen;
        else
        {
          // the file decreased in size. We're gonna reprint it
          len = 0;
          skipBytes = 0;
        }
      }
      fclose(f);
    }
    usleep(10000);
  }
}
