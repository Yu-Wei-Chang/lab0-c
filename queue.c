#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    if (q->head) {
        list_ele_t *eleh, *elen; /* element head and next */
        eleh = q->head;
        while (eleh) {
            elen = eleh->next;
            free(eleh->value);
            free(eleh);
            if (elen)
                eleh = elen;
            else
                eleh = NULL;
        }
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (!q)
        return false;

    if ((newh = malloc(sizeof(list_ele_t))) == NULL)
        return false;

    if ((newh->value = malloc(strlen(s) + 1)) == NULL) {
        /* Free this element if we allocat value space failed. */
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s) + 1);
    newh->value[strlen(s)] = '\0';
    /* LIFO */
    newh->next = q->head;
    q->head = newh;
    /* Points tail to this element if this is the first element. */
    if (q->size == 0)
        q->tail = newh;
    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (!q)
        return false;

    if ((newh = malloc(sizeof(list_ele_t))) == NULL)
        return false;

    if ((newh->value = malloc(strlen(s) + 1)) == NULL) {
        /* Free this element if we allocat value space failed. */
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s) + 1);
    newh->value[strlen(s)] = '\0';
    /* FIFO */
    newh->next = NULL;
    if (q->size == 0) {
        /* Points head to this element if this is the first element. */
        q->head = newh;
    } else
        q->tail->next = newh;
    q->tail = newh;

    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *ele;
    if (!q || !q->head)
        return false;

    ele = q->head;
    q->head = q->head->next;
    q->size--;
    if (q->size == 0)
        q->tail = NULL;

    if (sp) {
        if (strlen(ele->value) > bufsize) {
            strncpy(sp, ele->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        } else {
            strncpy(sp, ele->value, strlen(ele->value));
            sp[strlen(ele->value)] = '\0';
        }
    }
    free(ele->value);
    free(ele);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *ele_prev, *ele_cur, *ele_next;
    if (!q || !q->head)
        return;

    ele_prev = NULL;
    q->tail = ele_cur = q->head;
    while (ele_cur) {
        ele_next = ele_cur->next;
        ele_cur->next = ele_prev;
        ele_prev = ele_cur;
        ele_cur = ele_next;
    }
    q->head = ele_prev;
}

static list_ele_t *q_mergeSort(list_ele_t *start)
{
    if (!start || !start->next)
        return start;

    list_ele_t *left = start;
    list_ele_t *right = start->next;
    left->next = NULL;

    left = q_mergeSort(left);
    right = q_mergeSort(right);

    for (list_ele_t *merge = NULL; left || right;) {
        if (!right || (left && (strncmp(left->value, right->value,
                                        strlen(left->value)) < 0))) {
            if (!merge) {
                start = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        } else {
            if (!merge) {
                start = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return start;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    list_ele_t *elet;
    if (!q || !q->head)
        return;

    if (q->size <= 1)
        return;

    q->head = q_mergeSort(q->head);
    elet = q->head;
    while (elet->next) {
        elet = elet->next;
    }
    q->tail = elet;
}
