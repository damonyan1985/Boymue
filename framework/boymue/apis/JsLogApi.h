// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.21
#ifndef JsLogApi_h
#define JsLogApi_h

#include "JsApiInterface.h"

namespace boymue {
class JsLogApi : public JsApiInterface {
 public:
  virtual const char* name() const;
  virtual void execute(const std::string& params, JsApiCallback* callback);
};
}  // namespace boymue
#endif