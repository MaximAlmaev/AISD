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
    Point<T>* points; // ������ �����
    size_t size; // ���������� �����
    size_t capacity; // ������� �������

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
    static const double epsilon; // �������� ��� ��������� ������������ �����

    // ����������� � ����������: ���������� �����
    Polyline(Point<T> point) : size(1), capacity(1) {
        points = new Point<T>[capacity];
        points[0] = point;
    }

    // ����������� � ����������: ���������� �����
    Polyline(size_t numPoints) : size(numPoints), capacity(numPoints) {
        points = new Point<T>[capacity];
        for (size_t i = 0; i < numPoints; ++i) {
            points[i] = Point<T>(0, 0); // ������������� �����
        }
    }

    // ����������� � ����������� (������� ������� �� ����� � ��������� [m1, m2])
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

    // ����������
    ~Polyline() {
        delete[] points;
    }

    // �������� [] ��� ������/������ �������
    Point<T>& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return points[index];
    }

    // �������� �������� ���� ������� (������������)
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

    // �������� �������� ������� � ������� (���������� ������� � �����)
    Polyline operator+(const Point<T>& point) const {
        Polyline result(size + 1);
        for (size_t i = 0; i < size; ++i) {
            result.points[i] = points[i];
        }
        result.points[size] = point;
        return result;
    }

    // �������� �������� ������� � ������� (������� ������� � ������)
    friend Polyline operator+(const Point<T>& point, const Polyline& polyline) {
        return polyline + point; // ���������� �������� �������� � �������
    }

    // ���������� ����� �������
    double length() const {
        double totalLength = 0.0;
        for (size_t i = 1; i < size; ++i) {
            totalLength += std::hypot(points[i].x - points[i - 1].x, points[i].y - points[i - 1].y);
        }
        return totalLength;
    }

    // �������� ��������� �� ���������
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

    // �������� ��������� �� �����������
    bool operator!=(const Polyline& other) const {
        return !(*this == other);
    }
};

// ������������� ������������ ����
template<typename T>
const double Polyline<T>::epsilon = 1e-5;

