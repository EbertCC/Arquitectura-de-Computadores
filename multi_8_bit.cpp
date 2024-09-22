#include <iostream>
#include <vector>

using namespace std;

// Función para sumar dos números binarios
void sum_binario(vector<int> &A, vector<int> &M) {
    int cont = 0;
    for (int i = A.size() - 1; i >= 0; --i) {
        int sum = A[i] + M[i] + cont;
        A[i] = sum % 2;
        cont = sum / 2;
    }
}

// Función para obtener el complemento a 2 de un número binario
void complemento_a_2(vector<int> &M) {
    // invertir los bits (NOT)
    for (int i = 0; i < M.size(); ++i) {
        M[i] = 1 - M[i];
    }
    //suma binaria 
    vector<int> one(M.size(), 0);
    one[one.size() - 1] = 1;
    sum_binario(M, one);
}

// desplazar a la derecha 
void arithmetic_shift_right(vector<int> &A, vector<int> &Q, int &Q_1) {
    Q_1 = Q[Q.size() - 1]; // guardar el ultimo elemento de Q
    for (int i = Q.size() - 1; i > 0; --i) {
        Q[i] = Q[i - 1]; // mover Q
    }
    Q[0] = A[A.size() - 1]; // mover el bit mas significativo de A a Q
    for (int i = A.size() - 1; i > 0; --i) {
        A[i] = A[i - 1]; // mover A
    }
}

// Algoritmo de Booth para la multiplicación
void algoritmo_booth(vector<int> A, vector<int> Q, vector<int> M) {
    int Q_1 = 0;  // bit Q_1 inicializado en 0
    int n = Q.size(); // tamanio de bits del multiplicador

    // complemento a 2 de M para la resta
    vector<int> M_neg = M;
    complemento_a_2(M_neg);

    // Mostrar el estado inicial
    cout << "Valores iniciales: ";
    for (int bit : A) cout << bit;
    cout << ", Q: ";
    for (int bit : Q) cout << bit;
    cout << ", Q-1: " << Q_1 << endl;

    // for para el tamanio de bits del multiplicador
    for (int i = 0; i < n; ++i) {
        // Verificar los dos últimos bits (Q_0 y Q_1)
        if (Q[Q.size() - 1] == 1 && Q_1 == 0) {
            sum_binario(A, M_neg);
        } else if (Q[Q.size() - 1] == 0 && Q_1 == 1) {
            sum_binario(A, M);
        }

        // desplazamiento a la derecha
        arithmetic_shift_right(A, Q, Q_1);

        // mostrar el ciclo actual
        cout << "Ciclo " << i + 1 << " - A: ";
        for (int bit : A) cout << bit;
        cout << ", Q: ";
        for (int bit : Q) cout << bit;
        cout << ", Q-1: " << Q_1 << endl;
    }

    // Mostrar el resultado final
    cout << "Resultado final : ";
    for (int bit : A) cout << bit;
    for (int bit : Q) cout << bit;
    cout << endl;
}

int main() {
    //Ejemplo  1
    // Representamos los números en 4 bits
    cout<<"Ejmplo 1 : "<<endl;
    vector<int> A(4, 0);  // Acumulador A inicializado a 0
    vector<int> Q = {0, 0, 1, 1};  // Multiplicador Q ( 3 )
    vector<int> M = {0, 1, 1, 1};  // Multiplicando M ( 7 )
    // Ejecutar el algoritmo de Booth
    algoritmo_booth(A, Q, M);

    cout<<endl<<endl;

    //Ejemplo 2 
    cout<<"Ejemplo 2 : "<<endl;
    vector<int> Z(4, 0);  // Acumulador A inicializado a 0
    vector<int> X = {1, 1, 0, 1};  // Multiplicador Q ( -3 )
    vector<int> Y = {0, 1, 0, 1};  // Multiplicando M ( 7 )
    // Ejecutar el algoritmo de Booth
    algoritmo_booth(Z, X, Y);

    return 0;
}
