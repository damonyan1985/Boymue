// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_MACRO_ASSEMBLER_H_
#define V8_MACRO_ASSEMBLER_H_

#include "src/assembler.h"


// Helper types to make boolean flag easier to read at call-site.
enum InvokeFlag {
  CALL_FUNCTION,
  JUMP_FUNCTION
};


// Flags used for the AllocateInNewSpace functions.
enum AllocationFlags {
  // No special flags.
  NO_ALLOCATION_FLAGS = 0,
  // Return the pointer to the allocated already tagged as a heap object.
  TAG_OBJECT = 1 << 0,
  // The content of the result register already contains the allocation top in
  // new space.
  RESULT_CONTAINS_TOP = 1 << 1,
  // Specify that the requested size of the space to allocate is specified in
  // words instead of bytes.
  SIZE_IN_WORDS = 1 << 2,
  // Align the allocation to a multiple of kDoubleSize
  DOUBLE_ALIGNMENT = 1 << 3,
  // Directly allocate in old space
  PRETENURE = 1 << 4,
};


#if V8_TARGET_ARCH_IA32
#include "src/asm/ia32/assembler-ia32.h"
#include "src/asm/ia32/assembler-ia32-inl.h"
#include "src/asm/ia32/macro-assembler-ia32.h"
#elif V8_TARGET_ARCH_X64
#include "src/asm/x64/assembler-x64.h"
#include "src/asm/x64/assembler-x64-inl.h"
#include "src/asm/x64/macro-assembler-x64.h"
#elif V8_TARGET_ARCH_ARM64
#include "src/asm/arm64/assembler-arm64.h"
#include "src/asm/arm64/assembler-arm64-inl.h"
#include "src/asm/arm64/constants-arm64.h"
#include "src/asm/arm64/macro-assembler-arm64.h"
#include "src/asm/arm64/macro-assembler-arm64-inl.h"
#elif V8_TARGET_ARCH_ARM
#include "src/asm/arm/assembler-arm.h"
#include "src/asm/arm/assembler-arm-inl.h"
#include "src/asm/arm/constants-arm.h"
#include "src/asm/arm/macro-assembler-arm.h"
#elif V8_TARGET_ARCH_PPC
#include "src/asm/ppc/assembler-ppc.h"
#include "src/asm/ppc/assembler-ppc-inl.h"
#include "src/asm/ppc/constants-ppc.h"
#include "src/asm/ppc/macro-assembler-ppc.h"
#elif V8_TARGET_ARCH_MIPS
#include "src/asm/mips/assembler-mips.h"
#include "src/asm/mips/assembler-mips-inl.h"
#include "src/asm/mips/constants-mips.h"
#include "src/asm/mips/macro-assembler-mips.h"
#elif V8_TARGET_ARCH_MIPS64
#include "src/asm/mips64/assembler-mips64.h"
#include "src/asm/mips64/assembler-mips64-inl.h"
#include "src/asm/mips64/constants-mips64.h"
#include "src/asm/mips64/macro-assembler-mips64.h"
#elif V8_TARGET_ARCH_S390
#include "src/asm/s390/assembler-s390.h"
#include "src/asm/s390/assembler-s390-inl.h"
#include "src/asm/s390/constants-s390.h"
#include "src/asm/s390/macro-assembler-s390.h"
#elif V8_TARGET_ARCH_X87
#include "src/asm/x87/assembler-x87.h"
#include "src/asm/x87/assembler-x87-inl.h"
#include "src/asm/x87/macro-assembler-x87.h"
#else
#error Unsupported target architecture.
#endif

namespace v8 {
namespace internal {

class FrameScope {
 public:
  explicit FrameScope(MacroAssembler* masm, StackFrame::Type type)
      : masm_(masm), type_(type), old_has_frame_(masm->has_frame()) {
    masm->set_has_frame(true);
    if (type != StackFrame::MANUAL && type_ != StackFrame::NONE) {
      masm->EnterFrame(type);
    }
  }

  ~FrameScope() {
    if (type_ != StackFrame::MANUAL && type_ != StackFrame::NONE) {
      masm_->LeaveFrame(type_);
    }
    masm_->set_has_frame(old_has_frame_);
  }

  // Normally we generate the leave-frame code when this object goes
  // out of scope.  Sometimes we may need to generate the code somewhere else
  // in addition.  Calling this will achieve that, but the object stays in
  // scope, the MacroAssembler is still marked as being in a frame scope, and
  // the code will be generated again when it goes out of scope.
  void GenerateLeaveFrame() {
    DCHECK(type_ != StackFrame::MANUAL && type_ != StackFrame::NONE);
    masm_->LeaveFrame(type_);
  }

 private:
  MacroAssembler* masm_;
  StackFrame::Type type_;
  bool old_has_frame_;
};

class FrameAndConstantPoolScope {
 public:
  FrameAndConstantPoolScope(MacroAssembler* masm, StackFrame::Type type)
      : masm_(masm),
        type_(type),
        old_has_frame_(masm->has_frame()),
        old_constant_pool_available_(FLAG_enable_embedded_constant_pool &&
                                     masm->is_constant_pool_available()) {
    masm->set_has_frame(true);
    if (FLAG_enable_embedded_constant_pool) {
      masm->set_constant_pool_available(true);
    }
    if (type_ != StackFrame::MANUAL && type_ != StackFrame::NONE) {
      masm->EnterFrame(type, !old_constant_pool_available_);
    }
  }

  ~FrameAndConstantPoolScope() {
    masm_->LeaveFrame(type_);
    masm_->set_has_frame(old_has_frame_);
    if (FLAG_enable_embedded_constant_pool) {
      masm_->set_constant_pool_available(old_constant_pool_available_);
    }
  }

  // Normally we generate the leave-frame code when this object goes
  // out of scope.  Sometimes we may need to generate the code somewhere else
  // in addition.  Calling this will achieve that, but the object stays in
  // scope, the MacroAssembler is still marked as being in a frame scope, and
  // the code will be generated again when it goes out of scope.
  void GenerateLeaveFrame() {
    DCHECK(type_ != StackFrame::MANUAL && type_ != StackFrame::NONE);
    masm_->LeaveFrame(type_);
  }

 private:
  MacroAssembler* masm_;
  StackFrame::Type type_;
  bool old_has_frame_;
  bool old_constant_pool_available_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(FrameAndConstantPoolScope);
};

// Class for scoping the the unavailability of constant pool access.
class ConstantPoolUnavailableScope {
 public:
  explicit ConstantPoolUnavailableScope(MacroAssembler* masm)
      : masm_(masm),
        old_constant_pool_available_(FLAG_enable_embedded_constant_pool &&
                                     masm->is_constant_pool_available()) {
    if (FLAG_enable_embedded_constant_pool) {
      masm_->set_constant_pool_available(false);
    }
  }
  ~ConstantPoolUnavailableScope() {
    if (FLAG_enable_embedded_constant_pool) {
      masm_->set_constant_pool_available(old_constant_pool_available_);
    }
  }

 private:
  MacroAssembler* masm_;
  int old_constant_pool_available_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ConstantPoolUnavailableScope);
};


class AllowExternalCallThatCantCauseGC: public FrameScope {
 public:
  explicit AllowExternalCallThatCantCauseGC(MacroAssembler* masm)
      : FrameScope(masm, StackFrame::NONE) { }
};


class NoCurrentFrameScope {
 public:
  explicit NoCurrentFrameScope(MacroAssembler* masm)
      : masm_(masm), saved_(masm->has_frame()) {
    masm->set_has_frame(false);
  }

  ~NoCurrentFrameScope() {
    masm_->set_has_frame(saved_);
  }

 private:
  MacroAssembler* masm_;
  bool saved_;
};


// Support for "structured" code comments.
#ifdef DEBUG

class Comment {
 public:
  Comment(MacroAssembler* masm, const char* msg);
  ~Comment();

 private:
  MacroAssembler* masm_;
  const char* msg_;
};

#else

class Comment {
 public:
  Comment(MacroAssembler*, const char*)  {}
};

#endif  // DEBUG


// Wrapper class for passing expected and actual parameter counts as
// either registers or immediate values. Used to make sure that the
// caller provides exactly the expected number of parameters to the
// callee.
class ParameterCount BASE_EMBEDDED {
 public:
  explicit ParameterCount(Register reg) : reg_(reg), immediate_(0) {}
  explicit ParameterCount(int imm) : reg_(no_reg), immediate_(imm) {}

  bool is_reg() const { return !reg_.is(no_reg); }
  bool is_immediate() const { return !is_reg(); }

  Register reg() const {
    DCHECK(is_reg());
    return reg_;
  }
  int immediate() const {
    DCHECK(is_immediate());
    return immediate_;
  }

 private:
  const Register reg_;
  const int immediate_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ParameterCount);
};


class AllocationUtils {
 public:
  static ExternalReference GetAllocationTopReference(
      Isolate* isolate, AllocationFlags flags) {
    if ((flags & PRETENURE) != 0) {
      return ExternalReference::old_space_allocation_top_address(isolate);
    }
    return ExternalReference::new_space_allocation_top_address(isolate);
  }


  static ExternalReference GetAllocationLimitReference(
      Isolate* isolate, AllocationFlags flags) {
    if ((flags & PRETENURE) != 0) {
      return ExternalReference::old_space_allocation_limit_address(isolate);
    }
    return ExternalReference::new_space_allocation_limit_address(isolate);
  }
};


}  // namespace internal
}  // namespace v8

#endif  // V8_MACRO_ASSEMBLER_H_
