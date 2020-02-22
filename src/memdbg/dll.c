/*
	Implementation of "doubly linked Lists"
 */
#include <_memdbg.h>

void
dll_init(head)
	dll_t	*head;
{
	head->next = head;
	head->prev = head;
	head->head = head;
}

void
dll_insert_next(head, elem)
	dll_t	*head, *elem;
{
	elem->next = head->next;
	elem->prev = head;
	elem->head = head->head;
	elem->next->prev = elem;
	head->next = elem;
}

void
dll_insert_prev(head, elem)
	dll_t	*head, *elem;
{
	elem->next = head;
	elem->prev = head->prev;
	elem->head = head->head;
	elem->prev->next = elem;
	head->prev = elem;
}

void *
dll_remove_next(head)
	dll_t	*head;
{
	dll_t	*elem = head->next;

	if(elem == head) {
		return((dll_t *) 0);
	}
	head->next = elem->next;
	head->next->prev = head;
	return(elem);
}

void *
dll_remove_prev(head)
	dll_t	*head;
{
	dll_t	*elem = head->prev;

	if(elem == head) {
		return((dll_t *) 0);
	}
	head->prev = elem->prev;
	head->prev->next = head;
	return(elem);
}

void *
dll_remove(elem)
	dll_t	*elem;
{
	if(elem->next == elem) {
		return((dll_t *) 0);
	}
	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;
	return(elem);
}

leng_t
dll_count(head)
	dll_t	*head;
{
	dll_t	*elem;
	leng_t	 count = 0;

	if(head->next == NULL) {
		goto _door;
	}
	for(elem = head->next; elem != head; elem = elem->next) {
		count++;
	}

_door:
	return(count);
}

void
dll_prn(
	dll_t	*ptr,
	FILE	*out,
	leng_t	 n
) {
	leng_t	 cnt = 0LL;

	if(ptr != NULL) {
		cnt = dll_count(ptr->head);
	}
	PRNN(n); PRNO("(%p)(dll_t *)[%llu] {\n", ptr, cnt);
	if(ptr != NULL) {
		n++;
		PRNN(n); PRNO("prev : %p\n", ptr->prev);
		PRNN(n); PRNO("next : %p\n", ptr->next);
		PRNN(n); PRNO("head : %p\n", ptr->head);
		n--;
	} 
	PRNN(n); PRNO("}\n");
}

void
dll_dump(
	dll_t	*ptr,
	char	*name,
	FILE	*out,
	leng_t	 n
) {
	PRNN(n); PRNO("(%p)(dll_t *)[%llu] %s : {\n", ptr, (leng_t)sizeof(*ptr), TSTR(name));
	if(ptr != NULL) {
		n++;
		PRNN(n); PRNO("prev : %p\n", ptr->prev);
		PRNN(n); PRNO("next : %p\n", ptr->next);
		PRNN(n); PRNO("head : %p\n", ptr->head);
		n--;
	} else {
		PRNN(n); PRNO("NULL\n");
	}
	PRNN(n); PRNO("}\n");
}

