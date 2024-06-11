#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 2048

#define ASSERT(_e, ...)                                                        \
  if (!(_e)) {                                                                 \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  }

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

typedef enum { DEBUG, INFO, WARNING, ERROR } LOG_LEVEL;

void logger_init(LOG_LEVEL base_level, char *base_fpath);
void logger_log(LOG_LEVEL log_level, const char *log_fname, const size_t line,
                const char *fmt, ...);
void logger_deinit(void);

#define DEB(...) logger_log(DEBUG, __FILENAME__, __LINE__, __VA_ARGS__)
#define LOG(...) logger_log(INFO, __FILENAME__, __LINE__, __VA_ARGS__)
#define WARN(...) logger_log(WARNING, __FILENAME__, __LINE__, __VA_ARGS__)
#define ERR(...) logger_log(ERROR, __FILENAME__, __LINE__, __VA_ARGS__)

#endif /* LOGGER_H */
