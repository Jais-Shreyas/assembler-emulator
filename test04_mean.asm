; Shreyas Jaiswal
; program to find the mean of n (n is power of 2) elements

redo:
ldc mean
ldnl 0            ; load current mean in A
ldc cnt           
ldnl 0            ; B = i
ldnl values       ; load A = array[i] and move current mean to B
add               ; A = currentsum + array[i]
stl mean          ; store A in mean
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
ldc mean          ; currently mean holds sum of elements
ldnl 0
ldc pow           ; load the number of left shifts
ldnl 0
shr               ; now mean holds the actual mean
stl mean          ; update mean
HALT


cnt: data 0
n: data 8         
pow: data 3       ; number of left shifts = log2(n)
values: data 2
data 3
data 4
data 5
data 7
data 8
data 9
data 10
mean: data 0