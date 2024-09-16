// Copyright 2024 the Boymue project authors. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef JsSystemGcApi_h
#define JsSystemGcApi_h

#include "JsApiInterface.h"

namespace boymue {
class JsSystemGcApi : public JsApiInterface {
public:
    JsSystemGcApi(BoymueApplication* context);

    virtual const char* name() const override;
    void execute(const String& params,
        JsApiCallback* callback) override;
};
}
#endif // !JsSystemGcApi_h
