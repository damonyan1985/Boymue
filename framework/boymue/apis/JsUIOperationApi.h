//
//  JsUIOperationApi.h
//  core
//
//  Created by yanbo on 2022/9/19.
//

#ifndef JsUIOperationApi_h
#define JsUIOperationApi_h

#include "JsApiInterface.h"

namespace boymue {
class JsUIOperationApi : public JsApiInterface {
public:
    JsUIOperationApi(BoymueApplication* context);
    virtual const char* name() const override;
    void execute(const String& params,
                 JsApiCallback* callback) override;
};
}
#endif /* JsUIOperationApi_h */
