#include "JsLogApi.h"

namespace boymue {
const char* JsLogApi::name() const { return "log"; }

JsLogApi::JsLogApi(BoymueApplication* context) : JsApiInterface(context) {}

void JsLogApi::execute(const std::string& params, JsApiCallback* callback) {
  printf("Boymue: %s\n", params.c_str());
  //if (callback) {
  //  callback->callback("");
  //}
}
}  // namespace boymue
