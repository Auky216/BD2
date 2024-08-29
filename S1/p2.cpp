#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Alumno {
    string Nombre;
    string Apellido;
    string Carrera;
    float mensualidad;
};

class VariableRecord {
private:
    string nombreArchivo;

public:
    VariableRecord(string _nombreArchivo) {
        this->nombreArchivo = _nombreArchivo;
    }

    vector<Alumno> load() {
        ifstream file(this->nombreArchivo, ios::binary);
        vector<Alumno> alumnos;

        if (!file.is_open()) {
            cout << "Cannot open the file" << endl;
            return alumnos;
        }

        while (true) {
            Alumno record;
            short size;

            if (!file.read((char*) &size, sizeof(short))) break;
            char* buffer = new char[size + 1];
            file.read(buffer, size);
            buffer[size] = '\0';
            record.Nombre = buffer;
            delete[] buffer;

            if (!file.read((char*) &size, sizeof(short))) break;
            buffer = new char[size + 1];
            file.read(buffer, size);
            buffer[size] = '\0';
            record.Apellido = buffer;
            delete[] buffer;

            if (!file.read((char*) &size, sizeof(short))) break;
            buffer = new char[size + 1];
            file.read(buffer, size);
            buffer[size] = '\0';
            record.Carrera = buffer;
            delete[] buffer;

            file.read((char*) &record.mensualidad, sizeof(float));

            if (file.eof()) break;

            alumnos.push_back(record);
        }

        file.close();
        return alumnos;
    }

    void add(Alumno record) {
        ofstream file(this->nombreArchivo, ios::app | ios::binary);

        if (!file.is_open()) {
            cout << "Cannot open the file" << endl;
            return;
        }

        short size = record.Nombre.size();
        file.write((char*) &size, sizeof(short));
        file.write(record.Nombre.c_str(), size);

        size = record.Apellido.size();
        file.write((char*) &size, sizeof(short));
        file.write(record.Apellido.c_str(), size);

        size = record.Carrera.size();
        file.write((char*) &size, sizeof(short));
        file.write(record.Carrera.c_str(), size);

        file.write((char*) &record.mensualidad, sizeof(float));

        file.close();
    }

    Alumno readRecord(int pos){
        ifstream file(this->nombreArchivo, ios::binary);
        if(!file.is_open()){
            cout<<"Cannot open the file"<<endl;
            return Alumno();
        }

        Alumno record;
        short size;
        int count = 0;

        while (count <= pos) {
            if (!file.read((char*) &size, sizeof(short))) break;
            char* buffer = new char[size + 1];
            file.read(buffer, size);
            buffer[size] = '\0';
            record.Nombre = buffer;
            delete[] buffer;

            if (!file.read((char*) &size, sizeof(short))) break;
            buffer = new char[size + 1];
            file.read(buffer, size);
            buffer[size] = '\0';
            record.Apellido = buffer;
            delete[] buffer;

            if (!file.read((char*) &size, sizeof(short))) break;
            buffer = new char[size + 1];
            file.read(buffer, size);
            buffer[size] = '\0';
            record.Carrera = buffer;
            delete[] buffer;

            file.read((char*) &record.mensualidad, sizeof(float));

            count++;
        }

        file.close();
        return record;
    }
};

void getTxt(string namefile) {
    ifstream file(namefile);
    if (!file.is_open()) {
        cout << "Cannot open the file" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

vector<Alumno> getAlumnosTXT(const string& namefile) {
    ifstream file(namefile);
    vector<Alumno> alumnos;
    string line;

    if (!file.is_open()) {
        cout << "Cannot open the file" << endl;
        return alumnos;
    }

    getline(file, line);

    while (getline(file, line)) {
        Alumno alumno;
        size_t pos = 0;

        size_t nextPos = line.find('|', pos);
        alumno.Nombre = line.substr(pos, nextPos - pos);

        pos = nextPos + 1;
        nextPos = line.find('|', pos);
        alumno.Apellido = line.substr(pos, nextPos - pos);

        pos = nextPos + 1;
        nextPos = line.find('|', pos);
        alumno.Carrera = line.substr(pos, nextPos - pos);

        pos = nextPos + 1;
        alumno.mensualidad = stof(line.substr(pos));

        alumnos.push_back(alumno);
    }

    file.close();
    return alumnos;
}

int main() {
    VariableRecord file("alumnos.bin");
    vector<Alumno> alumnos = getAlumnosTXT("data_p2.txt");

    for (const Alumno& alumno : alumnos) {
        file.add(alumno);
    }

    vector<Alumno> alumnos2 = file.load();

    for (const Alumno& alumno : alumnos2) {
        cout<<"---------------------------------------------"<<endl;
        cout << endl;
        cout << "Nombre: "<<alumno.Nombre <<endl;
        cout << "Apellido: " << alumno.Apellido << endl;
        cout << "Carrera: " << alumno.Carrera << endl;
        cout << "Mensualidad: : " << alumno.mensualidad << endl;
        cout << endl;
        cout<<"---------------------------------------------"<<endl;
    }

    cout<<"Cout the record in position 5"<<endl;
    Alumno record = file.readRecord(5);
    cout << "Nombre: "<<record.Nombre <<endl;
    cout << "Apellido: " << record.Apellido << endl;
    cout << "Carrera: " << record.Carrera << endl;
    cout << "Mensualidad: : " << record.mensualidad << endl;
    cout << endl;



    return 0;
}
