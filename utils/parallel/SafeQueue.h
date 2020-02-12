//
// Created by Petr Flajsingr on 07/04/2018.
//

#ifndef UTILITIES_SAFEQUEUE_H
#define UTILITIES_SAFEQUEUE_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * @brief A template queue to pass data between threads - thread safe
 */
template <typename T> class SafeQueue {
public:
  SafeQueue() : keep_running(true){};
  SafeQueue(SafeQueue &&other) noexcept {
    queue = std::move(other.queue);
    keep_running = other.keep_running;
  }
  SafeQueue &operator=(SafeQueue &&other) noexcept {
    queue = std::move(other.queue);
    keep_running = other.keep_running;
    return *this;
  }
  /**
   * Add an item to the end of the queue
   * @param item item to be added
   */
  T &enqueue(T item) {
    std::unique_lock<std::mutex> lock(queueMutex);
    bool wasEmpty = queue.empty();

    queue.push(std::move(item));
    lock.unlock();

    if (wasEmpty)
      conditionVariable.notify_one();
    return queue.front();
  }

  std::optional<T> dequeue() {
    std::unique_lock<std::mutex> lock(queueMutex);
    while (keep_running && queue.empty()) {
      conditionVariable.wait(lock);
    }

    if (keep_running && !queue.empty()) {
      auto item = std::move(queue.front());
      queue.pop();
      return item;
    }
    return std::nullopt;
  }

  bool isEmpty() { return queue.empty(); }

  void shutdown() {
    keep_running = false;
    conditionVariable.notify_all();
  }

  std::size_t size() { return queue.size(); }

private:
  std::mutex queueMutex;
  std::condition_variable conditionVariable;
  std::queue<T> queue;
  std::atomic<bool> keep_running;
};

#endif // UTILITIES_SAFEQUEUE_H