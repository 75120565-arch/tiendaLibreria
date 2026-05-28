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

void registrarLibro(){
    Libro nuevoLibro;
    bool encontrado = false;

    cout << "\n-- REGISTRAR LIBRO --" <<endl;

    cout <<"Titulo:  ";
    cin.ignore();
    getline(cin, nuevoLibro.titulo);

    if(nuevoLibro.titulo == ""){
        cout<< "El titulo no puede estar vacio." <<endl;
        return;
    }
    cout << "Autor: ";
    getline(cin, nuevoLibro.autor);

    cout << "Categoria: ";
    getline(cin, nuevoLibro.categoria);

    cout << "Stock: ";
    cin >> nuevoLibro.stock;

    cout << "Precio: ";
    cin >> nuevoLibro.precio;

    int posicion = funcionHash(nuevoLibro.titulo);

    Nodo* actual = tablaHash[posicion];
    while (actual != NULL){
        if(actual->dato.titulo == nuevoLibro.titulo){
            encontrado = true;
        }
        actual = actual->siguiente;
    }

    if (encontrado){
        cout<< "Ese libro ya existe en el inventario." <<endl;
    } else{
        Nodo* nuevoNodo = new Nodo();
        nuevoNodo->dato = nuevoLibro;

        nuevoNodo->siguiente = tablaHash[posicion];
        tablaHash[posicion] = nuevoNodo;

        cout << "Libro registrado correctamente en posicion "<< posicion << "."<< endl;
    }
}

void buscarLibro(){
    string tituloBuscado;
    bool encontrado = false;

    cout << "\n-- BUSCAR LIBRO --"<< endl;
    cout <<"Ingrese el titulo a buscar: ";
    cin.ignore();
    getline(cin, tituloBuscado);

    int posicion = funcionHash(tituloBuscado);

    Nodo* actual = tablaHash[posicion];
    while(actual != NULL){
        if(actual->dato.titulo == tituloBuscado) {
            encontrado = true;
            cout<<"\nLibro encontrado:"<<endl;
            cout<<"  Titulo  : "<< actual->dato.titulo << endl;
            cout<<"  Autor  : "<< actual->dato.autor << endl;
            cout<<"  Categoria  : "<< actual->dato.categoria << endl;
            cout<<"  Stock  : "<< actual->dato.stock << endl;
            cout<<"  Precio  : "<< actual->dato.precio << endl;
        }
        actual = actual->siguiente;
    }

    if(!encontrado){
        cout << "Libro no encontrado." <<endl;
    }

}

void mostrarInventario(){
    cout <<"\n-- INVENTARIO COMPLETO --" <<endl;

    bool hayLibros = false;

    for(int i=0; i< TAMANO_TABLA;i++){

        Nodo* actual = tablaHash[i];
        while (actual !=NULL){
            hayLibros = true;
            cout << "\n[Posicion " << i <<"]"<<endl;
            cout <<" Titulo   :  "<< actual->dato.titulo <<endl;
            cout <<" Autor   :  "<< actual->dato.autor <<endl;
            cout <<" Categoria   :  "<< actual->dato.categoria <<endl;
            cout <<" Stock   :  "<< actual->dato.stock <<endl;
            cout <<" Precio   :  "<< actual->dato.precio <<endl;
            actual = actual->siguiente;
        }
    }

    if(!hayLibros){
        cout<<"El inventario esta vacio."<<endl;
    }
}

void actualizarStock(){
    string tituloBuscado;
    bool encontrado = false;

    cout << "\n     ACTUALIZAR STOCK    "<<endl;
    cout<<"Ingrese el titulo del libro: ";
    cin.ignore();
    getline(cin, tituloBuscado);

    int posicion = funcionHash(tituloBuscado);

    Nodo* actual = tablaHash[posicion];
    while(actual != NULL && !encontrado){
        if(actual->dato.titulo == tituloBuscado){
            encontrado = true;

            int nuevoStock;
            cout<<"Stock actual: "<<actual->dato.stock <<endl;
            cout<<"Ingrese el nuevo stock: ";
            cin >> nuevoStock;

            if(nuevoStock<0){
                cout<<"El stock no puede ser negativo." <<endl;
            } else{
                actual->dato.stock = nuevoStock;

                if(actual->dato.stock == 0){
                    cout <<"Stock actualizado correctamente."<<endl;
                }
            }
        }
        actual = actual->siguiente;
    }

    if (!encontrado){
        cout << "Libro no encontrado."<<endl;
    }
}

void mostrarMenu(){
    cout << "       SISTEMA DE LIBRERIA     "<<endl;
    cout<<""<<endl;
    cout<<" 1. Registrar libro"<<endl;
    cout<<" 2. Buscar libro"<<endl;
    cout<<" 3. Mostrar inventario"<<endl;
    cout<<" 4. Actualizar stock"<<endl;
    cout<<" 5. Eliminar libro"<<endl;
    cout<<" 6. Registrar venta"<<endl;
    cout<<" 7. Ver ventas recientes"<<endl;
    cout<<" 8. Salir"<<endl;
    cout<<""<<endl;
    cout<<"Elige una opcion: "<<endl;
}
int main(){

    inicializarTabla();

    int opcion;

    do{
        mostrarMenu();
        cin>> opcion;

        switch (opcion){
            case 1: registrarLibro();   break;
            case 2: buscarLibro();  break;
            case 3: mostrarInventario();    break;
            case 4: actualizarStock();  break;
            case 8: cout <<"\nSaliendo del sistema..."<<endl;   break;
            default: cout<<"\nOpcion invalida."<<endl;  break;
        }
    }   while(opcion != 8);

    return 0;
}