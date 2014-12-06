#ifndef ALLOC_ALGO_H
#define ALLOC_ALGO_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

class malloc_allocator {
	public:
		static void* allocate(size_t n) {
			printf("malloc allocator:%d\n",n); //debug
			void* res = malloc(n);
			if(NULL == res) {
				printf("Error: Out Of Memory\n");
				//NEED: oom handler;
			}
			return res;
		}

		static void deallocate(void* p) {
			free(p);
		}

		static void* reallocate(void *p,size_t new_size) {
			void* res = realloc(p,new_size);
			if(NULL == res) {
				printf("Error: Out Of Memory\n");
				//NEED: oom handler;
			}
			return res;
		}
};
const int ALIGN = 8;
const int MAX_SIZE = 128;
const int NFREELIST = MAX_SIZE/ALIGN;

class default_allocator {
	private:
		static char* S_free_list[NFREELIST];
		static char* S_start;//start of free space;
		static char* S_end;// end of free sapce;
		static size_t S_size;//size of allocated memory;
		//NEED: a lock;
		static size_t round_up(size_t n) {
			return ((n + static_cast<size_t>(ALIGN) -1) & ~(static_cast<size_t>(ALIGN)-1));
		}

		static size_t free_list_index(size_t n) {
			return ((n + static_cast<size_t>(ALIGN) -1) / static_cast<size_t>(ALIGN) - 1);
		}
		
		static void set_next(void *p,void* n) {
			*(reinterpret_cast<void**>(p)) = n;
		}
		
		static void* get_next(void *p) {
			return *(reinterpret_cast<void**>(p));
		}
		
		static void* do_alloc(size_t obj_size,int &obj_n);
		static void* re_build(size_t obj_size);
	public:
		static void* allocate(size_t n) {
			void *ret;
			if(n > static_cast<size_t>(MAX_SIZE)) {
				ret = malloc_allocator::allocate(n);
			} else {
				size_t index =  free_list_index(n);
				if(NULL != S_free_list[index]) {
					ret = S_free_list[index];
					S_free_list[index] = reinterpret_cast<char*>(get_next(ret));
				} else {
					ret = re_build(round_up(n));
				}
				return ret;
			}
		}

		static void deallocate(void *p, size_t n) {
			if(n > static_cast<size_t>(MAX_SIZE)) {
				malloc_allocator::deallocate(p);
			} else {
				size_t index = free_list_index(n);
				set_next(p,S_free_list[index]);
				S_free_list[index] = reinterpret_cast<char*>(p);
			}
		}

		static void* reallocate(void *p, size_t old_size, size_t new_size) {
			if(old_size > static_cast<size_t>(MAX_SIZE) && 
					new_size > static_cast<size_t>(MAX_SIZE)) {
				return malloc_allocator::reallocate(p,new_size);
			}
			if(round_up(old_size) == round_up(new_size)) return p;
			void *ret = allocate(new_size);
			size_t copy_size = old_size > new_size ? new_size : old_size;
			memcpy(ret,p,copy_size);
			deallocate(p,old_size);
			return ret;
		}
};
char* default_allocator::S_start = 0;
char* default_allocator::S_end = 0;
size_t default_allocator::S_size = 0;
char* default_allocator::S_free_list[NFREELIST] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void* default_allocator::re_build(size_t obj_size) {
	printf("round up %d\n",obj_size); //debug
	int obj_n = 20; //what's the approximate number?
	char *begin = reinterpret_cast<char*>(do_alloc(obj_size,obj_n));
	if(NULL == begin || 1 == obj_n) return begin;
	// build free list;
	char *next, *curr;
	curr = begin + obj_size;
	next = curr + obj_size;
	char *limit = begin + obj_n * obj_size;
	while(next < limit) {
		set_next(curr,next);
		curr = next;
		next = curr + obj_size;
	}
	set_next(curr,NULL);//last object set_next = NULL;
	size_t index = free_list_index(obj_size);
	S_free_list[index] = begin + obj_size;
	//set_next(S_free_list[index],begin + obj_size);
	return begin;
}

void* default_allocator::do_alloc(size_t obj_size,int &obj_n) {
	void *ret;
	size_t total_size = obj_size * obj_n;
	size_t free_size = S_end - S_start;
	if(free_size >= total_size) {
		ret = S_start;
		S_start += total_size;
		return ret;
	} else if(free_size >= obj_size){
		obj_n = free_size / obj_size;
		ret = S_start;
		S_start += obj_n * obj_size;
		return ret;
	} // else have to perform malloc;
	//make use of free_size;
	if(0 != free_size) {
		size_t index =  free_list_index(obj_size);
		set_next(S_start,get_next(S_free_list[index]));
		S_free_list[index] = S_start;
	}
	// delegate to first level, for oom handler.
	S_start = reinterpret_cast<char*>(malloc_allocator::allocate(total_size));
	if(0 == S_start) { // oom handler make no sense, try to use what we have
		for(size_t i= obj_size;i<= MAX_SIZE;i+=ALIGN) {
			size_t index = free_list_index(i);
			if(NULL != S_free_list[index]) {
				S_start = S_free_list[index];
				S_end = S_start + i;
				S_free_list[index] = reinterpret_cast<char*>(get_next(S_free_list[index]));
				return (do_alloc(obj_size,obj_n));//adjust S_start;
			}
		}
		return NULL;//have to return NULL;
	}
	S_size += total_size;
	S_end = S_start + total_size;
	return (do_alloc(obj_size,obj_n));//adjust S_start;
}

#endif
