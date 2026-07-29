#include <CEssentials.h>

#define strequ(a, b) (strcmp(a, b) == 0)

int arraylist() {
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

int linkedlist() {
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

int stack() {
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

int string1() {
  CE__String s1 = CE__newString("Ah ok no ❤️ ma ci sta");
  CE__String s2 = CE__newString("real");

  CE__insertString(s1, 11, s2);

  printf("result: ");
  CE__printstr(s1);

  CE__freeString(s1);
  CE__freeString(s2);

  return 0;
}

int string2() {
  CE__String s1 = CE__newString("Ah okay no ma ci sta");

  CE__appendCstr(s1, " Allah");

  printf("%s\n", CE__strcstr(s1));

  CE__freeString(s1);

  return 0;
}

int string3() {
  CE__String s1 = CE__newString("Ah okay no ma ci sta");
  CE__String s2 = CE__newString("real");
  CE__String s3 = CE__newString("real");

  int c1 = CE__strcmp(s1, s2);
  int c2 = CE__strcmp(s2, s3);
  int c3 = CE__strcmp(s1, s3);

  char* t1 = CE__strcstr(s1);
  char* t2 = CE__strcstr(s2);
  char* t3 = CE__strcstr(s3);

  printf("%s <> %s = %d\n%s <> %s = %d\n%s <> %s = %d\n", t1, t2, c1, t2, t3, c2, t1, t3, c3);

  free(t1);
  free(t2);
  free(t3);

  CE__String temp = CE__newString("okay");

  CE__String r = CE__strfind(s1, temp);

  CE__freeString(temp);

  printf("%s\n", CE__strcstr(r));

  free(r);

  CE__strdrain(s1, 3, 9);

  char* tmp = CE__strcstr(s1);

  printf("%s\n", tmp);

  free(tmp);

  CE__freeString(s1);
  CE__freeString(s2);
  CE__freeString(s3);

  return 0;
}

int bitfield() {

  CE__BitField field = CE__newBitField();

  CE__setBitField(field, 0, true);

  CE__freeBitField(field);

  return 0;
}

int main(int argc, char* argv[]) {

  if (argc != 2) {
    perror("Invalid CLI arguments");
    return 1;
  }

  char* test = argv[1];

  if (strequ(test, "arraylist"))
    return arraylist();
  if (strequ(test, "linkedlist"))
    return linkedlist();
  if (strequ(test, "stack"))
    return stack();
  if (strequ(test, "string1"))
    return string1();
  if (strequ(test, "string2"))
    return string2();
  if (strequ(test, "string3"))
    return string3();
  if (strequ(test, "bitfield"))
    return bitfield();

  return 0;
}
