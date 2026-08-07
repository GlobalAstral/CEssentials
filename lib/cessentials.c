#include <CEssentials.h>

static const char* const ERRORS[] = {
  "SUCCESS",
  "Value is null",
  "Other value is null",
  "Value is freed",
  "Other value is freed",
  "Cannot Allocate",
  "Index Out of Bounds",
  "Type is Empty",
  "Not Found"
};

const char* const CE__converrno(int err) {
  size_t index = -err;
  if (index >= sizeof(ERRORS) / sizeof(char*))
    return nullptr;
  return ERRORS[index];
}

void* CE__memdup(void* mem, size_t size) {
  void* ret = CE__malloc(size);
  guard(ret == nullptr, nullptr);
  memcpy(ret, mem, size);
  return ret;
}

int CE__memswap(void* mem1, void* mem2, size_t size) {
  if (mem1 == mem2)
    return OK;
  if (memcmp(mem1, mem2, size) == 0)
    return OK;
  void* temp = CE__memdup(mem1, size);
  guard(!temp, CANNOT_ALLOCATE);
  memmove(mem1, mem2, size);
  memmove(mem2, temp, size);
  CE__free(temp);
  return OK;
}

unsigned long long CE__alignUp(unsigned long long num, unsigned long long alignment) {
  if (!alignment)
    return num;
  return (((num + alignment - 1) / alignment) * alignment);
}

bool CE__isAligned(unsigned long long num, unsigned long long alignment) {
  if (alignment == 0)
    return false;
  return (num % alignment) == 0;
}

bool CE__fileExists(char* path) {
  FILE* f = fopen(path, "r");
  guard(f == nullptr, false);
  fclose(f);
  return true;
}

char* CE__getFilename(char* path) {
  char* ret = path;

  while (*path) {
    if (*path == PATH_SEPARATOR)
      ret = path + 1;
    path++;
  }
  return ret;
}

char* CE__getExtension(char* path) {
  char* fname = CE__getFilename(path);
  char* original = fname;
  char* ret = nullptr;

  while (*fname) {
    if (*fname == '.' && fname != original)
      ret = fname + 1;
    fname++;
  }
  return ret;
}

static CE__Malloc _m = malloc;
static CE__Realloc _r = realloc;
static CE__Calloc _c = calloc;
static CE__Free _f = free;

void CE__selectAllocators(CE__Malloc m, CE__Realloc r, CE__Calloc c, CE__Free f) {
  _m = m;
  _r = r;
  _c = c;
  _f = f;
}

void* CE__malloc(size_t size) {
  return _m(size);
}

void* CE__realloc(void* buf, size_t size) {
  return _r(buf, size);
}

void* CE__calloc(size_t size, size_t ele_size) {
  return _c(size, ele_size);
}

void CE__free(void* buf) {
  _f(buf);
}

size_t CE__fileSize(char* path) {
  FILE* f = fopen(path, "rb");
  guard(f == nullptr, 0);

  const size_t buf_size = 4096;
  char buf[buf_size];
  size_t chars;
  size_t filesize = 0;

  while ((chars = fread(buf, sizeof(char), buf_size, f)) > 0)
    filesize += chars;

  fclose(f);
  
  return filesize;
}

size_t CE__fileSize(char* path) {
  guard(path == nullptr, -1);
  FILE* f = fopen(path, "rb");
  guard(f == nullptr, -1);

  const size_t cap = 4096;

  char buf[cap];
  size_t len = 0;
  size_t read;
  
  while ((read = fread(buf, 1, cap, f)) > 0)
    len += read;

  return len;
}

char* CE__readFile(char* path, size_t* length) {
  guard(path == nullptr, nullptr);
  guard(length == nullptr, nullptr);

  FILE* f = fopen(path, "rb");
  guard(f == nullptr, nullptr);

  size_t capacity = 4096;
  char* buffer = (char*)CE__malloc(capacity);
  if (buffer == nullptr) {
    fclose(f);
    return nullptr;
  }

  size_t len = 0;
  size_t read;
  while (true) {
    if (len >= capacity) {
      capacity *= 2;
      char* new_ptr = (char*)CE__realloc(buffer, capacity);
      if (new_ptr == nullptr) {
        CE__free(buffer);
        fclose(f);
        return nullptr;
      }
      buffer = new_ptr;
    }

    read = fread(buffer + len, 1, capacity - len, f);

    if (read == 0)
      break;
    
    len += read;
  }

  char* new_ptr = (char*)CE__realloc(buffer, len + 1);
  if (new_ptr == nullptr) {
    CE__free(buffer);
    fclose(f);
    return nullptr;
  }
  buffer = new_ptr;
  buffer[len] = 0;
  *length = len;

  fclose(f);
  return buffer;
}

int CE__vwriteFile(char* path, char* fmt, va_list list) {
  guard(path == nullptr, -1);
  guard(fmt == nullptr, -1);

  FILE* f = fopen(path, "w");
  if (f == nullptr)
    return -1;
  int temp = vfprintf(f, fmt, list);
  fclose(f);
  return temp;
}

int CE__writeFile(char* path, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int temp = CE__vwriteFile(path, fmt, list);
  va_end(list);
  return temp;
}

int CE__vappendFile(char* path, char* fmt, va_list list) {
  guard(path == nullptr, -1);
  guard(fmt == nullptr, -1);
  FILE* f = fopen(path, "a");
  if (f == nullptr)
    return -1;
  int temp = vfprintf(f, fmt, list);
  fclose(f);
  return temp;
}

int CE__appendFile(char* path, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int temp = CE__vappendFile(path, fmt, list);
  va_end(list);
  return temp;
}
