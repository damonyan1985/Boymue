//
//  JsRequestApi.hpp
//  core
//
//  Created by yanbo on 2022/10/9.
//

#ifndef JsRequestApi_hpp
#define JsRequestApi_hpp

#include "JsApiInterface.h"

namespace boymue {
class JsRequestApi : public JsApiInterface {
public:
    JsRequestApi(BoymueApplication* context);
    
    virtual const char* name() const override;
    void execute(const String& params,
                 JsApiCallback* callback) override;
};
}

#endif /* JsRequestApi_hpp */
