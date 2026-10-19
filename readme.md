# 16-bit CPU emulator and assembler
A custom 16-bit risc style ISA

- The emulator requires SDL3

- The CPU can access 64KB of memory, little endian, and each instruction is 16-bits wide

![](example.gif)

## Frame Buffer and keyboard
### frame buffer
- address - 0xb1df
- size - 160x120
- 8-bit RGB (3-bit, 3-bit, 2-bit)

### keyboard
- keyboard - 0xfce0

## opcodes
|opcodes|
|-------|
|add    |
|sub    |
|and    |
|or     |
|xor    |
|not    |
|shl    |
|shr    |
|ashr   |
|ror    |
|mul    |
|dvrm   |
|mov    |
|movh   |
|movl   |
|load   |
|loadb  |
|loadbu |
|store  |
|storeb |
|jmp    |
|jc     |
|call   |
|ret    |

## opcode format
<table>
    <tr>
        <th>15</th>
        <th>14</th>
        <th>13</th>
        <th>12</th>
        <th>11</th>
        <th>10</th>
        <th>9</th>
        <th>8</th>
        <th>7</th>
        <th>6</th>
        <th>5</th>
        <th>4</th>
        <th>3</th>
        <th>2</th>
        <th>1</th>
        <th>0</th>
        <th>instruction</th>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=3>source register</td>
        <td></td>
        <td>R/I</td>
        <td rowspan=2>add, sub, and, or, xor</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=4>immediate</td>
        <td>R/I</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=3>source register</td>
        <td>A/S</td>
        <td>R/I</td>
        <td rowspan=2>load, loadb, loadbu, store, storeb</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=4>immediate</td>
        <td>R/I</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=3>source register</td>
        <td>S/U</td>
        <td>H/L</td>
        <td>mul</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=3>source register</td>
        <td>S/U</td>
        <td>D/R</td>
        <td>dvrm</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=3>source register</td>
        <td colspan=4></td>
        <td>R/I</td>
        <td rowspan=2>mov</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=7>immediate</td>
        <td>R/I</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=8>immediate</td>
        <td>movh, movl</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>destination register</td>
        <td colspan=7></td>
        <td>R/I</td>
        <td rowspan=2>jmp, call</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=10>immediate</td>
        <td>R/I</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=7>immediate</td>
        <td colspan=4>condition</td>
        <td>jc</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>source/destination register</td>
        <td colspan=8></td>
        <td>not</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=3>source/destination register</td>
        <td colspan=4>shamt</td>
        <td colspan=4></td>
        <td>shl, shr, ashr, ror</td>
    </tr>
    <tr>
        <td colspan=5>opcode</td>
        <td colspan=11></td>
        <td>ret</td>
    </tr>
</table>

## registers
|register |
|---------|
|r0 (zero)|
|r1       |
|r2       |
|r3       |
|r4       |
|r5       |
|r6 (sp)  |
|r7 (addr)|
|PC       |
|flags    |

<table>
    <tr>
        <th colspan=4>flags</th>
    </tr>
    <tr>
        <td>Carry flag</td>
        <td>Sign flag</td>
        <td>Overflow flag</td>
        <td>Zero flag</td>
    </tr>
</table>

## Assembler instructions
|instructions|
|------------|
|add         |
|sub         |
|and         |
|or          |
|xor         |
|not         |
|shl         |
|shr         |
|ashr        |
|ror         |
|mul         |
|mulh        |
|umulh       |
|div         |
|udiv        |
|rem         |
|urem        |
|mov         |
|movh        |
|movl        |
|load        |
|loadb       |
|loadbu      |
|store       |
|storeb      |
|jmp         |
|je          |
|jne         |
|jg          |
|ujg         |
|jge         |
|ujge        |
|jl          |
|ujl         |
|jle         |
|ujle        |
|call        |
|ret         |
|push        |
|pop         |
|cmp         |

## Syntax
add, sub, and, or, xor:
```
    instruction reg, reg, reg/imm
    immediate range: -8 - 7
```

load, loadb, loadbu, store, storeb:
```
    instruction reg, [reg +/- reg/imm]
    immediate range: -8 - 7
```

mul, mulh, umulh, div, udiv, rem, urem:
```
    instruction reg, reg, reg
```

mov:
```
    instruction reg, reg/imm
    immediate range: -64 - 63
```

movh, movl:
```
    instruction reg, imm
    immediate range: 0 - 255
```

jmp, call:
```
    instruction reg/label
    label range: -512 - 511
```

jc/ujc:
```
    instruction label
    label range: -64 - 63
```

not, push, pop:
```
    instruction reg
```

shl, shr, ashr, ror:
```
    instruction reg, imm
    immediate range: 0 - 15
```

cmp:
```
    instruction reg, reg/imm
    immediate range: -8 - 7
```

ret:
```
    instruction
```

## How to run

- compile emulator and assembler
```
make
```

- how to run:
```
bash run.sh <filename>.asm
```

## Notes
- call and ret are implemented using link/address register, so nested calls require push/pop of r7
- push, pop and cmp are pseudo-instructions
- movh should be preceded with movl or the high bits will be corrupted
- div and rem by zero gives zero
- r6 is set to 0 initially. r6 should be set before using
- comments must be preceded with ';'
