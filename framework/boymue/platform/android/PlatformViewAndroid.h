#ifndef PlatformViewAndroid_h
#define PlatformViewAndroid_h

#include "AndroidSurfaceGL.h"
#include <memory>

namespace boymue {
class PlatformViewAndroid {
public:
    PlatformViewAndroid();
    ~PlatformViewAndroid();

private:
    std::unique_ptr<AndroidSurfaceGL> m_surface;
};

}

#endif