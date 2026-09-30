#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

void addPoint(vector<vector<double>>& points, double x, double y, double z = 0.0) {
    points.push_back({x, y, z});
}

void savePointsToTXT(const string& filename, const vector<vector<double>>& points) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Помилка відкриття файлу!" << endl;
        return;
    }
    for (const auto& point : points) {
        file << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    file.close();
}

void inputPointsManually(vector<vector<double>>& points, int n, int dim, const string& point_type) {
    cout << "\nВводьте координати для " << point_type << " точок:\n";
    for (int i = 0; i < n; ++i) {
        double x, y, z = 0.0;
        cout << point_type << " точка " << i + 1 << ": ";
        if (dim == 2) {
            cin >> x >> y;
            addPoint(points, x, y);
        } else {
            cin >> x >> y >> z;
            addPoint(points, x, y, z);
        }
    }
}

void generatePointsRandomly(vector<vector<double>>& points, int n, int dim, bool isTest) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> disX(isTest ? 10.0 : 30.0, isTest ? 90.0 : 70.0);
    uniform_real_distribution<> disY(isTest ? 20.0 : 40.0, isTest ? 80.0 : 60.0);
    uniform_real_distribution<> disZ(isTest ? 10.0 : 30.0, isTest ? 90.0 : 70.0);
    for (int i = 0; i < n; ++i) {
        double x = disX(gen);
        double y = disY(gen);
        double z = (dim == 3) ? disZ(gen) : 0.0;
        addPoint(points, x, y, z);
    }
}

void initializePoints(vector<vector<double>>& train_points, vector<vector<double>>& test_points, int& dim) {
    int inputChoice, n_train, n_test;
    cout << "Оберіть вимірність простору:\n2. 2D простір\n3. 3D простір\nВаш вибір: ";
    cin >> dim;
    while (dim != 2 && dim != 3) { cout << "Введіть 2 або 3: "; cin >> dim; }
    cout << "\nОберіть спосіб задання точок:\n1. Ввести власноруч\n2. Згенерувати випадково\nВаш вибір: ";
    cin >> inputChoice;
    while (inputChoice != 1 && inputChoice != 2) { cout << "Введіть 1 або 2: "; cin >> inputChoice; }
    cout << "\nКількість НАВЧАЛЬНИХ точок (для побудови еліпсів): "; cin >> n_train;
    cout << "Кількість ТЕСТОВИХ точок (для перевірки): "; cin >> n_test;
    if (inputChoice == 1) {
        inputPointsManually(train_points, n_train, dim, "навчальних");
        inputPointsManually(test_points, n_test, dim, "тестових");
    } else {
        generatePointsRandomly(train_points, n_train, dim, false);
        generatePointsRandomly(test_points, n_test, dim, true);
        cout << "\nУспішно згенеровано " << n_train << " навчальних та " << n_test << " тестових точок.\n";
    }
    savePointsToTXT("train_points.txt", train_points);
    savePointsToTXT("test_points.txt", test_points);
}

double getDistance(const vector<double>& p1, const vector<double>& p2) {
    return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}

double findFurthestPoints(const vector<vector<double>>& points, int& idx1, int& idx2) {
    double maxDist = -1.0;
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double d = getDistance(points[i], points[j]);
            if (d > maxDist) { maxDist = d; idx1 = i; idx2 = j; }
        }
    }
    return maxDist;
}

vector<double> crossProduct(const vector<double>& v1, const vector<double>& v2) {
    return { v1[1]*v2[2] - v1[2]*v2[1], v1[2]*v2[0] - v1[0]*v2[2], v1[0]*v2[1] - v1[1]*v2[0] };
}

double getDistanceToLine(const vector<double>& p, const vector<double>& a, const vector<double>& b) {
    vector<double> ab = {b[0]-a[0], b[1]-a[1], b[2]-a[2]};
    vector<double> ap = {p[0]-a[0], p[1]-a[1], p[2]-a[2]};
    double ab_norm = sqrt(ab[0]*ab[0] + ab[1]*ab[1] + ab[2]*ab[2]);
    if (ab_norm == 0) return getDistance(p, a);
    vector<double> cp = crossProduct(ap, ab);
    return sqrt(cp[0]*cp[0] + cp[1]*cp[1] + cp[2]*cp[2]) / ab_norm;
}

vector<double> normalizeVector(const vector<double>& v) {
    double len = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (len == 0) return {0.0, 0.0, 0.0};
    return {v[0]/len, v[1]/len, v[2]/len};
}

void findTwoSides2D(const vector<vector<double>>& points, int idx1, int idx2, double& maxH1, double& maxH2) {
    double A = points[idx2][1] - points[idx1][1];
    double B = points[idx1][0] - points[idx2][0];
    double C = points[idx2][0]*points[idx1][1] - points[idx1][0]*points[idx2][1];
    double denom = sqrt(A*A + B*B);
    maxH1 = 0; maxH2 = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i == idx1 || i == idx2) continue;
        double val = A*points[i][0] + B*points[i][1] + C;
        double dist = abs(val) / denom;
        if (val > 0 && dist > maxH1) maxH1 = dist;
        else if (val < 0 && dist > maxH2) maxH2 = dist;
    }
}

void findTwoSides3D(const vector<vector<double>>& points, int idx1, int idx2, int& idx3, double& maxD1, double& maxD2) {
    double maxDistToLine = -1.0; idx3 = -1;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i == idx1 || i == idx2) continue;
        double dist = getDistanceToLine(points[i], points[idx1], points[idx2]);
        if (dist > maxDistToLine) { maxDistToLine = dist; idx3 = i; }
    }
    vector<double> v1 = {points[idx2][0]-points[idx1][0], points[idx2][1]-points[idx1][1], points[idx2][2]-points[idx1][2]};
    vector<double> v2 = {points[idx3][0]-points[idx1][0], points[idx3][1]-points[idx1][1], points[idx3][2]-points[idx1][2]};
    vector<double> n = crossProduct(v1, v2);
    double A = n[0], B = n[1], C = n[2];
    double D = -(A*points[idx1][0] + B*points[idx1][1] + C*points[idx1][2]);
    double denom = sqrt(A*A + B*B + C*C);
    maxD1 = 0; maxD2 = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i == idx1 || i == idx2 || i == idx3) continue;
        double val = A*points[i][0] + B*points[i][1] + C*points[i][2] + D;
        double dist = abs(val) / denom;
        if (val > 0 && dist > maxD1) maxD1 = dist;
        else if (val < 0 && dist > maxD2) maxD2 = dist;
    }
}

int main() {
    vector<vector<double>> train_points, test_points;
    int dim;
    initializePoints(train_points, test_points, dim);
    if (train_points.size() < 3) {
        cout << "Недостатньо навчальних точок для методу Петуніна!" << endl;
        return 0;
    }
    int idx1, idx2;
    double diameter = findFurthestPoints(train_points, idx1, idx2);
    vector<double> globalCenter, axisX, axisY, axisZ;
    double a, b, c = 0;
    double scale_b, scale_c = 1.0;
    if (dim == 2) {
        double h1, h2;
        findTwoSides2D(train_points, idx1, idx2, h1, h2);
        globalCenter = {
            (train_points[idx1][0] + train_points[idx2][0]) / 2.0,
            (train_points[idx1][1] + train_points[idx2][1]) / 2.0,
            0.0
        };
        axisX = {train_points[idx2][0] - train_points[idx1][0], train_points[idx2][1] - train_points[idx1][1], 0.0};
        axisX = normalizeVector(axisX);
        axisY = {-axisX[1], axisX[0], 0.0}; 
        axisZ = {0.0, 0.0, 1.0};
        double shiftY = (h1 - h2) / 2.0;
        globalCenter[0] += shiftY * axisY[0];
        globalCenter[1] += shiftY * axisY[1];
        a = diameter / 2.0;
        b = (h1 + h2) / 2.0;
        scale_b = a / b;
    } else if (dim == 3) {
        int idx3;
        double d1, d2;
        findTwoSides3D(train_points, idx1, idx2, idx3, d1, d2);
        axisX = {train_points[idx2][0] - train_points[idx1][0], train_points[idx2][1] - train_points[idx1][1], train_points[idx2][2] - train_points[idx1][2]};
        axisX = normalizeVector(axisX);
        vector<double> v_13 = {train_points[idx3][0] - train_points[idx1][0], train_points[idx3][1] - train_points[idx1][1], train_points[idx3][2] - train_points[idx1][2]};
        axisZ = crossProduct(axisX, v_13);
        axisZ = normalizeVector(axisZ);
        axisY = crossProduct(axisZ, axisX);
        axisY = normalizeVector(axisY);
        double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9, minZ = 1e9, maxZ = -1e9;
        for (const auto& p : train_points) {
            vector<double> v = {p[0] - train_points[idx1][0], p[1] - train_points[idx1][1], p[2] - train_points[idx1][2]};
            double lx = v[0]*axisX[0] + v[1]*axisX[1] + v[2]*axisX[2];
            double ly = v[0]*axisY[0] + v[1]*axisY[1] + v[2]*axisY[2];
            double lz = v[0]*axisZ[0] + v[1]*axisZ[1] + v[2]*axisZ[2];
            if (lx < minX) minX = lx; if (lx > maxX) maxX = lx;
            if (ly < minY) minY = ly; if (ly > maxY) maxY = ly;
            if (lz < minZ) minZ = lz; if (lz > maxZ) maxZ = lz;
        }
        double localCX = (minX + maxX) / 2.0;
        double localCY = (minY + maxY) / 2.0;
        double localCZ = (minZ + maxZ) / 2.0;
        globalCenter = {
            train_points[idx1][0] + localCX*axisX[0] + localCY*axisY[0] + localCZ*axisZ[0],
            train_points[idx1][1] + localCX*axisX[1] + localCY*axisY[1] + localCZ*axisZ[1],
            train_points[idx1][2] + localCX*axisX[2] + localCY*axisY[2] + localCZ*axisZ[2]
        };
        a = (maxX - minX) / 2.0;
        b = (maxY - minY) / 2.0;
        c = (maxZ - minZ) / 2.0;
        scale_b = a / b;
        scale_c = a / c;
    }
    vector<double> radii;
    for (const auto& p : train_points) {
        double vx = p[0] - globalCenter[0];
        double vy = p[1] - globalCenter[1];
        double vz = p[2] - globalCenter[2];
        double lx = vx*axisX[0] + vy*axisX[1] + vz*axisX[2];
        double ly = vx*axisY[0] + vy*axisY[1] + vz*axisY[2];
        double lz = vx*axisZ[0] + vy*axisZ[1] + vz*axisZ[2];
        double sqX = lx;
        double sqY = ly * scale_b;
        double sqZ = lz * scale_c;
        radii.push_back(sqrt(sqX*sqX + sqY*sqY + sqZ*sqZ));
    }
    sort(radii.begin(), radii.end());

    for (size_t i = 0; i < test_points.size(); ++i) {
        double vx = test_points[i][0] - globalCenter[0];
        double vy = test_points[i][1] - globalCenter[1];
        double vz = test_points[i][2] - globalCenter[2];
        
        double lx = vx*axisX[0] + vy*axisX[1] + vz*axisX[2];
        double ly = vx*axisY[0] + vy*axisY[1] + vz*axisY[2];
        double lz = vx*axisZ[0] + vy*axisZ[1] + vz*axisZ[2];
        
        double sqX = lx;
        double sqY = ly * scale_b;
        double sqZ = lz * scale_c;
        double test_r = sqrt(sqX*sqX + sqY*sqY + sqZ*sqZ);
        
        int count = 0;
        for (double r : radii) {
            if (test_r <= r) count++;
        }
        cout << "Тестова точка " << i + 1 << " присутня у " << count << " з " << radii.size() << " кіл/сфер." << endl;
    }
    ofstream file("petunin_data.txt");
    if (file.is_open()) {
        file << globalCenter[0] << " " << globalCenter[1] << " " << globalCenter[2] << "\n";
        file << axisX[0] << " " << axisX[1] << " " << axisX[2] << "\n";
        file << axisY[0] << " " << axisY[1] << " " << axisY[2] << "\n";
        file << axisZ[0] << " " << axisZ[1] << " " << axisZ[2] << "\n";
        file << a << " " << b << " " << c << "\n";
        for (double r : radii) file << r << " ";
        file << "\n";
        file.close();
        cout << "\nДані успішно експортовано для візуалізації в Python!" << endl;
    }

    return 0;
}