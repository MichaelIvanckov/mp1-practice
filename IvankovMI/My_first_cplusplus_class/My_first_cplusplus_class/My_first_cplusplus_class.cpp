#include <iostream>
#include <string>
#include <cmath>


namespace vf_ {

struct Vector_float {                 //почему-то структура, а не класс
    typedef unsigned int uint;
    uint size;
    float* values;
    Vector_float() {};                //конструктор по умолчанию
    Vector_float(uint);
    Vector_float(const Vector_float&);
    ~Vector_float();
    float len() const;
    float operator* (const Vector_float&) const;         //тут я первый раз написал параметр по ссылке
    Vector_float operator+ (const Vector_float&) const;
    std::string to_str() const;
};
Vector_float::Vector_float(uint n) {
    size = n;
    values = new float[n];
}
Vector_float::Vector_float(const Vector_float& src) {
    size = src.size;
    values = new float[size];
    for (int i = 0; i < size; i++)
        this->values[i] = src.values[i];                  //тут я первый раз написал this, хоть и не оправданно, но зато наглядно
}
Vector_float::~Vector_float() {
    delete [] values;
}
float Vector_float::len() const {
    float l = 0;
    for (int i = 0; i < this->size; i++)
        l += this->values[i] * values[i];                     //куча неоправданных this
    return std::pow(l, 0.5f);
}
float Vector_float::operator*(const Vector_float& other) const {
    float l = 0;
    if (size != other.size)
        throw;
    for (int i = 0; i < size; i++)
        l += values[i] * other.values[i];
    return l;
}
Vector_float Vector_float::operator+(const Vector_float& other) const {
    Vector_float result;
    if (size != other.size)
        throw;
    for (int i = 0; i < size; i++)
        result.values[i] = values[i] + other.values[i];
    return result;
}
std::string Vector_float::to_str() const {
    std::string str = "Vector_float(";
    str.append(std::to_string(size));
    str.append(": ");
    for (int i = 0; i < size - 1; i++) {
        str.append(std::to_string(values[i]));
        str.append(", ");
    }
    str.append(std::to_string(values[size - 1]));
    str.append(")");
    return str;
}

}


namespace f_ {

    //и сразу шаблон зафигачил
    template<typename T>
    struct Vector {                 //почему-то структура, а не класс
        typedef unsigned int uint;
        uint size;
        T* values;
        Vector() {};                //конструктор по умолчанию
        Vector(uint);
        Vector(const Vector&);
        ~Vector();
        T len() const;
        T operator* (const Vector&) const;
        Vector operator+ (const Vector&) const;
        std::string to_str() const;
    };
    template<typename T>
    Vector<T>::Vector(uint n) {
        size = n;
        values = new T[n];
    }
    template<typename T>
    Vector<T>::Vector(const Vector& src) {
        size = src.size;
        values = new T[size];
        for (int i = 0; i < size; i++)
            this->values[i] = src.values[i];                  //неоправданный this
    }
    template<typename T>
    Vector<T>::~Vector() {
        delete[] values;
    }
    template<typename T>
    T Vector<T>::len() const {
        T l = 0;
        for (int i = 0; i < this->size; i++)
            l += this->values[i] * values[i];                     //куча неоправданных this
        return std::pow(l, 0.5f);
    }
    template<typename T>
    T Vector<T>::operator*(const Vector& other) const {
        T l = 0;
        if (size != other.size)
            throw;
        for (int i = 0; i < size; i++)
            l += values[i] * other.values[i];
        return l;
    }
    template<typename T>
    Vector<T> Vector<T>::operator+(const Vector& other) const {
        Vector result;
        if (size != other.size)
            throw;
        for (int i = 0; i < size; i++)
            result.values[i] = values[i] + other.values[i];
        return result;
    }
    template<typename T>
    std::string Vector<T>::to_str() const {
        std::string str = "Vector(";
        str.append(std::to_string(size));
        str.append(": ");
        for (int i = 0; i < size - 1; i++) {
            str.append(std::to_string(values[i]));
            str.append(", ");
        }
        str.append(std::to_string(values[size - 1]));
        str.append(")");
        return str;
    }

}


using namespace vf_;
using namespace f_;


int main()
{
    std::cout << "Hello World!\n";
    return 0;
}