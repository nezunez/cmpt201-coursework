#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L

int main(void) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  printf("Please enter some text: ");

  nread = getline(&line, &len, stdin);

  if (nread == -1) {
    perror("getline");
    free(line);
    return -1;
  }

  printf("Tokens:\n");

  char *saveptr;
  char *ret = strtok_r(line, " ", &saveptr);

  while (ret != NULL) {
    printf(" %s\n", ret);
    ret = strtok_r(NULL, " ", &saveptr);
  }

  free(line);
  return 0;
}
