// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.21
#ifndef JsLogApi_h
#define JsLogApi_h

#include "JsApiInterface.h"

namespace boymue {
class JsLogApi : public JsApiInterface {
 public:
  JsLogApi(BoymueApplication* context);
  virtual const char* name() const override;
  virtual void execute(const std::string& params, JsApiCallback* callback) override;

 private:
  BoymueApplication* m_context;
};
}  // namespace boymue
#endif
