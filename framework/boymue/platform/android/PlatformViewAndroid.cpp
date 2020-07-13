#include "PlatformViewAndroid.h"
#include "AndroidContextGL.h"

namespace boymue {
PlatformViewAndroid::PlatformViewAndroid()
{
    // 创建GL上下文环境
    std::shared_ptr<AndroidContextGL> android_context = std::make_shared<AndroidContextGL>(
        std::make_unique<AndroidEnvironmentGL>());

    m_surface
        = std::make_unique<AndroidSurfaceGL>(android_context);
}
}