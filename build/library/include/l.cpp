#include <iostream>
#include <string>
using std::cout, std::string;
class Building {
    private:
        int year;
        std::string type;
    public:

    Building(int y = 2, string t = "school") {
        this->year = y;
        this->type = t;
    }

    ~Building(){
        cout << "delite object";
    }
        void set_data(int y, string t){
            year = y;
            type = t;
        }

        void get_info(){
            cout << "Type: " << ". Year:" << year << std::endl;
        }
};

class PC {
private:
    int diagonal;
    string os;
public:
    PC(int diagonal, string os) {
        this->diagonal = diagonal;
        this->os = os;
    }
};

class Laptop: public PC {
private: 
    float weight;
public:
    Laptop(int diagonal, string os, float weight) :PC(diagonal, os){
        this->weight = weight;
    }
};

template <typename T, typename T2>
void print_arr(T* arr, int len){
    for(T2 i = 0; i < len; i++){
        cout << *(arr + i) << " ";
    }
    cout << std::endl;
}

template <class T>
class Arrays {
private:
int len;
T*arr ;
public:
    Arrays(T* a, int len){
        this->len = len;
        arr = new T[len];
        for(int i = 0; i < len; i++){
            *(arr + i) = *(a + i);
        }
    }
    void get() {
        for(int i = 0; i < len; i++){
            cout << *(arr + i) << " ";
        }
            cout << std::endl;
    }

    ~Arrays() {
        delete[] arr;
    }
};

int main(){
    Building school(12);
    // school.type = "Ўкола";
    // school.type = 2000;
    school.set_data(2, "school");
    school.get_info();
    Laptop mac(16, "Mac", 1.4);
    int arr1[] = {5, 6, 3, 2, 0, -4};
    float arr2[] = {5.34f, 4.42f, 4.42f};
    print_arr<int, int>(arr1, 6);
    print_arr<float, int>(arr2, 3);
    int arr3[] = {3, 6, 9};
    Arrays obj1(arr3, 3);
    float arr4[] = {3, 6, 9};
    Arrays<int> obj1(arr3, 3);



    return 0;
}