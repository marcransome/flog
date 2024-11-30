#include <stdlib.h>

extern void mock_assert(const int result, const char * const expression,
    const char * const file, const int line);

#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);

extern void * _test_malloc(const size_t size, const char *file, const int line);
extern void * _test_realloc(void *ptr, const size_t size, const char *file, const int line);
extern void * _test_calloc(const size_t number_of_elements, const size_t size,
                   const char *file, const int line);
extern void _test_free(void * const ptr, const char *file, const int line);

#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size, file, line) _test_realloc(ptr, size, __FILE__, __LINE__)
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)
