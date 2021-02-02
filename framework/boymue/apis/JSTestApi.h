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
  virtual BoymueApplication* context() const override;
  virtual void execute(const std::string& params,
                       JsApiCallback* callback) override;

 private:
  BoymueApplication* m_context;
};
}  // namespace boymue
#endif
