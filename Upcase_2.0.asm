.text
.globl __start
__start:
    la $a0, prm1
    li $v0, 4
    syscall
    
    la $a0, orig
    li $v0, 4
    syscall
    
    la $s0, orig
    li $t0, 0                  # contador
    li $t2, 1                  # se leyó un espacio
    li $t3, 0                  # indicador de rango minúsculas
    li $t4, 0
    li $t6, 0x20               # ASCII space
    li $t7, 0x61               # ASCII 'a'
    li $t8, 0x7a               # ASCII 'z'
    add $t0, $t0, $s0          # iniciar el contador

loop:
    lb $t1, 0($t0)             # cargar el carácter actual
    beq $t1, $zero, endLoop    # fin si es null
    slt $t3, $t1, $t7          # $t3 = 1 si $t1 < 'a'
    slt $t4, $t8, $t1          # $t4 = 1 si $t1 > 'z'
    or $t3, $t3, $t4           # $t3 = 1 si está fuera del rango de minúsculas
    beq $t2, $zero, nospace    # si no se leyó un espacio
    bne $t3, $zero, nospace    # si no está en el rango de minúsculas, saltar
    
    addi $t1, $t1, -32         # convierte a mayúscula (corrección)
    sb $t1, 0($t0)             # guardar el carácter convertido

nospace:
    bne $t1, $t6, nospacenow   # si no es un espacio, saltar
    li $t2, 1                  # se leyó un espacio
    j endspace

nospacenow:
    li $t2, 0                  # no es un espacio

endspace:
    addi $t0, $t0, 1           # mover al siguiente carácter
    j loop

endLoop:
    la $a0, prm2
    li $v0, 4
    syscall
    
    la $a0, orig
    li $v0, 4
    syscall
    
    li $v0, 10                 # finalizar
    syscall

.data
orig: .asciiz "La cadena original con Letras todas minusculas"
prm1: .asciiz "Original: "
prm2: .asciiz "\nUpcased : "