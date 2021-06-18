#pragma once
#include <oglpch.h>

namespace ogl {

	template<typename N>
	class LinkedList_Node {
		template<typename> friend class LinkedList; // LinkedList is friend
	public:
		LinkedList_Node(const LinkedList_Node& other) = delete; // Delete copy constructor
		LinkedList_Node(LinkedList_Node&& other) { // Move constructor
			if (before != nullptr) {
				before->next = &other;
			}
			if (next != nullptr) {
				next->before = &other;
			}
		}

		bool HasNext() { return this->next != nullptr; }
		bool HasPrev() { return this->before != nullptr; }

		LinkedList_Node& Next() {
			if (!HasNext()) throw std::out_of_range("No next element in linked list");
			return *next;
		}

		LinkedList_Node& Prev() {
			if (!HasPrev()) throw std::out_of_range("No previous element in linked list");
			return *before;
		}

	private:
		LinkedList_Node(const N& val, LinkedList_Node<N>* next, LinkedList_Node<N>* before) : value(val), next(next), before(before) { }
		LinkedList_Node(const N& val, LinkedList_Node<N>* next) : LinkedList_Node(val, next, nullptr) { }
		LinkedList_Node(const N& val) : LinkedList_Node(val, nullptr, nullptr) { }

		

	public:
		N value;
	private:
		LinkedList_Node<N>* before;
		LinkedList_Node<N>* next;
	};

	template<typename T>
	class LinkedList {
	public:
		using NodeType = LinkedList_Node<T>;
		LinkedList(const T& startValue) : m_Start(nullptr), m_End(nullptr) {
			T* node = new NodeType(startValue);
			m_Start = node;
			m_End = node;
		}

		LinkedList() : m_Start(nullptr), m_End(nullptr) { }
		~LinkedList() {
			if (Empty()) return;
			NodeType* current = m_Start;
			while (current->HasNext()) {
				current = current->next;
				delete current->before;
			}

			delete m_End;
		}

		bool Empty() { return m_Start == nullptr;  }

		void PushFront(const T& value) {
			NodeType* newNode = new NodeType(value);
			if (m_Start == nullptr) {
				m_Start = newNode;
				m_End = newNode;
				return;
			}

			newNode->next = m_Start;
			m_Start->before = newNode;
			m_Start = newNode;
		}

		void PushBack(const T& value) {
			NodeType* newNode = new NodeType(value);
			if (m_End == nullptr) {
				m_Start = newNode;
				m_End = newNode;
				return;
			}
			newNode->before = m_End;
			m_End->next = newNode;
			m_End = newNode;
		}

		T& PopFront() {
			if (Empty()) throw std::out_of_range("Trying to pop value from empty linked list");
			T value = m_Start->value; //copy value because node is going to be destroyed
			if (m_Start == m_End) {
				m_Start = nullptr;
				m_End = nullptr;
				return value;
			}

			m_Start->next->before = nullptr;
			NodeType* toDelete = m_Start;
			m_Start = m_Start->next;
			delete toDelete;
			return value;
		}

		T& PopBack() {
			if (Empty()) throw std::out_of_range("Trying to pop value from empty linked list");
			T value = m_End->value; //copy value because node is going to be destroyed
			if (m_Start == m_End) {
				m_Start = nullptr;
				m_End = nullptr;
				return value;
			}

			m_End->before->next = nullptr;
			NodeType* toDelete = m_End;
			m_End = m_End->before;
			delete toDelete;
			return value;
		}

		NodeType& Front() {
			return *m_Start;
		}

		NodeType& Back() {
			return *m_End;
		}

	private:
		NodeType* m_Start;
		NodeType* m_End;
	};


}