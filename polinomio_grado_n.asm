.data
prompt_degree: .asciiz "Ingrese el grado n del polinomio: "
prompt_coeff:  .asciiz "Ingrese el coeficiente a"
colon_space:   .asciiz ": "
prompt_x:      .asciiz "Ingrese el valor de x: "
result_msg:    .asciiz "El valor del polinomio es: "
zero_float:    .float 0.0           # Define a floating-point zero

.text
.globl main
main:
    # Solicitar el grado n del polinomio
    la $a0, prompt_degree
    li $v0, 4          # Syscall para imprimir cadena
    syscall

    # Leer entero n
    li $v0, 5          # Syscall para leer entero
    syscall
    move $s0, $v0      # $s0 = n

    # Verificar que el grado sea no negativo
    blt $s0, $zero, exit_program  # Si n < 0, salir

    # Calcular n + 1 (número de coeficientes)
    addi $t0, $s0, 1   # $t0 = n + 1

    # Calcular el número de bytes necesarios para los coeficientes
    sll $t1, $t0, 2    # $t1 = (n + 1) * 4 bytes

    # Reservar memoria para los coeficientes
    li $v0, 9          # Syscall para sbrk
    move $a0, $t1      # Número de bytes a reservar
    syscall
    move $s1, $v0      # $s1 apunta al inicio del array de coeficientes

    # Leer los coeficientes del polinomio
    li $t2, 0          # $t2 = i (índice)

coefficients_loop:
    bge $t2, $t0, coefficients_done  # Si i >= n + 1, fin del bucle

    # Mostrar mensaje "Ingrese el coeficiente a"
    la $a0, prompt_coeff
    li $v0, 4
    syscall

    # Calcular y mostrar el índice del coeficiente (n - i)
    move $t4, $s0       # $t4 = n
    sub $t4, $t4, $t2   # $t4 = n - i
    li $v0, 1           # Syscall para imprimir entero
    move $a0, $t4
    syscall

    # Mostrar ": "
    la $a0, colon_space
    li $v0, 4
    syscall

    # Leer el coeficiente
    li $v0, 5           # Syscall para leer entero
    syscall
    move $t5, $v0       # $t5 = coeficiente a_i

    # Almacenar el coeficiente en memoria
    sll $t6, $t2, 2     # $t6 = i * 4 (desplazamiento)
    add $t7, $s1, $t6   # $t7 = dirección del coeficiente
    sw $t5, 0($t7)      # Guardar coeficiente en memoria

    # Incrementar i
    addi $t2, $t2, 1
    j coefficients_loop

coefficients_done:
    # Solicitar el valor de x
    la $a0, prompt_x
    li $v0, 4
    syscall

    # Leer x en punto flotante
    li $v0, 6           # Syscall para leer flotante
    syscall
    mov.s $f0, $f0      # $f0 = x

    # Inicializar P = 0.0 en punto flotante
    # Cargar 0.0 desde la sección de datos
    l.s $f12, zero_float

    # Inicializar i = 0 para la evaluación del polinomio
    li $t3, 0           # $t3 = i

evaluation_loop:
    bge $t3, $t0, evaluation_done  # Si i >= n + 1, fin del bucle

    # Cargar a_i desde memoria
    sll $t6, $t3, 2     # $t6 = i * 4
    add $t7, $s1, $t6   # $t7 = dirección de a_i
    lw $t5, 0($t7)      # $t5 = a_i

    # Convertir a_i a punto flotante
    mtc1 $t5, $f2
    cvt.s.w $f2, $f2    # $f2 = float(a_i)

    # Calcular P = P * x + a_i
    mul.s $f12, $f12, $f0   # $f12 = P * x
    add.s $f12, $f12, $f2   # $f12 = P + a_i

    # Incrementar i
    addi $t3, $t3, 1
    j evaluation_loop

evaluation_done:
    # Mostrar mensaje "El valor del polinomio es: "
    la $a0, result_msg
    li $v0, 4
    syscall

    # Imprimir el resultado en punto flotante
    mov.s $f0, $f12     # Mover el resultado a $f0 para imprimir
    li $v0, 2           # Syscall para imprimir flotante
    syscall

    # Finalizar el programa
exit_program:
    li $v0, 10          # Syscall para salir
    syscall
