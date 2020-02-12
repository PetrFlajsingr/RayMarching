//
// Created by petr on 12/26/19.
//

#ifndef UTILITIES_THREADPOOL_H
#define UTILITIES_THREADPOOL_H

#include "SafeQueue.h"
#include <future>
#include <thread>

struct Task {
  void operator()() { execute(); }
  virtual void execute() = 0;

  virtual ~Task() = default;
};

template <typename T> struct PTask : public Task {
  explicit PTask(std::packaged_task<T()> &&task) : task(std::move(task)) {}
  void execute() override { task(); }
  std::packaged_task<T()> task;
};

class ThreadPool {
public:
  explicit ThreadPool(std::size_t threadCount) {
    using namespace MakeRange;
    for ([[maybe_unused]] auto _ : range(threadCount)) {
      threads.emplace_back([this] { threadLoop(); });
    }
  }
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = default;
  ThreadPool &operator=(ThreadPool &&) = default;

  template <typename F> std::future<decltype(std::declval<F>()())> push(F &&callable) {
    using result_type = decltype(callable());
    auto task = std::packaged_task<result_type()>(std::forward<F>(callable));
    auto future = task.get_future();
    queue.enqueue(std::make_unique<PTask<result_type>>(std::move(task)));
    return future;
  }

  ~ThreadPool() {
    queue.shutdown();
    for (auto &thread : threads) {
      thread.join();
    }
  }

private:
  std::vector<std::thread> threads;
  SafeQueue<std::unique_ptr<Task>> queue;

  void threadLoop() {
    while (true) {
      auto task = queue.dequeue();
      if (task.has_value()) {
        auto callable = std::move(task.value());
        (*callable)();
      } else {
        return;
      }
    }
  }
};

#endif // UTILITIES_THREADPOOL_H
