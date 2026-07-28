#include <stdio.h>
#include <CEssentials.h>

int main() {

  CE__String s1 = CE__newString("Ah okay no ma ci sta");

  CE__appendCstr(s1, " Allah");

  printf("%s\n", CE__strcstr(s1));

  CE__freeString(s1);

  return 0;
}
