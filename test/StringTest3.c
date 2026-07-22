#include <stdio.h>
#include <CEssentials.h>

int main() {

  CE__String s1 = CE__newString("Ah okay no ma ci sta");
  CE__String s2 = CE__newString("real");
  CE__String s3 = CE__newString("real");

  int c1 = CE__strcmp(&s1, &s2);
  int c2 = CE__strcmp(&s2, &s3);
  int c3 = CE__strcmp(&s1, &s3);

  char* t1 = CE__strcstr(&s1);
  char* t2 = CE__strcstr(&s2);
  char* t3 = CE__strcstr(&s3);

  printf("%s <> %s = %d\n%s <> %s = %d\n%s <> %s = %d\n", t1, t2, c1, t2, t3, c2, t1, t3, c3);

  free(t1);
  free(t2);
  free(t3);

  CE__String temp = CE__newString("okay");

  CE__String* r = CE__find(&s1, &temp);

  printf("%s\n", CE__strcstr(r));

  free(r);

  CE__freeString(&s1);
  CE__freeString(&s2);
  CE__freeString(&s3);

  return 0;
}
