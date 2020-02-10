
; xmm0 - a
; xmm1 - b
; xmm2 - c
; xmm3 - step
; rdi - buf (start of the graphics data)
; rsi - width
; rdx - height
; rcx - scale
; xmm4 - X = -scale + 2*scale(x / width)
; xmm5 - Y = aX^2 + bX + c
; xmm6 - bX
; xmm7 - scale in double
; r8 - prev x
; r9 - prev y ; rbx - y ; rax - x
section .data
    bytes_per_row dq 0x65

section .text
    global quadratic_func

quadratic_func:
    push rbp            ; push "calling procedure" frame pointer
    mov rbp, rsp        ; set new frame pointer
   
    mov [bytes_per_row], r8

    ; we draw first pixel outside of the loop
    xor rax, rax ; x = 0
    xor r15, r15
    mov r8, rax

    call calc_y

    cvtsd2si rbx, xmm5 ; if (y > height) go to end

    mov r9, rbx

    cmp rbx, rdx
    jge loop
    cmp rbx, 0
    jl loop

    call draw_pixel
loop:
    mov rax, r15
    add rax, 10

    call calc_y
   
    add rsi, 10
    cmp rax, rsi
    jge end
    sub rsi, 10

    cvtsd2si rbx, xmm5 ; if (y > height) go to end
    
    push rax
    push rdx
    push rbx

    sub rax, r8
    mov r12, rax
    sub rbx, r9
    mov r13, rbx

    pop rbx
    pop rdx
    pop rax

    cmp r13, r12
    jg inverse_up

    push rax
    push rdx
    mov rax, r13
    mov r13, -1
    mul r13
    mov r13, rax
    pop rdx
    pop rax

    cmp r13, r12
    jg inverse_down

    mov r14, rax
    mov r15, rax
    sub rax, 10
normal:
    inc rax
    cmp rax, rsi
    jge end

    cmp rax, r14
    jg loop

    call calc_y

    cvtsd2si rbx, xmm5

    cmp rbx, rdx
    jge normal
    cmp rbx, 0
    jl normal

    cvtsd2si r11, xmm3

    push rax
    push rdx
    push rbx
    
    sub rax, r8
    mul rax
    mov r10, rax
    sub rbx, r9
    mov rax, rbx
    mul rax
    add r10, rax

    mov rax, r11
    mul rax
    mov r11, rax

    pop rbx
    pop rdx
    pop rax

    cmp r10, r11
    jl normal
   
    call draw_pixel

    mov r8, rax
    mov r9, rbx

    jmp normal
end:
    mov     rsp, rbp    ; restore original stack pointer
    pop     rbp         ; restore "calling procedure" frame pointer
    ret



inverse_down:
    mov r11, rbx
    mov rbx, r9

    mov r15, rax
loop2:
    dec rbx
    cmp rbx, r11
    jl loop

    cmp rbx, 0
    jl loop2
    cmp rbx, rdx
    jge loop2

    call inverse_both

    jmp loop2


inverse_up:
    mov r11, rbx
    mov rbx, r9

    mov r15, rax
loop3:
    inc rbx
    cmp rbx, r11
    jg loop

    cmp rbx, 0
    jl loop3
    cmp rbx, rdx
    jge loop3

    call inverse_both

    jmp loop3


inverse_both:
    push rbp
    mov rbp, rsp

    call calc_x

    cvtsd2si r12, xmm4
    cvtsd2si r13, xmm6

    cmp r12, r13
    jl second_bigger
first_bigger:
    cmp r13, r8
    jge draw_second
draw_first:
    mov rax, r12
    jmp draw
second_bigger:
    cmp r12, r8
    jge draw_first
draw_second:
    mov rax, r13

draw:
    cmp rax, rsi
    jge end
    cmp rax, 0
    jl inverse_end

    push r11
    cvtsd2si r11, xmm3

    push rax
    push rdx
    push rbx
    
    sub rax, r8
    mul rax
    mov r10, rax
    sub rbx, r9
    mov rax, rbx
    mul rax
    add r10, rax

    mov rax, r11
    mul rax
    mov r11, rax

    pop rbx
    pop rdx
    pop rax

    cmp r10, r11
    pop r11
    jl inverse_end

    call draw_pixel
    mov r8, rax
    mov r9, rbx

inverse_end:
    mov rsp, rbp
    pop rbp
    ret

calc_y:
    push rbp
    mov rbp, rsp
    push r9

    mov r10, rax
    shl r10, 1   ; X1 = 2*x
    cvtsi2sd xmm4, r10
    cvtsi2sd xmm9, rsi
    cvtsi2sd xmm7, rcx
    divsd xmm4, xmm9   ; X2 = 2 * x / width
    mulsd xmm4, xmm7   ; X3 = 2*scale*(x/width)
    subsd xmm4, xmm7   ; X = f(x) = X3 - scale

    movsd xmm5, xmm4
    mulsd xmm5, xmm4   ; X^2
    mulsd xmm5, xmm0   ; aX^2
    movsd xmm6, xmm4
    mulsd xmm6, xmm1   ; BX
    addsd xmm5, xmm6
    addsd xmm5, xmm2   ; Y = f(X)

    mov r9, 2
    cvtsi2sd xmm10, r9
    movsd xmm9, xmm7   ; xmm9 = scale
    mulsd xmm9, xmm10  ; scale * 2
    divsd xmm5, xmm9   ; y1 = Y / (2*scale)
    cvtsi2sd xmm9, rdx
    mulsd xmm5, xmm9   ; y2 = y1 * height
    divsd xmm9, xmm10
    addsd xmm5, xmm9   ; y = y2 + height / 2

    
    pop r9
    mov rsp, rbp
    pop rbp
    ret

; results in xmm4 and xmm6
calc_x:
    push rbp
    mov rbp, rsp
    push r9
    push r12

    mov r10, rbx
    shl r10, 1   ; Y1 = 2*x
    cvtsi2sd xmm4, r10
    cvtsi2sd xmm9, rdx
    cvtsi2sd xmm7, rcx
    divsd xmm4, xmm9   ; Y2 = 2 * y / height
    mulsd xmm4, xmm7   ; Y3 = 2*scale*(y/height)
    subsd xmm4, xmm7   ; Y = f(y)

    movsd xmm5, xmm1
    mulsd xmm5, xmm1   ; b^2
    mov r12, 4
    cvtsi2sd xmm6, r12
    mulsd xmm6, xmm0
    movsd xmm7, xmm2
    subsd xmm7, xmm4
    mulsd xmm6, xmm7
    subsd xmm5, xmm6    ; delta

    cvtsd2si r12, xmm5
    cmp r12, 0
    jl calc_end

    sqrtsd xmm5, xmm5
    
    xor r12, r12
    cvtsi2sd xmm4, r12
    subsd xmm4, xmm1
    movsd xmm6, xmm4
    addsd xmm4, xmm5
    subsd xmm6, xmm5
    mov r12, 2
    cvtsi2sd xmm7, r12
    mulsd xmm7, xmm0
    divsd xmm4, xmm7
    divsd xmm6, xmm7

    cvtsi2sd xmm7, rcx

    mov r9, 2
    cvtsi2sd xmm10, r9
    movsd xmm9, xmm7   ; xmm9 = scale
    mulsd xmm9, xmm10  ; scale * 2
    divsd xmm4, xmm9   ; y1 = Y / (2*scale)
    cvtsi2sd xmm9, rdx
    mulsd xmm4, xmm9   ; y2 = y1 * height
    divsd xmm9, xmm10
    addsd xmm4, xmm9   ; y = y2 + height / 2

    mov r9, 2
    cvtsi2sd xmm10, r9
    movsd xmm9, xmm7   ; xmm9 = scale
    mulsd xmm9, xmm10  ; scale * 2
    divsd xmm6, xmm9   ; y1 = Y / (2*scale)
    cvtsi2sd xmm9, rdx
    mulsd xmm6, xmm9   ; y2 = y1 * height
    divsd xmm9, xmm10
    addsd xmm6, xmm9   ; y = y2 + height / 2

    ; back to pixels
calc_end:
    pop r12
    pop r9
    mov rsp, rbp
    pop rbp
    ret

draw_pixel:
    push rbp
    mov rbp, rsp

    push rax
    push rcx
    push rdx
    push r11

    mov rcx, 3
    mul rcx
    mov rcx, rax
    
    xor rax, rax
    mov rax, [bytes_per_row]
    ;mov rax, rsi
    mul rbx
    ;mov r10, 3
    ;mul r10
    add rax, rcx

    mov r11, rdi
    add r11, rax
    mov al, 0

    mov [r11], al
    mov [r11+1], al
    mov [r11+2], al

    pop r11
    pop rdx
    pop rcx
    pop rax

    mov rsp, rbp
    pop rbp
    ret
