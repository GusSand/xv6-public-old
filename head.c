#include "types.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

void
head(int fd, int num_lines)
{
  int i, n;
  int l = 0;

  // Read 512 bytes at a time
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      printf(1, "%c", buf[i]);
      if(buf[i] == '\n')
        l++;
      if (l == num_lines) {
        // This is one of the few valid uses of goto -- we need to get
        // out of a nested loop. We could use a break statement in
        // combination with setting a flag, but a goto is cleaner.
        goto done;
      }
    }
  }

done:

  if(n < 0){
    printf(1, "head: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd = 0, i;
  int num_lines = 10;

  if (argc > 3) {
    printf(1, "usage: %s [-NUM] [FILE]\n", argv[0]);
    exit();
  }

  // Note: the specification in the homework didn't say anything
  // about what to do if the user enters multiple -NUM options or
  // multiple filenames. So if that happens here we'll take only
  // the last of each one.
  for (i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      // argv[i]+1 is the number without the leading "-"
      num_lines = atoi(argv[i]+1);
    }
    else {
      fd = open(argv[i], O_RDONLY);
      if (fd < 0) {
        printf(2, "error: couldn't open %s for reading\n", argv[i]);
        exit();
      }
    }
  }
  
  // Now that we have a file descriptor and a number of lines,
  // go ahead and do the work of printing out the lines
  head(fd, num_lines);
  
  // Close the file. Note that OS will actually do this for us
  // when the process exits, but it's good hygiene to close
  // file descriptors when we're done with them.
  if (fd) close(fd);

  exit();
}
