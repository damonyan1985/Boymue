// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.09

#ifndef JsEngine_h
#define JsEngine_h

#include <string>

namespace boymue {
class JsRuntime {
 public:
  virtual ~JsRuntime() {}
  virtual void evaluateJs(const std::string& jsSource) = 0;
};

class JsInitor;
class JsEngine {
 public:
  JsEngine();
  ~JsEngine();

  JsRuntime* createRuntime();

 private:
  JsInitor* m_initor;
};
}  // namespace boymue
#endif