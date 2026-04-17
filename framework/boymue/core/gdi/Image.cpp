#include "Image.h"
#include "Loader.h"
#include "SkImageDecoder.h"
#include "StringUtil.h"
#include "TaskRunner.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <mutex>
#include <sstream>
#include <utility>

namespace boymue {

class ImageCache {
public:
    ImageCache();
    ~ImageCache();

    ImageCache(const ImageCache&) = delete;
    ImageCache& operator=(const ImageCache&) = delete;

    void setCacheDirectory(const String& path);
    const String& cacheDirectory() const { return m_cacheDir; }

    bool memoryGet(const String& key, SkBitmap* out) const;
    void memoryPut(const String& key, const SkBitmap& bitmap);
    void memoryRemove(const String& key);
    void memoryClear();
    void setMemoryCacheLimit(size_t maxEntries);

    bool fileRead(const String& key, Vector<uint8_t>* out) const;
    bool fileWrite(const String& key, const uint8_t* data, size_t len);
    bool fileExists(const String& key) const;

private:
    String filePathForKey(const String& key) const;
    void evictIfNeeded();

    mutable std::mutex m_mutex;
    HashMap<String, SkBitmap> m_memory;
    String m_cacheDir;
    size_t m_maxMemoryEntries;
};

class ImageLoader {
public:
    using NetworkCallback = std::function<void(bool success, const SkBitmap& bitmap)>;

    static ImageLoader& instance();

    bool loadLocal(const String& pathOrUri, SkBitmap* out);

    /// requestId==0 时不做生命周期登记（仅内部使用）；非 0 时与 registerPending 配对
    void loadNetwork(const String& url, uint64_t requestId, const NetworkCallback& callback);

    /// 为一次网络加载登记 Image*，返回请求 id；回调路径上须 tryConsumeRequest
    uint64_t registerPending(Image* image);
    /// 若 id 仍在登记表中则移除并返回 true（表示可安全向对应 Image 交付结果）
    bool tryConsumeRequest(uint64_t requestId);
    void cancelPending(uint64_t requestId);
    void cancelAllForImage(const Image* image);

    void setUiTaskRunner(TaskRunner* runner) { m_ui_task_runner = runner; }
    TaskRunner* uiTaskRunner() const { return m_ui_task_runner; }

private:
    ImageLoader();
    ImageLoader(const ImageLoader&) = delete;
    ImageLoader& operator=(const ImageLoader&) = delete;

    ImageCache m_cache;
    Loader m_loader;

    std::mutex m_request_mutex;
    uint64_t m_next_request_id = 1;
    HashMap<uint64_t, Image*> m_requestToImage;
    /// 非空时 bmnet 回调线程仅做解码，NetworkImageClient 将用户 callback 投递到该线程
    TaskRunner* m_ui_task_runner = nullptr;
};

namespace {

uint64_t fnv1a64(const String& s) {
    uint64_t h = 1469598103934665603ull;
    for (unsigned char c : s) {
        h ^= static_cast<uint64_t>(c);
        h *= 1099511628211ull;
    }
    return h;
}

bool isNetworkUrl(const String& s) {
    return StringUtil::startWith(s, "http://") ||
           StringUtil::startWith(s, "https://");
}

String stripFileUri(const String& s) {
    if (StringUtil::startWith(s, "file:///")) {
        return s.substr(8);
    }
    if (StringUtil::startWith(s, "file://")) {
        return s.substr(7);
    }
    return s;
}

bool ensureParentDir(const String& filePath) {
    namespace fs = std::filesystem;
    fs::path p(filePath);
    if (!p.has_parent_path()) {
        return true;
    }
    std::error_code ec;
    fs::create_directories(p.parent_path(), ec);
    return !ec;
}

}  // namespace

// --- ImageCache ---

ImageCache::ImageCache() : m_maxMemoryEntries(128) {}

ImageCache::~ImageCache() = default;

void ImageCache::setCacheDirectory(const String& path) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cacheDir = path;
}

String ImageCache::filePathForKey(const String& key) const {
    std::ostringstream oss;
    oss << m_cacheDir;
    if (!m_cacheDir.empty() && m_cacheDir.back() != '/' && m_cacheDir.back() != '\\') {
        oss << '/';
    }
    oss << std::hex << fnv1a64(key) << '_' << std::dec << key.size() << ".imgcache";
    return oss.str();
}

void ImageCache::evictIfNeeded() {
    while (m_memory.size() > m_maxMemoryEntries && !m_memory.empty()) {
        m_memory.erase(m_memory.begin());
    }
}

bool ImageCache::memoryGet(const String& key, SkBitmap* out) const {
    if (!out) {
        return false;
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_memory.find(key);
    if (it == m_memory.end()) {
        return false;
    }
    return it->second.deepCopyTo(out);
}

void ImageCache::memoryPut(const String& key, const SkBitmap& bitmap) {
    if (key.empty() || bitmap.empty()) {
        return;
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    SkBitmap copy;
    if (!bitmap.deepCopyTo(&copy)) {
        return;
    }
    m_memory[key] = std::move(copy);
    evictIfNeeded();
}

void ImageCache::memoryRemove(const String& key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_memory.erase(key);
}

void ImageCache::memoryClear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_memory.clear();
}

void ImageCache::setMemoryCacheLimit(size_t maxEntries) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_maxMemoryEntries = maxEntries ? maxEntries : 1;
    evictIfNeeded();
}

bool ImageCache::fileRead(const String& key, Vector<uint8_t>* out) const {
    if (!out || m_cacheDir.empty()) {
        return false;
    }
    String path = filePathForKey(key);
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        return false;
    }
    f.seekg(0, std::ios::end);
    const auto sz = f.tellg();
    if (sz <= 0) {
        return false;
    }
    f.seekg(0);
    out->resize(static_cast<size_t>(sz));
    f.read(reinterpret_cast<char*>(out->data()), sz);
    return static_cast<bool>(f);
}

bool ImageCache::fileWrite(const String& key, const uint8_t* data, size_t len) {
    if (!data || len == 0 || m_cacheDir.empty()) {
        return false;
    }
    String path = filePathForKey(key);
    if (!ensureParentDir(path)) {
        return false;
    }
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f) {
        return false;
    }
    f.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(len));
    return static_cast<bool>(f);
}

bool ImageCache::fileExists(const String& key) const {
    if (m_cacheDir.empty()) {
        return false;
    }
    namespace fs = std::filesystem;
    return fs::exists(filePathForKey(key));
}

// --- ImageLoader ---

namespace {

class NetworkImageClient : public LoaderClient {
public:
    String url;
    ImageCache* cache;
    ImageLoader::NetworkCallback callback;

    void onResultCallback(const uint8_t* data, size_t len) override {
        bool ok = false;
        SkBitmap bmp;
        if (data && len > 0) {
            if (cache) {
                cache->fileWrite(url, data, len);
            }
            if (SkImageDecoder::DecodeMemory(data, len, &bmp) && !bmp.empty()) {
                ok = true;
                if (cache) {
                    cache->memoryPut(url, bmp);
                }
            }
        }
        ImageLoader::NetworkCallback onDone = std::move(callback);
        TaskRunner* ui = ImageLoader::instance().uiTaskRunner();
        if (ui) {
            NetworkImageClient* self = this;
            ui->postTask([self, okCopy = ok, bmp = std::move(bmp),
                          onDone = std::move(onDone)]() mutable {
                if (onDone) {
                    onDone(okCopy, bmp);
                }
                delete self;
            });
            return;
        }
        if (onDone) {
            onDone(ok, bmp);
        }
        delete this;
    }
};

}  // namespace

ImageLoader& ImageLoader::instance() {
    static ImageLoader loader;
    return loader;
}

ImageLoader::ImageLoader() = default;

uint64_t ImageLoader::registerPending(Image* image) {
    std::lock_guard<std::mutex> lock(m_request_mutex);
    const uint64_t id = m_next_request_id++;
    m_requestToImage[id] = image;
    return id;
}

bool ImageLoader::tryConsumeRequest(uint64_t requestId) {
    std::lock_guard<std::mutex> lock(m_request_mutex);
    return m_requestToImage.erase(requestId) > 0;
}

void ImageLoader::cancelPending(uint64_t requestId) {
    if (requestId == 0) {
        return;
    }
    std::lock_guard<std::mutex> lock(m_request_mutex);
    m_requestToImage.erase(requestId);
}

void ImageLoader::cancelAllForImage(const Image* image) {
    std::lock_guard<std::mutex> lock(m_request_mutex);
    for (auto it = m_requestToImage.begin(); it != m_requestToImage.end();) {
        if (it->second == image) {
            it = m_requestToImage.erase(it);
        } else {
            ++it;
        }
    }
}

bool ImageLoader::loadLocal(const String& pathOrUri, SkBitmap* out) {
    if (!out) {
        return false;
    }
    if (isNetworkUrl(pathOrUri)) {
        return false;
    }
    const String fsPath = stripFileUri(pathOrUri);
    if (fsPath.empty()) {
        return false;
    }
    const String& cacheKey = pathOrUri;

    if (m_cache.memoryGet(cacheKey, out) && !out->empty()) {
        return true;
    }

    if (!SkImageDecoder::DecodeFile(fsPath.c_str(), out)) {
        return false;
    }
    if (!out->empty()) {
        m_cache.memoryPut(cacheKey, *out);
    }
    return true;
}

void ImageLoader::loadNetwork(const String& url, uint64_t requestId,
                              const NetworkCallback& callback) {
    if (!callback) {
        return;
    }

    const NetworkCallback wrapped = [requestId, callback](bool ok, const SkBitmap& bmp) {
        if (requestId == 0) {
            callback(ok, bmp);
            return;
        }
        // 登记表中无此 id：Image 已析构或请求已被取消/覆盖，禁止调用 inner（避免悬空 this）
        if (!ImageLoader::instance().tryConsumeRequest(requestId)) {
            return;
        }
        callback(ok, bmp);
    };

    if (!isNetworkUrl(url)) {
        SkBitmap empty;
        wrapped(false, empty);
        return;
    }

    SkBitmap bmp;
    if (m_cache.memoryGet(url, &bmp) && !bmp.empty()) {
        wrapped(true, bmp);
        return;
    }

    Vector<uint8_t> raw;
    if (m_cache.fileRead(url, &raw) && !raw.empty()) {
        if (SkImageDecoder::DecodeMemory(raw.data(), raw.size(), &bmp) && !bmp.empty()) {
            m_cache.memoryPut(url, bmp);
            wrapped(true, bmp);
            return;
        }
    }

    auto* client = new NetworkImageClient();
    client->url = url;
    client->cache = &m_cache;
    client->callback = wrapped;
    m_loader.get(url, String(), client);
}

// --- Image ---

void Image::setNetworkImageUiTaskRunner(TaskRunner* runner) {
    ImageLoader::instance().setUiTaskRunner(runner);
}

Image::Image() {}

Image::~Image() {
    ImageLoader::instance().cancelAllForImage(this);
}

void Image::createImage(const void* buffer, size_t size) {
    SkImageDecoder::DecodeMemory(buffer, size, &m_bitmap);
}

bool Image::loadFromFile(const char* path) {
    if (!path || !*path) {
        return false;
    }
    return SkImageDecoder::DecodeFile(path, &m_bitmap);
}

bool Image::load(const char* pathOrUrl, LoadCallback callback, void* userData) {
    if (!pathOrUrl || !*pathOrUrl) {
        return false;
    }

    ImageLoader& loader = ImageLoader::instance();

    const String src(pathOrUrl);
    if (!isNetworkUrl(src)) {
        if (m_networkRequestId != 0) {
            loader.cancelPending(m_networkRequestId);
            m_networkRequestId = 0;
        }
        const bool ok = loadFromFile(pathOrUrl);
        if (callback) {
            callback(ok, m_bitmap, userData);
        }
        return ok;
    }

    if (m_networkRequestId != 0) {
        loader.cancelPending(m_networkRequestId);
        m_networkRequestId = 0;
    }

    const uint64_t rid = loader.registerPending(this);
    m_networkRequestId = rid;

    Image* self = this;
    loader.loadNetwork(
        src, rid,
        [self, rid, callback, userData](bool success, const SkBitmap& bitmap) {
            if (success) {
                SkBitmap copy;
                if (bitmap.deepCopyTo(&copy)) {
                    self->m_bitmap = std::move(copy);
                } else {
                    self->m_bitmap = bitmap;
                }
            }
            if (self->m_networkRequestId == rid) {
                self->m_networkRequestId = 0;
            }
            if (callback) {
                callback(success, self->m_bitmap, userData);
            }
        });
    return true;
}

const SkBitmap& Image::bitmap() const { return m_bitmap; }

}  // namespace boymue
