#include "JsLogApi.h"

namespace boymue {
const char* JsLogApi::name() const { return "log"; }

void JsLogApi::execute(const std::string& params, JsApiCallback* callback) {
  printf("Boymue: %s", params.c_str());
  if (callback) {
    callback->callback("");
  }
}
}  // namespace boymue