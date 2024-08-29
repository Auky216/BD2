#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Alumno
{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[20];
    int ciclo;
    float mensualidad;
    int next;
};

ostream &operator<<(ostream &stream, const Alumno &record)
{
    stream.write(record.codigo, 5);
    stream.write(record.nombre, 11);
    stream.write(record.apellidos, 20);
    stream.write(record.carrera, 20);
    stream.write(reinterpret_cast<const char*>(&record.ciclo), sizeof(int));
    stream.write(reinterpret_cast<const char*>(&record.mensualidad), sizeof(float));
    stream.write(reinterpret_cast<const char*>(&record.next), sizeof(int));
    return stream;
}

istream &operator>>(istream &stream, Alumno &record)
{
    stream.read(record.codigo, 5);
    stream.read(record.nombre, 11);
    stream.read(record.apellidos, 20);
    stream.read(record.carrera, 20);
    stream.read(reinterpret_cast<char*>(&record.ciclo), sizeof(int));
    stream.read(reinterpret_cast<char*>(&record.mensualidad), sizeof(float));
    stream.read(reinterpret_cast<char*>(&record.next), sizeof(int));
    return stream;
}

class FixedRecord
{
private:
    string filename;

public:
    FixedRecord(const string& filename) : filename(filename)
    {
        vector<Alumno> alumnos = this->load();

        ofstream file(this->filename, ios::out | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        int head = -1;
        file.write(reinterpret_cast<const char*>(&head), sizeof(int));

        for (const auto& alumno : alumnos)
        {
            file << alumno;
        }

        file.close();
    }

    static void writeMockData(const string& filename = "data.dat")
    {
        Alumno alumnos[] = {
                {"0001", "Juan", "Perez", "CS", 1, 1.11, -2},
                {"0002", "Jose", "Gomez", "DS", 2, 2.22, -2},
                {"0003", "Diego", "Lopez", "CS", 3, 3.33, -2},
                {"0004", "Antonio", "Martinez", "IN", 4, 4.44, -2},
                {"0005", "Luis", "Sanchez", "SI", 5, 5.58, -2}
        };

        ofstream file(filename, ios::out | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        int head = -1;
        file.write(reinterpret_cast<const char*>(&head), sizeof(int));

        for (const auto& alumno : alumnos)
        {
            file << alumno;
        }
        file.close();
    }

    void printData()
    {
        ifstream file(this->filename, ios::in | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        int head;
        file.read(reinterpret_cast<char*>(&head), sizeof(int));
        cout << "Head: " << head << endl;

        Alumno alumno;
        int pos = 0;
        while (file.read(reinterpret_cast<char*>(&alumno), sizeof(Alumno)))
        {
            cout << "Pos: " << pos << " "
                 << string(alumno.codigo, 5) << " "
                 << string(alumno.nombre, 11) << " "
                 << string(alumno.apellidos, 20) << " "
                 << string(alumno.carrera, 20) << " "
                 << alumno.ciclo << " "
                 << alumno.mensualidad << " "
                 << alumno.next << endl;
            pos++;
        }

        file.close();
    }

    int countRecords()
    {
        ifstream file(this->filename, ios::in | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        int count = 0;
        int head;
        file.read(reinterpret_cast<char*>(&head), sizeof(int));

        Alumno record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Alumno)))
        {
            count++;
        }

        file.close();
        return count;
    }

    vector<Alumno> load()
    {
        ifstream file(this->filename, ios::in | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        vector<Alumno> alumnos;
        int head;
        file.read(reinterpret_cast<char*>(&head), sizeof(int));

        Alumno record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Alumno)))
        {
            alumnos.push_back(record);
        }

        file.close();
        return alumnos;
    }

    bool delete_pos(int pos)
    {
        fstream file(this->filename, ios::in | ios::out | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        int head;
        file.read(reinterpret_cast<char*>(&head), sizeof(int));

        file.seekp(pos * sizeof(Alumno) + sizeof(int), ios::beg);

        Alumno record;
        file.read(reinterpret_cast<char*>(&record), sizeof(Alumno));

        record.next = head;

        file.seekp(pos * sizeof(Alumno) + sizeof(int), ios::beg);
        file.write(reinterpret_cast<const char*>(&record), sizeof(Alumno));

        head = pos;
        file.seekp(0, ios::beg);
        file.write(reinterpret_cast<const char*>(&head), sizeof(int));

        file.close();

        return true;
    }

    void add(const Alumno& record)
    {
        fstream file(this->filename, ios::in | ios::out | ios::binary);
        if (!file.is_open())
        {
            cerr << "Error al abrir el archivo" << endl;
            exit(1);
        }

        int head;
        file.read(reinterpret_cast<char*>(&head), sizeof(int));

        if (head == -1)
        {
            file.seekp(0, ios::end);
            Alumno newRecord = record;
            newRecord.next = -2;
            file.write(reinterpret_cast<const char*>(&newRecord), sizeof(Alumno));
        }
        else
        {
            file.seekp(head * sizeof(Alumno) + sizeof(int), ios::beg);

            Alumno tempRecord;
            file.read(reinterpret_cast<char*>(&tempRecord), sizeof(Alumno));
            int tempNext = tempRecord.next;

            Alumno newRecord = record;
            newRecord.next = -2;

            file.seekp(head * sizeof(Alumno) + sizeof(int), ios::beg);
            file.write(reinterpret_cast<const char*>(&newRecord), sizeof(Alumno));

            head = head;
            file.seekp(0, ios::beg);
            file.write(reinterpret_cast<const char*>(&tempNext), sizeof(int));
        }

        file.close();
    }
};


int main()
{

    cout << "========================================" << endl;
    cout << "||              FREE LIST             ||" << endl;
    cout << "========================================" << endl;

    FixedRecord::writeMockData();
    FixedRecord fixedRecord("data.dat");

    cout << "-> Datos iniciales:" << endl;

    fixedRecord.printData();

    cout << "-> Eliminando 2 (2)" << endl;
    fixedRecord.delete_pos(2);
    fixedRecord.printData();

    cout << "-> Eliminando 3 (3->2)" << endl;
    fixedRecord.delete_pos(3);
    fixedRecord.printData();

    cout << "-> Eliminando 4 (4->3->2)" << endl;

    fixedRecord.delete_pos(4);

    fixedRecord.printData();


    cout << "-> Total records: " << fixedRecord.countRecords() << endl;

    cout << "-> Agregando nuevos registros:" << endl;

    Alumno alumno1 = {"0006", "Adrian", "Ramirez", "IN", 6, 6.66, -2};
    Alumno alumno2 = {"0007", "Pedro", "Fernandez", "CS", 7, 7.77, -2};
    Alumno alumno3 = {"0008", "Ana", "Morales", "DS", 8, 8.88, -2};
    Alumno alumno4 = {"0009", "Maria", "Hernandez", "AR", 9, 9.99, -2};

    cout << "-> Alumno1 (3->2)" << endl;

    fixedRecord.add(alumno1);
    fixedRecord.printData();
    cout << "-> Alumno2 (2)" << endl;

    fixedRecord.add(alumno2);
    fixedRecord.printData();
    cout << "-> Alumno3 ()" << endl;

    fixedRecord.add(alumno3);
    fixedRecord.printData();
    cout << "-> Alumno4 ()" << endl;

    fixedRecord.add(alumno4);

    fixedRecord.printData();
    cout << "Total records: " << fixedRecord.countRecords() << endl;

    cout << "-> Eliminando 1 (1)" << endl;
    fixedRecord.delete_pos(1);
    fixedRecord.printData();
    cout << "-> Eliminando 4 (4->1)" << endl;
    fixedRecord.delete_pos(4);
    fixedRecord.printData();

    return 0;
}