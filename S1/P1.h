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

    vector<Alumno> load(){

    }

    void add(Alumno record){

    }

    Alumno readRecord(int pos){

    }

    bool Delete(int pos){

    }




};

int main(){
    ifstream file;
    file.open("p1_test1.txt");
    string text;

    while(true){

        getline(file,text);
        cout << text << endl;
        if(file.eof()){
            break;
        }
    }
}