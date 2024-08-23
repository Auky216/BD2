#include "iostream"
#include "string"
#include "fstream"
#include "vector"


using namespace std;

struct Students{
    char name[12];
    char lastname[12];
};

void operator >> (ifstream &stream, Students &students){
    stream.get(students.name,12);
    stream.get(students.lastname,12);
    stream.get();
}

ostream &operator << (ostream& stream,Students& record){
    stream.write(record.name,12);
    stream.write(record.lastname,12);

    stream<<flush;
    return  stream;
}

int main(){
    ifstream file;
    file.open("test.txt");
    char buffer[150];
    vector<Students> vecStudents;

    while(true){
        Students students;

        file >> students;

        // Is the file is empty
        if(file.eof()){
            break;
        }

        // Extract the name and lastname from the buffer
        vecStudents.push_back(students);


    }

    file.close();

    for(int i = 0; i < vecStudents.size(); i++){
        cout <<"Name: "<< vecStudents[i].name << " " <<"Lastname: "<< vecStudents[i].lastname << endl;
    }


}