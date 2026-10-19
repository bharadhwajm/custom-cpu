movl r1, 0xdf
movh r1, 0xb1

movl r2, 160
mov r3, 50
mul r2, r2, r3
add r2, r2, r1
movl r4, 159
add r3, r2, r4
movl r4, 0x00
movh r4, 0x10
store r2, [r4]
store r3, [r4+2]

movl r2, 160
mov r3, 59
mul r2, r2, r3
movl r3, 79
add r2, r2, r3
add r2, r2, r1
movl r4, 0x04
movh r4, 0x10
store r2, [r4]
store r3, [r4+2]

mov r2, -1
movl r3, 160
movl r4, 0x08
movh r4, 0x10
store r2, [r4]
store r3, [r4+2]

movl r2, 160
mov r3, r1
movl r4, 119
mul r4, r2, r4
add r4, r1, r4
movl r5, 0xff
drawBorder:
    storeb r5, [r3]
    storeb r5, [r4]
    add r3, r3, 1
    add r4, r4, 1
    sub r2, r2, 1
    cmp r2, r0
    jne drawBorder

movl r1, 0x00
movh r1, 0x10
load r2, [r1]
load r3, [r1+2]
movl r1, 160
mov r4, 20
movl r5, 0xff
drawPaddle:
    storeb r5, [r2]
    storeb r5, [r3]
    add r2, r2, r1
    add r3, r3, r1
    sub r4, r4, 1
    cmp r4, r0
    jne drawPaddle

movl r1, 0x04
movh r1, 0x10
load r2, [r1]
movl r5, 0xff
drawBall:
    storeb r5, [r2]

gameLoop:
    call moveBall
    movl r1, 0xe0
    movh r1, 0xfc
    load r2, [r1]
    cmp r2, 1
    je moveUpRight
    cmp r2, 2
    je moveDownRight
    cmp r2, 3
    je moveUpLeft
    cmp r2, 4
    je moveDownLeft
    jmp gameLoop

moveUpRight:
    movl r1, 0x02
    movh r1, 0x10
    load r2, [r1]
    jmp moveUp

moveDownRight:
    movl r1, 0x02
    movh r1, 0x10
    load r2, [r1]
    jmp moveDown

moveUpLeft:
    movl r1, 0x00
    movh r1, 0x10
    load r2, [r1]
    jmp moveUp

moveDownLeft:
    movl r1, 0x00
    movh r1, 0x10
    load r2, [r1]
    jmp moveDown

jmpGameLoop:
    jmp gameLoop

moveUp:
    movl r3, 160
    sub r4, r2, r3
    loadbu r5, [r4]
    cmp r5, r0
    jne jmpGameLoop
    movl r5, 0xff
    storeb r5, [r4]
    store r4, [r1]
    mov r4, 19
    mul r4, r3, r4
    add r4, r2, r4
    storeb r0, [r4]
    jmp jmpGameLoop

moveDown:
    movl r3, 160
    mov r4, 20
    mul r4, r3, r4
    add r4, r2, r4
    loadbu r5, [r4]
    cmp r5, r0
    jne jmpGameLoop
    movl r5, 0xff
    storeb r5, [r4]
    mov r4, r2
    storeb r0, [r4]
    add r4, r4, r3
    store r4, [r1]
    jmp jmpGameLoop

moveBall:
    movl r1, 0x04
    movh r1, 0x10

    load r2, [r1]
    load r3, [r1+4]
    add r5, r2, r3
    loadbu r4, [r5]
    cmp r4, r0
    jne xBlocked
    load r3, [r1+6]
    add r5, r5, r3
    loadbu r4, [r5]
    cmp r4, r0
    jne yBlocked

    load r3, [r1+2]
    cmp r3, r0
    je missed
    movl r4, 159
    cmp r3, r4
    je missed
    load r4, [r1+4]
    add r3, r3, r4
    store r3, [r1+2]

    mov r4, r5
    movl r5, 0xff
    storeb r5, [r4]
    storeb r0, [r2]
    store r4, [r1]
    ret

xBlocked:
    sub r3, r0, r3
    store r3, [r1+4]
    ret

yBlocked:
    sub r3, r0, r3
    store r3, [r1+6]
    ret

missed:
    movl r1, 0xdf
    movh r1, 0xb1
    movl r2, 160
    mov r3, 59
    mul r2, r2, r3
    movl r3, 79
    add r2, r2, r3
    add r3, r1, r2
    movl r1, 0x04
    movh r1, 0x10
    load r2, [r1]
    storeb r0, [r2]
    movl r5, 0xff
    storeb r5, [r3]
    store r3, [r1]
    movl r3, 79
    store r3, [r1+2]
    mov r3, -1
    movl r4, 160
    store r3, [r1+4]
    store r4, [r1+6]
    ret

loop: jmp loop
