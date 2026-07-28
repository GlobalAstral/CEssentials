#include <CEssentials.h>
#include <stdio.h>

int main() {

  CE__LinkedList ll = CE__newLinkedList(sizeof(int));

  CE__appendLinkedList(ll, &(int){0});
  CE__appendLinkedList(ll, &(int){1});
  CE__appendLinkedList(ll, &(int){2});
  CE__appendLinkedList(ll, &(int){3});
  CE__appendLinkedList(ll, &(int){4});
  CE__appendLinkedList(ll, &(int){5});
  CE__appendLinkedList(ll, &(int){6});
  CE__appendLinkedList(ll, &(int){7});
  CE__appendLinkedList(ll, &(int){8});
  CE__appendLinkedList(ll, &(int){9});
  
  CE__Iterator it = CE__LinkedListBegin(ll);
  do {
    int x = castderef(int, CE__IteratorGet(it));
    printf("%d\n", x);
  } while (CE__IteratorNext(it));

  CE__FreeIterator(it);

  bool flag = CE__LinkedListContains(ll, &(int){8});
  printf("Contains 8: %d\n", flag);

  CE__removeLinkedList(ll, 0);

  it = CE__LinkedListBegin(ll);

  do {
    int x = castderef(int, CE__IteratorGet(it));
    printf("%d\n", x);
  } while (CE__IteratorNext(it));

  CE__FreeIterator(it);

  CE__freeLinkedList(ll);

  return 0;
}
