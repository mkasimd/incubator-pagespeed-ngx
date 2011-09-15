/*
 * Copyright 2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Author: jmaessen@google.com (Jan Maessen)

#ifndef NET_INSTAWEB_UTIL_PUBLIC_TIMER_BASED_ABSTRACT_LOCK_H_
#define NET_INSTAWEB_UTIL_PUBLIC_TIMER_BASED_ABSTRACT_LOCK_H_

#include "net/instaweb/util/public/basictypes.h"
#include "net/instaweb/util/public/named_lock_manager.h"

// DEPRECATED.  Use SchedulerBasedAbstractLock instead.
// TODO(jmaessen): Remove this class after the cutover.

namespace net_instaweb {
class Function;
class Timer;

// A TimerBasedAbstractLock implements a Lock by spinning on a TryLock, using a
// Timer to perform exponential sleep backoff.  Note that this means that it may
// not obtain a long-held lock in a timely fashion after it has been unlocked.
class TimerBasedAbstractLock : public NamedLock {
 public:
  virtual ~TimerBasedAbstractLock();
  // Lock blocks until it obtains the lock.
  virtual bool LockTimedWait(int64 wait_ms);
  virtual void LockTimedWait(int64 wait_ms, Function* callback);

  virtual bool LockTimedWaitStealOld(int64 wait_ms, int64 steal_ms);
  virtual void LockTimedWaitStealOld(int64 wait_ms, int64 steal_ms,
                                     Function* callback);

 protected:
  virtual Timer* timer() const = 0;

 private:
  typedef bool (TimerBasedAbstractLock::*TryLockMethod)(int64 steal_ms);
  bool TryLockIgnoreSteal(int64 steal_ignored);
  bool BusySpin(TryLockMethod try_lock, int64 steal_ms);
  void Spin(TryLockMethod try_lock, int64 steal_ms, int64 max_interval_ms);
  bool SpinFor(TryLockMethod try_lock, int64 steal_ms, int64 wait_ms);
};

}  // namespace net_instaweb

#endif  // NET_INSTAWEB_UTIL_PUBLIC_TIMER_BASED_ABSTRACT_LOCK_H_
