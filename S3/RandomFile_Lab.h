#include <iostream>
#include <cassert>
#include <cstring>
#include <fstream>
#include <sstream>
#include <map>
#include "vector"
using namespace std;

struct Record {
    int codigo;
    char nombre[12];
    char apellido[12];
    int ciclo;

    Record() {
        memset(nombre, 0, sizeof(nombre));
        memset(apellido, 0, sizeof(apellido));
    }

    Record(int codigo, const string& nombre, const string& apellido, int ciclo) {
        this->codigo = codigo;
        strncpy(this->nombre, nombre.c_str(), sizeof(this->nombre) - 1);
        strncpy(this->apellido, apellido.c_str(), sizeof(this->apellido) - 1);
        this->ciclo = ciclo;
    }

    void showData() const {
        cout << "\nCodigo: " << codigo;
        cout << "\nNombre: " << nombre;
        cout << "\nApellido: " << apellido;
        cout << "\nCiclo : " << ciclo << endl;
    }

    bool operator==(const Record& other) const {
        return codigo == other.codigo && strcmp(nombre, other.nombre) == 0 && strcmp(apellido, other.apellido) == 0 && ciclo == other.ciclo;
    }
};

class RandomFile {
private:
    string fileName;
    string indexName;
    map<int, int> index;

public:
    RandomFile(string _fileName) : fileName(_fileName), indexName(_fileName + "_ind") {
        readIndex();
    }

    ~RandomFile() {
        writeIndex();
    }

    map<int, int> getIndex() const {
        return index;
    }

    void readIndex() {
        ifstream file(indexName);
        if (file.is_open()) {
            int key, pos;
            while (file >> key >> pos) {
                index[key] = pos;
            }
            file.close();
        }
    }

    void writeIndex() const {
        ofstream file(indexName);
        if (file.is_open()) {
            for (const auto& [key, pos] : index) {
                file << key << " " << pos << endl;
            }
            file.close();
        }
    }

    void write_record(const Record& record) {
        fstream dataFile(fileName, ios::binary | ios::in | ios::out | ios::ate);
        if (!dataFile.is_open()) {
            dataFile.open(fileName, ios::binary | ios::out);
            dataFile.close();
            dataFile.open(fileName, ios::binary | ios::in | ios::out | ios::ate);
        }

        int pos = dataFile.tellp();
        dataFile.write(reinterpret_cast<const char*>(&record), sizeof(record));
        dataFile.close();

        index[record.codigo] = pos;
        cout << "Registro escrito: Codigo=" << record.codigo << ", Pos=" << pos << endl;
    }

    Record* find(int key) {
        auto it = index.find(key);
        if (it != index.end()) {
            int pos = it->second;
            ifstream dataFile(fileName, ios::binary);
            if (dataFile.is_open()) {
                dataFile.seekg(pos);
                Record* result = new Record();
                dataFile.read(reinterpret_cast<char*>(result), sizeof(*result));
                dataFile.close();
                cout << "Lee la posición: " << pos << endl;
                cout << "Record read:\n";
                result->showData();
                return result;
            } else {
                cout << "Error" << endl;
                return nullptr;
            }
        }
        return nullptr;
    }

    void scanAll() const {
        ifstream dataFile(fileName, ios::binary);
        if (dataFile.is_open()) {
            dataFile.seekg(0, ios::end);
            int fileSize = dataFile.tellg();
            dataFile.seekg(0, ios::beg);
            while (dataFile.tellg() < fileSize) {
                Record result;
                dataFile.read(reinterpret_cast<char*>(&result), sizeof(result));
                result.showData();
            }
            dataFile.close();
        } else {
            cout << "Error" << endl;
        }
    }

    void scanAllByIndex() const {
        for (const auto& [key, pos] : index) {
            ifstream dataFile(fileName, ios::binary);
            if (dataFile.is_open()) {
                dataFile.seekg(pos);
                Record result;
                dataFile.read(reinterpret_cast<char*>(&result), sizeof(result));
                cout << "Registro leído desde índice:\n";
                result.showData();
                dataFile.close();
            } else {
                cout << "Error" << endl;
            }
        }
    }

    void loadFromCSV(const string& csvFile) {
        ifstream file(csvFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string token;
                vector<string> fields;

                while (getline(ss, token, ',')) {
                    fields.push_back(token);
                }

                if (fields.size() == 4) {
                    try {
                        int codigo = stoi(fields[0]);
                        string nombre = fields[1];
                        string apellido = fields[2];
                        int ciclo = stoi(fields[3]);

                        write_record(Record(codigo, nombre, apellido, ciclo));
                    } catch (const std::invalid_argument& e) {
                        cerr << "Error " << line << endl;

                    } catch (const std::out_of_range& e) {
                        cerr << "Error " << line << endl;
                    }
                }
            }
            file.close();
        } else {
            cout << "Error" << endl;
        }
    }

};

//--------------- testing ------------------//
void testLeerIndice(RandomFile &rf) {
    assert(!rf.getIndex().empty() && "El índice no debería estar vacío después de leer desde el archivo.");
    cout << "testLeerIndice pasado." << endl;
}

void testBuscarRegistro(RandomFile &rf, int codigo, const Record &expected) {
    Record* result = rf.find(codigo);
    if (result) {
        cout << "Registro leído:\n";
        result->showData();
        cout << "Registro esperado:\n";
        expected.showData();
        assert(*result == expected && "El registro no concuerda con el registrado en BD.");
        cout << "testBuscarRegistro para el codigo " << codigo << " pasado." << endl;
        delete result; // Liberar la memoria
    } else {
        cout << "No se encontró el registro con código " << codigo << endl;
    }
}

int main() {
    RandomFile rf("rf_data.dat");

    // Cargar registros desde el archivo CSV
    rf.loadFromCSV("datos.csv");

    testLeerIndice(rf);

    testBuscarRegistro(rf, 88127937, Record(88127937, "Lucía", "Santos", 7));
    testBuscarRegistro(rf, 66994658, Record(66994658, "Marta", "Sanz", 3));
    testBuscarRegistro(rf, 28018198, Record(28018198, "Paula", "Fernández", 7));
    testBuscarRegistro(rf, 50061878, Record(50061878, "Marcos", "Martín", 5));

    cout << "Todos los tests pasaron correctamente." << endl;

    rf.scanAll();
    rf.scanAllByIndex();

    // Manualmente escribir el archivo de índice
    rf.writeIndex();

    return 0;
}
