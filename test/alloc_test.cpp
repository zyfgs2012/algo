#include "../alloc_algo.h"
/*Remain to be tested in use*/
int main() {
	char *vp[120];
	for(int i=0;i<20;i++) {
		vp[i] = reinterpret_cast<char*>(default_allocator::allocate(1));
		vp[i+20] = reinterpret_cast<char*>(default_allocator::allocate(32));
	}
	for(int i=0;i<5;i++) {
		default_allocator::deallocate(vp[i],1);
	}
	for(int i=0;i<5;i++) {
		vp[i] = reinterpret_cast<char*>(default_allocator::allocate(1));
	}
	for(int i=0;i<5;i++) {
		vp[i] = reinterpret_cast<char*>(default_allocator::reallocate(vp[i],1,16));
	}
	for(int i=0;i<5;i++) {
		default_allocator::deallocate(vp[i],16);
	}
	for(int i=0;i<5;i++) {
		vp[i] = reinterpret_cast<char*>(default_allocator::allocate(1));
	}

	return 0;
}
