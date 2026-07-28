#include <CEssentials.h>

int main() {
  puts("Creating...");
  CE__ArrayList arr = CE__newArrayList(sizeof(int));
  puts("Appending...");
  CE__appendArrayList(arr, &(int){1});
  CE__appendArrayList(arr, &(int){2});
  CE__appendArrayList(arr, &(int){3});
  CE__appendArrayList(arr, &(int){4});
  CE__appendArrayList(arr, &(int){5});
  CE__appendArrayList(arr, &(int){6});
  CE__appendArrayList(arr, &(int){7});
  CE__appendArrayList(arr, &(int){8});
  CE__appendArrayList(arr, &(int){9});

  CE__Iterator it = CE__ArrayListBegin(arr);

  do {
    printf("%d\n", castderef(int, CE__IteratorGet(it)));
  } while (CE__IteratorNext(it));
  
  puts("Inserting...");
  CE__insertArrayList(arr, 6, &(int){9});

  CE__FreeIterator(it);
  it = CE__ArrayListBegin(arr);

  do {
    printf("%d\n", castderef(int, CE__IteratorGet(it)));
  } while (CE__IteratorNext(it));

  printf("Contains 7: %d\n", CE__ArrayListContains(arr, &(int){7}));

  puts("Remove index 6");
  CE__removeArrayList(arr, 6);

  CE__FreeIterator(it);
  it = CE__ArrayListBegin(arr);

  do {
    printf("%d\n", castderef(int, CE__IteratorGet(it)));
  } while (CE__IteratorNext(it));

  puts("Section...");
  CE__ArrayList sec = CE__ArrayListSection(arr, 3, 8);

  CE__FreeIterator(it);
  it = CE__ArrayListBegin(sec);

  do {
    printf("%d\n", castderef(int, CE__IteratorGet(it)));
  } while (CE__IteratorNext(it));

  
  puts("Freeing...");
  CE__FreeIterator(it);
  CE__freeArrayList(arr);
  return 0;
}
