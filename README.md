Author: Shreyas Kumar Jaiswal
Roll: 2301CS52

Declaration of Authorship: This project is part of CS2102 DLCO MiniProject.
All the files submitted are designed and written by me.
The asm.cpp and emu.cpp files have my name and roll mentioned in them.

----------------------------------------------------------------------------

Both the files compile without any errors or warnings when compiled with "g++ <file>.cpp -Wall -W -pedantic"

Assembler:
1> Compile the asm.cpp file as: "g++ asm.cpp -o asm"
   There are two ways of passing the file name:
   a> Directly with the asm.exe file as: ".\asm.exe <filename>.asm"
   b> After running the asm.exe file, it will ask for filename as shown:
      "Enter file name to assemble:     "
      Enter the filename now.

2> It reads the .asm file and filter out the codes i.e., removes out any extra spaces and removes comments from the code.

3> In the first pass, it generates the operanddata vector pair to separate the operator and operand part from the code.
   It also creates an operationType array which stores the type of instruction present.
   It also store the label address in a map while traversing and in the end, check if all the labels are present or not.
   It also stores any possible errors which might be present in the code.

4> If any errors are present, it writes them in .log file and also prints on the console. And second pass in not executed.

5> When no error is present in the code, second pass is run.
   All the labels are replaced with their addresses. And converted accordingly converted to machine code.
  
6> Implementation of SET: 
    All the labels which are associated with SET type, wherever that label is present, it is replaced by the corresponding value.
    eg: The .asm code is as follows:
    -> ; test3.asm
    -> ; Test SET
    -> val: SET 75
    -> ldc     val
    -> adc     val2
    -> val2: SET 66

    And the machine code generated is: 
    -> 00000000 00004b00 ldc val                    ; val is replaced with 4b (75)
    -> 00000001 00004201 adc val2                   ; val2 is replaced with 42 (64)

    Here 75 is 4b in Hex, so ldl val gets converted to 00004b00, and similarly ldl val2 gets converted to 00004201

Emulator:
1> Loads machine code from the file, process it to find operation and operand, perform accordingly.

2> Detects infinte loop, segmentation faults, etc.

3> Trace of memory before and after execution can be done by using commands -before and -after.

4> Memory content can be obtained at any instant by the commmand -dump and registers content by -reg.

Testing of files: 
a> test1.asm
  Log data: 
    -> Log code generated in: .\test1.log
    -> Warning! HALT is missing in the code.
    -> Machine code generated in: .\test1.o
    -> Listing code generated in: .\test1.l

b> test2.asm
  Log data:
    -> Log code generated in: test2.log
    -> Warning! HALT is missing in the code.
    -> Error at line 2 -- "label:" -- Multiple Declaration of the same label "label".
    -> Error at line 3 -- "br nonesuch" -- No such label: "nonesuch"
    -> Error at line 4 -- "ldc 08ge" -- No such label: "08ge"
    -> Error at line 5 -- "ldc" -- Missing Operands
    -> Error at line 6 -- "add 5" -- No operand should be there.
    -> Error at line 7 -- "ldc 5, 6" -- Multiple Operands present.
    -> Error at line 8 -- "0def:" -- Invalid Label Name: "0def".
    -> Error at line 9 -- "fibble" -- Invalid Mnemonic.
    -> Error at line 10 -- "0def" -- Invalid Mnemonic.

c> test3.asm
  Log data:
    -> Log code generated in: .\test3.log
    -> Warning! HALT is missing in the code.
    -> Machine code generated in: .\test3.o
    -> Listing code generated in: .\test3.l


d> test4.asm
  Log data:
    -> Log code generated in: .\test4.log
    -> Machine code generated in: .\test4.o
    -> Listing code generated in: .\test4.l

e> infiniteloop.asm
  Emulator output:
    -> Enter command to execute: -run
    -> Infinite Loop detected in Program. Please fix.
    -> Please fix the code.

f> segmentationfault.asm
  Emulator output: 
    -> Enter command to execute: -run
    -> !! Segmentation fault. Please fix the code.

Memory format:
    +----------+-----------+-----------+-----------+-----------+  
    |  Address | Value(+0) | Value(+4) | Value(+8) | Value(+c) |  
    +----------+-----------+-----------+-----------+-----------+  
    |          |           |           |           |           |
    +----------+-----------+-----------+-----------+-----------+
g> test01_sum.asm  ; This program stores the sum of elements of an array and store in sum variable.

    Memory content before run:
      Address:                         Array starts Here
        ↓                                   ↓
    | 00000014 |  00000000 |  00000005 |  00000001 |  00000002 |  
    | 00000018 |  00000003 |  00000005 |  00000006 |  00000000 |
                                            ↑            ↑
                                Array ends here        sum(initially 0)

    Memory content after execution: 
      Address:                         Array starts Here
        ↓                                   ↓
    | 00000014 |  00000005 |  00000005 |  00000001 |  00000002 |  
    | 00000018 |  00000003 |  00000005 |  00000006 |  00000011 |  
                                            ↑            ↑
                                Array ends here      sum(final 17)

h> test02_min.asm    ; This program finds the minimum value in an array
    Memory content before run:
      Address:                         Array starts Here
        ↓                                   ↓
    | 00000018 |  00000000 |  00000005 |  00000064 |  00000049 |
    | 0000001c |  00000004 |  0000000f |  00000034 |  007fffff | 
                                            ↑            ↑
                                Array ends here      min(initially very large value)
    
    Memory content after run:
      Address:                         Array starts Here
        ↓                                   ↓
    | 00000018 |  00000005 |  00000005 |  00000064 |  00000049 |  
    | 0000001c |  00000004 |  0000000f |  00000034 |  00000004 |
                                            ↑            ↑
                                Array ends here      min(now set to 4)

i> test03_max.asm    ; This program finds the maximum value in an array
    Memory content before run:
      Address:               Array starts Here
        ↓                         ↓
    | 00000018 |  00000005 |  00000064 |  00000049 |  00000004 |  
    | 0000001c |  0000000f |  00000034 |  ff676980 |           | 
                                  ↑            ↑                                      
                      Array ends here      max(initially very small value)
    
    Memory content after run:
      Address:               Array starts Here
        ↓                         ↓
    | 00000018 |  00000005 |  00000064 |  00000049 |  00000004 |  
    | 0000001c |  0000000f |  00000034 |  00000064 |           |
                                 ↑            ↑
                     Array ends here      min(now set to 100)

j> test04_mean.asm
    Memory content before run:
      Address:               Array starts Here
        ↓                         ↓
    | 0000001c |  00000003 |  00000002 |  00000003 |  00000004 |  
    | 00000020 |  00000005 |  00000007 |  00000008 |  00000009 |  
    | 00000024 |  0000000a |  00000000 |           |           |
                     ↑            ↑                                    
         Array ends here      mean(initially 0)
    
    Memory content after run:
      Address:               Array starts Here
        ↓                         ↓
    | 0000001c |  00000003 |  00000002 |  00000003 |  00000004 |
    | 00000020 |  00000005 |  00000007 |  00000008 |  00000009 |  
    | 00000024 |  0000000a |  00000006 |           |           | 
                    ↑            ↑
        Array ends here      min(now set to 6)

k> bubblesort.asm
    Memory content before run:
      Address:    Array starts Here
        ↓            ↓
    | 00000034 |  00000003 |  00000002 |  0000000e |  00000036 |  
    | 00000038 |  0000004c |  fffffe0c |  00000054 |  00000027 |  
    | 0000003c |  00000001 |           |           |           | 
                     ↑                                                
             Array ends here     
    
    Memory content after run:
      Address:  Array starts Here
        ↓            ↓
    | 00000034 |  fffffe0c |  00000001 |  00000002 |  00000003 |  
    | 00000038 |  0000000e |  00000027 |  00000036 |  0000004c |  
    | 0000003c |  00000054 |           |           |           |
                     ↑          
         Array ends here      

    ; We can observe that the array has been sorted in ascending order successfully

l> test05_median.asm
    Memory content before run:
      Address:                                        Array starts Here
        ↓                                                ↓
    | 0000003c |  00000000 |  00000009 |  00000000 |  00000003 |  
    | 00000040 |  00000002 |  0000000e |  00000036 |  0000004c |  
    | 00000044 |  fffffe0c |  00000054 |  00000027 |  00000001 |  
    | 00000048 |  00000000 |           |           |           |
                     ↑                                   ↑                                               
                Median variable(initially 0)           Array ends here     
    
    Memory content after run:
      Address:                                       Array starts Here
        ↓                                                 ↓
    | 0000003c |  00000001 |  00000001 |  00000002 |  fffffe0c |  
    | 00000040 |  00000001 |  00000002 |  00000003 |  0000000e |  
    | 00000044 |  00000027 |  00000036 |  0000004c |  00000054 |  
    | 00000048 |  0000000e |           |           |           |
                     ↑                                   ↑                                               
                Median variable(now set to 14)           Array ends here     
      (which is in 5th position in sorted array of 9 elements)
