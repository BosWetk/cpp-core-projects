#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

class Method {
public:
    string x, y, n;
    int k = 1;
    string karats(string x1, string y1) {
        if (x1.length() <= 1 || y1.length() <= 1) {
            int n1, n2, res;
            n1 = convertor_st(x1);
            n2 = convertor_st(y1);
            res = n1 * n2;
            return to_string(res);
        }
        long long max_len, num_zero = 0, cut;
        string a, b, c, d, p, q, r;
        max_len = max(x1.length(), y1.length());
        num_zero = 2 * max_len - x1.length() - y1.length();
        x = edit(x1, max_len);
        y = edit(y1, max_len);
        cut = max_len / 2;
        a = take_parts(x, 0, max_len - cut);
        b = take_parts(x, max_len - cut, max_len);
        c = take_parts(y, 0, max_len - cut);
        d = take_parts(y, max_len - cut, max_len);
        p = karats(a, c);
        r = karats(add(a, b), add(c, d));
        q = karats(b, d);
        string res = add(add(pow10(p, 2 * cut), pow10(sub(r, add(p, q)), cut)), q);
        return res;
    }
    string toomc(string x1, string y1) {
        if (x1.length() == 1 && y1.length() == 1) {
            int n1, n2, res;
            n1 = convertor_st(x1);
            n2 = convertor_st(y1);
            res = n1 * n2;
            return to_string(res);
        }
        long long max_len, cut;
        string a, b, c, d, e, f;
        string C0, C1, C2, C3, C4;
        string r0, r1, r2, r3, r4;
        max_len = max(x1.length(), y1.length());
        x1 = edit(x1, max_len);
        y1 = edit(y1, max_len);
        cut = (max_len + 2) / 3;
        a = take_parts(x1, 0, max_len - 2 * cut);
        b = take_parts(x1, max_len - 2 * cut, max_len - cut);
        c = take_parts(x1, max_len - cut, max_len);
        d = take_parts(y1, 0, max_len - 2 * cut);
        e = take_parts(y1, max_len - 2 * cut, max_len - cut);
        f = take_parts(y1, max_len - cut, max_len);
        if (a == "") a = "0";
        if (b == "") b = "0";
        if (c == "") c = "0";
        if (d == "") d = "0";
        if (e == "") e = "0";
        if (f == "") f = "0";
        string x0 = c;
        string y0 = f;
        string x1v = add(add(a, b), c);
        string y1v = add(add(d, e), f);
        string t1 = add(a, c);
        k = 1;
        string xm1 = sub(t1, b);
        int sign_xm1 = k;
        k = 1;
        string t2 = add(d, f);
        string ym1 = sub(t2, e);
        int sign_ym1 = k;
        k = 1;
        if (xm1 == "0") sign_xm1 = 1;
        if (ym1 == "0") sign_ym1 = 1;
        int sign_C2 = sign_xm1 * sign_ym1;
        string twoA = add(a, a);
        string fourA = add(twoA, twoA);
        string tmpX = add(fourA, c);
        string twoB = add(b, b);
        k = 1;
        string xm2 = sub(tmpX, twoB);
        int sign_xm2 = k;
        k = 1;
        string twoD = add(d, d);
        string fourD = add(twoD, twoD);
        string tmpY = add(fourD, f);
        string twoE = add(e, e);
        k = 1;
        string ym2 = sub(tmpY, twoE);
        int sign_ym2 = k;
        k = 1;
        if (xm2 == "0") sign_xm2 = 1;
        if (ym2 == "0") sign_ym2 = 1;
        int sign_C3 = sign_xm2 * sign_ym2;
        string xinf = a;
        string yinf = d;
        C0 = toomc(x0, y0);
        C1 = toomc(x1v, y1v);
        string C2_abs = toomc(xm1, ym1);
        string C3_abs = toomc(xm2, ym2);
        C4 = toomc(xinf, yinf);
        r0 = C0;
        r4 = C4;
        int sign_acc = 1;
        string acc = "0";
        string tC0_3 = mul_small(C0, 3);
        acc = tC0_3; sign_acc = 1;
        string tC1_2 = mul_small(C1, 2);
        acc = add(acc, tC1_2);
        string tC2_6 = mul_small(C2_abs, 6);
        acc = add_signed(acc, sign_acc, tC2_6, -sign_C2, sign_acc);
        acc = add_signed(acc, sign_acc, C3_abs, sign_C3, sign_acc);
        string tC4_12 = mul_small(C4, 12);
        acc = add_signed(acc, sign_acc, tC4_12, -1, sign_acc);
        string r1_abs = div_small(acc, 6);
        r1 = r1_abs;
        acc = "0"; sign_acc = 1;
        string tC0_2 = mul_small(C0, 2);
        acc = tC0_2; sign_acc = -1;
        acc = add_signed(acc, sign_acc, C1, 1, sign_acc);
        acc = add_signed(acc, sign_acc, C2_abs, sign_C2, sign_acc);
        string tC4_2 = mul_small(C4, 2);
        acc = add_signed(acc, sign_acc, tC4_2, -1, sign_acc);
        string r2_abs = div_small(acc, 2);
        r2 = r2_abs;
        acc = "0"; sign_acc = 1;
        string tC0_3b = mul_small(C0, 3);
        acc = tC0_3b; sign_acc = -1;
        acc = add_signed(acc, sign_acc, C1, 1, sign_acc);
        string tC2_3 = mul_small(C2_abs, 3);
        acc = add_signed(acc, sign_acc, tC2_3, sign_C2, sign_acc);
        acc = add_signed(acc, sign_acc, C3_abs, -sign_C3, sign_acc);
        string tC4_12b = mul_small(C4, 12);
        acc = add_signed(acc, sign_acc, tC4_12b, 1, sign_acc);
        string r3_abs = div_small(acc, 6);
        r3 = r3_abs;
        string res = add(add(add(add(pow10(r4, 4 * cut), pow10(r3, 3 * cut)),pow10(r2, 2 * cut)),pow10(r1, cut)),r0);
        return res;
    }
    long long Schönhage(long long x1, long long y1) {
    long long maxb = std::max(bitlen(x1), bitlen(y1));
    long long k = findk(maxb);
    return SchoenhageRec(x1, y1, k);
    }
    string strass(string x1,string y1){
        if (x1.length() <= 2 and y1.length() <= 2) {
            int n1,n2,res;
            n1 = convertor_st(x1);
            n2 = convertor_st(y1);
            res = n1 * n2;
            return to_string(res);
        }
        long long max_len,cut;
        string a,b,c,d,e,f;
        string C0,C1,C2,C3,C4,C_be;
        max_len=max(x1.length(),y1.length());
        x1=edit(x1,max_len);
        y1=edit(y1,max_len);
        cut=(max_len+2)/3;
        a = take_parts(x1,0,max_len-2*cut);
        b = take_parts(x1,max_len-2*cut,max_len-cut);
        c = take_parts(x1,max_len-cut,max_len);
        d = take_parts(y1,0,max_len-2*cut);
        e = take_parts(y1,max_len-2*cut,max_len-cut);
        f = take_parts(y1,max_len-cut,max_len);
        if (a == "") a = "0";
        if (b == "") b = "0";
        if (c == "") c = "0";
        if (d == "") d = "0";
        if (e == "") e = "0";
        if (f == "") f = "0";
        C0=strass(c,f);
        C4=strass(a,d);
        C_be=strass(e,b);
        C3=sub(strass(add(a,b),add(e,d)),add(C_be,C4));
        C1=sub(strass(add(c,b),add(e,f)),add(C_be,C0));
        C2=sub(strass(add(add(a,b),c),add(add(d,e),f)),add(add(add(C0,C1),C3),C4));
        string res=add(add(add(add(pow10(C4,cut*4),pow10(C3,cut*3)),pow10(C2,cut*2)),pow10(C1,cut)),C0);
        return res;
    }
    string inverse_cook(string n, int prec) {
        n = strip_leading_zeros(n);
        if (n == "0") return "NaN";
        string r = "1";
        string res = "0.";
        for (int i = 0; i < prec; ++i) {
            r = mul_small(r, 10);
            int digit = 0;
            string best = "0";
            for (int d = 1; d <= 9; ++d) {
                string cand = mul_small(n, d);
                if (cmp(cand, r) <= 0) {
                    digit = d;
                    best = cand;
                } else {
                    break;
                }
            }
            res.push_back(char('0' + digit));
            if (digit > 0) {
                k = 1;
                r = sub(r, best);
            }
        }
        return res;
    }
    string divide_cook(string a, string b, int prec) {
        a = strip_leading_zeros(a);
        b = strip_leading_zeros(b);
        if (b == "0") return "NaN";
        string cur = "0";
        string res = "";
        for (size_t i = 0; i < a.size(); ++i) {
            if (cur == "0") cur = string(1, a[i]);
            else cur.push_back(a[i]);
            cur = strip_leading_zeros(cur);
            int digit = 0;
            string best = "0";
            for (int d = 1; d <= 9; ++d) {
                string cand = mul_small(b, d);
                if (cmp(cand, cur) <= 0) {
                    digit = d;
                    best = cand;
                } else {
                    break;
                }
            }
            res.push_back(char('0' + digit));
            if (digit > 0) {
                k = 1;
                cur = sub(cur, best);
            }
        }
        res = strip_leading_zeros(res);
        if (prec <= 0) return res;
        res.push_back('.');
        for (int i = 0; i < prec; ++i) {
            cur = mul_small(cur, 10);
            int digit = 0;
            string best = "0";
            for (int d = 1; d <= 9; ++d) {
                string cand = mul_small(b, d);
                if (cmp(cand, cur) <= 0) {
                    digit = d;
                    best = cand;
                } else {
                    break;
                }
            }
            res.push_back(char('0' + digit));
            if (digit > 0) {
                k = 1;
                cur = sub(cur, best);
            }
        }
        return res;
    }
    bool lehmann(string s) {
        unsigned long long n = stoull(s);
        if (n < 2) return false;
        if (n % 2 == 0) return n == 2;
        unsigned long long m = (n - 1) / 2;
        for (unsigned long long a = 2; a < n && a < 12; ++a) {
            unsigned long long x = modpow(a, m, n);
            if (x != 1 && x != n - 1) {
                return false;
            }
        }
        return true;
    }
    bool rabin_miller(string s) {
        unsigned long long n = stoull(s);
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;

        unsigned long long d = n - 1;
        int r = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            ++r;
        }
        unsigned long long bases[] = {2, 3, 5, 7, 11};
        int nb = 5;
        for (int i = 0; i < nb && bases[i] < n; ++i) {
            unsigned long long a = bases[i];
            unsigned long long x = modpow(a, d, n);
            if (x == 1 || x == n - 1) continue;

            bool ok = false;
            for (int j = 1; j < r; ++j) {
                x = modmul(x, x, n);
                if (x == n - 1) {
                    ok = true;
                    break;
                }
            }
            if (!ok) return false;
        }
        return true;
    }
    bool solovay_strassen(string s) {
        long long n = stoll(s);
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;
        long long bases[] = {2, 3, 5, 7, 11};
        int nb = 5;
        for (int i = 0; i < nb && bases[i] < n; ++i) {
            long long a = bases[i];
            if (gcd_ll(a, n) > 1) return false;
            int j = jacobi(a, n);
            if (j == 0) return false;
            long long p = (n - 1) / 2;
            unsigned long long mod = modpow((unsigned long long)((a % n + n) % n),
                                            (unsigned long long)p,
                                            (unsigned long long)n);
            long long jmod = (j == -1) ? (n - 1) : 1;
            if (mod != (unsigned long long)jmod) return false;
        }
        return true;
    }
    void sieve_eratosthenes(string s) {
        int N = stoi(s);
        if (N < 2) {
            cout << "No primes\n";
            return;
        }
        vector<bool> is_prime(N + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i <= N; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= N; j += i) {
                    is_prime[j] = false;
                }
            }
        }
        cout << "Primes up to " << N << ": ";
        bool first = true;
        for (int i = 2; i <= N; ++i) {
            if (is_prime[i]) {
                if (!first) cout << ", ";
                cout << i;
                first = false;
            }
        }
        cout << "\n";
    }
private:
    string edit(string x, int m) {
        while ((int)x.length() < m) {
            x = "0" + x;
        }
        return x;
    }
    string take_parts(string x, long long b, long long count) {
        string a = "";
        for (long long i = b; i < count; i++) {
            a += x[i];
        }
        return a;
    }
    int convertor_st(string x) {
        return stoi(x);
    }
    string add(string a, string b) {
        int i = a.size() - 1;
        int j = b.size() - 1;
        int carry = 0;
        string res = "";
        while (i >= 0 || j >= 0 || carry) {
            int da = (i >= 0 ? a[i] - '0' : 0);
            int db = (j >= 0 ? b[j] - '0' : 0);
            int sum = da + db + carry;
            carry = sum / 10;
            res.insert(res.begin(), char('0' + (sum % 10)));
            i--;
            j--;
        }
        return res;
    }
    string sub(string a, string b) {
        if (a.length() == b.length()) {
            for (int i = 0; i < (int)a.length(); i++) {
                if (a[i] > b[i]) {
                    break;
                } else if (a[i] < b[i]) {
                    string temp = a;
                    a = b;
                    b = temp;
                    k *= -1;
                    break;
                }
            }
        } else if (a.length() < b.length()) {
            string temp = a;
            a = b;
            b = temp;
            k *= -1;
        }
        int i = a.size() - 1;
        int j = b.size() - 1;
        int borrow = 0;
        string res;
        while (i >= 0) {
            int da = a[i] - '0';
            int db = (j >= 0 ? b[j] - '0' : 0);
            da -= borrow;
            if (da < db) {
                da += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            res.insert(res.begin(), char('0' + (da - db)));
            i--;
            j--;
        }
        int pos = 0;
        while (pos + 1 < (int)res.size() && res[pos] == '0') pos++;
        return res.substr(pos);
    }
    string pow10(string s, long long k) {
        if (s == "0") return "0";
        while (k--) s += '0';
        return s;
    }
    string div_small(string a, int d) {
        int rem = 0;
        string res;
        for (int i = 0; i < (int)a.size(); i++) {
            int cur = rem * 10 + (a[i] - '0');
            int q = cur / d;
            rem = cur % d;
            res.push_back(char('0' + q));
        }
        int pos = 0;
        while (pos + 1 < (int)res.size() && res[pos] == '0')
            pos++;
        return res.substr(pos);
    }
    string mul_small(string a, int m) {
        if (m == 0 || a == "0") return "0";
        int carry = 0;
        string res;
        for (int i = (int)a.size() - 1; i >= 0; --i) {
            int cur = (a[i] - '0') * m + carry;
            res.insert(res.begin(), char('0' + (cur % 10)));
            carry = cur / 10;
        }
        while (carry) {
            res.insert(res.begin(), char('0' + (carry % 10)));
            carry /= 10;
        }
        int pos = 0;
        while (pos + 1 < (int)res.size() && res[pos] == '0') pos++;
        return res.substr(pos);
    }
    string add_signed(string a, int sign_a, string b, int sign_b, int &sign_res) {
        if (a == "0") {
            sign_res = (b == "0" ? 1 : sign_b);
            return b;
        }
        if (b == "0") {
            sign_res = (a == "0" ? 1 : sign_a);
            return a;
        }

        if (sign_a == sign_b) {
            sign_res = sign_a;
            return add(a, b);
        } else {
            k = 1;
            string diff = sub(a, b);
            int s = sign_a * k;
            if (diff == "0") s = 1;
            sign_res = s;
            return diff;
        }
    }
    string strip_leading_zeros(string s) {
        int pos = 0;
        while (pos + 1 < (int)s.size() && s[pos] == '0') pos++;
        return s.substr(pos);
    }
    int cmp(string a, string b) {
        a = strip_leading_zeros(a);
        b = strip_leading_zeros(b);
        if (a.size() < b.size()) return -1;
        if (a.size() > b.size()) return 1;
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    }
    unsigned long long modmul(unsigned long long a, unsigned long long b, unsigned long long m) {
        unsigned long long r = 0;
        a %= m;
        while (b) {
            if (b & 1) {
                r += a;
                if (r >= m) r -= m;
            }
            a <<= 1;
            if (a >= m) a -= m;
            b >>= 1;
        }
        return r;
    }
    unsigned long long modpow(unsigned long long a, unsigned long long e, unsigned long long m) {
        unsigned long long r = 1 % m;
        a %= m;
        while (e) {
            if (e & 1) r = modmul(r, a, m);
            a = modmul(a, a, m);
            e >>= 1;
        }
        return r;
    }
    long long gcd_ll(long long a, long long b) {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) {
            long long t = a % b;
            a = b;
            b = t;
        }
        return a;
    }
    int jacobi(long long a, long long n) {
        if (n <= 0 || (n & 1) == 0) return 0;
        a %= n;
        if (a < 0) a += n;
        int t = 1;
        while (a != 0) {
            while ((a & 1) == 0) {
                a >>= 1;
                long long r = n % 8;
                if (r == 3 || r == 5) t = -t;
            }
            std::swap(a, n);
            if ((a % 4 == 3) && (n % 4 == 3)) t = -t;
            a %= n;
        }
        return (n == 1) ? t : 0;
    }
    long long SchoenhageRec(long long x1, long long y1, long long k) {
    if (k == 0 || x1 <= 10 || y1 <= 10) {
        return x1 * y1;
    }
    long long q;
    long long m1, m2, m3, m4, m5, m6;
    long long u1,u2,u3,u4,u5,u6;
    long long v1,v2,v3,v4,v5,v6;
    long long w1,w2,w3,w4,w5,w6;
    long long M,M1,M2,M3,M4,M5,M6;
    long long t1,t2,t3,t4,t5,t6;
    long long w;
    q = (long long)(0.5 * (pow(3.0, k) + 1.0));
    m1 = (long long)(pow(2.0, 6.0*q - 1.0) - 1.0);
    m2 = (long long)(pow(2.0, 6.0*q + 1.0) - 1.0);
    m3 = (long long)(pow(2.0, 6.0*q + 2.0) - 1.0);
    m4 = (long long)(pow(2.0, 6.0*q + 3.0) - 1.0);
    m5 = (long long)(pow(2.0, 6.0*q + 5.0) - 1.0);
    m6 = (long long)(pow(2.0, 6.0*q + 7.0) - 1.0);
    u1 = x1 % m1;  v1 = y1 % m1;
    u2 = x1 % m2;  v2 = y1 % m2;
    u3 = x1 % m3;  v3 = y1 % m3;
    u4 = x1 % m4;  v4 = y1 % m4;
    u5 = x1 % m5;  v5 = y1 % m5;
    u6 = x1 % m6;  v6 = y1 % m6;
    w1 = SchoenhageRec(u1, v1, k-1) % m1;
    w2 = SchoenhageRec(u2, v2, k-1) % m2;
    w3 = SchoenhageRec(u3, v3, k-1) % m3;
    w4 = SchoenhageRec(u4, v4, k-1) % m4;
    w5 = SchoenhageRec(u5, v5, k-1) % m5;
    w6 = SchoenhageRec(u6, v6, k-1) % m6;
    M  = m1*m2*m3*m4*m5*m6;
    M1 =      m2*m3*m4*m5*m6;
    M2 = m1*      m3*m4*m5*m6;
    M3 = m1*m2*      m4*m5*m6;
    M4 = m1*m2*m3*      m5*m6;
    M5 = m1*m2*m3*m4*      m6;
    M6 = m1*m2*m3*m4*m5;
    t1 = invmod(M1 % m1, m1);
    t2 = invmod(M2 % m2, m2);
    t3 = invmod(M3 % m3, m3);
    t4 = invmod(M4 % m4, m4);
    t5 = invmod(M5 % m5, m5);
    t6 = invmod(M6 % m6, m6);
    w =  w1 * t1 * M1
       + w2 * t2 * M2
       + w3 * t3 * M3
       + w4 * t4 * M4
       + w5 * t5 * M5
       + w6 * t6 * M6;
    return w % M;
    }
    long long bitlen(long long x) {
            long long count = 0;
            while (x > 1) {
                x /= 2;
                count += 1;
            }
            return count + 1;
    }
    long long findk(long long a) {
        long long p = 26, k = 0;
        while (p < a) {
            k += 1;
            p = static_cast<long long>(std::pow(3.0, k + 2) + 17.0);
        }
        return k;
    }

 long long invmod(long long a, long long m) {
        long long m0 = m, t, q;
        long long x0 = 0, x1 = 1;
        if (m == 1) return 0;
        a %= m;
        if (a < 0) a += m;
        while (a > 1) {
            q = a / m;
            t = m;
            m = a % m;
            a = t;
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }
        if (x1 < 0) x1 += m0;
        return x1;
    }
};
void output() {
    string a[10] = {
        "Karatsuba","Toom–Cook","Schönhage","Strassen",
        "Cook","Cook","Lehmer","Rabin–Miller",
        "Solovay–Strassen","Sieve of Eratosthenes",
    };
    for (int i = 0; i < 4; i++) {
        cout << i + 1
             << ".Multiplication of non-negative integers using the "
             << a[i] << " method;" << endl;
    }
    cout << "5.High-precision inverse calculation (algorithm of "
         << a[4] << ");" << endl;
    cout << "6.Dividing integers using " << a[5] << " algorithm;"
         << endl;
    for (int i = 6; i < 10; i++) {
        cout << i + 1
             << ".Checking the primality of a number using the "
             << a[i] << " method;" << endl;
    }
    cout << "Choose what you want to do:";
}
string input(string x) {
    string val;
    cout << "Enter the " << x << ": ";
    cin >> val;
    return val;
}
int main() {
    output();
    int num;
    cin >> num;
    switch (num) {
        case 1: {
            Method Karatsuba;
            Karatsuba.x = input("first value");
            Karatsuba.y = input("second value");
            cout << Karatsuba.karats(Karatsuba.x, Karatsuba.y);
            break;
        }
        case 2: {
            Method ToomCook;
            ToomCook.x = input("first value");
            ToomCook.y = input("second value");
            cout << ToomCook.toomc(ToomCook.x, ToomCook.y);
            break;
        }
        case 3: {
            Method Schonhage;
            Schonhage.x = input("x");
            Schonhage.y = input("y");
            std::cout << Schonhage.Schönhage(stoll(Schonhage.x),stoll(Schonhage.y));
            break;
        }
        case 4: {
            Method Strassen;
            Strassen.x = input("first value");
            Strassen.y = input("second value");
            cout << Strassen.strass(Strassen.x, Strassen.y);
            break;
        }
        case 5: {
            Method Cook1;
            Cook1.n = input("n");
            int prec;
            cout << "Enter precision (number of digits after decimal point): ";
            cin >> prec;
            cout << Cook1.inverse_cook(Cook1.n, prec) << "\n";
            break;
        }
        case 6: {
            Method Cook2;
            Cook2.x = input("dividend");
            Cook2.y = input("divisor");
            int prec;
            cout << "Enter precision (number of digits after decimal point): ";
            cin >> prec;
            cout << Cook2.divide_cook(Cook2.x, Cook2.y, prec) << "\n";
            break;
        }
        case 7: {
            Method Lehmer;
            Lehmer.n = input("n");
            bool isPrime = Lehmer.lehmann(Lehmer.n);
            cout << (isPrime ? "Probably prime\n" : "Composite\n");
            break;
        }
        case 8: {
            Method RabinMiller;
            RabinMiller.n = input("n");
            bool isPrime = RabinMiller.rabin_miller(RabinMiller.n);
            cout << (isPrime ? "Probably prime\n" : "Composite\n");
            break;
        }
        case 9: {
            Method SolovayStrassen;
            SolovayStrassen.n = input("n");
            bool isPrime = SolovayStrassen.solovay_strassen(SolovayStrassen.n);
            cout << (isPrime ? "Probably prime\n" : "Composite\n");
            break;
        }
        case 10: {
            Method Eratosthene;
            Eratosthene.n = input("limit");
            Eratosthene.sieve_eratosthenes(Eratosthene.n);
            break;
        }
        default:
            cout << "The incorrect number of method.\n";
    };
    system("pause");
    cout<<endl;
}
