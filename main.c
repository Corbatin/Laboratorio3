// Nombres: Alvaro Soto Albornoz - Benjamín Melis Guerra
// Nombre Profesora: Nicolas Thériault
// IDE: Visual Studio Code 1.62.3
// SO: Windows 10
// Fecha: [REDACTED] de [REDACTED] - 2022
// Este programa [REDACTED]

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define P (long long)3781996138433268199
#define Pa (long long)1761129190
#define Pb (long long)892783079
#define bAJO (long long)2147483647

long long **asignar_matriz(int n, int m);                        // Reservar la memoria y crear una matriz nula
void llenar_matriz(long long **matriz, int filas, int columnas); // Llenar la matriz nula con valores al azar utilizando la funcion rand()
void llenar_submatriz(long long **matriz, long long **matriz_original, int iFilas, int fFilas, int iCol, int fCol);
void imprimir_matriz(long long **array, int filas, int columnas); // Funcion que imprime la matriz
void menu();
long long **sumaMatrices(long long **matriz1, long long **matriz2, int filas, int columnas);               // Suma de Matrices
long long **restaMatrices(long long **matriz1, long long **matriz2, int filas, int columnas);              // Resta de matrices
long long **multiplicacion_clasica(long long **matriz_1, long long **matriz_2, int dim);                   // Multiplicacion de matrices de forma clasicalong long **multiplicacion_clasica(long long **matriz_1, long long **matriz_2, int filas_1, int filas_2, int col_1, int col_2)
long long **Mult_Strassen(long long **matriz_1, long long **matriz_2, int dim);                            // Multiplicacion de matrices con el algoritmo de Strassen
long long **recombinarMatriz(long long **C11, long long **C12, long long **C21, long long **C22, int dim); // Funcion que recombina las 4 sub matrices en la matriz mas grande
long long **matriz_par(long long **matriz, int *dim);                                                      // Funcion que aumenta a un numero par las dimensiones de una matriz de orden impar
long long **matriz_inversa(long long **matriz, int dim);
long long **matriz_negativa(long long **matriz, int dim);
long long **matriz_cuadrada(long long **matriz, int *dim);

void resta_lineas(long long **matriz, int dim, int minu, int sustra, long long valor);
void cambiar_linea(long long **matriz, int dim, int linea, long long valor);
void Matriz_inversaG(long long **matriz, long long **identidad, int filas, int columnas);
void eye(long long **matriz, int fila, int columnas);

// TRUCOS PROFE
long long MultP(long long a, long long b);
long long RestaP(long long a, long long b);
long long SumaP(long long a, long long b);
long long InvP(long long A);

int main()
{
    // srand(0);
    long long **matriz, matriz_invG;

    menu();
    return 0;
}
void menu()
{
    int op, dim, el;
    long long **matriz;
    clock_t tiempo1, tiempo2;

    printf("\nEscoga la dimension de la matriz: ");
    scanf("%d", &dim);

    matriz = asignar_matriz(dim, dim);
    llenar_matriz(matriz, dim, dim);
    // imprimir_matriz(matriz, dim, dim);
    do
    {
        printf("\nQue desea ver?");
        printf("\n 1) Generar matriz nueva. ");
        printf("\n 2) Tiempo de Matriz Invertida. ");
        printf("\n 0) Salir. ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            // No se como hacer este funcionar
            for (int i = 0; i < dim; i++)
            {
                free(matriz[i]);
            }
            free(matriz);
            printf("\nEscoga la dimension de la nueva Matriz\n");

            scanf("%d", &dim);
            matriz = asignar_matriz(dim, dim);
            llenar_matriz(matriz, dim, dim);
            printf("\nDesea ver las matrices?\n");
            printf("\n1) Si\n");
            printf("\n2) No\n");
            scanf("%i", &el);
            if (el == 1)
            {
                imprimir_matriz(matriz, dim, dim);
            }
            else if (el == 2)
            {
                break;
            }
            printf("\nMatriz creada exitosamente! ");
            break;
        case 2:

            tiempo1 = clock();
            matriz = matriz_inversa(matriz, dim);
            tiempo2 = clock();
            printf("\nTiempo de ejecucion: %f\n", ((double)tiempo2 - (double)tiempo1) / ((double)CLOCKS_PER_SEC));
            printf("\nDesea ver la matriz?\n");
            printf("\n1) Si\n");
            printf("\n2) No\n");
            scanf("%i", &el);
            if (el == 1)
            {
                imprimir_matriz(matriz, dim, dim);
            }
            else if (el == 2)
            {
                break;
            }
            else
                break;
            break;
        case 0:
            printf("\n Saliendo . . .");
            break;
        default:
            printf("\nSeleccione una opcion Valida. ");
            break;
        }
    } while (op);
    for (int i = 0; i < dim; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

long long **asignar_matriz(int n, int m)
{
    int i, j;
    long long **array;
    array = (long long **)calloc(n, sizeof(long long *)); // se reserva memoria  para la matriz de x filas que contiene direcciones de memoria a las segundas dimensiones.
    if (array == NULL)
    {
        printf("\nMemoria alocada incorrectamente....\n");
        exit(0);
    }
    for (i = 0; i < n; i++)
    {
        array[i] = (long long *)calloc(m, sizeof(long long)); // se reserva memoria para las segundas dimensiones , x columnas
    }
    if (array == NULL)
    {
        printf("\nMemoria alocada incorrectamente....\n");
        exit(0);
    }
    // en memoria ya tenemos reservado espacio para una matriz de x por x --> array[x][x]
    return &*array; // retorno de un puntero doble
}

long long **matriz_inversa(long long **matriz, int dim)
{
    long long **M11, **M12, **M21, **M22, **M11_inv, **Mcombinado_inv, **matriz_resultado;
    int largo, ajuste;
    if (matriz == NULL)
    {
        printf("Memoria alocada como las weas.");
        exit(0);
    }
    if (dim <= 1)
    {
        matriz_resultado = asignar_matriz(1, 1);
        if (matriz[0][0] == 0)
        {
            printf("Esta matriz no se puede invertir");
            matriz_resultado[0][0] = 0;
            return matriz_resultado;
        }

        **matriz_resultado = InvP(matriz[0][0]);
        return matriz_resultado;
    }
    // printf(" a ver %i", &dim);
    matriz = matriz_cuadrada(matriz, &dim);
    ajuste = dim / 2;

    M11 = asignar_matriz(ajuste, ajuste);
    M12 = asignar_matriz(ajuste, ajuste);
    M21 = asignar_matriz(ajuste, ajuste);
    M22 = asignar_matriz(ajuste, ajuste);

    for (int i = 0; i < ajuste; i++)
    {
        for (int j = 0; j < ajuste; j++)
        {
            M11[i][j] = matriz[i][j];
            M12[i][j] = matriz[i][j + ajuste];
            M21[i][j] = matriz[i + ajuste][j];
            M22[i][j] = matriz[i + ajuste][j + ajuste];
        }
    }

    M11_inv = asignar_matriz(ajuste, ajuste);
    Mcombinado_inv = asignar_matriz(ajuste, ajuste);
    M11_inv = matriz_inversa(M11, ajuste);

    if (M11_inv[0][0] == 0 && ajuste > 1)
    {
        printf("\nNo hay mano....\n");
        exit(1);
    }

    Mcombinado_inv = Mult_Strassen(M21, M11_inv, ajuste);
    Mcombinado_inv = Mult_Strassen(Mcombinado_inv, M12, ajuste);
    Mcombinado_inv = restaMatrices(M22, Mcombinado_inv, ajuste, ajuste);

    Mcombinado_inv = matriz_inversa(Mcombinado_inv, ajuste);
    M11 = Mult_Strassen(M11_inv, M12, ajuste);
    M11 = Mult_Strassen(M11, Mcombinado_inv, ajuste);
    M11 = Mult_Strassen(M11, M21, ajuste);
    M11 = Mult_Strassen(M11, M11_inv, ajuste);
    M11 = sumaMatrices(M11_inv, M11, ajuste, ajuste);

    M12 = Mult_Strassen(Mult_Strassen(matriz_negativa(M11_inv, ajuste), M12, ajuste), Mcombinado_inv, ajuste);

    M21 = Mult_Strassen(matriz_negativa(Mcombinado_inv, ajuste), M21, ajuste);
    M21 = Mult_Strassen(M21, M11_inv, ajuste);

    M22 = Mcombinado_inv;

    matriz_resultado = recombinarMatriz(M11, M12, M21, M22, dim);

    return matriz_resultado;
}

long long **matriz_negativa(long long **matriz, int dim)
{
    long long **matriz_resultado;
    matriz_resultado = asignar_matriz(dim, dim);
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            matriz_resultado[i][j] = MultP(matriz[i][j], -1);
        }
    }
    return matriz_resultado;
}

long long **Mult_Strassen(long long **matriz_1, long long **matriz_2, int dim)
{
    long long **matriz_a11, **matriz_a12, **matriz_a21, **matriz_a22;
    long long **matriz_b11, **matriz_b12, **matriz_b21, **matriz_b22;
    long long **matrizResultado;
    long long **S1, **S2, **S3, **S4, **S5, **T1, **T2, **T3, **T4, **T5, **M1, **M2, **M3, **M4, **M5, **M6, **M7, **C11, **C12, **C21, **C22, **aux, **aux2;
    int dim2, largo;

    if (dim <= 40)
    {
        matrizResultado = multiplicacion_clasica(matriz_1, matriz_2, dim);
        return matrizResultado;
    }
    // Comprobar que la dimension de las matrices sea par
    dim2 = dim;
    matriz_1 = matriz_par(matriz_1, &dim);
    matriz_2 = matriz_par(matriz_2, &dim2);

    largo = dim / 2;

    // PASO 1: SUBMATRICES
    // Sub division de matrizes original en 4 submatrices cada una
    matriz_a11 = asignar_matriz(dim / 2, dim / 2);
    matriz_a12 = asignar_matriz(dim / 2, dim / 2);
    matriz_a21 = asignar_matriz(dim / 2, dim / 2);
    matriz_a22 = asignar_matriz(dim / 2, dim / 2);

    matriz_b11 = asignar_matriz(dim / 2, dim / 2);
    matriz_b12 = asignar_matriz(dim / 2, dim / 2);
    matriz_b21 = asignar_matriz(dim / 2, dim / 2);
    matriz_b22 = asignar_matriz(dim / 2, dim / 2);

    for (int i = 0; i < largo; i++)
    {
        for (int j = 0; j < largo; j++)
        {
            matriz_a11[i][j] = matriz_1[i][j];
            matriz_a12[i][j] = matriz_1[i][j + largo];
            matriz_a21[i][j] = matriz_1[i + largo][j];
            matriz_a22[i][j] = matriz_1[i + largo][j + largo];

            matriz_b11[i][j] = matriz_2[i][j];
            matriz_b12[i][j] = matriz_2[i][j + largo];
            matriz_b21[i][j] = matriz_2[i + largo][j];
            matriz_b22[i][j] = matriz_2[i + largo][j + largo];
        }
    }

    // Sumas de la matriz 1
    // Primer paso de suma de matrices de Strassen
    S1 = sumaMatrices(matriz_a11, matriz_a22, dim / 2, dim / 2);
    S2 = sumaMatrices(matriz_a21, matriz_a22, dim / 2, dim / 2);
    S3 = sumaMatrices(matriz_a11, matriz_a12, dim / 2, dim / 2);
    S4 = restaMatrices(matriz_a21, matriz_a11, dim / 2, dim / 2);
    S5 = restaMatrices(matriz_a12, matriz_a22, dim / 2, dim / 2);

    // Sumas de la matriz 2
    // Segundo paso de suma de matrices de Strassen
    T1 = sumaMatrices(matriz_b11, matriz_b22, dim / 2, dim / 2);
    T2 = sumaMatrices(matriz_b21, matriz_b22, dim / 2, dim / 2);
    T3 = sumaMatrices(matriz_b11, matriz_b12, dim / 2, dim / 2);
    T4 = restaMatrices(matriz_b21, matriz_b11, dim / 2, dim / 2);
    T5 = restaMatrices(matriz_b12, matriz_b22, dim / 2, dim / 2);

    // Multiplicaciones de Strassen
    M1 = Mult_Strassen(S1, T1, dim / 2);
    M2 = Mult_Strassen(S2, matriz_b11, dim / 2);
    M3 = Mult_Strassen(matriz_a11, T5, dim / 2);
    M4 = Mult_Strassen(matriz_a22, T4, dim / 2);
    M5 = Mult_Strassen(S3, matriz_b22, dim / 2);
    M6 = Mult_Strassen(S4, T3, dim / 2);
    M7 = Mult_Strassen(S5, T2, dim / 2);

    // Recombinacion de Strassen
    aux = sumaMatrices(M1, M4, dim / 2, dim / 2);
    aux2 = restaMatrices(M7, M5, dim / 2, dim / 2);
    C11 = sumaMatrices(aux, aux2, dim / 2, dim / 2);
    C12 = sumaMatrices(M3, M5, dim / 2, dim / 2);
    C21 = sumaMatrices(M2, M4, dim / 2, dim / 2);
    aux = restaMatrices(M1, M2, dim / 2, dim / 2);
    aux2 = sumaMatrices(M3, M6, dim / 2, dim / 2);
    C22 = sumaMatrices(aux, aux2, dim / 2, dim / 2);

    // Recombinacion de matrices en la matriz resultado
    matrizResultado = recombinarMatriz(C11, C12, C21, C22, dim);
    return matrizResultado;
}

void llenar_matriz(long long **matriz, int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            matriz[i][j] = ((long long)(rand() % P)); // Se asignan valores aleatorios a cada coordenadas de la matriz
        }
    }
}

void llenar_submatriz(long long **matriz, long long **matriz_original, int iFilas, int iCol, int filas, int col)
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matriz[i][j] = matriz_original[iFilas + i][iCol + j]; // Se asignan valores aleatorios a cada coordenadas de la matriz
        }
    }
}

void imprimir_matriz(long long **array, int filas, int columnas)
{
    int i, j;
    printf("\nIMPRIMIENDO MATRIZ\n");
    printf("\n==================\n");

    for (i = 0; i < filas; i++)
    {
        printf("\n");
        for (j = 0; j < columnas; j++)
        {
            printf("%lld ", array[i][j]);
        }
    }
}

long long **multiplicacion_clasica(long long **matriz_1, long long **matriz_2, int dim)
{
    long long suma, **Matriz_resultado;
    Matriz_resultado = asignar_matriz(dim, dim);

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            suma = 0;
            for (int k = 0; k < dim; k++)
            {
                suma = SumaP(suma, MultP(matriz_1[i][k], matriz_2[k][j]));
            }
            Matriz_resultado[i][j] = SumaP(suma, Matriz_resultado[i][j]);
        }
    }
    return Matriz_resultado;
}

long long **sumaMatrices(long long **matriz1, long long **matriz2, int filas, int columnas)
{
    long long **Mresultado;
    Mresultado = asignar_matriz(filas, columnas);
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            Mresultado[i][j] = SumaP(matriz1[i][j], matriz2[i][j]);
        }
    }
    return Mresultado;
}
long long **restaMatrices(long long **matriz1, long long **matriz2, int filas, int columnas)
{
    long long resta = 0, **matrizResultado;
    matrizResultado = asignar_matriz(filas, columnas);
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            matrizResultado[i][j] = RestaP(matriz1[i][j], matriz2[i][j]);
        }
    }
    return matrizResultado;
}

long long **recombinarMatriz(long long **C11, long long **C12, long long **C21, long long **C22, int dim)
{
    long long **matrizResultado;
    int val;
    val = dim / 2;
    matrizResultado = asignar_matriz(dim, dim);
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (i < dim / 2 && j < dim / 2)
            {
                // asigna los valores de la submatriz M11 a la matriz resultado
                matrizResultado[i][j] = C11[i][j];
            }
            else if (i < dim / 2 && j >= dim / 2)
            {
                // asigna los valores de la submatriz M12 a la matriz resultado
                matrizResultado[i][j] = C12[i][j - val];
            }
            else if (i >= dim / 2 && j < dim / 2)
            {
                // asigna los valores de la submatriz M21 a la matriz resultado
                matrizResultado[i][j] = C21[i - val][j];
            }
            else
            {
                // asigna los valores de la submatriz M22 a la matriz resultado
                matrizResultado[i][j] = C22[i - val][j - val];
            }
        }
    }
    return matrizResultado;
}

long long **matriz_par(long long **matriz, int *dim)
{
    long long **matriz_par;
    int Dim;
    Dim = *dim;
    if (Dim % 2 != 0) // Comprueba si la matriz tiene dimension par
    {
        matriz_par = asignar_matriz(Dim + 1, Dim + 1);        // Asigna la memoria para dejar la matriz con dimension par
        llenar_submatriz(matriz_par, matriz, 0, 0, Dim, Dim); // Rellena la matriz nueva con los valores de la original
        Dim++;
        *dim = Dim;        // Actualiza la dimension de la matriz
        return matriz_par; // Devuelve la matriz con la dimension par
    }
    else
        return matriz; // Devuelve la matriz orginal
}

long long **matriz_cuadrada(long long **matriz, int *dim)
{
    long long **matriz_par;
    int Dim = *dim;
    if (Dim % 2 != 0) // Comprueba si la matriz tiene dimension par
    {
        matriz_par = asignar_matriz(Dim + 1, Dim + 1);        // Asigna la memoria para dejar la matriz con dimension par
        llenar_submatriz(matriz_par, matriz, 0, 0, Dim, Dim); // Rellena la matriz nueva con los valores de la original
        matriz_par[Dim][Dim] = 1;
        Dim++;
        *dim = Dim;        // Actualiza la dimension de la matriz
        return matriz_par; // Devuelve la matriz con la dimension par
    }
    else
        return matriz; // Devuelve la matriz orginal
}

long long MultP(long long a, long long b)
{
    long long a0, a1, b0, b1, d0, d1, d2, d3;
    long long c;

    a0 = a & bAJO;
    a1 = a >> 31;
    b0 = b & bAJO;
    b1 = b >> 31;
    d0 = a0 * b0;
    d3 = a1 * b1;
    a1 += a0;
    b1 += b0;
    d2 = d0 + d3;
    d1 = a1 * b1;
    d1 -= d2;
    d3 += (d1 >> 31);
    d2 = d1 & bAJO;
    d2 += d0 >> 31;
    d1 = d0 & bAJO;
    a1 = d3 / Pa;
    b1 = ((d3 % Pa) << 31) + d2 - (a1 * Pb);
    while (b1 < 0)
    {
        b1 += P;
    }
    while (b1 >= P)
    {
        b1 -= P;
    }
    a0 = b1 / Pa;
    b0 = ((b1 % Pa) << 31) + d1 - (a0 * Pb);
    while (b0 < 0)
    {
        b0 += P;
    }
    while (b0 >= P)
    {
        b0 -= P;
    }
    return (b0);
}

long long RestaP(long long a, long long b)
{
    if (a < b)
    {
        return ((P + a) - b);
    }
    else
    {
        return (a - b);
    }
}

long long SumaP(long long a, long long b)
{
    long long c;
    c = a + b;
    if (c < P)
    {
        return (c);
    }
    else
    {
        return (c - P);
    }
}

long long InvP(long long A)
{
    long long a, b, s1, s2, r, u;
    a = A;
    b = P;
    s1 = 1;
    s2 = 0;
    if (A == 0)
    {
        printf("Error, division entre 0\n");
        return (0);
    }
    while ((a % 2) == 0)
    {
        a >>= 1;
        if ((s1 % 2) == 0)
        {
            s1 >>= 1;
        }
        else
        {
            s1 = (s1 + P) >> 1;
        }
    }
    if (b > a)
    {
        r = b;
        b = a;
        a = r;
        u = s2;
        s2 = s1;
        s1 = u;
    }
    while ((b != 0) && (b != a))
    {
        r = a - b;
        u = RestaP(s1, s2);
        while ((r % 2) == 0)
        {
            r >>= 1;
            if ((u % 2) == 0)
            {
                u >>= 1;
            }
            else
            {
                u = (u + P) >> 1;
            }
        }
        if (r < b)
        {
            a = b;
            b = r;
            s1 = s2;
            s2 = u;
        }
        else
        {
            a = r;
            s1 = u;
        }
    }
    return (s1);
}
