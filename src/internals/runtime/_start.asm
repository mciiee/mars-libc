.globl _start
.include "include.asm"
.text
_start:
  addi $sp, $sp, -16
  sw $a0, 0($sp)
  sw $a1, 4($sp)

  jal __libc_init

  lw $a0, 0($sp)
  lw $a1, 4($sp)
  # Not really necessary, but symmetry is cool
  addi $sp, $sp, 16

  jal main

  addi $a0, $v0, 0
  li $v0, 17
  syscall
