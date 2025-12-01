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
void contarComprasEstudiante(const char* cedula);
void pausar();
void limpiarBuffer();

int main() {
    int opcion;
    system("cls");
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
                system("cls");
                registrarEstudiante();
                system("cls");
                break;
            case 2:
                system("cls");
                agregarProducto();
                system("cls");
                break;
            case 3:
                system("cls");
                realizarCompra();
                system("cls");
                break;
            case 4:
                system("cls");
                recargarSaldo();
                system("cls");
                break;
            case 5:
                system("cls");
                eliminarEstudiante();
                system("cls");
                break;
            case 6:
                system("cls");
                menuConsultas();
                system("cls");
                break;
            case 7:
                 system("cls");
                cout << "Saliendo del sistema." << endl;
                system("cls");
                break;
            default:
                system("cls");
                cout << "Opcion invalida. Intente de nuevo." << endl;
                pausar();
        }
    } while(opcion != 7);
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

    cout << "\nEstudiante Registrado" << endl;
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
    cout << "\nProducto Agregado" << endl;
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
    ofstream archivoCompras("compras.dat", ios::binary | ios::app);
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
                system("cls");
                consultarEstudiante();
                system("cls");                
                break;
            case 2:
                system("cls");
                listarProductos();
                system("cls");
                break;
            case 3:
                system("cls");
                estudiantesSaldoBajo();
                system("cls");
                break;
            case 4:
                system("cls");
                generarArchivoRetiros();
                system("cls");
                break;
            case 5:
                system("cls");
                cout << "Regresando al menu principal." << endl;
                system("cls");
                break;
            default:
                system("cls");
                cout << "Opcion invalida. Intente de nuevo." << endl;
                pausar();
        }
    } while(opcion != 5);
}
void consultarEstudiante() {
    cout << "\n--- CONSULTAR ESTUDIANTE ---" << endl;

    char cedula[20];
    Estudiante est;

    cout << "Cedula del estudiante: ";
    cin.getline(cedula, 20);

    if (!buscarEstudiante(cedula, est)) {
        cout << "\nEstudiante no encontrado." << endl;
        pausar();
        return;
    }
    cout << "Nombre: " << est.nombre << endl;
    cout << "Grado: " << est.grado << endl;
    cout << "Saldo: $" << fixed << setprecision(2) << est.saldo << endl;
    cout << "Estado: " << (est.activo ? "Activo" : "Eliminado") << endl;
    pausar();
}
void listarProductos() {
    cout << "\n--- LISTA DE PRODUCTOS ---" << endl;

    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) {
        cout << "No hay productos registrados." << endl;
        pausar();
        return;
    }
    Producto prod;
    int contador = 0;
    while (archivo.read(reinterpret_cast<char*>(&prod), sizeof(Producto))) {
        if (prod.activo) {
            contador++;
            cout << "\nProducto #" <<contador<<endl;
            cout << "Codigo: " << prod.codigo << endl;
            cout << "Nombre: " << prod.nombre << endl;
            cout << "Precio: $" << fixed << setprecision(2) << prod.precio << endl;
            cout << "Stock: " << prod.stock << " unidades" << endl;
        }
    }
    if (contador == 0) {
        cout << "No hay productos activos." << endl;
    }
    archivo.close();
    pausar();
}
void estudiantesSaldoBajo() {
    cout << "\n--- ESTUDIANTES CON SALDO MENOR A $5,000 ---" << endl;

    ifstream archivo("estudiantes.dat", ios::binary);
    if (!archivo) {
        cout << "No hay estudiantes registrados." << endl;
        pausar();
        return;}
        Estudiante est;
        int contador = 0;
        while(archivo.read(reinterpret_cast<char*>(&est),sizeof(Estudiante))){
            if(est.activo && est.saldo < 5000){
                contador++;
                cout << contador << ". " << est.nombre << " (" << est.cedula << ")" << endl;
                cout << "   Grado: " << est.grado << endl;
                cout << "   Saldo: $" << fixed << setprecision(2) << est.saldo << endl;
            }
        }
        if(contador == 0){
            cout << "No hay estudiantes con saldo menor a $5,000." << endl;
        }
        archivo.close();
        pausar();
}
void generarArchivoRetiros(){
    cout << "\n--- GENERAR ARCHIVO DE RETIROS ---" << endl;

    ifstream archivoLectura("estudiantes.dat", ios::binary);
    if (archivoLectura.fail()) {
        cout << "no hay estudiantes registrados."<<endl;
        pausar();
        return;
    }
    ofstream archivoRetiros("retiros.txt");
    if (archivoRetiros.fail()){
        cout << "Error al crear el archivo de retiros."<<endl;
        pausar();
        return;
    }
    archivoRetiros << "   ESTUDIANTES ELIMINADOS - ESTUMERCADO" << endl;

    Estudiante est;
    int contador = 0;
    while (archivoLectura.read(reinterpret_cast<char*>(&est), sizeof(Estudiante))) {
        if (!est.activo) {
            contador++;
            archivoRetiros << "Estudiante #" << contador << endl;
            archivoRetiros << "Cedula: " << est.cedula << endl;
            archivoRetiros << "Nombre: " << est.nombre << endl;
            archivoRetiros << "Grado: " << est.grado << endl;
            archivoRetiros << "Ultimo saldo: $" << fixed << setprecision(2) << est.saldo << endl;
            archivoRetiros << "----------------------------------------" << endl;
        }
    }
    archivoRetiros.close();
    if (contador == 0) {
    archivoRetiros << "No hay estudiantes eliminados." << endl;}
    cout << "\nArchivo Generado" << endl;
    cout << "Total de estudiantes eliminados: " << contador << endl;
    pausar();
}

//funciones utilitarias
bool buscarEstudiante(const char* cedula, Estudiante& est){
    ifstream archivo("estudiantes.dat",ios::binary);
    if(!archivo)return false;
    while(archivo.read(reinterpret_cast<char*>(&est),sizeof(Estudiante))){
        if(strcmp(est.cedula,cedula)==0 && est.activo){
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}
bool buscarProducto(const char* codigo, Producto& prod){
    ifstream archivo("productos.dat",ios::binary);
    if(!archivo)return false;
    while(archivo.read(reinterpret_cast<char*>(&prod),sizeof(Producto))){
        if(strcmp(prod.codigo,codigo)==0 && prod.activo){
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}
void actualizarEstudiante(const Estudiante& est){
    fstream archivo("estudiantes.dat",ios::binary | ios::in | ios::out);
    if(!archivo)return;
    Estudiante temp;
    while(archivo.read(reinterpret_cast<char*>(&temp),sizeof(Estudiante))){
        if(strcmp(temp.cedula,est.cedula)==0){
            archivo.seekp(-static_cast<int>(sizeof(Estudiante)),ios::cur);
            archivo.write(reinterpret_cast<const char*>(&est),sizeof(Estudiante));
            break;
        }
    }
    archivo.close();


}
void contarComprasEstudiante(const char* cedula){
    int contador = 0;
    ifstream archivo("compras.dat",ios::binary);
    if(!archivo)return;
    Compra comp;
    while(archivo.read(reinterpret_cast<char*>(&comp),sizeof(Compra))){
        if(strcmp(comp.cedulaEstudiante,cedula)==0){
            contador++;
        }
    }
}
void actualizarProducto(const Producto& prod){
    fstream archivo("productos.dat",ios::binary | ios::in | ios::out);
    if(!archivo)return;
    Producto temp;
    while(archivo.read(reinterpret_cast<char*>(&temp),sizeof(Producto))){
        if(strcmp(temp.codigo,prod.codigo)==0){
            archivo.seekp(-static_cast<int>(sizeof(Producto)),ios::cur);
            archivo.write(reinterpret_cast<const char*>(&prod),sizeof(Producto));
            break;
        }
    }
    archivo.close();
}
void obtenerFechaActual(char* fecha) {
    time_t ahora = time(0);
    tm* tiempoLocal = localtime(&ahora);
    
    sprintf(fecha, "%02d/%02d/%04d", 
            tiempoLocal->tm_mday,
            tiempoLocal->tm_mon + 1,
            tiempoLocal->tm_year + 1900);
}
void pausar(){
    cout<<"\nPresione ENTER para continuar...";
    cin.get();
}
void limpiarBuffer() {
    cin.ignore(10000, '\n');
}