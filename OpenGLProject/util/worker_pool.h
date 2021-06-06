#pragma once
#include <array>
#include <thread>
#include <mutex>
#include <functional>

#include "linked_list.h"

namespace ogl {
	template<int numThreads>
	class WorkerPool {
	public:
		typedef std::function<void()> Task;

		WorkerPool() {
			m_Workers = std::array<std::thread, numThreads>();
			m_Queue = LinkedList<Task>();
		}

		void __threadLoop() {
			while (m_Running) {

				m_QueueMutex.lock();
				if (m_Queue.Empty()) {
					m_QueueMutex.unlock();
					continue;
				}
				
				//we must have task at this point
				Task t = m_Queue.PopFront();
				// unlock mutex and run task
				m_QueueMutex.unlock();
				t();
			}
		}

	private:
		bool m_Running;
		LinkedList<Task> m_Queue;
		std::mutex m_QueueMutex;
		std::array<std::thread, numThreads> m_Workers;
	};
}