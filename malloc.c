#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct __attribute__((__packed__)) __malloc_link
{
    void* start;
    void* next;
};

struct __malloc_link __MALLOC_GUARD = {NULL, NULL};
struct __malloc_link* __MALLOC_GLOBAL_HEAD = &__MALLOC_GUARD;


#ifdef DEBUG
void
print_linked_list(struct __malloc_link* head)
{
    while (head)
    {
        fprintf(stdout, "\tnode[%p] = {start = %p; next %p}\n",
                        head, head->start, head->next);
        head = head->next;
    }
}
#endif

void*
malloc(size_t size)
{
    struct __malloc_link* header;
    void* candidate;

    if (size == 0)
        return NULL;

#ifdef DEBUG
    fprintf(stdout, "+ malloc %zu bytes requested.\n", size);
    fprintf(stdout, "\tcurrent sbrk: %p\n", sbrk(0));
#endif

    candidate = sbrk(size + sizeof(struct __malloc_link));

    /* failed */
    if (candidate < 0)
        return candidate;

    /* setup header */
    header = (struct __malloc_link*) candidate;
    header->start = candidate + sizeof(struct __malloc_link);

    /* refresh head of list */
    header->next = __MALLOC_GLOBAL_HEAD;
    __MALLOC_GLOBAL_HEAD = header;

#ifdef DEBUG
    print_linked_list(__MALLOC_GLOBAL_HEAD);
#endif

    return header->start;
}

void
free(void* pointer)
{
    struct __malloc_link* prev = __MALLOC_GLOBAL_HEAD;
    struct __malloc_link* current = __MALLOC_GLOBAL_HEAD;
    
#ifdef DEBUG
    fprintf(stdout, "+ free address 0x%p\n", pointer);
#endif

    while (current && (current->start != pointer))
    {
        prev = current;
        current = current->next;
#ifdef DEBUG
        fprintf(stdout, "\twhile loop, checking current->next == %p\n",
                        current->next);
#endif
    }

#ifdef DEBUG
    fprintf(stdout, "\tmatched: start = %p, pointer = %p\n", current->start,
                    pointer);
#endif

    if (current && current->start)
    {
        /* remove from linked list */
        if (current == __MALLOC_GLOBAL_HEAD)
        {
            __MALLOC_GLOBAL_HEAD = current->next;
            brk(current);
        }
        else
            prev->next = current->next;

    }

#ifdef DEBUG
    print_linked_list(__MALLOC_GLOBAL_HEAD);
#endif

    return;
}
