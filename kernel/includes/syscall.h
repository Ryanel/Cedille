#ifndef SYSCALL_H
#define SYSCALL_H
/// Generic Syscall Handler
typedef void (*syscall_handler_t)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t); 
#endif