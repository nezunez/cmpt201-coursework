#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(char *str) {
  write(STDERR_FILENO, str, strlen(str));
  exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *start = sbrk(256);
  if (start == (void *)-1)
    handle_error("sbrk failed");

  struct header *block1 = (struct header *)start;
  struct header *block2 = (struct header *)((char *)start + 128);

  block1->size = 128;
  block1->next = NULL;

  block2->size = 128;
  block2->next = block1;

  print_out("first block: %p\n", &block1, sizeof(block1));  // block1 address
  print_out("second block: %p\n", &block2, sizeof(block2)); // block2 address
  print_out("first block size: %lu\n", &block1->size,
            sizeof(block1->size)); // block1 size
  print_out("first block next: %p\n", &block1->next,
            sizeof(block1->next)); // block1 next
  print_out("second block size: %lu\n", &block2->size,
            sizeof(block2->size)); // block2 size
  print_out("second block next: %p\n", &block2->next,
            sizeof(block2->next)); // block2 next

  uint8_t *data1 = (uint8_t *)block1 + sizeof(struct header);
  uint8_t *data2 = (uint8_t *)block2 + sizeof(struct header);
  size_t data_size = 128 - sizeof(struct header);

  memset(data1, 0x00, data_size);
  memset(data2, 0x01, data_size);

  printf("Printing the header a second time:\n");
  print_out("first block size: %lu\n", &block1->size, sizeof(block1->size));
  print_out("first block next: %p\n", &block1->next, sizeof(block1->next));
  print_out("second block size: %lu\n", &block2->size, sizeof(block2->size));
  print_out("second block next: %p\n", &block2->next, sizeof(block2->next));

  for (size_t i = 0; i < data_size; i++) {
    uint64_t val = data1[i];
    print_out("%lu\n", &val, sizeof(val));
  }

  for (size_t i = 0; i < data_size; i++) {
    uint64_t val = data2[i];
    print_out("%lu\n", &val, sizeof(val));
  }

  return 0;
}
