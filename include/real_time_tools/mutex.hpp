#ifndef MUTEX_HPP
#define MUTEX_HPP

#if defined(XENOMAI)
  #include <native/mutex.h>
  #include <native/cond.h>
  #include <native/timer.h>
#elif defined(RT_PREEMPT)
  #include <pthread.h>
  #include <sys/time.h>
#else // defined(NON_REAL_TIME)
  #include <mutex>
  #include <condition_variable>
#endif

namespace real_time_tools{

#if defined(XENOMAI)
  typedef RT_MUTEX rt_mutex;
  typedef RT_COND rt_cond;
#elif defined(RT_PREEMPT)
  typedef pthread_mutex_t rt_mutex;
  typedef pthread_cond_t rt_cond;
#else
  typedef std::mutex rt_mutex;
  typedef std::condition_variable rt_cond;
#endif

class RealTimeMutex{
public:
  RealTimeMutex()
  {
    int res = 0;
#if defined(XENOMAI)
    res = rt_mutex_create(mutex, nullptr);
#elif defined(RT_PREEMPT)
    res = pthread_mutex_init(mutex, nullptr);
#else // defined(NON_REAL_TIME)
    mutex_ = nullptr;
    mutex_ = new std::mutex();
#endif
    if (res > 0)
    {
      rt_printf("RealTimeMutex::RealTimeMutex(): "
                "error while creating mutex with code %d", res);
    }
  }

  ~RealTimeMutex()
  {
    int res = 0;
#if defined(XENOMAI)
    res = rt_mutex_delete(mutex_);
#elif defined(RT_PREEMPT)
    res = pthread_mutex_destroy(mutex_);
#else // defined(NON_REAL_TIME)
    if(mutex_)
    {
      delete(mutex_);
    }
#endif
    if (res > 0)
    {
      rt_printf("RealTimeMutex::~RealTimeMutex(): "
                "error while destroying mutex with code %d", res);
    }
    mutex_ = nullptr;  
  }

  void lock()
  {
    int res = 0;
#if defined(XENOMAI)
    res = rt_mutex_acquire(mutex_, TM_INFINITE);
#elif defined(RT_PREEMPT)
    res = pthread_mutex_lock(mutex_);
#else // defined(NON_REAL_TIME)
    mutex_->lock();
#endif
    if (res)
    {
      rt_printf("RealTimeMutex::lock(): "
                "error while locking mutex with code %d", res);
    }
  }

  void unlock()
  {
    int res = 0;
#if defined(XENOMAI)
    res = rt_mutex_release(mutex);
#elif defined(RT_PREEMPT)
    res = pthread_mutex_unlock(mutex);
#else // defined(NON_REAL_TIME)
    mutex_->unlock();
#endif
    if (res)
    {
      rt_printf("RealTimeMutex::unlock(): "
                "error while locking mutex with code %d", res);
    }
  }

private:
  rt_mutex* mutex_;
};

} // namespace

#endif // Header protection