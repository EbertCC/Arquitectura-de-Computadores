#include <iostream>
#include <cmath> 
#include <bitset>

using namespace std;
// Función para separar el signo, exponente y mantisa de un número flotante
void separarPartes(float num, int &signo, int &exponente, int &mantisa) {

    // convertir a bits el numero
    int bits = *(int*)&num;

    // asigna el signo dependiendo del bot 31
    signo = (bits >> 31) & 1; 

    // los bits 23-30 exponente 
    exponente = (bits >> 23) & 0xFF; 

    // los bits 0-22 es la mantisa
    mantisa = bits & 0x7FFFFF;
    mantisa |= (1 << 23); //añadir el 1 implicitamente
}

// Funcion multiplicar numeros flotantes
float multiplicarFlotantes(float num1, float num2) {
    int signo1, exponente1, mantisa1;   
    int signo2, exponente2, mantisa2;

    // separar las partes de los 2 numeros 
    separarPartes(num1, signo1, exponente1, mantisa1);
    separarPartes(num2, signo2, exponente2, mantisa2);

    // 1. Las consultas por si alguno de los valores ingresados es 0.
    if (num1 == 0.0f || num2 == 0.0f) {
        return 0.0f;
    }

    // Calcular el signo del resultado
    int signoResultado = signo1 ^ signo2;

    // 2.La suma binaria de los exponentes, procesamiento de suma en biased representation y sus verificaciones de overflow/underflow.
    int exponenteResultado = (exponente1 + exponente2) - 127;

    //3. Multiplicacion de los significandos (sin signo).
    long long mantisaResultado = (long long)mantisa1 * mantisa2;

    //4. Normalizaci´on, redondeo.
    if (mantisaResultado & (1LL << 47)) {
        mantisaResultado >>= 24;
        exponenteResultado++;
    } else {
        mantisaResultado >>= 23;
    }

    // 2.La suma binaria de los exponentes, procesamiento de suma en biased representation y sus verificaciones de overflow/underflow.
    if (exponenteResultado >= 255) {
        return signoResultado ? -INFINITY : INFINITY;
    } else if (exponenteResultado <= 0) {
        return 0.0f; // Underflow
    }

    //5. Ensamblaje del resultado final en un tercer registro de 32 bits punto flotante
    int resultado = (signoResultado << 31) | ((exponenteResultado & 0xFF) << 23) | (mantisaResultado & 0x7FFFFF);
    return *(float*)&resultado;
}

int main() {
    float num1 = 3.2f;
    float num2 = 1.71f;

    float resultado = multiplicarFlotantes(num1, num2);
    float resultadoC = num1 * num2;

    //Verificar si existen discrepancias con la operación de multiplicación de punto flotante propia del lenguaje:
    cout<< "Resultado con el algoritmo personalizado: " << resultado <<endl;
    cout<< "Resultado con la multiplicacion estandar de C++: " << resultadoC <<endl;

    //Visualizar los patrones de bits de una variable punto flotante:
    int bits = *(int*)&resultado;
    cout << "Bits del resultado: " << bitset<32>(bits) << endl;

    return 0;
}
