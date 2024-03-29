// Copyright 2009 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_SIMULATOR_H_
#define V8_SIMULATOR_H_

#if V8_TARGET_ARCH_IA32
#include "src/asm/ia32/simulator-ia32.h"
#elif V8_TARGET_ARCH_X64
#include "src/asm/x64/simulator-x64.h"
#elif V8_TARGET_ARCH_ARM64
#include "src/asm/arm64/simulator-arm64.h"
#elif V8_TARGET_ARCH_ARM
#include "src/asm/arm/simulator-arm.h"
#elif V8_TARGET_ARCH_PPC
#include "src/asm/ppc/simulator-ppc.h"
#elif V8_TARGET_ARCH_MIPS
#include "src/mips/simulator-mips.h"
#elif V8_TARGET_ARCH_MIPS64
#include "src/asm/mips64/simulator-mips64.h"
#elif V8_TARGET_ARCH_S390
#include "src/asm/s390/simulator-s390.h"
#elif V8_TARGET_ARCH_X87
#include "src/asm/x87/simulator-x87.h"
#else
#error Unsupported target architecture.
#endif

#endif  // V8_SIMULATOR_H_
