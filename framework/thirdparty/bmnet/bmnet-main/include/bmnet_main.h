#ifndef bmnet_main_h
#define bmnet_main_h

#ifdef _WINDOWS
#ifdef _WINDLL
#define BOYMUE_PORTING_API __declspec(dllexport)
#else
#define BOYMUE_PORTING_API __declspec(dllimport)
#endif
#else
#define BOYMUE_PORTING_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef void (*bmnet_get_callback)(const uint8_t *data, size_t len, uintptr_t ext);

// ArgumentList 相关类型定义（用于 convert_args 宏生成的函数）
typedef union {
    int32_t i32_value;      // 类型码 1 = i32
    size_t usize_value;      // 类型码 3 = usize
    const char* str_value;   // 类型码 2 = String
} ArgumemntValue;

typedef struct {
    uint8_t arg_type;  // 1 = i32, 2 = String, 3 = usize
    ArgumemntValue arg_value;
} Argumemnt;

typedef struct {
    int32_t arg_count;
    Argumemnt* arg_list;
} ArgumemntList;

typedef void (*FnCallback)(ArgumemntList args);

// headers为json串
BOYMUE_PORTING_API void bmnet_get(const char* url, 
    const char* headers, 
    bmnet_get_callback cb, 
    uintptr_t ext);
BOYMUE_PORTING_API void bmnet_post(const char* url, 
    const char* headers, 
    const char* data, 
    bmnet_get_callback cb,
    uintptr_t ext);

void bmnet_timeout(uint64_t time);

// 使用 convert_args 宏实现的 HTTP GET 请求
// 参数：args[0] = url (String), args[1] = headers (String, 可选，JSON 格式字符串), args[2] = ext (usize)
// 返回值通过 callback 返回，类型为 (String, usize) 元组
//   result_args[0] = String (响应内容)
//   result_args[1] = usize (ext 参数，原样返回)
BOYMUE_PORTING_API void bmnet_get_ext_impl(ArgumemntList args, FnCallback callback);

#ifdef __cplusplus
}
#endif

#endif