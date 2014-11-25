#include <cstdio>
#include "../link_algo.h"
//#include "../alloc_algo.h"

struct STest {
	int val;
	SLINK link;
};

struct DTest {
	int val;
	DLINK link;
};

int main() {
#if 1
	SList<STest> s;
	STest st[7];
	st[0].val = 0;
	st[1].val = 1;
	st[2].val = 2;
	st[3].val = 3;
	st[4].val = 4;
	st[5].val = 5;
	st[6].val = 6;
	s.push(&st[0]);
	s.push(&st[1]);
	s.push(&st[2]);
	s.push(&st[3]);
	s.push(&st[4]);
	s.push(&st[5]);
	s.push(&st[6]);
	int n = s.size();
	for(int i=0;i<=3;i++) {
		printf("top:%d,contains:%d:%d\n",s.top()->val,i,s.contain(&st[i]));
		s.pop();
	}
	s.remove(&st[0]);
	n = s.size();
	printf("contain 0:%d,size:%d\n",s.contain(&st[0]),n);
	printf("top:%d\n",s.top()->val);
	s.remove(&st[2]);
	n = s.size();
	printf("contain 2:%d,size:%d\n",s.contain(&st[2]),n);
	printf("top:%d\n",s.top()->val);
	s.pop();
	printf("contain 1:%d,size:%d\n",s.contain(&st[1]),s.size());

	s.push(&st[0]);
	s.push(&st[1]);
	s.push(&st[2]);
	s.push(&st[3]);
	s.push(&st[4]);
	s.push(&st[5]);
	s.push(&st[6]);

	s.reverse();
	n = s.size();
	for(int i=0;i<=n/2;i++) {
		printf("top:%d ",s.top()->val);
		s.pop();
	}
	printf("\n");
	
	s.reverse();
	n = s.size();
	printf("%d\n",n);
	for(int i=0;i<n;i++) {
		printf("top:%d ",s.top()->val);
		s.pop();
	}
	printf("\n");

#elif 0

	DList<DTest> d;
	DTest dt[6];
	dt[1].val = 1;
	dt[2].val = 2;
	dt[3].val = 3;
	dt[4].val = 4;
	dt[5].val = 5;
	
	d.push_front(&dt[3]);
	d.push_back(&dt[4]);
	d.push_front(&dt[2]);
	d.push_back(&dt[5]);
	d.push_front(&dt[1]);
	int n = d.size();
	for(int i=0;i<n;i++) {
		printf("bottom:%d,contains:%d:%d\n",d.bottom()->val,i,d.contain(&dt[i]));
		d.pop_back();
	}
	printf("size:%d\n",d.size());

	d.push_back(&dt[3]);
	d.push_front(&dt[2]);
	d.push_back(&dt[4]);
	d.push_front(&dt[1]);
	d.push_back(&dt[5]);
	n = d.size();
	for(int i=0;i<n;i++) {
		printf("top:%d,contains:%d:%d\n",d.top()->val,5-i,d.contain(&dt[5-i]));
		d.pop_front();
	}
	printf("size:%d\n",d.size());

	d.push_front(&dt[5]);
	d.push_front(&dt[4]);
	d.push_front(&dt[3]);
	d.push_front(&dt[2]);
	d.push_front(&dt[1]);
	d.remove(&dt[0]);
	d.remove(&dt[1]);
	d.remove(&dt[3]);
	d.remove(&dt[5]);
	n = d.size();
	for(int i=0;i<n;i++) {
		printf("bottom:%d\n",d.bottom()->val);
		d.pop_back();
	}
	printf("size:%d\n",d.size());

#endif
	return 0;
}
