#include <stdio.h>
#include <CEssentials.h>

int main() {

  CE__String s1 = CE__newString("Ah ok no ❤️ ma ci sta");
  CE__String s2 = CE__newString("real");

  CE__insertString(s1, 11, s2);

  printf("result: ");
  CE__printstr(s1);

  CE__freeString(s1);
  CE__freeString(s2);

  return 0;
}
