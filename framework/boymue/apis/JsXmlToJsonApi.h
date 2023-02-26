//
//  JsXmlToJsonApi.h
//  core
//
//  Created by yanbo on 2022/9/20.
//

#ifndef JsXmlToJsonApi_h
#define JsXmlToJsonApi_h

#include "JsApiInterface.h"
#include "rapidjson/document.h"

namespace boymue {
class JsXmlToJsonApi : public JsApiInterface {
public:
    JsXmlToJsonApi(BoymueApplication* context);
    virtual const char* name() const override;
    void execute(const String& params,
                 JsApiCallback* callback) override;

    ThreadExecutor* executor();
};
}

#endif /* JsXmlToJsonApi_h */
