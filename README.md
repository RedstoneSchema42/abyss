\# ABYSS



You can only submit a binary.



You can only calculate the rotor manually.



Once you go out of bounds, the virtual machine will crash directly.



\## Submission



Submit the compiled executable, not the source file.



\- Linux/macOS: `abyss` (no suffix)

\- Windows: `abyss.exe`



The input file suffix does not matter. The program only reads the characters `0` and `1` from the file.



\## Encoding Rules



Binary -> convert every 8 bits to an integer `V`.



If `V` is not a valid ASCII character in `"QWERTYUIOPASDFGH"`, your program is rejected.



Otherwise, map the letter to its index `E` (0-15).



Now you get a stream of nibbles (half-byte ciphertext).



\## Rotor Decryption Algorithm



Decrypting each nibble:



```text

op = (E - R) \& 0x0F

R  = (R + op + PC) \& 0x0F

```



\## Memory Layout



`memory\[0-255]` is both the heap and the stack.



The stack grows down from `memory\[255]`.



The heap grows up from `memory\[0]`.



No protection. The stack will overwrite your heap, and the heap will overwrite your stack.



Stack Pointer and Heap Pointer are running towards each other in the dark.



\## Instruction Set (16 Instructions)



```text

0x0: Halt



0x1: Push to stack, followed by 2 nibble parameters

&#x20;    Example: 1 4 1 = push 65



0x2: Duplicate top of stack



0x3: Swap top two elements of stack



0x4: Pop top of stack



0x5: Add

&#x20;    Pop b, pop a, push a+b



0x6: Subtract

&#x20;    Pop b, pop a, push a-b



0x7: Multiply



0x8: Divide



0x9: Jump

&#x20;    Pop address



0xA: Jump if zero

&#x20;    Pop address a, pop condition c

&#x20;    If c == 0, then PC = a



0xB: Store in memory

&#x20;    Pop address, pop value



0xC: Load from memory

&#x20;    Pop address



0xD: System call

&#x20;    Pop selector sel

&#x20;    1 = output char

&#x20;    0 = input char

```



\## The Only Allowed Workflow



Do all the calculations yourself with paper and pen. No compiler.



1\. Write raw instructions.

&#x20;  Example: print `'A'`



&#x20;  ```text

&#x20;  1 4 1    (PUSH 65)

&#x20;  1 0 1    (PUSH 1)

&#x20;  D        (SYS)

&#x20;  0        (HALT)

&#x20;  ```



2\. Calculate Rotor encryption manually.



3\. Find the corresponding letters.



4\. Convert to ASCII binary.



5\. Type the `1`s and `0`s into a file.



6\. Run the provided binary with your `0/1` file as the first argument:



&#x20;  Linux/macOS:



&#x20;  ```bash

&#x20;  ./abyss input.txt

&#x20;  ```



&#x20;  Windows:



&#x20;  ```bash

&#x20;  abyss.exe input.txt

&#x20;  ```



&#x20;  The input file may have any suffix, for example `input.txt`, `input.bin`, or no suffix. The program only reads the characters `0` and `1`; other characters are ignored.

