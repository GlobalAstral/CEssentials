#include <CEssentials.h>

size_t CE__StackLen(CE__Stack stack) {
  return CE__LinkedListLen(stack);
}

CE__Stack CE__newStack(size_t element_size) {
  return CE__newLinkedList(element_size);
}

void CE__freeStack(CE__Stack stack) {
  CE__freeLinkedList(stack);
}

int CE__pushStack(CE__Stack stack, void* element) {
  return CE__appendLinkedList(stack, element);
}

void* CE__popStack(CE__Stack stack) {
  size_t i = CE__StackLen(stack) - 1;
  void* element = CE__LinkedListGet(stack, i);
  CE__removeLinkedList(stack, i);
  return element;
}

void* CE__peekStack(CE__Stack stack) {
  return CE__LinkedListGet(stack, CE__StackLen(stack) - 1);
}

CE__Iterator CE__StackBegin(CE__Stack stack) {
  return CE__LinkedListBegin(stack);
}

CE__Iterator CE__StackRBegin(CE__Stack stack) {
  return CE__LinkedListRBegin(stack);
}
