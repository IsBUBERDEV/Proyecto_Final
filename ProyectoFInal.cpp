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
    int stock;
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
                cout << "Saliendo del sistema." << endl;
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
void registrarEstudiante() {
    cout << "\n--- REGISTRAR ESTUDIANTE ---" << endl;

    Estudiante nuevoEst;
    Estudiante temp;

    cout << "Cedula: ";
    cin.getline(nuevoEst.cedula, 20);

    // Validar que no exista
    if (buscarEstudiante(nuevoEst.cedula, temp)) {
        cout << "\nError: Ya existe un estudiante con esta cedula." << endl;
        pausar();
        return;
    }

    cout << "Nombre completo: ";
    cin.getline(nuevoEst.nombre, 100);

    cout << "Grado: ";
    cin.getline(nuevoEst.grado, 20);

    cout << "Saldo inicial (minimo $5,000): $";
    cin >> nuevoEst.saldo;
    limpiarBuffer();

    if (nuevoEst.saldo < 5000) {
        cout << "\nError: El saldo inicial debe ser minimo $5,000." << endl;
        pausar();
        return;
    }

    nuevoEst.activo = true;

    // Guardar en archivo
    ofstream archivo("estudiantes.dat", ios::binary | ios::app);
    if (!archivo) {
        cout << "\nError al abrir el archivo de estudiantes." << endl;
        pausar();
        return;
    }

    archivo.write(reinterpret_cast<char*>(&nuevoEst), sizeof(Estudiante));
    archivo.close();

    cout << "\n¡Estudiante registrado exitosamente!" << endl;
    pausar();
}
void agregarProducto() {
    cout << "\n--- AGREGAR PRODUCTO AL INVENTARIO ---" << endl;
    Producto nuevoProd;
    Producto temp;
    cout << "Codigo del producto: ";
    cin.getline(nuevoProd.codigo, 20);
    // Validar que no exista
    if (buscarProducto(nuevoProd.codigo, temp)) {
        cout << "\nError: Ya existe un producto con este codigo." << endl;
        pausar();
        return;
    }
    cout << "Nombre del producto: ";
    cin.getline(nuevoProd.nombre, 100);
    cout << "Precio: $";
    cin >> nuevoProd.precio;
    limpiarBuffer();
    cout << "Cantidad en stock: ";
    cin >> nuevoProd.stock;
    limpiarBuffer();
    if (nuevoProd.precio <= 0 || nuevoProd.stock < 0) {
        cout << "\nError: Precio y stock deben ser valores validos." << endl;
        pausar();
        return;
    }
    nuevoProd.activo = true;
    // Guardar en archivo
    ofstream archivo("productos.dat", ios::binary | ios::app);
    if (!archivo) {
        cout << "\nError al abrir el archivo de productos." << endl;
        pausar();
        return;
}
    archivo.write(reinterpret_cast<char*>(&nuevoProd), sizeof(Producto));
    archivo.close();
    cout << "\n¡Producto agregado exitosamente!" << endl;
    pausar();
}
void realizarCompra() {
    cout << "\n--- REALIZAR COMPRA ---" << endl;
    char cedula[20];
    char codigo[20];
    Estudiante est;
    Producto prod;
    cout << "Cedula del estudiante: ";
    cin.getline(cedula, 20);
    if(!buscarEstudiante(cedula, est)) {
        cout << "\nError: Estudiante no encontrado." << endl;
        pausar();
        return;}
    cout << "Codigo del producto: ";
    cin.getline(codigo, 20);
    if(!buscarProducto(codigo, prod)) {
        cout << "\nError: Producto no encontrado." << endl;
        pausar();
        return;
    }
    if(prod.stock <= 0) {
        cout << "\nError: Producto sin stock disponible." << endl;
        pausar();
        return;
    }
    if(est.saldo < prod.precio) {
        cout << "\nError: Saldo insuficiente." << endl;
        cout << "Saldo actual: $" << fixed << setprecision(2) << est.saldo << endl;
        cout << "Precio del producto: $" << prod.precio << endl;
        pausar();
        return;
    }
    // Registrar compra
    Compra nuevaCompra;
    obtenerFechaActual(nuevaCompra.fecha);
    strcpy(nuevaCompra.nombreProducto, prod.nombre);
    nuevaCompra.valor = prod.precio;
    strcpy(nuevaCompra.cedulaEstudiante, cedula);
    ofstream archivoCompras("compras.dat", ios::binary / ios::app);
    if(!archivoCompras) {
        cout << "\nError al registrar la compra." << endl;
        pausar();
        return;
    }
    archivoCompras.write(reinterpret_cast<char*>(&nuevaCompra), sizeof(Compra));
    archivoCompras.close();
    // Actualizar saldo y stock
    est.saldo -= prod.precio;
    prod.stock--;
    actualizarEstudiante(est);
    actualizarProducto(prod);
    cout << "\nCompra Realizada" << endl;
    cout << "Producto: " << prod.nombre << endl;
    cout << "Valor: $" << fixed << setprecision(2) << prod.precio << endl;
    cout << "Saldo restante: $" << est.saldo << endl;
    pausar();

}
void recargarSaldo() {
    cout << "\n--- RECARGAR SALDO ---" << endl;

    char cedula[20];
    double recarga;
    Estudiante est;
    cout << "Cedula del estudiante: ";
    cin.getline(cedula, 20);
    if(!buscarEstudiante(cedula, est)) {
        cout << "\nError: Estudiante no encontrado." << endl;
        pausar();
        return;
    }
    cout << "Monto a recargar: $";
    cin >> recarga;
    limpiarBuffer();
    if(recarga <= 0 || recarga > 500000) {
        cout << "\nError: La recarga debe estar entre $1 y $500,000." << endl;
        pausar();
        return;
    }
    est.saldo += recarga;
    actualizarEstudiante(est);
    cout << "\nRecarga Hecha" << endl;
    cout << "Nuevo saldo: $" << fixed << setprecision(2) << est.saldo << endl;
    pausar();
}
void eliminarEstudiante() {
    cout << "\n--- ELIMINAR ESTUDIANTE ---" << endl;
    char cedula[20];
    Estudiante est;
    cout << "Cedula del estudiante a eliminar: ";
    cin.getline(cedula, 20);
    if(!buscarEstudiante(cedula, est)) {
        cout << "\nError: Estudiante no encontrado." << endl;
        pausar();
        return;
    }
    est.activo = false;
    actualizarEstudiante(est);
    cout << "\nEstudiante eliminado" << endl;
    pausar();
}
void menuConsultas() {
    int opcion;
    do {
        cout << "\n--- MENU DE CONSULTAS ---" << endl;
        cout << "1. Consultar Estudiante" << endl;
        cout << "2. Listar Productos" << endl;
        cout << "3. Estudiantes con Saldo Bajo" << endl;
        cout << "4. Generar Archivo de Retiros" << endl;
        cout << "5. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1:
                consultarEstudiante();
                break;
            case 2:
                listarProductos();
                break;
            case 3:
                estudiantesSaldoBajo();
                break;
            case 4:
                generarArchivoRetiros();
                break;
            case 5:
                cout << "Regresando al menu principal." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                pausar();
        }
    } while(opcion != 5);
}