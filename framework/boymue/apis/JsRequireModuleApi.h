//
//  JsRequireModuleApi.hpp
//  core
//
//  Created by yanbo on 2022/9/22.
//

#ifndef JsRequireModuleApi_h
#define JsRequireModuleApi_h

#include "JsApiInterface.h"

namespace boymue {
class JsRequireModuleApi : public JsApiInterface {
public:
    JsRequireModuleApi(BoymueApplication* context);
    
    virtual const char* name() const override;
    void execute(const String& params,
                 JsApiCallback* callback) override;
    
private:
    // 防止重复导入相同的脚本
    HashMap<String, bool> m_scriptSet;
};
}

#endif /* JsRequireModuleApi_h */
