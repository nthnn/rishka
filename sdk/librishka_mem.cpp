/* 
 * This file is part of the Rishka distribution (https://github.com/nthnn/rishka).
 * Copyright (c) 2024 Nathanne Isip.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "librishka.h"
#include "librishka_impl.hpp"

#define MEMORY_POOL_SIZE 524288U

/** @cond HIDE_STRUCT */
typedef struct memory_pool {
    u64 size;
    i32 free;
    struct memory_pool* next;
} memory_pool;
/** @endcond */

static u8 memory_pool_block[MEMORY_POOL_SIZE];
static memory_pool* free_list = (memory_pool*) nil;

#define ALIGN8(x) (((((x) - 1) >> 3) << 3) + 8)

static void split_block(memory_pool* block, u64 size) {
    memory_pool* new_block = (memory_pool*)
        ((u32*) block + sizeof(memory_pool) + size);

    new_block->size = block->size - size - sizeof(memory_pool);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->free = 0;
    block->next = new_block;
}

static void merge_blocks() {
    memory_pool* current = free_list;

    while(current != nil && current->next != nil) {
        if(current->free && current->next->free) {
            current->size += sizeof(memory_pool) + current->next->size;
            current->next = current->next->next;
        }

        current = current->next;
    }
}

void Memory::initialize() {
    free_list = (memory_pool*) memory_pool_block;
    free_list->size = MEMORY_POOL_SIZE - sizeof(memory_pool);
    free_list->free = 1;
    free_list->next = (memory_pool*) nil;
}

any Memory::alloc(usize size) {
    size = ALIGN8(size);

    memory_pool* current = free_list;
    memory_pool* previous = (memory_pool*) nil;

    while(current != nil) {
        if(current->free && current->size >= size) {
            if(current->size > size + sizeof(memory_pool))
                split_block(current, size);
            else current->free = 0;

            return (void*)((u32*) current + sizeof(memory_pool));
        }

        previous = current;
        current = current->next;
    }

    return nil;
}

void Memory::free(any ptr) {
    if(ptr == nil)
        return;

    memory_pool* block = (memory_pool*)((u32*) ptr - sizeof(memory_pool));
    block->free = 1;

    merge_blocks();
}

any Memory::calloc(usize num, usize size) {
    usize total_size = num * size;
    any ptr = Memory::alloc(total_size);

    if(ptr != nil)
        Memory::set(ptr, 0, total_size);
    return ptr;
}

any Memory::realloc(any ptr, usize size) {
    if(ptr == nil)
        return Memory::alloc(size);

    if(size == 0) {
        Memory::free(ptr);
        return nil;
    }

    memory_pool* block = (memory_pool*)((u8*) ptr - sizeof(memory_pool));
    if(block->size >= size)
        return ptr;

    any new_ptr = Memory::alloc(size);
    if(new_ptr == nil)
        return nil;

    usize copy_size = (block->size < size) ? block->size : size;
    for(usize i = 0; i < copy_size; i++)
        ((rune*) new_ptr)[i] = ((rune*) ptr)[i];

    Memory::free(ptr);
    return new_ptr;
}

__attribute__((optimize("no-tree-loop-distribute-patterns")))
any Memory::set(any ptr, u8 value, usize num) {
    u8* p = (u8*) ptr;

    while(num--)
        *p++ = value;
    return ptr;
}