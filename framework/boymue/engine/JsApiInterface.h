// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.21

#ifndef JsApiInterface_h
#define JsApiInterface_h

#include <string>
#include "ThreadExecutor.h"

namespace boymue {
class BoymueApplication;
// Api回调
class JsApiCallback {
public:
  virtual void callback(const std::string& result) = 0;
};
// Native端Api接口
class JsApiInterface {
 public:
  virtual ~JsApiInterface(){};
  virtual const char* name() const = 0;
  virtual BoymueApplication* context() const = 0;
  virtual ThreadExecutor* executor() { return nullptr;  }
  virtual void execute(const std::string& params, JsApiCallback* callback) = 0;
};
}  // namespace boymue

#endif  // !JsApiInterface_h