#include <CEssentials.h>

int main() {

  CE__Stack stack = CE__newStack(sizeof(int));

  CE__pushStack(stack, &(int){0});
  CE__pushStack(stack, &(int){1});
  CE__pushStack(stack, &(int){2});
  CE__pushStack(stack, &(int){3});
  CE__pushStack(stack, &(int){4});
  CE__pushStack(stack, &(int){5});
  CE__pushStack(stack, &(int){6});
  CE__pushStack(stack, &(int){7});
  CE__pushStack(stack, &(int){8});
  CE__pushStack(stack, &(int){9});

  CE__Iterator it = CE__StackBegin(stack);
  do {
    printf("%d\n", castderef(int, CE__IteratorGet(it)));
  } while (CE__IteratorNext(it));

  CE__FreeIterator(it);
  
  CE__popStack(stack);
  CE__popStack(stack);
  CE__popStack(stack);
  CE__popStack(stack);

  it = CE__StackBegin(stack);
  do {
    printf("%d\n", castderef(int, CE__IteratorGet(it)));
  } while (CE__IteratorNext(it));

  CE__FreeIterator(it);

  CE__freeStack(stack);

  return 0;
}
