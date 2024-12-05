#include <iostream>
#include <complex>
#include <exception>


template<typename T>
class Polynomial
{
    T* _data;
    size_t _size;

public:
    Polynomial(const size_t size)
    {
        _data = new T[size]{ 0 };
        _size = size;
    }

    Polynomial(const T* data, const size_t size)
    {
        _data = new T[size];
        for (int i = 0; i < size; i++)
        {
            _data[i] = data[i];
        }
        _size = size;
    }

    Polynomial(const Polynomial& other) : _size(other.getSize()) 
    {
        _data = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = other[i];
        }
    }

    size_t getSize() const
    {
        return _size;
    }

    T operator[](size_t ind) const {
        if (ind >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[ind];
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            delete[] _data;
            _size = other._size;
            _data = new T[_size];
            for (size_t i = 0; i < _size; ++i) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    void set(const size_t degree, const T& value)
    {
        if (degree >= _size)
            this->expand(degree);
        _data[degree] = value;
    }
    
    Polynomial operator + (const Polynomial& other) const
    {
        size_t newSize = std::max(_size, other.getSize());
        Polynomial tmp(newSize);
        for (size_t i = 0; i < newSize; i++)
        {
            T thisValue = (i < _size) ? _data[i] : 0;
            T otherValue = (i < other.getSize()) ? other[i] : 0;
            tmp.set(i, thisValue + otherValue);
        }
        return tmp;
    }

    Polynomial operator - (const Polynomial& other) const
    {
        size_t newSize = std::max(_size, other.getSize());
        Polynomial tmp(newSize);
        for (size_t i = 0; i < newSize; i++)
        {
            T thisValue = (i < _size) ? _data[i] : 0;
            T otherValue = (i < other.getSize()) ? other[i] : 0;
            tmp.set(i, thisValue - otherValue);
        }
        return tmp;
    }

    Polynomial operator * (const T& other) const
    {
        Polynomial tmp(_size);
        for (int i = 0; i < _size; i++)
        {
            tmp.set(i, _data[i] * other);
        }
        return tmp;
    }

    T calculate(const T& xValue) const
    {
        T result = 0;
        T xPower = 1;
        for (size_t i = 0; i < _size; i++)
        {
            result += _data[i] * xPower;
            xPower *= xValue;
        }
        return result;
    }

    void shrink_to_fit()
    {
        size_t ind = _size;
        while (_data[ind] == 0 && ind != 0)
        {
            ind--;
        }
        T* tmp = new T[ind];
        for (int i = 0; i < ind; i++)
        {
            tmp[i] = _data[i];
        }
        delete[] _data;
        _data = tmp;
        _size = ind + 1;
    }

    void expand(const size_t degree)
    {
        if (degree > _size)
        {
            T* tmp = new T[degree]{ 0 };
            for (int i = 0; i < _size; i++)
            {
                tmp[i] = _data[i];
            }
            delete[] _data;
            _data = tmp;
            _size = degree;
        }
    }

    ~Polynomial() { delete[] _data; }
};


template<typename T>
Polynomial<T> operator*(const T& scalar, const Polynomial<T>& poly)
{
    return poly * scalar;
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& poly)
{
    for (size_t i = 0; i < poly.getSize(); i++)
    {
        if (i > 0)
            os << " + ";
        os << poly[i] << "x^" << i;
    }
    return os;
}


template<typename T>
Polynomial<T> findIntegral(Polynomial<T> poly)
{
    Polynomial<T> integrated(poly.getSize() + 1);
    for (size_t i = 0; i < poly.getSize(); i++)
    {
        integrated.set(i + 1, poly[i] / static_cast<T>(i + 1));
    }
    return integrated;
}


int main()
{
    double data1[] = { 1, 4, 5, 3, 9 };
    double data2[] = { 2, 4, 2, -4 };
    Polynomial<double> pol1(data1, 5);
    Polynomial<double> pol2(data2, 4);
    Polynomial<double> pol = pol1 + pol2;
    std::cout << pol;
    std::cout << "\nCalculated: " << pol.calculate(2);
    std::cout << "\nIntegrated: ";
    std:: cout << findIntegral(pol);

    std::cout << "\n\nComplex:\n";
    std::complex<float> data1c[] = { std::complex<float>(2, -2), std::complex<float>(1, 3), 3, 5, 6};
    std::complex<float> data2c[] = { std::complex<float>(1, -4), std::complex<float>(2, -3), 3, 5};
    Polynomial<std::complex<float>> pol1c(data1c, 5);
    Polynomial<std::complex<float>> pol2c(data2c, 4);
    Polynomial<std::complex<float>> polc = 3 * pol1c;
    std::cout << polc;
    std::cout << "\nCalculated: " << polc.calculate(2);
    std::cout << "\nIntegrated: ";
    Polynomial<std::complex<float>> integr = findIntegral(polc);
    std::cout << integr;
}