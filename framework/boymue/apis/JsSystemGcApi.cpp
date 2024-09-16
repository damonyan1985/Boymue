// Copyright 2024 the Boymue project authors. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "JsSystemGcApi.h"
#include "BoymueApplication.h"

namespace boymue {

JsSystemGcApi::JsSystemGcApi(BoymueApplication* context)
	: JsApiInterface(context) {}

void JsSystemGcApi::execute(const String& params,
	JsApiCallback* callback) {
	context()->runtime()->gc();
}

const char* JsSystemGcApi::name() const { return "gc"; }
}