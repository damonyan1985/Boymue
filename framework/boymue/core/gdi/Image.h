#ifndef Image_h
#define Image_h

#include "SkBitmap.h"
#include <cstdint>

namespace boymue {

class ImageCache;
class ImageLoader;
class TaskRunner;

class Image {
public:
    using LoadCallback = void(*)(bool success, const SkBitmap& bitmap, void* userData);

    Image();
    /// 网络图 bmnet 返回后的解码仍在工作线程；用户 NetworkCallback / Image::LoadCallback 在该 runner 上执行（如 UI 线程）。传 nullptr 则在工作线程直接回调。
    static void setNetworkImageUiTaskRunner(TaskRunner* runner);
    void createImage(const void* buffer, size_t size);
    /// 从本地路径解码（对标资源/文件加载）
    bool loadFromFile(const char* path);
    /// 统一加载本地或网络资源；网络请求为异步回调
    bool load(const char* pathOrUrl, LoadCallback callback = nullptr, void* userData = nullptr);
    const SkBitmap& bitmap() const;

    ~Image();

private:
    SkBitmap m_bitmap;
    /// 当前未完成网络请求在 ImageLoader 中的登记 id（0 表示无）
    uint64_t m_networkRequestId = 0;
};
}

#endif // !Image_h
