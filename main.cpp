#include <bits/stdc++.h>
using namespace std;

// Tablero de 3x3
char tablero[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

int nodosExplorados = 0;
int hojasEvaluadas = 0;

// Comprueba si existe un ganador. Regresa 'X', 'O' o ' ' si todavía no hay ganador.
char ganador() {
    // Revisar filas
    for (int i = 0; i < 3; i++) {
        if (tablero[i][0] != ' ' &&
            tablero[i][0] == tablero[i][1] &&
            tablero[i][1] == tablero[i][2]) {
            return tablero[i][0];
        }
    }

    // Revisar columnas
    for (int j = 0; j < 3; j++) {
        if (tablero[0][j] != ' ' &&
            tablero[0][j] == tablero[1][j] &&
            tablero[1][j] == tablero[2][j]) {
            return tablero[0][j];
        }
    }

    // Revisar diagonal principal
    if (tablero[0][0] != ' ' &&
        tablero[0][0] == tablero[1][1] &&
        tablero[1][1] == tablero[2][2]) {
        return tablero[0][0];
    }

    // Revisar diagonal secundaria
    if (tablero[0][2] != ' ' &&
        tablero[0][2] == tablero[1][1] &&
        tablero[1][1] == tablero[2][0]) {
        return tablero[0][2];
    }

    return ' ';
}

// Comprueba si todavía existen casillas vacías.
bool hayMovimientos() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (tablero[i][j] == ' ') return true;
    return false;
}

// Imprime el tablero.
void mostrarTablero() {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        cout << " ";
        for (int j = 0; j < 3; j++) {
            cout << tablero[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "---+---+---\n";
    } cout << "\n";
}

// Puntua lineas abiertas: premia las amenazas de X y penaliza las de O.
int evaluarPosicion() {
    int puntuacion = 0;

    // Define las 8 líneas posibles.
    const int lineas[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    // Examina cada línea
    for (const auto& linea : lineas) {
        // Cuenta cuántas fichas X y O contiene la línea.
        int equis = 0;
        int circulos = 0;
        for (int indice : linea) {
            char casilla = tablero[indice/3][indice%3];
            if (casilla == 'X') equis++;
            if (casilla == 'O') circulos++;
        }

        // Una linea ocupada por ambos jugadores ya no puede convertirse en una línea ganadora.
        if (equis > 0 && circulos > 0) continue;

        // Asigna puntuaciones.
        if (equis > 0) {
            puntuacion += (equis == 3 ? 100 : equis == 2 ? 10 : 1);
        } else if (circulos > 0) {
            puntuacion -= (circulos == 3 ? 100 : circulos == 2 ? 10 : 1);
        }
    }

    return puntuacion;
}

void imprimirSangria(int profundidad) {
    for (int i = 0; i < profundidad; i++) cout << "  ";
}

// MINIMAX SIN PODA: Explora todos los movimientos hasta un estado terminal.
int minimax(bool maximizando, int profundidad = 0, bool mostrarRama = false) {
    nodosExplorados++;

    // Caso base: alguien ganó.
    char resultado = ganador();
    if (resultado == 'X') {
        if (mostrarRama) {
            imprimirSangria(profundidad);
            cout << "Terminal: gana X, valor = 1\n";
        }
        hojasEvaluadas++;
        return 1;
    }
    if (resultado == 'O') {
        if (mostrarRama) {
            imprimirSangria(profundidad);
            cout << "Terminal: gana O, valor = -1\n";
        }
        hojasEvaluadas++;
        return -1;
    }

    // Caso base: empate.
    if (!hayMovimientos()) {
        if (mostrarRama) {
            imprimirSangria(profundidad);
            cout << "Terminal: empate, valor = 0\n";
        }
        hojasEvaluadas++;
        return 0;
    }

    if (mostrarRama) {
        imprimirSangria(profundidad);
        cout << "Nodo " << (maximizando ? "MAX (X)" : "MIN (O)") << "\n";
    }

    bool ramaHijaMostrada = false;

    // MAX: X intenta maximizar.
    if (maximizando) {
        // Explorar todas las casillas disponibles.
        int mejorValor = -1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tablero[i][j] == ' ') {
                    // Simular movimiento.
                    tablero[i][j] = 'X';

                    bool mostrarHija = mostrarRama && !ramaHijaMostrada;
                    if (mostrarHija) {
                        imprimirSangria(profundidad);
                        cout << "Prueba X en (" << i << ", " << j << ")\n";
                        ramaHijaMostrada = true;
                    }

                    // Evaluar movimiento.
                    int valor = minimax(false, profundidad + 1, mostrarHija);

                    // Deshacer movimiento.
                    tablero[i][j] = ' ';

                    // Guarda mejor valor.
                    mejorValor = max(mejorValor, valor);
                }
            }
        }
        if (mostrarRama) {
            imprimirSangria(profundidad);
            cout << "MAX devuelve valor = " << mejorValor << "\n";
        }
        return mejorValor;
    }

    // MIN: el oponente intenta minimizar.
    else {
        // Explora todas las casillas disponibles.
        int mejorValor = 1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tablero[i][j] == ' ') {
                    // Simular movimiento.
                    tablero[i][j] = 'O';

                    bool mostrarHija = mostrarRama && !ramaHijaMostrada;
                    if (mostrarHija) {
                        imprimirSangria(profundidad);
                        cout << "Prueba O en (" << i << ", " << j << ")\n";
                        ramaHijaMostrada = true;
                    }

                    // Evaluar movimiento.
                    int valor = minimax(true, profundidad + 1, mostrarHija);

                    // Deshacer movimiento.
                    tablero[i][j] = ' ';

                    // Guarda el mejor valor.
                    mejorValor = min(mejorValor, valor);
                }
            }
        }
        if (mostrarRama) {
            imprimirSangria(profundidad);
            cout << "MIN devuelve valor = " << mejorValor << "\n";
        }
        return mejorValor;
    }
}

// Encuentra el mejor movimiento para la computadora.
void mejorMovimiento(char pc) {
    bool maximizando = pc == 'X';
    int mejorValor = maximizando ? -1 : 1;
    int mejorEvaluacion = maximizando ? numeric_limits<int>::min() : numeric_limits<int>::max();
    int mejorFila = -1, mejorColumna = -1;

    nodosExplorados = 0;
    hojasEvaluadas = 0;
    cout << "\n===== Analisis minimax de la computadora =====\n";

    // Probar cada movimiento disponible.
    bool movGanador = false;
    bool mostrarRama = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (tablero[i][j] == ' ') {
                // Simular movimiento.
                tablero[i][j] = pc;
                mostrarTablero();
                bool mostrarEstaRama = mostrarRama;
                if (mostrarEstaRama) {
                    cout << "Rama de ejemplo: " << pc << " en (" << i << ", " << j << ")\n";
                }

                // Checar si el movimiento es ganador.
                if (ganador() == pc) {
                    mejorValor = pc == 'X' ? 1 : -1;
                    mejorEvaluacion = evaluarPosicion();
                    mejorFila = i;
                    mejorColumna = j;
                    movGanador = true;
                    ++nodosExplorados;
                    ++hojasEvaluadas;
                    if (mostrarEstaRama) {
                        imprimirSangria(1);
                        cout << "Terminal: gana " << pc << ", valor = " << mejorValor << "\n";
                    }
                    mostrarRama = false;
                    break;
                }

                // Evaluar movimiento usando minimax.
                int valor = minimax(pc == 'O', 1, mostrarEstaRama);
                int evaluacion = evaluarPosicion();
                mostrarRama = false;

                // Mostrar puntuación del movimiento.
                cout << "\n" << pc << " en (" << i << ", " << j << ") -> Puntuacion: "
                     << valor << ", Evaluacion posicional: " << evaluacion << "\n";

                // Deshacer movimiento.
                tablero[i][j] = ' ';

                // Conservar el movimiento con mejor puntuación.
                // if (maximizando && valor > mejorValor || !maximizando && valor < mejorValor) {
                if (maximizando && valor > mejorValor || !maximizando && valor < mejorValor ||
                    valor == mejorValor && (maximizando && evaluacion > mejorEvaluacion || !maximizando && evaluacion < mejorEvaluacion)) {
                    mejorValor = valor;
                    mejorEvaluacion = evaluacion;
                    mejorFila = i;
                    mejorColumna = j;
                }
            }
        }
        if (movGanador) break;
    }

    // Mostrar resumen del análisis de minimax
    cout << "\nResumen: " << nodosExplorados << " nodos explorados y "
         << hojasEvaluadas << " resultados terminales (sin poda).\n";

    // Ejecutar el mejor movimiento de la computadora.
    cout << pc << " decide jugar en (" << mejorFila << ", " << mejorColumna << ") -> Puntuacion: " << mejorValor
         << ", Evaluacion posicional: " << mejorEvaluacion << ".\n";
    tablero[mejorFila][mejorColumna] = pc;
}

int main() {
    int fila, columna;
    char jugador;
    string entrada;

    cout << "===== TIC-TAC-TOE =====\n";

    // Preguntar al jugador que simbolo quiere ser (O o X).
    do {
        cout << "Elige tu simbolo (X/O): "; cin >> jugador;
        jugador = static_cast<char>(toupper(static_cast<unsigned char>(jugador)));
        if (jugador != 'X' && jugador != 'O')
            cout << "Eleccion invalida. Escribe X o O.\n";
    } while (jugador != 'X' && jugador != 'O');
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Asignar simbolo a la computadora.
    char pc = jugador == 'X' ? 'O' : 'X';

    // Mostrar tablero inicial
    cout << "Tu eres " << jugador << " y la computadora es " << pc << ".\n";
    mostrarTablero();

    // El jugador X siempre es el primero en jugar.
    char turno = 'X';

    while (true) {
        if (turno == pc) { // Turno de la computadora.
            mejorMovimiento(pc);

        } else { // Turno del jugador.

            // Pedir al jugador ingresar su jugada
            cout << "Ingresa fila y columna (0-2): ";

            // Checar que los datos ingresados sean enteros
            if (!getline(cin, entrada)) { cout << "\nNo se pudo leer la jugada.\n"; return 1; }
            istringstream flujoEntrada(entrada);
            string sobrante;
            if (!(flujoEntrada >> fila >> columna) || (flujoEntrada >> sobrante)) {
                cout << "Entrada invalida. Ingresa dos enteros separados por un espacio.\n";
                continue;
            }

            // Validar movimiento del jugador.
            if (fila < 0 || fila > 2 || columna < 0 || columna > 2 || tablero[fila][columna] != ' ') {
                cout << "Movimiento invalido. Intenta otra vez.\n";
                continue;
            }
            tablero[fila][columna] = jugador;
        }

        mostrarTablero();

        // Terminar el juego si hay ganador
        if (ganador() == jugador) { cout << "¡Ganaste!\n"; break; }
        if (ganador() == pc) { cout << "La computadora gano.\n"; break; }
        if (!hayMovimientos()) { cout << "Empate.\n"; break; }

        // Cambiar turno.
        turno = turno == 'X' ? 'O' : 'X';
    }

    return 0;
}
