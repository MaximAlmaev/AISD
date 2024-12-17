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
    static const double epsilon; // точность для сравнения вещественных чисел

    // Конструктор с параметром: координата точки
    Polyline(Point<T> point) : size(1), capacity(1) {
        points = new Point<T>[capacity];
        points[0] = point;
    }

    // Конструктор с параметром: количество точек
    Polyline(size_t numPoints) : size(numPoints), capacity(numPoints) {
        points = new Point<T>[capacity];
        for (size_t i = 0; i < numPoints; ++i) {
            points[i] = Point<T>(0, 0); // Инициализация точек
        }
    }

    // Конструктор с параметрами (создает ломаную из точек в диапазоне [m1, m2])
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

    // Деструктор
    ~Polyline() {
        delete[] points;
    }

    // Оператор [] для чтения/записи вершины
    Point<T>& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return points[index];
    }

    // Оператор сложения двух ломаных (конкатенация)
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

    // Оператор сложения ломаной и вершины (добавление вершины в конец)
    Polyline operator+(const Point<T>& point) const {
        Polyline result(size + 1);
        for (size_t i = 0; i < size; ++i) {
            result.points[i] = points[i];
        }
        result.points[size] = point;
        return result;
    }

    // Оператор сложения вершины и ломаной (вставка вершины в начало)
    friend Polyline operator+(const Point<T>& point, const Polyline& polyline) {
        return polyline + point; // Используем оператор сложения с ломаной
    }

    // Вычисление длины ломаной
    double length() const {
        double totalLength = 0.0;
        for (size_t i = 1; i < size; ++i) {
            totalLength += std::hypot(points[i].x - points[i - 1].x, points[i].y - points[i - 1].y);
        }
        return totalLength;
    }

    // Оператор сравнения на равенство
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

    // Оператор сравнения на неравенство
    bool operator!=(const Polyline& other) const {
        return !(*this == other);
    }
};

// Инициализация статического поля
template<typename T>
const double Polyline<T>::epsilon = 1e-5;

Polyline<double> createIsoscelesTriangle(double angle, double sideLength) {
    if (angle <= 0 || angle >= 180) {
        throw std::invalid_argument("Angle must be between 0 and PI");
    }
    if (sideLength <= 0) {
        throw std::invalid_argument("Side length must be positive");
    }

    Polyline<double> triangle(3);

    // Базовая точка
    triangle[0] = Point<double>(0, 0);
    // Вторая точка
    triangle[1].x = sideLength * cos(angle / 2.0);
    triangle[1].y = sideLength * sin(angle / 2.0);
    // Третья точка
    triangle[2].x = 2.0 * sideLength * cos(angle / 2.0);
    triangle[2].y = 0.0;


    return triangle;
}
int main() {
    setlocale(LC_ALL, "");
    try {
        // Создаем ломаную
        Polyline<double> Iso_Triangle = createIsoscelesTriangle(60.0,2.0);
        std::cout << "Длина ломанной: " << Iso_Triangle.length() << std::endl;

        // Пример использования операторов
        Point<double> newPoint(1, 1);
        Polyline<double> extendedShape = Iso_Triangle + newPoint;
        std::cout << "Длина ломанной с еще одной точкой: " << extendedShape.length() << std::endl;

        // Проверка на равенство
        if (Iso_Triangle != extendedShape) {
            std::cout << "Ломанные разные." << std::endl;
        }

        // Демонстрация оператора сложения ломаной и точки
        Polyline<double> anotherExtendedShape = newPoint + Iso_Triangle;
        std::cout << "Длина ломанной, созданной добавлением точки в начале: " << anotherExtendedShape.length() << std::endl;

        // Использование оператора []
        std::cout << "Координаты первой точки ломаной: ("
            << Iso_Triangle[0].x << ", " << Iso_Triangle[0].y << ")" << std::endl;

        // Изменение координат точки в ломаной
        Iso_Triangle[0] = Point<double>(-1, -1);
        std::cout << "После изменения, первая точка ломаной: ("
            << Iso_Triangle[0].x << ", " << Iso_Triangle[0].y << ")" << std::endl;

        // Проверка на равенство после изменения
        if (Iso_Triangle == extendedShape) {
            std::cout << "Ломанные равны." << std::endl;
        }
        else {
            std::cout << "Ломанные разные после изменения." << std::endl;
        }

        // Создание новой ломаной с заданным количеством точек и диапазоном
        Polyline<double> randomShape(5, 0.0, 10.0);
        std::cout << "Длина случайной ломаной: " << randomShape.length() << std::endl;

        // Использование конструктора копирования
        Polyline<double> copiedShape = Iso_Triangle; // Копируем dShape
        std::cout << "Длина скопированной ломаной: " << copiedShape.length() << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
