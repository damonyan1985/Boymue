#ifndef bmnet_main_h
#define bmnet_main_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef void (*bmnet_get_callback)(const uint8_t *data, size_t len);

void bmnet_get(const char* url, bmnet_get_callback cb);

#ifdef __cplusplus
}
#endif

#endif