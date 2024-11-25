#include<list_algos.h>

typedef int (*List_compare)(const void* a, const void* b);

int List_bubble_sort(List* list, List_compare cmp){
    if(List_count(list) <= 1) {
        return 0;
    }

    LIST_FOREACH(list, first, next, cur){
        for(ListNode* node = cur->next; node; node = node->next){
            if(cmp(cur->value, node->value) > 0){ // if cur > node
                void* temp = cur->value;
                cur->value = node->value;
                node->value = temp;
            }
        }
    }

    return 0;
}


List* List_merge_sort(List* list, List_compare cmp)
{
    if(List_count(list) <= 1) {
        return list;
    }

    List* left = List_create();
    List* right = List_create();
    int middle = List_count(list) / 2;

    LIST_FOREACH(list, first, next, cur) {
        if(middle > 0) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }

        middle--;
    }

    List* sort_left = List_merge_sort(left, cmp);
    List* sort_right = List_merge_sort(right, cmp);

    if(sort_left != left) List_destroy(left);
    if(sort_right != right) List_destroy(right);

    return List_merge(sort_left, sort_right, cmp);
}


List* List_merge(List* left, List* right, List_compare cmp){
   List *merge = List_create();

    while(List_count(left) > 0 || List_count(right) > 0) {
        if(List_count(left) > 0 && List_count(right) > 0) {
            if(cmp(List_first(left), List_first(right)) <= 0) {
                List_push(merge, List_shift(left));
            } else {
                List_push(merge, List_shift(right));
            }
        } else if(List_count(left) > 0) {
            List_push(merge, List_shift(left));
        } else if(List_count(right) > 0) {
            List_push(merge, List_shift(right));
        }
    }

    return merge; 
}
