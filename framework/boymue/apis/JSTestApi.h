// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.02.02
#ifndef JsTestApi_h
#define JsTestApi_h

#include "JsApiInterface.h"

namespace boymue {
class JsTestApi : public JsApiInterface {
 public:
  JsTestApi(BoymueApplication* context);
  virtual const char* name() const override;
  virtual void execute(const String& params,
                       JsApiCallback* callback) override;
};
}  // namespace boymue
#endif
