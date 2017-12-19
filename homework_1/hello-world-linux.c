#include <stdio.h>
#include <string.h>
#define NAME "Brandon Glajchen"
#define BUFSIZE 35

int main () {
  char buf[BUFSIZE];

  snprintf (buf, BUFSIZE, "Hello 415, I am %s!\n", NAME);
  //printf ("%s\n", buf);
  write(2, buf, BUFSIZE);
}
