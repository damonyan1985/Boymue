#ifndef Image_h
#define Image_h

#include "SkBitmap.h"
#include "StringUtil.h"
#include <cstdint>

namespace boymue {

class ImageCache;
class ImageLoader;
class TaskRunner;

/// 图片加载完成通知（本地同步或网络异步）；调用时 Image::bitmap() 已为最新内容。
class ImageLoadClient {
public:
    virtual ~ImageLoadClient() = default;
    virtual void onImageLoadComplete() = 0;
};

class Image {
public:
    Image();
    /// 网络图 bmnet 返回后的解码仍在工作线程；用户 NetworkCallback / ImageLoadClient 在该 runner 上执行（如 UI 线程）。传 nullptr 则在工作线程直接回调。
    static void setNetworkImageUiTaskRunner(TaskRunner* runner);
    void createImage(const void* buffer, size_t size);
    /// 统一加载本地或网络资源（本地经 ImageLoader::loadLocal）；client 非空时在加载结束后调用 onImageLoadComplete（本地为同步调用）
    bool load(const String& pathOrUrl, ImageLoadClient* client = nullptr);
    const SkBitmap& bitmap() const;

    ~Image();

private:
    SkBitmap m_bitmap;
    /// 当前未完成网络请求在 ImageLoader 中的登记 id（0 表示无）
    uint64_t m_networkRequestId = 0;
};
}

#endif // !Image_h
