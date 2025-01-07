; program to sort elements of an array

outerloop:
  
  ; initialise j to 0
  ldc 0              ; A = 0
  ldc 0              ; B = 0, A = 0
  stnl j             ; j = 0

  innerloop:          
    ldc j
    ldnl 0           ; load address of j in A
    ldnl array       ; A = array[j]
    ldc j            ; B = array[j], A = address of j
    ldnl 0           ; A = j
    adc 1            ; A = j + 1;
    ldnl array       ; A = array[j + 1]
    sub              ; A = array[j] - array[j+1] i.e.,  A = B-A
    ; if A < 0 then we don't swap
    brlz noswap
      ; B still holds array[j], we put that in temp, but A is not 0
      stl 0x10000
      ldc 0
      stnl temp        ; temp = array[j]
      ; now load arr[j+1] in A
      ldc j            ; A = address of j
      ldnl 0           ; A = j
      adc 1            ; A = j + 1;
      ldnl array      ; A = array[j + 1]
      ; update array[j] = arr[j+1]
      ldc j            ; now B = arr[j+1], A holds address of j
      ldnl 0           ; A holds j
      stnl array      ; array[j] = array[j+1]
      ; now do array[j+1] = temp
      ldc temp         ; A holds address of temp
      ldnl 0           ; A holds temp value which is array[j]
      ldc j            ; temp now in B, A = address of j
      ldnl 0           ; A = j
      adc 1            ; A = j + 1
      stnl array      ; array[j+1] = temp
    noswap:
    ldc j
    ldnl 0           ; A = j
    adc 1            ; A = j+1
    ; move it to B and load address of j in A
    ldc j            ; B = j+1, A = address of j
    stnl 0           ; j = j+1
    ; now, if j == n - 1, exit inner loop
    ldnl 0           ; A = j
    ldc n
    ldnl 0           ; B = j, A = n
    adc -1           ; A = n-1
    sub              ; A = j - (n-1), if A < 0, means j is small, so branch to inner loop
    brlz innerloop
    ; now update n   
    ldc n
    ldnl 0
    adc -1
    ldc n            ; A = address of n, B = n-1
    stnl 0           ; n = n-1
    ; now if n == 1, we skip outerloop too
    ; B still has n-1, check if B == 1
    ; bring n-1 in A
    stl 0x10000
    adc -1          
    ; check if A == 0
    brz exitouterloop
    br outerloop    ; if j == n-1, innerloop is over, go to outerloop after updating n
exitouterloop:

HALT

j: data 0           ; index variable for interation
n: data 9           ; count of elements in the array
temp: data 0        ; temperory variable to do swaps
array: data 3       ; all the elements listed sequentialy
data 2
data 14
data 54
data 76
data -500
data 84
data 39
data 1