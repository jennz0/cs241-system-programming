/**
 * malloc
 * CS 241 - Spring 2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>


typedef struct metadata_entry_t {
    void * ptr;
    size_t size;
    int free;//0 in use or 1 avalable
    size_t requested;

    struct metadata_entry_t * next;
    struct metadata_entry_t * prev;

} metadata_entry_t;

static metadata_entry_t * head = NULL;
static size_t total_requested = 0;
static size_t total_sbrk = 0;

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    // implement calloc!
    size_t memsum = num * size;
    void * result = malloc(memsum);
    if(!result) {return NULL;}
    /*for(size_t i = 0; i < total; i++) {
        ((char*)result)[i] = 0;
    }*///method 1
    memset(result, 0, memsum);//method 2
    return result;
    
}
void clscprev(metadata_entry_t *p) {
    p->size += p->prev->size+sizeof(metadata_entry_t);
    p->prev = p->prev->prev;
    if (p->prev==NULL) {
        head = p;
      
    }
    else {
      p->prev->next = p;
    }
}
int split(size_t size, metadata_entry_t *entry) {
  if (entry->size >= 2*size && (entry->size-size) >= 1024) {
    metadata_entry_t *newentry = entry->ptr + size;
    
    newentry->ptr = newentry + 1;
    newentry->free = 1;
    newentry->size = entry->size - size - sizeof(metadata_entry_t);
    newentry->next = entry;
    if (entry->prev) {
      entry->prev->next = newentry;
    } else {
      head = newentry;
    }
    newentry->prev = entry->prev;
    entry->size = size;
    entry->prev = newentry;

    return 1;
  }
  return 0;
}

void *malloc(size_t size) {
    // implement malloc!
    metadata_entry_t *p = head;
    metadata_entry_t *chosen =NULL;
    if (total_sbrk - total_requested >= size) {
        while (p != NULL) {
            if((size_t)p->size >= size && p->free) {
                chosen = p;
                if (split(size, p)) {
                    total_requested += sizeof(metadata_entry_t); // consider metadata when coalescing
                }
                break;
            }
            p = p->next;
        }
    }

    if (chosen!= NULL) {
        chosen->free = 0;
        total_requested += chosen->size;
        //return chosen->ptr;
    } else {
        if (head && head->free) {
            if (sbrk(size-head->size) == (void *)-1) {return NULL;}
            total_sbrk += size-head->size;
            head->size = size;
            head->free = 0;
            chosen = head;
            total_requested += head->size;
        } else { 
            chosen = sbrk(sizeof(metadata_entry_t)+size);
            if (chosen == (void *)-1) {return NULL;}
            chosen->size = size;
            chosen->free = 0;
            chosen->ptr = chosen + 1;
            total_sbrk += sizeof(metadata_entry_t)+size;
            total_requested += sizeof(metadata_entry_t)+size;
            chosen->next = head;
            if (head) {
                chosen->prev = head->prev;
                head->prev = chosen;
            } else {
                chosen->prev = NULL;
            }
            head = chosen;
            
        }
    }
    return chosen->ptr;
}

void clscFragment(metadata_entry_t *ptrpara) {
  if (ptrpara->prev && ptrpara->prev->free == 1) {
    
    clscprev(ptrpara);
    total_requested -= sizeof(metadata_entry_t);
  }
  if (ptrpara->next && ptrpara->next->free == 1) {
    ptrpara->next->size += ptrpara->size+sizeof(metadata_entry_t);
    ptrpara->next->prev = ptrpara->prev;
    if (ptrpara->prev) {
      ptrpara->prev->next = ptrpara->next;
    } else {
      head = ptrpara->next;
    }
    total_requested -= sizeof(metadata_entry_t);
  }
}
//////////////////////////////
void free(void *ptr) {
    if (!ptr) return;
    metadata_entry_t * p = ptr - sizeof(metadata_entry_t);
    assert(p->free == 0);
    p->free = 1;
    total_requested -= p->size;
    clscFragment(p);

    // implement free!
    /*if(!ptr) return;
    metadata_entry_t * p = head;
    */
}


void *realloc(void *ptr, size_t size) {
    // implement realloc!/
    ///////
    if(ptr ==NULL) {return malloc(size);}
    metadata_entry_t* entry = ((metadata_entry_t*)ptr) -1;
    assert(entry->ptr==ptr);
    assert(entry->free==0);
    if (!size) {free(ptr);}

    size_t oldsize = entry->size;
    /*split(size, entry);

    if (oldsize > size) {
        return NULL;
    }*/
    if (split(size, entry)) {
      total_requested -= entry->prev->size;
    }
    // printf("realloc segfault end\n");
    if (entry->size >= size) {
      return ptr;
    } else if (entry->prev && entry->prev->free && entry->size+entry->prev->size+sizeof(metadata_entry_t) >= size) {
      total_requested += entry->prev->size;
      clscprev(entry);
      return entry->ptr;
    }

    void* result = malloc(size);
    size_t minsize = oldsize;
    if (size < oldsize) {
        minsize = size;
    }
    memcpy(result, ptr, minsize);
    free(ptr);
    return result;
}
