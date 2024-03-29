// Copyright 2006-2008 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if V8_TARGET_ARCH_X87

#include "src/asm/x87/frames-x87.h"

#include "src/asm/x87/assembler-x87-inl.h"
#include "src/asm/x87/assembler-x87.h"
#include "src/assembler.h"
#include "src/frames.h"

namespace v8 {
namespace internal {

Register JavaScriptFrame::fp_register() { return ebp; }
Register JavaScriptFrame::context_register() { return esi; }
Register JavaScriptFrame::constant_pool_pointer_register() {
  UNREACHABLE();
  return no_reg;
}

Register StubFailureTrampolineFrame::fp_register() { return ebp; }
Register StubFailureTrampolineFrame::context_register() { return esi; }
Register StubFailureTrampolineFrame::constant_pool_pointer_register() {
  UNREACHABLE();
  return no_reg;
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_X87
