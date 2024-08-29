#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Matricula {
    string codigo;
    int ciclo;
    float mensualidad;
    string observacion;
};

class VariableRecord {
private:
    string archivoDatos;    // Archivo binario con los registros
    string archivoMetadata; // Archivo de metadatos con las posiciones

public:
    VariableRecord(const string& datos, const string& metadata)
            : archivoDatos(datos), archivoMetadata(metadata) {}

    vector<Matricula> load() {
        vector<Matricula> matriculas;
        ifstream metaFile(archivoMetadata, ios::binary);
        ifstream dataFile(archivoDatos, ios::binary);

        if (!metaFile.is_open() || !dataFile.is_open()) {
            cout << "Cannot open the file" << endl;
            return matriculas;
        }

        vector<long> positions;
        long pos;
        while (metaFile.read(reinterpret_cast<char*>(&pos), sizeof(long))) {
            positions.push_back(pos);
        }

        cout << "Loaded positions from metadata:" << endl;
        for (long position : positions) {
            cout << "Position: " << position << endl;
        }

        for (long position : positions) {
            dataFile.seekg(position);
            Matricula record;

            short size;

            if (!dataFile.read(reinterpret_cast<char*>(&size), sizeof(short))) {
                cout << "Error reading size for codigo" << endl;
                break;
            }
            string buffer(size, '\0');
            dataFile.read(&buffer[0], size);
            record.codigo = buffer;

            if (!dataFile.read(reinterpret_cast<char*>(&record.ciclo), sizeof(int))) {
                cout << "Error reading ciclo" << endl;
                break;
            }

            if (!dataFile.read(reinterpret_cast<char*>(&record.mensualidad), sizeof(float))) {
                cout << "Error reading mensualidad" << endl;
                break;
            }

            if (!dataFile.read(reinterpret_cast<char*>(&size), sizeof(short))) {
                cout << "Error reading size for observacion" << endl;
                break;
            }
            buffer.resize(size);
            dataFile.read(&buffer[0], size);
            record.observacion = buffer;

            matriculas.push_back(record);
        }

        metaFile.close();
        dataFile.close();
        return matriculas;
    }

    void add(const Matricula& record) {
        ofstream dataFile(archivoDatos, ios::app | ios::binary);

        if (!dataFile.is_open()) {
            cout << "Cannot open the data file" << endl;
            return;
        }

        // Position before writing the record
        long positionBefore = dataFile.tellp();

        // Write record to data file first
        short size = record.codigo.size();
        dataFile.write(reinterpret_cast<const char*>(&size), sizeof(short));
        dataFile.write(record.codigo.c_str(), size);

        dataFile.write(reinterpret_cast<const char*>(&record.ciclo), sizeof(int));
        dataFile.write(reinterpret_cast<const char*>(&record.mensualidad), sizeof(float));

        size = record.observacion.size();
        dataFile.write(reinterpret_cast<const char*>(&size), sizeof(short));
        dataFile.write(record.observacion.c_str(), size);

        // Position after writing the record
        long positionAfter = dataFile.tellp();

        // Position of the new record
        long position = positionAfter - positionBefore;

        dataFile.close();

        ofstream metaFile(archivoMetadata, ios::app | ios::binary);

        if (!metaFile.is_open()) {
            cout << "Cannot open the metadata file" << endl;
            return;
        }

        // Write position to metadata file after the record has been written
        metaFile.write(reinterpret_cast<const char*>(&position), sizeof(long));

        metaFile.close();
    }

    Matricula readRecord(int pos) {
        ifstream metaFile(archivoMetadata, ios::binary);
        ifstream dataFile(archivoDatos, ios::binary);

        if (!metaFile.is_open() || !dataFile.is_open()) {
            cout << "Cannot open the file" << endl;
            return Matricula();
        }

        vector<long> positions;
        long filePos;
        while (metaFile.read(reinterpret_cast<char*>(&filePos), sizeof(long))) {
            positions.push_back(filePos);
        }

        if (pos < 0 || pos >= positions.size()) {
            cout << "Position out of bounds" << endl;
            return Matricula();
        }

        dataFile.seekg(positions[pos]);

        Matricula record;
        short size;

        if (!dataFile.read(reinterpret_cast<char*>(&size), sizeof(short))) {
            cout << "Error reading size for codigo" << endl;
            return Matricula();
        }
        string buffer(size, '\0');
        dataFile.read(&buffer[0], size);
        record.codigo = buffer;

        if (!dataFile.read(reinterpret_cast<char*>(&record.ciclo), sizeof(int))) {
            cout << "Error reading ciclo" << endl;
            return Matricula();
        }

        if (!dataFile.read(reinterpret_cast<char*>(&record.mensualidad), sizeof(float))) {
            cout << "Error reading mensualidad" << endl;
            return Matricula();
        }

        if (!dataFile.read(reinterpret_cast<char*>(&size), sizeof(short))) {
            cout << "Error reading size for observacion" << endl;
            return Matricula();
        }
        buffer.resize(size);
        dataFile.read(&buffer[0], size);
        record.observacion = buffer;

        metaFile.close();
        dataFile.close();
        return record;
    }
};

int main() {
    remove("matriculas.bin");
    remove("metadata.bin");

    VariableRecord vr("matriculas.bin", "metadata.bin");

    Matricula m1 = {"123", 1, 1500.50, "Sin observaciones"};
    Matricula m2 = {"456", 2, 1700.75, "Requiere revisión"};
    vr.add(m1);
    vr.add(m2);

    vector<Matricula> matriculas = vr.load();
    for (const Matricula& m : matriculas) {
        cout << "Codigo: " << m.codigo << endl;
        cout << "Ciclo: " << m.ciclo << endl;
        cout << "Mensualidad: " << m.mensualidad << endl;
        cout << "Observacion: " << m.observacion << endl;
        cout << "-----------------------------" << endl;
    }

    Matricula m = vr.readRecord(1);
    cout << "Registro en posición 1:" << endl;
    cout << "Codigo: " << m.codigo << endl;
    cout << "Ciclo: " << m.ciclo << endl;
    cout << "Mensualidad: " << m.mensualidad << endl;
    cout << "Observacion: " << m.observacion << endl;

    return 0;
}