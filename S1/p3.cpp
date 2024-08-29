#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Matricula
{
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;

    int size()
    {
        return sizeof(int) + codigo.size() + sizeof(int) + sizeof(float) + sizeof(int) + observaciones.size();
    }

    void writeToFile(ofstream &stream)
    {
        int size;
        size = codigo.size();
        stream.write((char *)&size, sizeof(int));
        stream.write(codigo.c_str(), size);

        stream.write((char *)&ciclo, sizeof(int));
        stream.write((char *)&mensualidad, sizeof(float));

        size = observaciones.size();
        stream.write((char *)&size, sizeof(int));
        stream.write(observaciones.c_str(), size);
    }

    void readFromFile(ifstream &stream)
    {
        int size;
        stream.read((char *)&size, sizeof(int));
        char *buffer = new char[size];
        stream.read(buffer, size);
        codigo = string(buffer, size);
        delete[] buffer;

        stream.read((char *)&ciclo, sizeof(int));
        stream.read((char *)&mensualidad, sizeof(float));

        stream.read((char *)&size, sizeof(int));
        buffer = new char[size];
        stream.read(buffer, size);
        observaciones = string(buffer, size);
        delete[] buffer;
    }
};

class NumberRecord
{
private:
    string filename;

public:
    NumberRecord(string filename)
    {
        this->filename = filename;
    }

    vector<Matricula> loadRecords()
    {
        vector<Matricula> records;
        ifstream file(this->filename, ios::in | ios::binary);
        ifstream metadata("data_p3_metadata.dat", ios::in | ios::binary);
        if (!file.is_open() || !metadata.is_open())
        {
            cerr << "Error opening file" << endl;
            return records;
        }

        int start, end;
        while (true)
        {
            metadata.read((char *)&start, sizeof(start));
            metadata.read((char *)&end, sizeof(end));
            if (metadata.eof())
                break;

            file.seekg(start, ios::beg);
            Matricula record;
            record.readFromFile(file);
            records.push_back(record);
        }
        return records;
    }

    void addRecord(Matricula record)
    {
        ofstream file(this->filename, ios::out | ios::app | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error opening file" << endl;
            return;
        }
        record.writeToFile(file);
        file.close();

        fstream metadata("data_p3_metadata.dat", ios::in | ios::out | ios::app | ios::binary);
        if (!metadata.is_open())
        {
            cerr << "Error opening metadata file" << endl;
            return;
        }

        int pos = 0;
        metadata.seekg(-sizeof(int), ios::end);
        metadata.read((char *)&pos, sizeof(pos));
        metadata.seekp(0, ios::end);
        metadata.write((char *)&pos, sizeof(pos));
        pos += record.size();
        metadata.write((char *)&pos, sizeof(pos));

        metadata.close();
    }

    Matricula readRecord(int pos)
    {
        ifstream file(this->filename, ios::in | ios::binary);
        ifstream metadata("data_p3_metadata.dat", ios::in | ios::binary);
        if (!file.is_open() || !metadata.is_open())
        {
            cerr << "Error opening file" << endl;
            return Matricula();
        }

        int start, end;
        metadata.seekg(pos * 2 * sizeof(int), ios::beg);
        metadata.read((char *)&start, sizeof(start));
        metadata.read((char *)&end, sizeof(end));

        file.seekg(start, ios::beg);
        Matricula record;
        record.readFromFile(file);
        return record;
    }
};

int main()
{
    // Cambiar la inicialización para leer los datos de los archivos existentes
    NumberRecord record("data_p3_data.dat");

    // Cargar los registros desde el archivo
    vector<Matricula> records = record.loadRecords();

    cout << "------------------------------------" << endl;
    cout << "Cargando datos iniciales" << endl;
    cout << "------------------------------------" << endl << endl;

    for (int i = 0; i < records.size(); i++)
    {
        cout << i + 1 << ": " << records[i].codigo << " " << records[i].ciclo << " "
             << records[i].mensualidad << " " << records[i].observaciones << endl;
    }

    Matricula newRecord = {"010", 10, 1500.00, "Al dia"};
    record.addRecord(newRecord);

    records = record.loadRecords();

    cout << "------------------------------------" << endl;
    cout << "Cargando datos después de agregar" << endl;
    cout << "------------------------------------" << endl;

    for (int i = 0; i < records.size(); i++)
    {
        cout << i + 1 << ": " << records[i].codigo << " " << records[i].ciclo << " "
             << records[i].mensualidad << " " << records[i].observaciones << endl;
    }

    // Leer un registro específico
    Matricula record2 = record.readRecord(2);
    cout << "------------------------------------" << endl;
    cout << "Leyendo registro en la posición 2 (partiendo de 0)" << endl;
    cout << "------------------------------------" << endl;
    cout << record2.codigo << " " << record2.ciclo << " " << record2.mensualidad << " "
         << record2.observaciones << endl;

    return 0;
}
