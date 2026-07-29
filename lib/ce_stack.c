#include <CEssentials.h>

size_t CE__StackLen(CE__Stack stack) {
  return CE__ArrayListLen(stack);
}

size_t CE__StackEleSz(CE__Stack stack) {
  return CE__ArrayListEleSz(stack);
}

CE__Stack CE__newStack(size_t element_size) {
  return CE__newArrayList(element_size);
}

void CE__freeStack(CE__Stack stack) {
  CE__freeArrayList(stack);
}

int CE__pushStack(CE__Stack stack, void* element) {
  return CE__appendArrayList(stack, element);
}

void* CE__popStack(CE__Stack stack) {
  return CE__popArrayList(stack);
}

void* CE__peekStack(CE__Stack stack) {
  return CE__ArrayListAt(stack, CE__StackLen(stack) - 1);
}

CE__Iterator CE__StackBegin(CE__Stack stack) {
  return CE__ArrayListBegin(stack);
}

CE__Iterator CE__StackRBegin(CE__Stack stack) {
  return CE__ArrayListRBegin(stack);
}
