#include <CEssentials.h>

int main() {
  CE__ArrayList arr = CE__newArrayList(sizeof(int));

  CE__appendArrayList(&arr, &(int){1});
  CE__appendArrayList(&arr, &(int){2});
  CE__appendArrayList(&arr, &(int){3});
  CE__appendArrayList(&arr, &(int){4});
  CE__appendArrayList(&arr, &(int){5});
  CE__appendArrayList(&arr, &(int){6});
  CE__appendArrayList(&arr, &(int){7});
  CE__appendArrayList(&arr, &(int){8});
  CE__appendArrayList(&arr, &(int){9});

  for (size_t i = 0; i < arr.length; i++)
    printf("%d\n", *(arr.buffer + i * arr.element_size));
  
  CE__insertArrayList(&arr, 6, &(int){9});

  for (size_t i = 0; i < arr.length; i++)
    printf("%d\n", *(arr.buffer + i * arr.element_size));

  printf("Contains: %d\n", CE__ArrayListContains(&arr, &(int){7}));

  CE__ArrayListView sec = CE__ArrayListSection(&arr, 3, 8);

  for (size_t i = 0; i < sec->length; i++)
    printf("%d\n", *(sec->buffer + i * sec->element_size));

  CE__freeArrayList(&arr);
  return 0;
}
