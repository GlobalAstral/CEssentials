#include <CEssentials.h>
#include <CE__Iterator.h>

typedef struct CE__LLNode {
  void* item;
  struct CE__LLNode* next;
  struct CE__LLNode* previous;
} CE__LLNode;

struct CE__LinkedList {
  CE__LLNode* head;
  CE__LLNode* tail;
  size_t length;
  size_t element_size;
};

size_t CE__LinkedListLen(CE__LinkedList ll) {
  return ll->length;
}

CE__LinkedList CE__newLinkedList(size_t element_size) {
  CE__LinkedList ret = (CE__LinkedList)malloc(sizeof(*ret));
  *ret = (struct CE__LinkedList) {
    .head = nullptr,
    .tail = nullptr,
    .length = 0,
    .element_size = element_size
  };
  return ret;
}

void CE__freeLinkedList(CE__LinkedList ll) {
  if (ll->head == nullptr && ll->tail == nullptr)
    return;
  
  CE__LLNode* current = ll->head;

  while (current != nullptr) {
    free(current->item);
    CE__LLNode* next = current->next;
    free(current);
    current = next;
  }
  free(ll);
}

CE__LLNode* getNode(CE__LinkedList ll, size_t index) {
  if (index >= ll->length)
    return nullptr;

  size_t half = ll->length >> 1;

  if (index > half) {
    CE__LLNode* current = ll->tail;
    size_t i = ll->length-1;
    while (current != nullptr) {
      if (i == index)
        return current;
      i--;
      current = current->previous;
    }
    return nullptr;
  }

  CE__LLNode* current = ll->head;
  size_t i = 0;
  while (current != nullptr) {
    if (i == index)
      return current;
    i++;
    current = current->next;
  }
  return nullptr;
}

int CE__insertLinkedList(CE__LinkedList ll, size_t index, void* element) {
  guard(ll == nullptr, VALUE_IS_NULL);
  guard(index > ll->length, INDEX_OUT_OF_BOUNDS);
  guard(element == nullptr, OTHER_VALUE_IS_NULL);

  CE__LLNode* node = (CE__LLNode*)malloc(sizeof(CE__LLNode));
  void* data = malloc(ll->element_size);
  memcpy(data, element, ll->element_size);
  
  *node = (CE__LLNode) {
    .item = data,
  };

  if (ll->head == nullptr && ll->tail == nullptr && index == ll->length) {
    node->next = nullptr;
    node->previous = nullptr;
    ll->head = node;
    ll->tail = node;
    ll->length++;
    return OK;
  }

  if (index == ll->length) {
    node->previous = ll->tail;
    node->next = nullptr;
    ll->tail->next = node;
    ll->tail = node;
    ll->length++;
    return OK;
  }

  CE__LLNode* i = getNode(ll, index);
  guard(i == nullptr, INDEX_OUT_OF_BOUNDS);

  node->previous = i->previous;
  node->next = i;
  i->previous = node;
  ll->length++;
  return OK;
}

int CE__appendLinkedList(CE__LinkedList ll, void* element) {
  return CE__insertLinkedList(ll, ll->length, element);
}

void* CE__LinkedListFind(CE__LinkedList ll, void* item) {
  guard(ll == nullptr, nullptr);
  guard(item == nullptr, nullptr);

  CE__LLNode* current = ll->head;
  while (current != nullptr) {
    if (memcmp(item, current->item, ll->element_size) == 0)
      return current;
    current = current->next;
  }
  return nullptr;
}

bool CE__LinkedListContains(CE__LinkedList ll, void* item) {
  return CE__LinkedListFind(ll, item) != nullptr;
}

int CE__removeLinkedList(CE__LinkedList ll, size_t index) {
  guard(ll == nullptr, VALUE_IS_NULL);
  guard(index >= ll->length, INDEX_OUT_OF_BOUNDS);

  if (index == 0) {
    CE__LLNode* new_head = ll->head->next;
    new_head->previous = nullptr;
    free(ll->head->item);
    free(ll->head);
    ll->head = new_head;
    ll->length--;
    return OK;
  }

  if (index == ll->length-1) {
    CE__LLNode* new_tail = ll->tail->previous;
    new_tail->next = nullptr;
    free(ll->tail->item);
    free(ll->tail);
    ll->tail = new_tail;
    ll->length--;
    return OK;
  }

  CE__LLNode* node = getNode(ll, index);
  node->previous->next = node->next;
  node->next->previous = node->previous;
  free(node->item);
  free(node);
  ll->length--;
  return OK;
}

bool CE__LinkedListNext(CE__Iterator it) {
  CE__LLNode* node = (CE__LLNode*)it->pointer;
  if (node->next == nullptr)
    return false;
  it->pointer = (byte*)node->next;
  return true;
}

void* CE__LinkedListIGet(CE__Iterator it) {
  if (it->pointer == nullptr)
    return nullptr;
  CE__LLNode* node = (CE__LLNode*)it->pointer;
  return node->item;
}

CE__Iterator CE__LinkedListBegin(CE__LinkedList ll) {
  CE__Iterator ret = (CE__Iterator)malloc(sizeof(*ret));
  *ret = (struct CE__Iterator) {
    .length = ll->length,
    .index = 0,
    .step = 0,
    .pointer = (byte*)ll->head,
    .get = CE__LinkedListIGet,
    .next = CE__LinkedListNext,
  };
  return ret;
}

bool CE__LinkedListPrev(CE__Iterator it) {
  CE__LLNode* node = (CE__LLNode*)it->pointer;
  if (node->previous == nullptr)
    return false;
  it->pointer = (byte*)node->previous;
  return true;
}

CE__Iterator CE__LinkedListRBegin(CE__LinkedList ll) {
  CE__Iterator ret = (CE__Iterator)malloc(sizeof(*ret));
  *ret = (struct CE__Iterator) {
    .length = ll->length,
    .index = ll->length-1,
    .step = 0,
    .pointer = (byte*)ll->tail,
    .get = CE__LinkedListIGet,
    .next = CE__LinkedListPrev,
  };
  return ret;
}

void* CE__LinkedListGet(CE__LinkedList ll, size_t index) {
  guard(ll == nullptr, nullptr);
  guard(index >= ll->length, nullptr);

  CE__LLNode* node = getNode(ll, index);
  guard(node == nullptr, nullptr);

  return node->item;
}
