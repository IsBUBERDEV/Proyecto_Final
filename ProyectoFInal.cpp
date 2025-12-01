#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

struct Estudiante {
    char cedula[20];
    char nombre[100];
    char grado[20];
    double saldo;
    bool activo;
};
struct Producto {
    char codigo[20];
    char nombre[100];
    double precio;
    int stocck;
    bool activo;
};
struct Compra {
    char fecha[20];
    char nombreProducto[100];
    double valor;
    char cedulaEstudiante[20];
};

void mostrarMenu();
void registrarEstudiante();
void agregarProducto();
void realizarCompra();
void recargarSaldo();
void eliminarEstudiante();
void menuConsultas();
void consultarEstudiante();
void listarProductos();
void estudiantesSaldoBajo();
void generarArchivoRetiros();
bool buscarEstudiante(const char* cedula, Estudiante& est);
bool buscarProducto(const char* codigo, Producto& prod);
void actualizarEstudiante(const Estudiante& est);
void actualizarProducto(const Producto& prod);
void obtenerFechaActual(char* fecha);
int contarComprasEstudiante(const char* cedula);
void pausar();
void limpiarBuffer();

int main() {
    int opcion;

    cout << "========================================" << endl;
    cout << "   SISTEMA DE GESTION ESTUMERCADO" << endl;
    cout << "========================================" << endl;
    do {
        mostrarMenu();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();

        switch(opcion) {
            case 1:
                registrarEstudiante();
                break;
            case 2:
                agregarProducto();
                break;
            case 3:
                realizarCompra();
                break;
            case 4:
                recargarSaldo();
                break;
            case 5:
                eliminarEstudiante();
                break;
            case 6:
                menuConsultas();
                break;
            case 7:
                cout << "Saliendo del sistema. ¡Hasta luego!" << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                pausar();
        }
    } while(opcion != 9);
}
void mostrarMenu() {
    cout<<"\n========================================"<<endl;
    cout<<"            MENU PRINCIPAL"<<endl;
    cout<<"========================================"<<endl;
    cout<<"1. Registrar Estudiante"<<endl;
    cout<<"2. Agregar Producto al Inventario"<<endl;
    cout<<"3. Realizar Compra"<<endl;
    cout<<"4. Recargar Saldo Estudiante"<<endl;
    cout<<"5. Eliminar Estudiante"<<endl;
    cout<<"6. Consultas"<<endl;
    cout<<"7. Salir"<<endl;
    cout<<"========================================"<<endl;
}