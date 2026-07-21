#include <stdio.h>
#include <CEssentials.h>

int main() {

  CE__String s1 = CE_newString("Ah ok no ❤️ ma ci sta");
  CE__String s2 = CE_newString("real");

  CE_insertString(&s1, 11, &s2);

  printf("result: %s", s1.buffer);

  CE_freeString(&s1);
  CE_freeString(&s2);

  return 0;
}
