
MAIN:	mov r3, LIST[sz]
MAIN: .string"abcdef"

abcdefghijklmnopqrstuvwxyza1234: .data -2048, -2048, -2048, -2048, -2048, -2048, 1999, 2023

LIST: .data 6. -9, len
LIST1: d .data 6, -9,. len
LIST2: .data 6, --9, len
LIST3: .data 6, ++9, len
LIST4: .data 6, MAIN, len
LIST5: .data 6, -9 len
LIST6: .data 2048, -9, len
LIST7: .data -2049, -9, len

STR: .string h "abcdef"
STR1: .string ,"abcdef"

.define len == 4

.define h len = 4
.define len5 = h 4
.define len6 = 4 h
.define len1 = (4)
.define len2 = 4+4
.define len3 = 4+4+4
.define len4 = 4*4

ARR: .data 1, 2, 3

mov , r3, ARR[2]
mov, r3, ARR[2]
MAIN:	, mov r3, ARR[2]
, mov r3, ARR[2]
mov r3, ARR[2],
mov , r3,, ARR[2]
mov r3 ARR[2]
mov r3,r2, ARR[2]
mov r3, ARR[2]r2
mov r3, ARR[2] r2
mov r3,
mov r3
mov
mov ,

: hlt
he llo: hlt

not r3, ARR[2]
not r3,
not
not , r3
lea r3, STR
lea r3, STR[2], r2

hlt ,
hlt r2
hlt r2, r3
hlt r2, r3, r4