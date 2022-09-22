// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.02.02
#include "JSTestApi.h"

namespace boymue {
const char* JsTestApi::name() const { return "test"; }

JsTestApi::JsTestApi(BoymueApplication* context) : JsApiInterface(context) {}

void JsTestApi::execute(const String& params, JsApiCallback* callback) {
  printf("Boymue: %s\n", params.c_str());
  if (callback) {
    callback->callback("heihei");
  }
}
}  // namespace boymue
