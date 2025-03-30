#include "list.h"

#define Stack_create List_create
#define Stack_destroy List_destroy
#define Stack_push List_push
#define Stack_pop List_pop

#define Stack_peek List_last
#define Stack_count List_count
#define STACK_FOREACH(L, V) ListNode *V = NULL; for(V = L->first; V != NULL; V = V->next)
#define Stack List
