#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
using namespace std;

struct Libro {
    string titulo;
    string autor;
    string categoria;
    int    stock;
    double precio;
};

struct NodoLibro {
    Libro      dato;
    NodoLibro* siguiente;
};

struct NodoCola {
    string    titulo;
    NodoCola* siguiente;
};

struct NodoPila {
    string    accion;
    NodoPila* siguiente;
};

NodoLibro* cabezaLista = nullptr;
NodoCola*  frenteCola  = nullptr;
NodoCola*  finCola     = nullptr;
NodoPila*  topePila    = nullptr;

const int NUM_CATEGORIAS = 9;
const string CATEGORIAS[NUM_CATEGORIAS] = {
    "Narrativa",
    "Poesia",
    "Infantil y Juvenil",
    "Historia",
    "Ciencia y Tecnologia",
    "Expositivo / Informativo",
    "Argumentativo",
    "Educacion",
    "Otros"
};

string elegirCategoria() {
    cout << "\nSeleccione una categoria:\n";
    for (int i = 0; i < NUM_CATEGORIAS; i++) {
        cout << "  " << (i + 1) << ". " << CATEGORIAS[i] << "\n";
    }

    int opcion;
    while (true) {
        cout << "Opcion: ";
        if (cin >> opcion && opcion >= 1 && opcion <= NUM_CATEGORIAS) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return CATEGORIAS[opcion - 1];
        }
        cout << "[!] Opcion invalida. Ingrese un numero entre 1 y " << NUM_CATEGORIAS << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string aMinusculas(const string& texto) {
    string resultado = texto;
    transform(resultado.begin(), resultado.end(), resultado.begin(),
              [](unsigned char c){ return tolower(c); });
    return resultado;
}

bool esSoloNumeros(const string& texto) {
    if (texto.empty()) return false;
    for (char c : texto) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string pedirTextoValido(const string& mensaje) {
    string valor;
    while (true) {
        cout << mensaje;
        getline(cin, valor);
        if (valor.empty()) {
            cout << "[!] El campo no puede estar vacio.\n";
        } else if (esSoloNumeros(valor)) {
            cout << "[!] El campo no puede contener solo numeros.\n";
        } else {
            return valor;
        }
    }
}

int pedirEnteroNoNegativo(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= 0) {
            limpiarBuffer();
            return valor;
        }
        cout << "[!] Valor invalido. Ingrese un numero entero mayor o igual a 0.\n";
        limpiarBuffer();
    }
}

double pedirDoublePositivo(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor > 0) {
            limpiarBuffer();
            return valor;
        }
        cout << "[!] Valor invalido. Ingrese un numero mayor que 0.\n";
        limpiarBuffer();
    }
}

void pushHistorial(const string& accion) {
    NodoPila* nuevo = new NodoPila;
    nuevo->accion    = accion;
    nuevo->siguiente = topePila;
    topePila         = nuevo;
}

void peekHistorial() {
    if (topePila == nullptr) {
        cout << "[!] El historial esta vacio.\n";
        return;
    }
    cout << "\nUltima accion registrada:\n";
    cout << ">> " << topePila->accion << "\n";
}

void popHistorial() {
    if (topePila == nullptr) {
        cout << "[!] El historial esta vacio. No hay acciones que eliminar.\n";
        return;
    }
    NodoPila* aEliminar = topePila;
    cout << "Accion eliminada: " << aEliminar->accion << "\n";
    topePila = topePila->siguiente;
    delete aEliminar;
}

NodoLibro* buscarNodo(const string& titulo);

void encolarReposicion(const string& titulo) {
    if (buscarNodo(titulo) == nullptr) {
        cout << "[!] El libro \"" << titulo << "\" no existe en el inventario.\n";
        cout << "    Registre el libro primero antes de solicitar su reposicion.\n";
        return;
    }

    string tituloBuscado = aMinusculas(titulo);
    NodoCola* actual = frenteCola;
    while (actual != nullptr) {
        if (aMinusculas(actual->titulo) == tituloBuscado) {
            cout << "[!] Ya existe una reposicion pendiente para \"" << titulo << "\".\n";
            return;
        }
        actual = actual->siguiente;
    }

    NodoCola* nuevo = new NodoCola;
    nuevo->titulo    = titulo;
    nuevo->siguiente = nullptr;

    if (finCola == nullptr) {
        frenteCola = nuevo;
        finCola    = nuevo;
    } else {
        finCola->siguiente = nuevo;
        finCola            = nuevo;
    }

    cout << "[OK] Reposicion registrada para: " << titulo << "\n";
    pushHistorial("Reposicion registrada: " + titulo);
}

void desencolarReposicion() {
    if (frenteCola == nullptr) {
        cout << "[!] No hay reposiciones pendientes.\n";
        return;
    }

    NodoCola* aEliminar = frenteCola;
    cout << "Reposicion atendida: " << aEliminar->titulo << "\n";
    frenteCola = frenteCola->siguiente;

    if (frenteCola == nullptr) {
        finCola = nullptr;
    }

    pushHistorial("Reposicion atendida: " + aEliminar->titulo);
    delete aEliminar;
}

NodoLibro* buscarNodo(const string& titulo) {
    string tituloBuscado = aMinusculas(titulo);
    NodoLibro* actual    = cabezaLista;

    while (actual != nullptr) {
        if (aMinusculas(actual->dato.titulo) == tituloBuscado) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void mostrarLibro(const Libro& libro) {
    cout << "  Titulo    : " << libro.titulo    << "\n";
    cout << "  Autor     : " << libro.autor     << "\n";
    cout << "  Categoria : " << libro.categoria << "\n";
    cout << "  Stock     : " << libro.stock     << "\n";
    cout << "  Precio    : S/ " << libro.precio << "\n";
}

void registrarLibro() {
    cout << "\nREGISTRAR LIBRO\n";

    string titulo = pedirTextoValido("Titulo    : ");

    if (buscarNodo(titulo) != nullptr) {
        cout << "[!] Ya existe un libro con ese titulo.\n";
        return;
    }

    Libro nuevo;
    nuevo.titulo    = titulo;
    nuevo.autor     = pedirTextoValido("Autor     : ");
    nuevo.categoria = elegirCategoria();
    nuevo.stock     = pedirEnteroNoNegativo("Stock     : ");
    nuevo.precio    = pedirDoublePositivo("Precio    : ");

    NodoLibro* nodoNuevo = new NodoLibro;
    nodoNuevo->dato      = nuevo;
    nodoNuevo->siguiente = nullptr;

    if (cabezaLista == nullptr) {
        cabezaLista = nodoNuevo;
    } else {
        NodoLibro* actual = cabezaLista;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nodoNuevo;
    }

    cout << "[OK] Libro registrado exitosamente.\n";
    pushHistorial("Libro registrado: " + nuevo.titulo);
}

void buscarLibro() {
    cout << "\nBUSCAR LIBRO\n";
    string titulo = pedirTextoValido("Titulo a buscar: ");

    NodoLibro* encontrado = buscarNodo(titulo);

    if (encontrado == nullptr) {
        cout << "[!] No se encontro ningun libro con ese titulo.\n";
        return;
    }

    cout << "\nLibro encontrado:\n";
    mostrarLibro(encontrado->dato);
}

void modificarLibro() {
    cout << "\nMODIFICAR LIBRO\n";
    string titulo = pedirTextoValido("Titulo del libro a modificar: ");

    NodoLibro* nodo = buscarNodo(titulo);

    if (nodo == nullptr) {
        cout << "[!] No se encontro el libro. No se puede modificar.\n";
        return;
    }

    cout << "\nDatos actuales:\n";
    mostrarLibro(nodo->dato);
    cout << "\nIngrese los nuevos datos:\n";

    nodo->dato.autor     = pedirTextoValido("Nuevo autor     : ");
    nodo->dato.categoria = elegirCategoria();
    nodo->dato.stock     = pedirEnteroNoNegativo("Nuevo stock     : ");
    nodo->dato.precio    = pedirDoublePositivo("Nuevo precio    : ");

    cout << "[OK] Libro modificado exitosamente.\n";
    pushHistorial("Libro modificado: " + nodo->dato.titulo);
}

void eliminarLibro() {
    cout << "\nELIMINAR LIBRO\n";
    string titulo = pedirTextoValido("Titulo del libro a eliminar: ");

    if (cabezaLista == nullptr) {
        cout << "[!] El inventario esta vacio.\n";
        return;
    }

    NodoLibro* actual   = cabezaLista;
    NodoLibro* anterior = nullptr;
    string     tituloBuscado = aMinusculas(titulo);

    while (actual != nullptr) {
        if (aMinusculas(actual->dato.titulo) == tituloBuscado) {
            break;
        }
        anterior = actual;
        actual   = actual->siguiente;
    }

    if (actual == nullptr) {
        cout << "[!] No se encontro el libro. No se puede eliminar.\n";
        return;
    }

    string tituloReal = actual->dato.titulo;

    NodoCola* nCola = frenteCola;
    while (nCola != nullptr) {
        if (aMinusculas(nCola->titulo) == tituloBuscado) {
            cout << "[!] Advertencia: este libro tiene una reposicion pendiente en la cola.\n";
            cout << "    Considere atenderla (opcion 7) antes de eliminar el libro.\n";
            break;
        }
        nCola = nCola->siguiente;
    }

    cout << "Confirma eliminar \"" << tituloReal << "\"? (s/n): ";
    string resp;
    getline(cin, resp);
    if (resp != "s" && resp != "S") {
        cout << "Eliminacion cancelada.\n";
        return;
    }

    if (anterior == nullptr) {
        cabezaLista = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }

    pushHistorial("Libro eliminado: " + tituloReal);
    delete actual;
    cout << "[OK] Libro \"" << tituloReal << "\" eliminado del inventario.\n";
}

void mostrarInventario() {
    cout << "\nINVENTARIO COMPLETO\n";

    if (cabezaLista == nullptr) {
        cout << "El inventario esta vacio.\n";
        return;
    }

    NodoLibro* actual   = cabezaLista;
    int        contador = 1;
    int        sinStock = 0;

    while (actual != nullptr) {
        cout << "\nLibro #" << contador++ << "\n";
        mostrarLibro(actual->dato);
        if (actual->dato.stock == 0) {
            cout << "  [!] SIN STOCK - considere registrar una reposicion\n";
            sinStock++;
        }
        actual = actual->siguiente;
    }

    cout << "\nTotal de titulos: " << (contador - 1) << "\n";
    if (sinStock > 0) {
        cout << "[!] " << sinStock << " libro(s) con stock agotado.\n";
    }
}

void guardarLibros() {
    ofstream archivo("libros.txt");

    if (!archivo.is_open()) {
        cout << "[!] Error: no se pudo guardar libros.txt\n";
        return;
    }

    NodoLibro* actual = cabezaLista;
    while (actual != nullptr) {
        archivo << actual->dato.titulo    << "\n";
        archivo << actual->dato.autor     << "\n";
        archivo << actual->dato.categoria << "\n";
        archivo << actual->dato.stock     << "\n";
        archivo << actual->dato.precio    << "\n";
        archivo << "---\n";
        actual = actual->siguiente;
    }

    archivo.close();
}

void cargarLibros() {
    ifstream archivo("libros.txt");
    if (!archivo.is_open()) return;

    string linea;
    while (true) {
        Libro libro;

        if (!getline(archivo, libro.titulo))    break;
        if (!getline(archivo, libro.autor))     break;
        if (!getline(archivo, libro.categoria)) break;

        string stockStr, precioStr;
        if (!getline(archivo, stockStr))  break;
        if (!getline(archivo, precioStr)) break;

        libro.stock  = stoi(stockStr);
        libro.precio = stod(precioStr);

        getline(archivo, linea);

        NodoLibro* nodoNuevo = new NodoLibro;
        nodoNuevo->dato      = libro;
        nodoNuevo->siguiente = nullptr;

        if (cabezaLista == nullptr) {
            cabezaLista = nodoNuevo;
        } else {
            NodoLibro* actual = cabezaLista;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nodoNuevo;
        }
    }

    archivo.close();
}

void mostrarMenu() {
    cout << "\nSISTEMA DE INVENTARIO - LIBRERIA\n";
    cout << "1.  Registrar libro\n";
    cout << "2.  Buscar libro\n";
    cout << "3.  Modificar libro\n";
    cout << "4.  Eliminar libro\n";
    cout << "5.  Mostrar inventario\n";
    cout << "6.  Registrar reposicion\n";
    cout << "7.  Atender reposicion\n";
    cout << "8.  Mostrar ultima accion\n";
    cout << "9.  Eliminar ultima accion\n";
    cout << "10. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    cargarLibros();

    int opcion;

    do {
        mostrarMenu();

        if (!(cin >> opcion)) {
            cout << "[!] Opcion invalida. Ingrese un numero del 1 al 10.\n";
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                registrarLibro();
                guardarLibros();
                break;
            case 2:
                buscarLibro();
                break;
            case 3:
                modificarLibro();
                guardarLibros();
                break;
            case 4:
                eliminarLibro();
                guardarLibros();
                break;
            case 5:
                mostrarInventario();
                break;
            case 6: {
                cout << "\nREGISTRAR REPOSICION\n";
                string titulo = pedirTextoValido("Titulo del libro a reponer: ");
                encolarReposicion(titulo);
                break;
            }
            case 7:
                cout << "\nATENDER REPOSICION\n";
                desencolarReposicion();
                break;
            case 8:
                cout << "\nULTIMA ACCION\n";
                peekHistorial();
                break;
            case 9:
                cout << "\nELIMINAR ULTIMA ACCION\n";
                popHistorial();
                break;
            case 10:
                cout << "\nCerrando el sistema. Hasta pronto.\n";
                break;
            default:
                cout << "[!] Opcion no valida. Elija entre 1 y 10.\n";
                break;
        }

    } while (opcion != 10);

    while (cabezaLista != nullptr) {
        NodoLibro* temp = cabezaLista;
        cabezaLista = cabezaLista->siguiente;
        delete temp;
    }

    while (frenteCola != nullptr) {
        NodoCola* temp = frenteCola;
        frenteCola = frenteCola->siguiente;
        delete temp;
    }

    while (topePila != nullptr) {
        NodoPila* temp = topePila;
        topePila = topePila->siguiente;
        delete temp;
    }

    return 0;
}