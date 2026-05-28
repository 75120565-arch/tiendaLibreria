#include<iostream>
#include<string>
using namespace std;

const int TAMANO_TABLA = 10;
 
// Libro es como una ficha que guarda toda la informacion de un libro.
// Cada vez que registremos un libro, llenamos una de estas fichas.
struct Libro {
    string titulo;
    string autor;
    string categoria;
    int    stock;
    float  precio;
};
 
// Un Nodo es una caja que guarda un libro y ademas sabe donde esta la siguiente caja.
// Asi se forma la cadena: caja1 -> caja2 -> caja3 -> NULL (fin de la cadena).
// NULL significa que ya no hay mas cajas despues.
struct Nodo {
    Libro dato;       // el libro que guarda esta caja
    Nodo* siguiente;  // puntero: guarda la direccion de la siguiente caja en memoria
                      // si no hay siguiente, este puntero vale NULL
};
 
// Igual que Nodo pero para guardar ventas en la pila.
// Cada caja de venta guarda el titulo, el precio y apunta a la venta anterior.
struct NodoVenta {
    string titulo;
    float  precio;
    NodoVenta* siguiente; // apunta a la venta que se registro antes que esta
};
 
// La tabla hash es un arreglo de 10 punteros.
// Cada puntero apunta al primer Nodo de una lista enlazada (o a NULL si esta vacia).
// cada casilla de la tabla puede tener una cadena de libros colgando
Nodo* tablaHash[TAMANO_TABLA];
 
// tope es el puntero que siempre apunta a la venta mas reciente de la pila.
// Cuando no hay ventas, apunta a NULL.
NodoVenta* tope = NULL;

int funcionHash(string titulo){
    int suma = 0;
    for(int i=0; i< titulo.size(); i++){
        suma = suma + titulo[i];
    }
    return suma % TAMANO_TABLA;
}

void inicializarTabla(){
    for(int i=0; i< TAMANO_TABLA; i++){
        tablaHash[i] = NULL;
    }
}

int main(){


    return 0;
}