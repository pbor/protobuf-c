#include "t/issue262/issue262.pb-c.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  MyMessage__MySubMessage__MySubSubMessage subsub = MY_MESSAGE__MY_SUB_MESSAGE__MY_SUB_SUB_MESSAGE__INIT;
  MyMessage__MySubMessage__MySubSubMessage *subsubs[1];
  MyMessage__MySubMessage sub = MY_MESSAGE__MY_SUB_MESSAGE__INIT;
  MyMessage__MySubMessage *subs[1];
  MyMessage msg = MY_MESSAGE__INIT;
  MyMessage *msg2;
  unsigned char simple_pad[8];
  size_t size, size2;
  unsigned char *packed;
  ProtobufCBufferSimple bs = PROTOBUF_C_BUFFER_SIMPLE_INIT (simple_pad);

  subsub.a = 4;
  subsub.b = 2;
  subsubs[0] = &subsub;

  sub.a = 3;
  sub.b = 6;
  sub.n_c = 1;
  sub.c = subsubs;
  subs[0] = &sub;

  msg.n_a = 1;
  msg.a = subs;

  size = my_message__get_packed_size (&msg);
  packed = malloc (size);
  assert (packed);

  size2 = my_message__pack (&msg, packed);
  assert (size == size2);

  my_message__pack_to_buffer (&msg, &bs.base);
  assert (bs.len == size);
  assert (memcmp (bs.data, packed, size) == 0);

  PROTOBUF_C_BUFFER_SIMPLE_CLEAR (&bs);

  msg2 = my_message__unpack (NULL, size, packed);
  assert (msg2 != NULL);
  assert (msg2->a[0]->a == 3);
  assert (msg2->a[0]->b == 6);
  assert (msg2->a[0]->c[0]->a == 4);
  assert (msg2->a[0]->c[0]->b == 2);

  my_message__free_unpacked (msg2, NULL);
  free (packed);

  printf ("test succeeded.\n");

  return 0;
}
