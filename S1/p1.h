#include "iostream"
#include "string"
#include "fstream"
#include "vector"


using namespace std;


struct Alumno{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    int ciclo;
    float mensualidad;

    void setData(){
        cout<<"Codigo: ";cin>>codigo;
        cout<<"Nombre: ";cin>>nombre;
        cout<<"Apellido: ";cin>>apellidos;
        cout<<"Carrera: ";cin>>carrera;
        cout<<"Ciclo: ";cin>>ciclo;
        cout<<"Mensualidad: ";cin>>mensualidad;
    }

    void showData(){
        cout<<"Codigo: "<<codigo<<endl;
        cout<<"Nombre: "<<nombre<<endl;
        cout<<"Apellido: "<<apellidos<<endl;
        cout<<"Carrera: "<<carrera<<endl;
        cout<<"Ciclo: "<<ciclo<<endl;
        cout<<"Mensualidad: "<<mensualidad<<endl;

        cout<<"---------------------------------------------"<<endl;
    }
};

class FixedRecord{

private:
    string file_name;
    string delete_type;
public:
    FixedRecord(string file_name,string delete_type){
        this->file_name = file_name;
        this->delete_type = delete_type;
    }

    void writeRecord(Alumno record){
        ofstream file(this->file_name,ios::app | ios::binary);

        if(!file.is_open()){
            throw ("Cant open the file");
        }

        file.write((char*) &record, sizeof(Alumno));
        file.close();
    }

    void writeRecord(Alumno record,int pos){
        ofstream file(this->file_name,ios::app | ios::binary);
        if(!file.is_open()){
            throw  ("Cant open the file");
        }

        file.seekp(pos * sizeof (Alumno), ios::beg);
        file.write((char*) &record, sizeof (Alumno));
        file.close();

    }

    vector<Alumno> scanAll(){
        ifstream file(this->file_name,ios::app | ios::binary);
        if(!file.is_open()){
            throw  ("Cant open the file");
        }

        vector<Alumno> alumnos;
        Alumno record;

        while(file.peek() != EOF){
            record = Alumno ();
            file.read((char*) &record, sizeof (Alumno));
            alumnos.push_back(record);
        }

        file.close();

        return alumnos;

    }

    Alumno readRecord(int pos){
        ifstream file(this->file_name,ios::app |ios::binary);

        if(!file.is_open()){
            throw  ("Cant open the file");
        }
        Alumno record;
        file.seekg(pos * sizeof(Alumno), ios::beg);
        file.read((char*) &record,sizeof(Alumno));

        file.close();

        return record;

    }

    int size(){
        ifstream file(this->file_name, ios::app | ios::binary);

        if(!file.is_open()){
            throw  ("Cant open the file");
        }

        file.seekg(0, ios::end); //ubicar cursos al final del archivo
        long total_bytes = file.tellg(); // cantidad de byttes en el archivo

        file.close();

        return total_bytes / sizeof (Alumno);
    }


    void Move_The_Last(int pos) {
        int total_records = this->size();

        // Verifica si hay al menos un registro y que la posición esté dentro del rango válido
        if (total_records > 0 && pos < total_records) {
            // Leer el último registro
            Alumno last_record = this->readRecord(total_records - 1);

            // Escribir el último registro en la posición del registro a eliminar
            this->writeRecord(last_record, pos);

            // Abrimos el archivo en modo de lectura/escritura sin la opción de "append"
            fstream file(this->file_name, ios::in | ios::out | ios::binary);
            if (!file.is_open()) {
                throw("Can't open the file");
            }

            // Redimensionamos el archivo eliminando el último registro
            // Calculamos el tamaño que debe tener el archivo sin el último registro
            file.seekp((total_records - 1) * sizeof(Alumno), ios::beg);
            file.close();  // Cerrar antes de eliminar bytes.

            // Reabrir el archivo solo en modo escritura para truncar
            file.open(this->file_name, ios::in | ios::out | ios::binary);
            file.close();
        }
    }




    void Free_List(){

    }






};

int main(){

    FixedRecord file1("data.bin","MOVE_THE_LAST");
    Alumno record;

    

    return 0;
}
