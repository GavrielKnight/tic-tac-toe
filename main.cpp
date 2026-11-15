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

// Comprueba si existe un ganador.
// Regresa 'X', 'O' o ' ' si todavía no hay ganador.
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
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tablero[i][j] == ' ') {
                return true;
            }
        }
    }

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
    }
    cout << "\n";
}

// Puntua lineas abiertas: premia las amenazas de X y penaliza las de O.
int evaluarPosicion() {
    const int lineas[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };
    int puntuacion = 0;

    for (const auto& linea : lineas) {
        int equis = 0;
        int circulos = 0;

        for (int indice : linea) {
            char casilla = tablero[indice / 3][indice % 3];
            if (casilla == 'X') equis++;
            if (casilla == 'O') circulos++;
        }

        // Una linea bloqueada por ambos jugadores ya no ofrece una amenaza.
        if (equis > 0 && circulos > 0) continue;

        if (equis > 0) {
            puntuacion += (equis == 3 ? 100 : equis == 2 ? 10 : 1);
        } else if (circulos > 0) {
            puntuacion -= (circulos == 3 ? 100 : circulos == 2 ? 10 : 1);
        }
    }

    return puntuacion;
}

// MINIMAX SIN PODA: explora todos los movimientos hasta un estado terminal.
int minimax(bool maximizando) {
    nodosExplorados++;

    char resultado = ganador();

    // Caso base: alguien ganó.
    if (resultado == 'X') {
        hojasEvaluadas++;
        return 10;
    }
    if (resultado == 'O') {
        hojasEvaluadas++;
        return -10;
    }

    // Caso base: empate.
    if (!hayMovimientos()) {
        hojasEvaluadas++;
        return 0;
    }

    // MAX: la computadora intenta maximizar.
    if (maximizando) {

        int mejorValor = -1000;

        // Explorar todas las casillas disponibles.
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (tablero[i][j] == ' ') {

                    // Simular movimiento de X.
                    tablero[i][j] = 'X';

                    // Evaluar movimiento.
                    int valor = minimax(false);

                    // Deshacer movimiento.
                    tablero[i][j] = ' ';

                    // Guarda mejor valor para X (peor para O).
                    mejorValor = max(mejorValor, valor);
                }
            }
        }

        return mejorValor;
    }

    // MIN: el oponente intenta minimizar.
    else {

        int mejorValor = 1000;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (tablero[i][j] == ' ') {

                    // Simular movimiento de O.
                    tablero[i][j] = 'O';

                    // Evaluar movimiento.
                    int valor = minimax(true);

                    // Deshacer movimiento.
                    tablero[i][j] = ' ';

                    // Guarda el mejor valor para O (peor para X).
                    mejorValor = min(mejorValor, valor);
                }
            }
        }

        return mejorValor;
    }
}

// Encuentra el mejor movimiento para X.
void mejorMovimiento() {

    int mejorValor = -1000;
    int mejorEvaluacion = numeric_limits<int>::min();
    int mejorFila = -1;
    int mejorColumna = -1;

    nodosExplorados = 0;
    hojasEvaluadas = 0;
    cout << "\n--- Analisis minimax de la computadora ---\n";

    // Probar cada movimiento disponible.
    bool movGanador = false;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (tablero[i][j] == ' ') {
                // Realizar el movimiento.
                tablero[i][j] = 'X';

                // Checar si el movimiento es ganador
                if (ganador() == 'X') {
                    mejorValor = 10;
                    mejorEvaluacion = evaluarPosicion();
                    mejorFila = i;
                    mejorColumna = j;
                    movGanador = true; break;
                }

                // Evaluar el movimiento usando minimax (saber qué resultado obtendré si dejo que O responda de la mejor manera posible).
                int valor = minimax(false);
                int evaluacion = evaluarPosicion();

                cout << "\nX en (" << i << ", " << j << ") -> Puntuacion: "
                     << valor << ", evaluacion posicional: " << evaluacion << "\n";
                mostrarTablero();

                // Deshacer el movimiento.
                tablero[i][j] = ' ';

                // Conservar el movimiento con mejor puntuación.
                if (valor > mejorValor ||
                    (valor == mejorValor && evaluacion > mejorEvaluacion)) {
                    mejorValor = valor;
                    mejorEvaluacion = evaluacion;
                    mejorFila = i;
                    mejorColumna = j;
                }
            }
        }
        if (movGanador) break;
    }

    cout << "\nResumen: " << nodosExplorados << " nodos explorados y "
         << hojasEvaluadas << " resultados terminales (sin poda).\n";

    // Ejecutar el mejor movimiento.
    if (mejorFila != -1) {
        cout << "Decision: X juega en (" << mejorFila << ", "
             << mejorColumna << ") con puntuacion minimax " << mejorValor
             << " y evaluacion posicional " << mejorEvaluacion << ".\n";
        tablero[mejorFila][mejorColumna] = 'X';
    }
}

int main() {

    int fila, columna;

    cout << "TIC TAC TOE\n";
    cout << "Tu eres O y la computadora es X.\n";

    while (true) {

        mostrarTablero();

        // Turno del jugador humano.
        cout << "Ingresa fila y columna (0-2): ";
        cin >> fila >> columna;

        // Validar movimiento.
        if (fila < 0 || fila > 2 ||
            columna < 0 || columna > 2 ||
            tablero[fila][columna] != ' ') {

            cout << "Movimiento invalido. Intenta otra vez.\n";
            continue;
        }

        tablero[fila][columna] = 'O';

        if (ganador() == 'O') {
            mostrarTablero();
            cout << "¡Ganaste!\n";
            break;
        }

        if (!hayMovimientos()) {
            mostrarTablero();
            cout << "Empate.\n";
            break;
        }

        // Turno de la computadora.
        mejorMovimiento();

        if (ganador() == 'X') {
            mostrarTablero();
            cout << "La computadora gano.\n";
            break;
        }
        
        if (!hayMovimientos()) {
            mostrarTablero();
            cout << "Empate.\n";
            break;
        }

        // Esperar que Enter sea presionado
        cout << "\nPresiona ENTER para continuar ";
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n'); cin.get();

        // Limpiar la pantalla de consola
        // system("cls");
    }

    return 0;
}
