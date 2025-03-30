#include "list.h"

#define Queue_create List_create
#define Queue_destroy List_destroy
#define Queue_send List_push
#define Queue_recv List_shift
#define Queue_count List_count
#define Queue_peek List_first
#define QUEUE_FOREACH(L, V) ListNode *V = NULL; for(V = L->first; V != NULL; V = V->next)
#define Queue List
