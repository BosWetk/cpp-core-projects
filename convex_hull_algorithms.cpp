#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Point {
    double x;
    double y;
};

class Method {
public:
    vector<Point> points;
    vector<Point> kelly_kirkpatrick() {
        vector<Point> pts = points;
        if (pts.size() <= 1) return pts;
        int min_y = (int)pts[0].y;
        int max_y = (int)pts[0].y;
        for (int i = 0; i < (int)pts.size(); i++) {
            int yy = (int)pts[i].y;
            if (yy < min_y) min_y = yy;
            if (yy > max_y) max_y = yy;
        }
        int h = max_y - min_y + 1;
        vector<bool> has_y(h, false);
        vector<Point> left_y(h), right_y(h);
        for (int i = 0; i < (int)pts.size(); i++) {
            int yy = (int)pts[i].y - min_y;
            if (!has_y[yy]) {
                has_y[yy] = true;
                left_y[yy] = pts[i];
                right_y[yy] = pts[i];
            }
            else {
                if (pts[i].x < left_y[yy].x)  left_y[yy] = pts[i];
                if (pts[i].x > right_y[yy].x) right_y[yy] = pts[i];
            }
        }
        vector<Point> left_chain;
        vector<Point> right_chain;
        for (int i = 0; i < h; i++) {
            if (has_y[i]) left_chain.push_back(left_y[i]);
        }
        for (int i = h - 1; i >= 0; i--) {
            if (has_y[i]) right_chain.push_back(right_y[i]);
        }
        vector<Point> st;
        for (int i = 0; i < (int)left_chain.size(); i++) {
            Point p = left_chain[i];
            while (st.size() >= 2) {
                int m = st.size();
                double c = cross_value(st[m - 2], st[m - 1], p);
                if (c >= 0) {
                    st.pop_back();
                }
                else {
                    break;
                }
            }
            st.push_back(p);
        }
        left_chain = st;
        st.clear();
        for (int i = 0; i < (int)right_chain.size(); i++) {
            Point p = right_chain[i];
            while (st.size() >= 2) {
                int m = st.size();
                double c = cross_value(st[m - 2], st[m - 1], p);
                if (c >= 0) {
                    st.pop_back();
                }
                else {
                    break;
                }
            }
            st.push_back(p);
        }
        right_chain = st;
        vector<Point> hull;
        for (int i = 0; i < (int)left_chain.size(); i++) {
            hull.push_back(left_chain[i]);
        }
        for (int i = 1; i + 1 < (int)right_chain.size(); i++) {
            hull.push_back(right_chain[i]);
        }
        return hull;
    }
    vector<Point> andrew_hull() {
        vector<Point> pts = points;
        int n = (int)pts.size();
        if (n <= 1) return pts;
        for (int i = 0; i < n; i++) {
            int min_i = i;
            for (int j = i + 1; j < n; j++) {
                if (pts[j].x < pts[min_i].x ||
                    (pts[j].x == pts[min_i].x && pts[j].y < pts[min_i].y)) {
                    min_i = j;
                }
            }
            if (min_i != i) {
                Point tmp = pts[i];
                pts[i] = pts[min_i];
                pts[min_i] = tmp;
            }
        }
        vector<Point> lower;
        vector<Point> upper;
        for (int i = 0; i < n; i++) {
            while (lower.size() >= 2) {
                int m = lower.size();
                if (cross_value(lower[m - 2], lower[m - 1], pts[i]) <= 0) {
                    lower.pop_back();
                }
                else {
                    break;
                }
            }
            lower.push_back(pts[i]);
        }
        for (int i = n - 1; i >= 0; i--) {
            while (upper.size() >= 2) {
                int m = upper.size();
                if (cross_value(upper[m - 2], upper[m - 1], pts[i]) <= 0) {
                    upper.pop_back();
                }
                else {
                    break;
                }
            }
            upper.push_back(pts[i]);
        }
        if (!lower.empty()) lower.pop_back();
        if (!upper.empty()) upper.pop_back();

        vector<Point> hull = lower;
        for (int i = 0; i < (int)upper.size(); i++) {
            hull.push_back(upper[i]);
        }
        return hull;
    }
    vector<Point> graham_hull() {
        vector<Point> pts = points;
        int n = (int)pts.size();
        if (n <= 1) return pts;
        int pivot = 0;
        for (int i = 1; i < n; i++) {
            if (pts[i].y < pts[pivot].y ||
                (pts[i].y == pts[pivot].y && pts[i].x < pts[pivot].x)) {
                pivot = i;
            }
        }
        Point base = pts[pivot];
        swap(pts[0], pts[pivot]);
        for (int i = 2; i < n; i++) {
            Point cur = pts[i];
            double ang_cur = atan2(cur.y - base.y, cur.x - base.x);
            double dist_cur = (cur.x - base.x) * (cur.x - base.x) +
                (cur.y - base.y) * (cur.y - base.y);
            int j = i - 1;
            while (j >= 1) {
                double ang_j = atan2(pts[j].y - base.y, pts[j].x - base.x);
                double dist_j = (pts[j].x - base.x) * (pts[j].x - base.x) +
                    (pts[j].y - base.y) * (pts[j].y - base.y);
                if (ang_j < ang_cur or (fabs(ang_j - ang_cur) < 1e-12 && dist_j <= dist_cur)) {
                    break;
                }
                pts[j + 1] = pts[j];
                j--;
            }
            pts[j + 1] = cur;
        }
        vector<Point> st;
        st.push_back(pts[0]);
        if (n > 1) st.push_back(pts[1]);
        for (int i = 2; i < n; i++) {
            while (st.size() >= 2) {
                int m = st.size();
                double c = cross_value(st[m - 2], st[m - 1], pts[i]);
                if (c <= 0) {
                    st.pop_back();
                }
                else {
                    break;
                }
            }
            st.push_back(pts[i]);
        }
        return st;
    }
    vector<Point> quickhull_hull() {
        vector<Point> pts = points;
        int n = (int)pts.size();
        if (n <= 1) return pts;
        int idx_min = 0;
        int idx_max = 0;
        for (int i = 1; i < n; i++) {
            if (pts[i].x < pts[idx_min].x) idx_min = i;
            if (pts[i].x > pts[idx_max].x) idx_max = i;
        }
        Point A = pts[idx_min];
        Point B = pts[idx_max];
        vector<Point> up, down;
        for (int i = 0; i < n; i++) {
            if (i == idx_min || i == idx_max) continue;
            double c = cross_value(A, B, pts[i]);
            if (c > 0) up.push_back(pts[i]);
            else if (c < 0) down.push_back(pts[i]);
        }
        vector<Point> hull;
        hull.push_back(A);
        quickhull_side(A, B, up, hull);
        quickhull_side(B, A, down, hull);
        return hull;
    }
private:
    static double cross_value(const Point& a, const Point& b, const Point& c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }
    static int farthest_index(const Point& a, const Point& b, const vector<Point>& v) {
        int idx = -1;
        double best = 0.0;
        for (int i = 0; i < (int)v.size(); i++) {
            double cur = fabs(cross_value(a, b, v[i]));
            if (cur > best) {
                best = cur;
                idx = i;
            }
        }
        return idx;
    }
    static void quickhull_side(const Point& a, const Point& b,
        const vector<Point>& v,
        vector<Point>& hull) {
        int idx = farthest_index(a, b, v);
        if (idx == -1) {
            hull.push_back(b);
            return;
        }
        Point c = v[idx];
        vector<Point> s1, s2;
        for (int i = 0; i < (int)v.size(); i++) {
            if (i == idx) continue;
            double c1 = cross_value(a, c, v[i]);
            double c2 = cross_value(c, b, v[i]);
            if (c1 > 0) s1.push_back(v[i]);
            else if (c2 > 0) s2.push_back(v[i]);
        }
        quickhull_side(a, c, s1, hull);
        quickhull_side(c, b, s2, hull);
    }
};
void output_menu() {
    cout << "1. KellyKirkpatrick method\n";
    cout << "2. Andrew monotone chain\n";
    cout << "3. Graham scan\n";
    cout << "4. QuickHull\n";
    cout << "Choose method: ";
}
vector<Point> input_points() {
    int n;
    cout << "Enter number of points: ";
    cin >> n;
    vector<Point> pts(n);
    cout << "Enter points (x y):\n";
    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
    }
    return pts;
}
void print_hull(const vector<Point>& hull) {
    cout << "Convex hull has " << hull.size() << " points:\n";
    for (int i = 0; i < (int)hull.size(); i++) {
        cout << i + 1 << ": (" << hull[i].x << ", " << hull[i].y << ")\n";
    }
}
int main() {
    output_menu();
    int method_index;
    cin >> method_index;
    Method M;
    M.points = input_points();
    vector<Point> hull;
    switch (method_index) {
    case 1:
        hull = M.kelly_kirkpatrick();
        break;
    case 2:
        hull = M.andrew_hull();
        break;
    case 3:
        hull = M.graham_hull();
        break;
    case 4:
        hull = M.quickhull_hull();
        break;
    default:
        cout << "Wrong method number.\n";
        system("pause");
        return 0;
    }
    print_hull(hull);
    system("pause");
    return 0;
}
