#include <iostream>
#include <cmath>
#include <stdexcept>
#include <complex>
#include <random>

template<typename T>
struct Point {
    T x, y;

    Point(T x = 0, T y = 0) : x(x), y(y) {}
};

template<typename T>
class Polyline {
private:
    Point<T>* points; // массив точек
    size_t size; // количество точек
    size_t capacity; // емкость массива

    void resize(size_t newCapacity) {
        Point<T>* newPoints = new Point<T>[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newPoints[i] = points[i];
        }
        delete[] points;
        points = newPoints;
        capacity = newCapacity;
    }

public:
    static const double epsilon; // точность дл€ сравнени€ вещественных чисел

    //  онструктор с параметром: координата точки
    Polyline(Point<T> point) : size(1), capacity(1) {
        points = new Point<T>[capacity];
        points[0] = point;
    }

    //  онструктор с параметром: количество точек
    Polyline(size_t numPoints) : size(numPoints), capacity(numPoints) {
        points = new Point<T>[capacity];
        for (size_t i = 0; i < numPoints; ++i) {
            points[i] = Point<T>(0, 0); // »нициализаци€ точек
        }
    }

    //  онструктор с параметрами (создает ломаную из точек в диапазоне [m1, m2])
    Polyline(size_t numPoints, T m1, T m2) : size(numPoints), capacity(numPoints) {
        points = new Point<T>[capacity];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dis(m1, m2);

        for (size_t i = 0; i < numPoints; ++i) {
            points[i] = Point<T>(dis(gen), dis(gen));
        }
    }

    Polyline(const Polyline& other) : size(other.size), capacity(other.capacity) { 
        points = new Point<T>[capacity];
        for (size_t i = 0; i < size; ++i) {
            points[i] = other.points[i];
        }
    }

    // ƒеструктор
    ~Polyline() {
        delete[] points;
    }

    // ќператор [] дл€ чтени€/записи вершины
    Point<T>& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return points[index];
    }

    // ќператор сложени€ двух ломаных (конкатенаци€)
    Polyline operator+(const Polyline& other) const {
        Polyline result(size + other.size);
        for (size_t i = 0; i < size; ++i) {
            result.points[i] = points[i];
        }
        for (size_t i = 0; i < other.size; ++i) {
            result.points[size + i] = other.points[i];
        }
        return result;
    }

    // ќператор сложени€ ломаной и вершины (добавление вершины в конец)
    Polyline operator+(const Point<T>& point) const {
        Polyline result(size + 1);
        for (size_t i = 0; i < size; ++i) {
            result.points[i] = points[i];
        }
        result.points[size] = point;
        return result;
    }

    // ќператор сложени€ вершины и ломаной (вставка вершины в начало)
    friend Polyline operator+(const Point<T>& point, const Polyline& polyline) {
        return polyline + point; // »спользуем оператор сложени€ с ломаной
    }

    // ¬ычисление длины ломаной
    double length() const {
        double totalLength = 0.0;
        for (size_t i = 1; i < size; ++i) {
            totalLength += std::hypot(points[i].x - points[i - 1].x, points[i].y - points[i - 1].y);
        }
        return totalLength;
    }

    // ќператор сравнени€ на равенство
    bool operator==(const Polyline& other) const {
        if (size != other.size) return false;
        for (size_t i = 0; i < size; ++i) {
            if (std::abs(points[i].x - other.points[i].x) > epsilon ||
                std::abs(points[i].y - other.points[i].y) > epsilon) {
                return false;
            }
        }
        return true;
    }

    // ќператор сравнени€ на неравенство
    bool operator!=(const Polyline& other) const {
        return !(*this == other);
    }
};

// »нициализаци€ статического пол€
template<typename T>
const double Polyline<T>::epsilon = 1e-5;

