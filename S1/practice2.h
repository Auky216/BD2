#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Alumno {
public:
    char Nombre[12];
    char Apellidos[12];

    // Constructor para inicializar
    Alumno(const char* nombre = "", const char* apellidos = "") {
        strncpy(Nombre, nombre, sizeof(Nombre) - 1);
        Nombre[sizeof(Nombre) - 1] = '\0';
        strncpy(Apellidos, apellidos, sizeof(Apellidos) - 1);
        Apellidos[sizeof(Apellidos) - 1] = '\0';
        // Rellenar el resto con espacios
        for (size_t i = strlen(Nombre); i < sizeof(Nombre); ++i) {
            Nombre[i] = ' ';
        }
        for (size_t i = strlen(Apellidos); i < sizeof(Apellidos); ++i) {
            Apellidos[i] = ' ';
        }
    }
};

// Sobrecarga del operador << para ostream
ostream& operator<<(ostream& stream, const Alumno& record) {
    stream.write(record.Nombre, sizeof(record.Nombre));
    stream.write(record.Apellidos, sizeof(record.Apellidos));
    return stream;
}

// Sobrecarga del operador >> para istream
istream& operator>>(istream& stream, Alumno& record) {
    stream.read(record.Nombre, sizeof(record.Nombre));
    stream.read(record.Apellidos, sizeof(record.Apellidos));
    return stream;
}

// Lee un registro binario desde el archivo en la posición dada
Alumno readRecordBin(int i) {
    ifstream inFile("archivo.dat", ios::binary);
    Alumno record;
    if (!inFile) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return record; // Devolvemos un objeto vacío en caso de error
    }
    inFile.seekg(i * sizeof(Alumno));
    inFile.read((char*)&record, sizeof(record));
    inFile.close();
    return record;
}

int main() {
    Alumno record1("Adrian", "Auqui");
    Alumno record2("Jose", "Barrenchea");

    // Escritura en archivo binario
    ofstream outFile("archivo.dat", ios::app | ios::binary);
    if (!outFile) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return 1;
    }
    outFile.write((char*)&record1, sizeof(record1));
    outFile.write((char*)&record2, sizeof(record2));
    outFile.close();

    // Lectura de un archivo binario
    ifstream inFile("archivo.dat", ios::binary);
    if (!inFile) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return 1;
    }
    Alumno record;
    inFile.read((char*)&record, sizeof(record));
    inFile.close();

    // Imprimir los datos con espaciado
    cout << "Nombre        Apellidos" << endl;
    cout << record.Nombre << " " << record.Apellidos << endl;

    // Leer un registro binario desde el archivo en la posición 0
    Alumno record3 = readRecordBin(1);
    cout << record3.Nombre << " " << record3.Apellidos << endl;

    return 0;
}
