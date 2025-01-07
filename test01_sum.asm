; Shreyas Jaiswal
; program to find the sum of n elements

redo:
ldc sum
ldnl 0            ; load current sum in A
ldc cnt           
ldnl 0            ; B = i
ldnl values       ; load A = array[i] and move current sum to B
add               ; A = currentsum + array[i]
stl sum           ; store A in sum
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
values: data 1
data 2
data 3
data 5
data 6
sum: data 0