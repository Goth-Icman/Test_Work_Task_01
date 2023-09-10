#include <iostream>
#include <vector>
#include <random>
#include "omp.h"

#include "Curve_Type.h"

using namespace std;

vector <curType> Stack01; // основной контейнер кривых
vector <int> Stack02; // контейнер индексов окружностей основного массива

random_device rdGen; // генератор случайных чисел
//mt19937 gen(rdGen());

template <class URNG> void FillStack01(URNG &urng) { 
    // случайное заполнение первого контейнера
    uniform_int_distribution<int> pCount(7, 15); // случайное число элементов
    uniform_int_distribution<int> pCType(1, 3); // случайный тип кривой
    uniform_real_distribution<double> pv(0.1, 10); // случайное число для параметров
    
    int cType;
    int count = pCount(urng);
    double rx, ry, h1;
        
    for (int i = 0; i < count; i++) {
        cType = pCType(urng);
        switch (cType) {
            case 1: {
                rx = pv(urng);
                Stack01.push_back(curType(rx));
                break;
            }
            case 2: {
                rx = pv(urng);
                ry = pv(urng);
                Stack01.push_back(curType(rx, ry));
                break;
            }
            case 3: {
                rx = pv(urng);
                h1 = pv(urng);
                Stack01.push_back(curType(rx, float(h1)));
                break;
            }
            default: break;
        }
        
    }

}

vector <int> FillStack02 (vector <curType> Vec) {
    // Заполнение второго контейнера (окружности первого контейнера)

    vector <int> IVec(0);
    
    if (! Vec.empty()) {
         for (int i = 0; i < Vec.size(); i++) {
            if (Vec.at(i).curIndex == 1) {
                IVec.push_back(i);
            }
        }
    }
    return IVec;
}

vector <int> SortStack02 (vector <curType> Vec, vector <int> IVec) {
    // Сортировка по градации радиусов окружностей
    int i = 0;
    int j = 0;
    int buf = 0;
    if ((! Vec.empty()) && (! IVec.empty())) {
        for (i = 0; i < IVec.size() - 1; i++){
            for (j = i + 1; j < IVec.size(); j++) {
                if (Vec.at(IVec.at(i)).rX > Vec.at(IVec.at(j)).rX) {
                    buf = IVec.at(j);
                    IVec.at(j) = IVec.at(i);
                    IVec.at(i) = buf;
                    buf = 0;
                }
            }
        }
    }
    return IVec;
}

double GetRadialSumm (vector <curType> Vec, vector <int> IVec) {
    double summ = 0;
    if ((! Vec.empty()) && (! IVec.empty())) {
        for(int i = 0; i < IVec.size(); i++) {
                summ += Vec.at(IVec.at(i)).rX;
        }
    }
    return summ;
}

void PrintCurvePointCoord (double t) {
    //Вывод координат точки и компонентов нормированного касательногго вектора в этой точке
    for (int i = 0; i < Stack01.size(); i++) {
        switch (Stack01.at(i).curIndex) {
            case 1: {
                cout<<(i + 1)<<" Circle; R = "<<Stack01.at(i).rX<<endl;
                break;
            }
            case 2: {
                cout<<(i + 1)<<" Ellipse; Rx = "<<Stack01.at(i).rX<<"; Ry = "<<Stack01.at(i).rY<<endl;
                break;
            }
            case 3: {
                cout<<(i + 1)<<" Helix; R = "<<Stack01.at(i).rX<<"; Step = "<<Stack01.at(i).h<<endl;
                break;
            }
        }
        cout<<"Coordinares of point at t = "<<t<<": ("<<Stack01.at(i).GetCurPoint(t).x<<", "<<Stack01.at(i).GetCurPoint(t).y<<", "<<Stack01.at(i).GetCurPoint(t).z<<") "<<endl;
        cout<<"Coordinares of vector at t = "<<t<<": {"<<Stack01.at(i).GetDiffVec(t).x<<", "<<Stack01.at(i).GetDiffVec(t).y<<", "<<Stack01.at(i).GetDiffVec(t).z<<"} "<<endl;
        cout<<endl;
    }
}

int main () {
            
    FillStack01(rdGen); // Заполнение контейнера объектов случайным образом
    
    Stack02 = FillStack02(Stack01); // Заполнение второго контейнера индексами окружностей первого

    Stack02 = SortStack02(Stack01, Stack02); // сортировка второго контейнера

    double s = 0;
    s = GetRadialSumm(Stack01, Stack02); // Сумма радиусов окружностей второго контейнера

    PrintCurvePointCoord(3.14 / 4); // Вывод координат точек и касательных векторов кривых из первого контейнера, при t = pi / 4
    cout<<"Number of circles is "<<Stack02.size()<<endl; // Вывод количества окружностей в первом контейнере  
    cout<<endl; 
    cout<<"Radial summ is "<<s<<endl; // Вывод суммы радиусов
    cout<<endl; 
    
    // Вывод отсортированных радиусов окружностей (для проверки метода сортировки)
    for(int i = 0; i < Stack02.size(); i++) {
        cout<<Stack01.at(Stack02.at(i)).rX<<" ";
    }

    return 0;
}
