#ifndef LINK_ALGO_H
#define LINK_ALGO_H

/**invasive slink and dlink :has a dummy header*/
typedef struct __SLINK{
	__SLINK *next;
}SLINK;

#define SLINK_INIT(head)	((head)->next = NULL)

#define SLINK_IS_EMPTY(head)	((head)->next == NULL)

#define SLINK_TOP(head)	(head)->next

#define SLINK_POP(head)	(head)->next;							\
	                    if(NULL != (head)->next) {				\
                            (head)->next = (head)->next->next;	\
						}

#define SLINK_PUSH(head,node)	(node)->next = (head)->next;	\
								(head)->next = (node)


typedef struct __DLINK{
	__DLINK *prev;
	__DLINK *next;
}DLINK;

#define DLINK_INIT(head)	(head)->prev = (head)->next = (head)

#define DLINK_IS_EPMTY(head)	((head)->next == head && (head)->prev == head)

#define DLINK_INSERT_NEXT(head,node)	(node)->next = (head)->next;\
										(node)->prev =  (head);\
										(head)->next->prev = (node);\
										(head)->next = (node)

#define DLINK_INSERT_PREV(head,node)	(node)->prev = (head)->prev;\
										(node)->next =  (head);\
										(head)->prev->next = (node);\
										(head)->prev = (node)

#define DLINK_REMOVE(node)		(node)->prev->next = (node)->next;\
								(node)->next->prev = (node)->prev

#define DLINK_REPLACE(_old,_new)	(_old)->prev->next = (_new);\
									(_old->next->prev) = (_new);\
									(_new)->prev = (_old)->prev;\
									(_new)->next = (_old)->next

//get address of structure type
#define RECORD_OF(address,type,field) \
	(type*)((char*)address - (char*)&((type*)0)->field)



/*
 * template SList
 * T must contain SLINK link
 */
template <typename T>
class SList {
	protected:
		int m_count;
		SLINK m_head;
	public:
		SList() {
			SLINK_INIT(&m_head);
			m_count = 0;
		}
		~SList() {}
		void push(T *e) {
			SLINK_PUSH(&m_head,&e->link);
			m_count++;
		}

		T* pop() {
			SLINK *p = SLINK_POP(&m_head);
			if(NULL != p) {
				m_count--;
				SLINK_INIT(p);
				return RECORD_OF(p,T,link);
			}
			return NULL;
		}

		T* top() {
			SLINK *p = m_head.next;
			if(NULL != p) {
				return RECORD_OF(p,T,link);
			}
			return NULL;
		}
		
		bool contain(T* e) {
			SLINK *p;
			for(p = m_head.next;p;p = p->next) {
				if(&e->link == p) {
					return true;
				}
			}
			return false;
		}

		bool remove(T* e) {
			SLINK *p = &m_head;
			while(NULL != p->next) {
				if(p->next == &e->link) {
					p->next = p->next->next;
					SLINK_INIT(&e->link);
					m_count--;
					return true;
				}
				p = p->next;
			}
			return false;
		}

		void reverse() {
			if(1 >= m_count) return;
			SLINK *p, *c, *n;
			p = m_head.next;
			c = p->next;
			n = c->next;
			int temp = m_count;
			while(temp > 2) {
				c->next = p;
				p = c;
				c = n;
				n = n->next;
				temp--;
			}
			c->next = p;
			m_head.next = c;
		}

		int size() {
			return m_count;
		}
};


/*
 * template DList
 * T must contain DLINK link
 */
template <typename T>
class DList {
	protected:
		int m_count;
		DLINK m_head;
	public:
		DList() {
			DLINK_INIT(&m_head);
			m_count = 0;
		};
		~DList() {}
		void push_front(T* e) {
			DLINK_INSERT_NEXT(&m_head,&e->link);
			m_count++;
		}


		void push_back(T* e) {
			DLINK_INSERT_PREV(&m_head,&e->link);
			m_count++;
		}

		T* pop_front() {
			DLINK *p = m_head.next;
			if(p != &m_head) {
				DLINK_REMOVE(p);
				DLINK_INIT(p);
				m_count--;
				return RECORD_OF(p,T,link);
			}
			return NULL;
		}

		T* pop_back() {
			DLINK *p = m_head.prev;
			if(p != &m_head) {
				DLINK_REMOVE(p);
				DLINK_INIT(p);
				m_count--;
				return RECORD_OF(p,T,link);
			}
			return NULL;
		}

		T* top() {
			DLINK *p = m_head.next;
			if(p != &m_head) {
				return RECORD_OF(p,T,link);
			}
			return NULL;
		}
		
		T* bottom() {
			DLINK *p = m_head.prev;
			if(p != &m_head) {
				return RECORD_OF(p,T,link);
			}
			return NULL;
		}

		bool contain(T* e) {
			for(DLINK* p = m_head.next;p != &m_head;p = p->next) {
				if(&e->link == p) {
					return true;
				}
			}
			return false;
		}

		bool remove(T* e) {
			if(!contain(e)) {
				printf("[%s:%d]:%s\n",__FILE__,__LINE__,"element non-existing");
				return false;
			}
			DLINK_REMOVE(&e->link);
			DLINK_INIT(&e->link);
			m_count--;
			return true;
		}
		
		int size() {
			return m_count;
		}
};

#endif
