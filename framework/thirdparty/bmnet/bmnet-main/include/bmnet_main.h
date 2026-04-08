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

/** ArgumentList 参数类型（与 Argument.arg_type 取值一致，该字段仍为 uint8_t 以保证 ABI/布局稳定） */
typedef enum {
    BMNET_ARG_I32 = 1,
    BMNET_ARG_STRING = 2,
    BMNET_ARG_USIZE = 3,
} BmnetArgType;

/** BMNET_ARG_STRING：UTF-8 字节序列，str_len 为字节数（不含末尾 '\\0'） */
typedef struct {
    const char *str_value;
    size_t str_len;
} ArgumentString;

// ArgumentList 相关类型定义（用于 convert_args 宏生成的函数）
typedef union {
    int32_t i32_value;       /* BMNET_ARG_I32 */
    size_t usize_value;      /* BMNET_ARG_USIZE */
    ArgumentString str;      /* BMNET_ARG_STRING */
} ArgumentValue;

typedef struct {
    uint8_t arg_type; /* BmnetArgType，如 BMNET_ARG_I32 / BMNET_ARG_STRING / BMNET_ARG_USIZE */
    ArgumentValue arg_value;
} Argument;

typedef struct {
    int32_t arg_count;
    Argument* arg_list;
} ArgumentList;

typedef void (*FnCallback)(ArgumentList args);

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
//   result_args[0] = String (响应内容，ArgumentString: str_value + str_len 字节)
//   result_args[1] = usize (ext 参数，原样返回)
BOYMUE_PORTING_API void bmnet_get_ext_impl(ArgumentList args, FnCallback callback);

#ifdef __cplusplus
}
#endif

#endif