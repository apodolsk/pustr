.global xadd
.global xchg
.global cmpxchg
.global cmpxchg2

#if !__LP64__

xadd:
    movl 4(%esp), %eax
    movl 8(%esp), %edx
    
    lock xadd %eax, (%edx)
    
    ret
    
xchg:   
    movl    4(%esp), %eax
    movl    8(%esp), %ecx

    xchg    %eax, (%ecx)

    ret

cmpxchg:
    movl 4(%esp), %edx
    movl 8(%esp), %ecx
    movl 12(%esp), %eax

    lock cmpxchg %edx, (%ecx)
    ret

cmpxchg2:
    pushl %ebx
    pushl %esi
    
    movl 12(%esp), %ebx
    movl 16(%esp), %ecx
    movl 20(%esp), %esi
    movl 24(%esp), %eax
    movl 28(%esp), %edx

    lock cmpxchg8b  (%esi)

    popl %esi
    popl %ebx
    ret

#else
    
xadd:
    push %rbp
    mov %rsp, %rbp
    
    lock xadd %rdi, (%rsi)
    mov %rdi, %rax

    pop %rbp
    ret
    
xchg:
    push %rbp
    mov %rsp, %rbp
    
    lock xchg %rdi, (%rsi)
    mov %rdi, %rax

    pop %rbp    
    ret
    
cmpxchg:
    push %rbp
    mov %rsp, %rbp
    
    mov %rdx, %rax
    lock cmpxchg %rdi, (%rsi)

    pop %rbp    
    ret
    
cmpxchg2:
    push %rbp
    mov %rsp, %rbp
    
    push %rbx
    
    mov %rdi, %rbx
    mov %rcx, %rax
    mov %rsi, %rcx
    mov %rdx, %rsi
    mov %r8, %rdx

    lock cmpxchg16b  (%rsi)

    pop %rbx
    
    pop %rbp    
    ret
    
#endif
