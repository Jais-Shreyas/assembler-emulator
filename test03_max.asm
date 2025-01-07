; Shreyas Jaiswal
; program to find the max of n elements

redo:
ldc cnt
ldnl 0
ldnl values       ; current element in B
ldc max
ldnl 0            ; current max in A
sub               ; A < 0 if current < max
brlz noupdate     ; if A <= 0 no need to update max
stl 0x10000       ; current in A
ldc max           ; B = current, A = address of max
stnl 0            ; max = current
noupdate:
ldc cnt           ; load cnt
ldnl 0
adc 1             ; add 1 to cnt
ldc 0
stnl cnt          ; update cnt += 1
ldc n
ldnl 0            ; take cnt in A
ldc cnt           
ldnl 0            ; take n in A, cnt in B
sub               ; A = cnt - n
brz done          ; if cnt == n break
br redo
done:
HALT

cnt: data 0
n: data 5
values: data 100
data 73
data 4
data 15
data 52

max: data -10000000  ; some very small value