#ifndef HEAP_ALGO_H
#define HEAP_ALGO_H
#include "alloc_algo.h"
template <typename T>
class Heap {
	protected:
		int m_count;
		int m_ncapacity;
		T* m_phead;
	public:
		T* m_dp;
		Heap(int size) {
			m_phead = reinterpret_cast<T*>(default_allocator::allocate(sizeof(T) * size));
			m_dp = m_phead;
			m_ncapacity = size;
			m_count = 0;
		}
		~Heap() {
			default_allocator::deallocate(m_phead,sizeof(T) * m_ncapacity);
			m_phead = NULL;
		}
		int size() {
			return m_count;
		}
		bool is_full() {
			return m_count == m_ncapacity;
		}
		void push(T e) {
			int i;
			if(is_full()) {
				printf("heap full!\n");
				return;
			}
			for(i = ++m_count; m_phead[i/2] > e;i/=2)
				m_phead[i] = m_phead[i/2];
			m_phead[i] = e;
		}
		T pop() {
			if(0 == m_count) {
				printf("heap empty!\n");
				return m_phead[0];
			}
			T min_element = m_phead[1];
			T last_element = m_phead[m_count--];
			int i,child;
			for(i = 1;i*2 <= m_count;i = child) {
				child = i*2;
				if(child < m_count && m_phead[child] > m_phead[child+1])
					child++;
				if(last_element > m_phead[child])
					m_phead[i] = m_phead[child];
				else
					break;
			}
			m_phead[i] = last_element;
			return min_element;
		}
};

#endif
