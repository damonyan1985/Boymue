// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef BoymueView_h
#define BoymueView_h

#include "TaskThread.h"

namespace boymue {
class BoymueView {
 public:
  BoymueView();

  TaskRunner& getTaskRunner() const;

 private:
  TaskThread m_uiThread;
};
}  // namespace boymue
#endif
