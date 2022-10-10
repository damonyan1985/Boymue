#ifndef bmnet_main_h
#define bmnet_main_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef void (*bmnet_get_callback)(const uint8_t *data, size_t len, uintptr_t ext);

// headers为json串
void bmnet_get(const char* url, 
    const char* headers, 
    bmnet_get_callback cb, 
    uintptr_t ext);
void bmnet_post(const char* url, 
    const char* headers, 
    const char* data, 
    bmnet_get_callback cb,
    uintptr_t ext);

void bmnet_timeout(uint64_t time);    

#ifdef __cplusplus
}
#endif

#endif