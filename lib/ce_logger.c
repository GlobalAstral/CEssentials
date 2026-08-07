#include <CEssentials.h>
#include <stdarg.h>

struct CE__Logger {
  FILE* stream;
  bool autoflush;
  bool owned;
};

CE__Logger CE__newLogger(FILE* stream) {
  CE__Logger ret = (CE__Logger)CE__malloc(sizeof(*ret));
  guard(!ret, nullptr);
  ret->stream = stream;
  ret->autoflush = true;
  ret->owned = false;
  return ret;
}

CE__Logger CE__newLoggerPath(char* path) {
  FILE* f = fopen(path, "w");
  CE__Logger temp = CE__newLogger(f);
  temp->owned = true;
  return temp;
}

void CE__freeLogger(CE__Logger self) {
  if (self->owned)
    fclose(self->stream);
  CE__free(self);
}

int CE__log(CE__Logger self, char* prefix, char* color, char* fmt, va_list list) {
  int t = fprintf(self->stream, "%s", color);
  t += fprintf(self->stream, "[%s] ", prefix);
  t += vfprintf(self->stream, fmt, list);
  t += fprintf(self->stream, "\033[0m\n");
  if (self->autoflush)
    fflush(self->stream);
  return t;
}

int CE__LoggerTrace(CE__Logger self, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int r = CE__log(self, "TRACE", CE_ANSI_WHITE, fmt, list);
  va_end(list);
  return r;
}

int CE__LoggerDebug(CE__Logger self, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int r = CE__log(self, "DEBUG", CE_ANSI_CYAN, fmt, list);
  va_end(list);
  return r;
}

int CE__LoggerInfo(CE__Logger self, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int r = CE__log(self, "INFO", CE_ANSI_GREEN, fmt, list);
  va_end(list);
  return r;
}

int CE__LoggerWarn(CE__Logger self, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int r = CE__log(self, "WARN", CE_ANSI_YELLOW, fmt, list);
  va_end(list);
  return r;
}

int CE__LoggerError(CE__Logger self, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int r = CE__log(self, "ERROR", CE_ANSI_RED, fmt, list);
  va_end(list);
  return r;
}

int CE__LoggerFatal(CE__Logger self, char* fmt, ...) {
  va_list list;
  va_start(list, fmt);
  int r = CE__log(self, "FATAL", CE_ANSI_MAGENTA, fmt, list);
  va_end(list);
  return r;
}
