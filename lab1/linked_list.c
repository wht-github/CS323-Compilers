#include "linked_list.h"

node *linked_list_init(){
    node *head = (node*)malloc(sizeof(node));
    head->count = 0;
    head->next = NULL;
    return head;
}

void linked_list_free(node *head){
    node *cur = head;
    node *last;
    while(cur != NULL){
        last = cur;
        cur = cur->next;
        free(last);
    }
}

char linked_list_string[0x10000];

char *linked_list_tostring(node *head){
    node *cur = head->next;
    char *position;
    int length = 0;
    while(cur != NULL){
        position = linked_list_string + length;
        length += sprintf(position, "%d", cur->value);
        cur = cur->next;
        if(cur != NULL){
            position = linked_list_string + length;
            length += sprintf(position, "->");
        }
    }
    position = linked_list_string + length;
    length += sprintf(position, "%c", '\0');
    return linked_list_string;
}

int linked_list_size(node *head){
    return head->count;
}

void linked_list_append(node *head, int val){
    node *cur = head;
    node *new_node;
    while(cur->next != NULL){
        cur = cur->next;
    }
    new_node = (node*)malloc(sizeof(node));
    new_node->value = val;
    new_node->next = NULL;
    cur->next = new_node;
    head->count++;
}

/* your implementation goes here */
/* insert val at position index */
void linked_list_insert(node *head, int val, int index){

    if(head->count < index+1){
        return;
    } else if(head->count == index){
        linked_list_append(head,val);
        // head->count++;
    } else {
        int i = -1;
        node * tmp = head;
        do{
            tmp = tmp->next;
            ++i;
        }while(i != index-1);
        node *ins = (node*)malloc(sizeof(node));
        ins->value = val;
        ins->next = tmp->next;
        tmp->next = ins;
        head->count++; 
    }
    
}

/* delete node at position index */
void linked_list_delete(node *head, int index){
    if(head->count < index+1 || index < 0){
        return;
    } else {
        int i = 0;
        node * tmp = head;
        
        do{
            i++;
            tmp = tmp->next;
        }while (i != index);
        // tmp = tmp->next;
        node * x  =tmp->next;
        tmp->next = tmp->next->next;
        free(x);
        // if(tmp->next == NULL){
        //     tmp=NULL;
        // } else
        // {
        //     node * ttmp = tmp->next;
        //     if(ttmp->next==NULL)
        //         tmp->next = NULL;
        //     else{
        //         tmp->next = tmp->next->next;
        //         // free(ttmp);
        //         return;
        //     }
        //     // free(ttmp);
        // }
        
        head->count--;
    }
    
}

/* remove the first occurence node of val */
void linked_list_remove(node *head, int val){
    node *tbr = head;
    while (tbr->next != NULL && tbr->next->value != val)
    {
        tbr = tbr->next;
    }
    if(tbr->next != NULL){
        node * tmp = tbr->next;
        tbr->next = tbr->next->next;
        free(tmp);
        head->count--;
    }
    
}

/* remove all occurences of val */
void linked_list_remove_all(node *head, int val){
    node *tbr = head;
    while(tbr->next != NULL){
        while (tbr->next != NULL && tbr->next->value != val)
        {
            tbr = tbr->next;
        }
        if(tbr->next != NULL){
            node * tmp = tbr->next;
            tbr->next = tbr->next->next;
            free(tmp);
            head->count--;
        }
    }

}

/* get value at position index */
int linked_list_get(node *head, int index){
    if(head->count >= index+1){
        int i = -1;
        while(i != index){
            head = head->next;
            i++;
        }
        return head->value;
    } else {
        return -0x80000000;
    }

}

/* search the first index of val */
int linked_list_search(node *head, int val){
    node *tbr = head;
    int i = 0;
    while (tbr->next != NULL && tbr->next->value != val)
    {
        tbr = tbr->next;
        i++;
    }
    if(tbr->next != NULL){
        return i;
    }else{
        return -1;
    }
}

/* search all indexes of val */
node *linked_list_search_all(node *head, int val){
    node * h = linked_list_init();
    // node * x = h;
    node *tbr = head;
    int i = 0;
    while (1)
    {
        
        while (tbr->next != NULL && tbr->next->value != val)
        {
            tbr = tbr->next;
            i++;
        }
        if(tbr->next != NULL){
            linked_list_append(h,i++);
            tbr = tbr->next;
        }else
        {
            // linked_list_append(h,i);
            // printf("f");
            break;
        }
        
    }
    return h;

}
