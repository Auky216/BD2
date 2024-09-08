#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

struct Record
{
    int codigo;
    char nombre[12];
    char apellido[12];
    int ciclo;
    // Agregar campos extra si los necesitan

    void showData() {
        cout << "\nCodigo: " << codigo;
        cout << "\nNombre: " << nombre;
        cout << "\nApellido: " << apellido;
        cout << "\nCiclo : " << ciclo;
    }

    bool operator==(const Record& other) const {
        return codigo == other.codigo && strcmp(nombre, other.nombre) == 0 && strcmp(apellido, other.apellido) == 0 && ciclo == other.ciclo;
    }
};

template <typename TK>
class AVLFile {
private :
    string filename ;
    long pos_root ;

public :
    AVLFile ( string filename ) ;
    Record find (TK key) ;
    void insert(Record record);
    vector<Record> inorder() ;
    bool remove(TK key);
    //agregue todas las funciones necesarias
};


vector<Record> leerCSV(const string& filename) {
    ifstream file(filename);
    vector<Record> records;
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo.\n";
        return records;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Record record;

        getline(ss, token, ',');
        record.codigo = stoi(token);

        getline(ss, token, ',');
        strncpy(record.nombre, token.c_str(), sizeof(record.nombre));
        record.nombre[sizeof(record.nombre) - 1] = '\0';  // Asegurar el null-termination

        getline(ss, token, ',');
        strncpy(record.apellido, token.c_str(), sizeof(record.apellido));
        record.apellido[sizeof(record.apellido) - 1] = '\0';  // Asegurar el null-termination

        getline(ss, token, ',');
        record.ciclo = stoi(token);

        records.push_back(record);
    }

    file.close();
    return records;
}

void readFile(string filename){
    AVLFile<int> file(filename);
    cout<<"------------------------------------------\n";
    vector<Record> records = leerCSV(filename);
    bool passed = true;
    for (auto& record : records) {
        Record r = file.find(record.codigo);
        if (!(r == record)){
            passed = false;
            cout << "Error en el record con codigo: " << record.codigo << "\n";
            cout << "Se esperaba: \n";
            record.showData();
            cout << "\nSe obtuvo: \n";
            r.showData();
            cout << "\n";
            break;
        }
    }
    if (passed)
        cout << "Todos los records fueron leidos correctamente\n";
}


int main() {
    cout<<"hola";
    return 0;
}
