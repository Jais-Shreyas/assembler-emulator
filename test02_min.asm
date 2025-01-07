; Shreyas Jaiswal
; program to find the min of n elements

redo:
ldc cnt
ldnl 0
ldnl values       ; current element in B
ldc min
ldnl 0            ; current min in A
sub               ; A < 0 if current < min
brlz update ;     if A < 0 update min
br noupdate       ;
update: 
stl 0x10000       ; current in A
ldc min           ; B = current, A = address of min
stnl 0            ; min = current
noupdate:
ldc cnt           
ldnl 0
adc 1             
ldc 0
stnl cnt          ; update cnt i.e., i += 1
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

min: data 0x7fffff