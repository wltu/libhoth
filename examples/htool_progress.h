// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LIBHOTH_EXAMPLES_HTOOL_PROGRESS_H_
#define LIBHOTH_EXAMPLES_HTOOL_PROGRESS_H_

#include <stdint.h>
#include <time.h>

struct htool_progress {
  void (*func)(void*, uint64_t numerator, uint64_t denominator);
  void* param;
};

struct htool_progress_stderr {
  struct htool_progress progress;
  struct timespec start_time;
  const char* action_title;
};

void htool_progress_stderr_init(struct htool_progress_stderr* progress,
                                const char* action_title);

#endif  // LIBHOTH_EXAMPLES_HTOOL_PROGRESS_H_
