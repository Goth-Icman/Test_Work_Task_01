#include <iostream>
#include <cmath>

using namespace std;

struct Vector3d {
    public: double x, y, z;
};

struct curType {

    // Параметры

    public: 
        double rX; // радиус вдоль оси ОХ
        double rY; // радиус вдоль оси ОY
        float h; // шаг винта спирали
        int curIndex; // индекс кривой (1 - окружность; 2 - эллипс; 3 - спираль)

    private: 
        double x, y, z; // координата вдоль осей ОХ, OY, OZ
        double dx, dy, dz; // проекции касательного вектора на оси координат
        
        
    // Методы

    public: 
        Vector3d GetCurPoint(double); // Получить точку на кривой
        Vector3d GetDiffVec(double); // Получить касательный вектор в точке на кривой

        curType(double r) { // Конструктор окружности
            if (r >=0) {
                rX = r;
                rY = r;
            } else rX = rY = 0;
            curIndex = 1;
            x = y = z = 0;
            dx = dy = dz = 0;
            h = 0;
        }

        curType(double r1, double r2) { // Конструктор эллипса
            if ((r1 >=0) && (r2 >=0)) {
                rX = r1;
                rY = r2;
            } else rX = rY = 0;
            curIndex = 2;
            x = y = z = 0;
            dx = dy = dz = 0;
            h = 0;
        }

        curType(double r, float s) { // Конструктор спирали
            if (r >=0) {
                rX = r;
                rY = r;
            } else rX = rY = 0;
            curIndex = 3;
            x = y = z = 0;
            dx = dy = dz = 0;
            if (s > 0.0f) {
                h = s;
            } else {h = 1;}
        }
    
    private:

        Vector3d CircleCalc(double); // Расчет по отображению окружности или эллипса
        Vector3d HelixCalc(double); // Расчет по отображению спирали
        Vector3d DiffCircleCalc(double); // Расчет нормированного касательного вектора окружности или эллипса
        Vector3d DiffHelixCalc(double); // Расчет нормированного касательного вектора спирали
};


Vector3d curType::GetCurPoint(double t) {
    Vector3d vec;
    switch (curIndex) {
        case 1: {
            vec = CircleCalc(t); break;
        }
        case 2: {
            vec = CircleCalc(t); break;
        }
        case 3: {
            vec = HelixCalc(t); break;
        }
    }
    
    return vec;
}

Vector3d curType::GetDiffVec(double t) {
    Vector3d vec;
    switch (curIndex) {
        case 1: {
            vec = DiffCircleCalc(t); break;
        }
        case 2: {
            vec = DiffCircleCalc(t); break;
        }
        case 3: {
            vec = DiffHelixCalc(t); break;
        }
    }
    return vec;
}

Vector3d curType::CircleCalc(double t1) {
    Vector3d vec;

    x = rX * cos(t1);
    y = rY * sin(t1);
    z = 0;
    
    vec.x = x;
    vec.y = y;
    vec.z = z;

    return vec;
}

Vector3d curType::HelixCalc(double t1) {
    Vector3d vec;
    
    x = rX * cos(t1);
    y = rY * sin(t1);
    z = h * t1 / (2 * 3.14);

    vec.x = x;
    vec.y = y;
    vec.z = z;

    return vec;
}

Vector3d curType::DiffCircleCalc(double t1) {
    Vector3d vec;
    double nVec; // норма касательного вектора

    dx = - rX * sin(t1);
    dy = rY * cos(t1);
    dz = 0;

    nVec = sqrt(dx*dx + dy*dy);

    if (nVec == 0) nVec = 1;

    vec.x = dx / nVec;
    vec.y = dy / nVec;
    vec.z = dz;

    return vec;
}

Vector3d curType::DiffHelixCalc(double t1) {
    Vector3d vec;
    double nVec; // норма касательного вектора

    dx = - rX * sin(t1);
    dy = rY * cos(t1);
    dz = h / (2 * 3.14);
    
    nVec = sqrt(dx * dx + dy * dy + dz * dz);

    if (nVec == 0) nVec = 1;

    vec.x = dx / nVec;
    vec.y = dy / nVec;
    vec.z = dz / nVec;

    return vec;
}