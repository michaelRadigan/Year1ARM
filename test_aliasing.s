hello .req r1
mov hello,#0xC
mov r2,#10
ldr r3,=0xE
ldr r4,[r3]
ldr r5,[hello,r2,lsr #2]
