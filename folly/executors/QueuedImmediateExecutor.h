/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <queue>
#include <utility>

#include <folly/Executor.h>
#include <folly/ThreadLocal.h>
#include <folly/executors/InlineExecutor.h>
#include <folly/io/async/Request.h>

namespace folly {

/**
 * Runs inline like InlineExecutor, but with a queue so that any tasks added
 * to this executor by one of its own callbacks will be queued instead of
 * executed inline (nested). This is usually better behavior than Inline.
 */
class QueuedImmediateExecutor : public InlineLikeExecutor {
 public:
  static QueuedImmediateExecutor& instance();

  void add(Func func) override;

 private:
  struct Task {
    Func func;
    std::shared_ptr<RequestContext> ctx;
  };

  using Queue = std::queue<Task>;
  folly::ThreadLocal<std::pair</* running */ bool, Queue>> q_;
};

} // namespace folly
