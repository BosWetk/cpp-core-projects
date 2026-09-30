#include <iostream>
#include <vector>
#include <cmath>
#include <random>
void interface() {
    std::cout << "Menu:\n";
    std::cout << "1. Linear congruential method;\n";
    std::cout << "2. Quadratic congruential method;\n";
    std::cout << "3. Fibonacci numbers;\n";
    std::cout << "4. Inverse congruent sequence;\n";
    std::cout << "5. Merging method;\n";
    std::cout << "6. \"3 Sigma\" Rule;\n";
    std::cout << "7. Method of polar coordinates;\n";
    std::cout << "8. Method of ratios;\n";
    std::cout << "9. Logarithm method for generating an exponential distribution;\n";
    std::cout << "10. Arens Method;\n";
    std::cout << "Choose the mothod: ";
}
void lin_cond() {
    std::cout << "1.c and m must be relatively prime;\n";
    std::cout << "2.b = a - 1 must be divisible by every prime divisor of m;\n";
    std::cout << "3.If m is divisible by 4, then b = a - 1 must also be divisible by 4.\n";
}
void quad_cond(){
     std::cout << "1.c and m must be relatively prime;\n";
     std::cout<<"2.Both numbers d and a−1 are multiples of p, for every prime number p that is an odd divisor of m.\n";
     std::cout<<"3.If m is a multiple of 4, then d is even and d≡a−1(mod4).\n";
     std::cout<<" If m is a multiple of 2, then d≡a−1(mod2).\n";
     std::cout<<"4.d≠3c(mod9), if m is a multiple of 3.\n";
    }
void inv_cond(){
    std::cout<<"1.p must be a power of two: p = 2^e, where e ≥ 3.\n";
    std::cout<<"2.Parameter a must satisfy: a mod 4 = 1.\n";
    std::cout<<"3.Parameter c must satisfy: c mod 4 = 2.\n";
}
void output(std::vector<double> freq){
    std::cout<<"Interval\tFrequency\n";
    for(int i=0;i<9;i++){
        std::cout<<"["<<i*10<<";"<<(i+1)*10<<")\t\t"<<freq[i]<<std::endl;
    }
    std::cout<<"[90;100]\t"<<freq[9]<<std::endl;
}
void output_1(std::vector<double> freq){
    std::cout<<"Interval\tFrequency\n";
    for(int i=0;i<5;i++){
        std::cout<<"["<<i-3<<";"<<i-2<<")\t\t"<<freq[i]<<std::endl;
    }
    std::cout<<"[2;3]\t\t"<<freq[5]<<std::endl;
}
void output_3(std::vector<double> freq){
    std::cout<<"Interval\tFrequency\n";
    std::cout<<"[0;0.1)\t\t"<<freq[0]<<"\n";
    for(int i=1;i<9;i++){
        std::cout<<"["<<i*0.1<<";"<<(i+1)*0.1<<")\t"<<freq[i]<<std::endl;
    }
    std::cout<<"[0.9;1]\t\t"<<freq[9]<<std::endl;
}

std::vector<int> fac_pr_num(long long m){
    std::vector<int> pr_num={1};
    for(int i=2;i<=m;i++){
        if(m%i==0){
            while(m%i==0){
                m/=i;
                pr_num.push_back(i);
            }
        }
    }
    return pr_num;
}

bool gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a==1;
}

bool div_by_pr(long a,long m, long num){
    long long temp=m;
    for (int i = num; i * i <= temp; i++) {
        if (temp % i == 0) {
            if ((a) % i != 0) {
                return false;
            }
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    if (temp%2==0){
        while(temp%2==0){
            temp/=2;
        }
    }
    if (temp > 1 && a % temp != 0) {
        return false;
    }
    return true;
}
bool quad_check_cond(long long a, long long c, long long m, long long d) {
    if (m <= 0 or a < 0 or c < 0 or d < 0) return 0;
    if (!gcd(c, m)) return 0;
    if (!div_by_pr(a-1, m, 3) || !div_by_pr(d, m, 3)) return 0;
    if (m % 4 == 0) {
        if (d % 2 != 0 or d % 4 != (a-1) % 4) return 0;
    } else if (m % 2 == 0) {
        if (d % 2 != (a-1) % 2) return 0;
    }
    if (m % 3 == 0) {
        if (d % 9 == (3 * c) % 9) return 0;
    }
    return true;
}
bool lin_check_cond(long long m, long long a, long long c) {
    std::vector<int> prime_div_m;
    if (m <= a and m <= c and a < 0 and c < 0) return 0;
    if (m % 4 == 0 and (a - 1) % 4 != 0) return 0;
    if(!gcd(c,m)) return 0;
    if(!div_by_pr(a-1,m,2))return 0;
    return 1;

}
long long linear(long long m,long long a, long long c, long long x_n) {
    return (a * x_n + c) % m;
}
long long inversion(long long a, long long p) {
    long long t = 0, newt = 1;
    long long r = p, newr = a;
    while (newr != 0) {
        long long quotient = r / newr;
        long long temp_t = t;
        t = newt;
        newt = temp_t - quotient * newt;
        long long temp_r = r;
        r = newr;
        newr = temp_r - quotient * r;
    }
    if (r > 1) {
        return 0;
    }
    if (t < 0) {
        t += p;
    }
    return t;
}
std::vector<double> frequ(std::vector<double> freq_1, long long x_n, long long m){
    double man;
    man = (x_n * 100.0) / m;
    for(int i=0;i<10;i++){
        if(man < (i+1)*10 and man >= i*10){
            freq_1[i] += 0.0001;
        }
    }
    if(man == 100){
        freq_1[9] += 0.0001;
    }
    return freq_1;
}
std::vector<double>frequ_1(std::vector<double> freq,double x_n){
    for(int i=0;i<6;i++){
        if(x_n<i-2 and x_n>=i-3){
            freq[i]+=0.0001;
        }
    }
    if(x_n==3){
        freq[5]+=0.0001;
    }
    return freq;
}
std::vector<double> freq_2(std::vector<double> freq_1,double x_n){
    x_n+=3.0;
    x_n/=6.0;
    for(int i=0;i<10;i++){
        if(x_n<(i+1)*0.1 and x_n>=i*0.1){
            freq_1[i]+=0.0001;
        }
    }
    if(x_n==1){
        freq_1[9]+=0.0001;
    }
    return freq_1;
}
std::vector<double> freq3(std::vector<double> freq_1, double μ, double x){
    for(int i=0;i<10;i++){
        if(x<μ*(i+1) and x>=i*μ){
            freq_1[i]+=0.0001;
        }
    }
    if(x>=10*μ){
        freq_1[9]+=0.0001;
    }
    return freq_1;

}
int main()
{
    long long num,x_n = 1;
    interface();
    std::vector<int> name;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::vector<double> freq={0,0,0,0,0,0,0,0,0,0};
    std::vector<double> freq_3={0,0,0,0,0,0};
    name.reserve(10000);
    std::cin >> num;
    if (num == 1) {
        long long a, c, m;
        double sum=0;
        lin_cond();
        std::cout << "Enter the m: ";
        std::cin >> m;
        std::cout << "Enter the a: ";
        std::cin >> a;
        std::cout << "Enter the c: ";
        std::cin >> c;
        if (lin_check_cond(m, a, c)) {
            for (int i = 0; i < 10000; i++) {
                name.push_back(x_n);
                freq = frequ(freq, x_n, m-1);
                x_n = linear(m, a, c, x_n);
            }
            output(freq);
        }
        else {
            std::cout << "You entered incorrect values";
        }
    }
    else if (num == 2) {
        long long a, c, m, d;
        quad_cond();
        std::cout << "Enter the m: ";
        std::cin >> m;
        std::cout << "Enter the a: ";
        std::cin >> a;
        std::cout << "Enter the c: ";
        std::cin >> c;
        std::cout << "Enter the d: ";
        std::cin >> d;
        if(quad_check_cond(a,c,m,d)){
            for (int i = 0; i < 10000; i++) {
                name.push_back(x_n);
                freq = frequ(freq, x_n, m-1);
                x_n = linear(m, d*x_n+a, c, x_n);
            }
            output(freq);
        }
        else std::cout << "You entered incorrect values";
    }
    else if (num == 3) {
        long long m,x_n1=1,temp;
        std::cout<<"Enter the m: ";
        std::cin>>m;
        for (int i = 0; i < 10000; i++) {
                name.push_back(x_n);
                freq = frequ(freq, x_n, m-1);
                temp = linear(m, 1, x_n1, x_n);
                x_n1=x_n;
                x_n=temp;
            }
            output(freq);
    }
    else if (num == 4) {
        long long p,a,c,x_n=3;
        inv_cond();
        std::cout<<"Or p is a prime number\n";
        std::cout << "Enter the p: ";
        std::cin >> p;
        std::cout << "Enter the a: ";
        std::cin >> a;
        std::cout << "Enter the c: ";
        std::cin >> c;
        std::vector<int> pr_num=fac_pr_num(p);
            if((pr_num.size()>3 and pr_num[pr_num.size()-1]==2 and a%4==1 and c%4==2) or pr_num.size()==2){
            long long inv_val;
            for (int i = 0; i < 10000; i++) {
                name.push_back(x_n);
                freq = frequ(freq, x_n, p-1);
                inv_val=inversion(x_n,p);
                x_n = linear(p, a, c, inv_val);
                if(x_n==0){
                    name.push_back(x_n);
                    x_n = c%p;
                }
            }
            output(freq);
        }
        else std::cout << "You entered incorrect values";
    }
    else if (num == 5) {
        long long p,a,c,x_n1=1,temp,y_n=2;
        inv_cond();
        std::cout << "Enter the p: ";
        std::cin >> p;
        std::cout << "Enter the a: ";
        std::cin >> a;
        std::cout << "Enter the c: ";
        std::cin >> c;
        std::vector<int> pr_num=fac_pr_num(p);
        if(pr_num.size()>3 and pr_num[pr_num.size()-1]==2 and a%4==1 and c%4==2){
            long long inv_val;
            for (int i = 0; i < 10000; i++) {
                temp = linear(p, 1, x_n1, x_n);
                x_n1=x_n;
                x_n=temp;
                inv_val=inversion(y_n,p);
                y_n = linear(p, a, c, inv_val);
                if(y_n==0){
                    y_n = c%p;
                }
                long long result = linear(p, 1, -y_n, x_n);
                name.push_back(result);
                freq = frequ(freq, result, p-1);
            }
            output(freq);
        }
        else std::cout << "You entered incorrect values";
    }
    else if (num == 6) {
        double x,sum=0;
        for (int i = 0; i < 10000; i++) {
            double sum = 0.0;
            for (int j = 0; j < 12; j++) {
                sum += dis(gen);
            }
            x = sum - 6.0;
            name.push_back(x);
            freq=freq_2(freq,x);
            freq_3=frequ_1(freq_3,x);
        }
        output_1(freq_3);
        output_3(freq);
    }
    else if (num == 7) {
        double v1, v2, x1, x2;
        for(int i = 0; i < 5000; i++) {
            double s=1.2;
            while(s >= 1.0) {
                v1 = 2 * dis(gen) - 1;
                v2 = 2 * dis(gen) - 1;
                s = v1 * v1 + v2 * v2;
            }
            x1 = v1 * std::sqrt(-2.0 * std::log(s) / s);
            x2 = v2 * std::sqrt(-2.0 * std::log(s) / s);
            name.push_back(x1);
            name.push_back(x2);
            freq = freq_2(freq, x1);
            freq_3 = frequ_1(freq_3, x1);
            freq = freq_2(freq, x2);
            freq_3 = frequ_1(freq_3, x2);
        }
        output_1(freq_3);
        output_3(freq);
    }
    else if (num == 8) {
        bool a=true;
        for(int i=0;i<10000;i++){
        double U=0,V,x;
        while (a==true){
        while(U==0){
            U=dis(gen);
        }
        V=dis(gen);
        x=std::sqrt(8/std::exp(1))*((V-0.5)/U);
        if(x*x<=-4*std::log(U)) a=false;
        }
        a=true;
        name.push_back(x);
        freq = freq_2(freq, x);
        freq_3 = frequ_1(freq_3, x);
            }
        output_1(freq_3);
        output_3(freq);    
     }
    else if (num == 9) {
        double x,temp=0,μ=-1;
        while(μ<=0){
        std::cout<<"Enter the positive μ: ";
        std::cin>>μ;
        if(μ<=0) std::cout<<"You entered incorrect value\n";
        }
        for(int i=0;i<10000;i++){
            x=-μ*std::log(dis(gen));
            name.push_back(x);
            freq=freq3(freq,μ,x);
        }
        output(freq);
    }
    else if (num == 10) {
        double X,Y,a=0,V,U;
        while(a<=1){
            std::cout<<"Enter a>1: ";
            std::cin>>a;
            if(a<=1) std::cout<<"You entered incorrect value\n";
        }
        while(name.size()!=10000){
            U=dis(gen);
            Y=std::tan(U*M_PI);
            X=sqrt(2*a-1)*Y+a-1;
            if(X>0){
                V=dis(gen);
                if(V<=(1+Y*Y)*std::exp((a-1)*std::log(X/(a-1))-sqrt(2*a-1)*Y)){
                    freq=freq3(freq,a,X);
                    name.push_back(X);
                }
            }
        }
        output(freq);
    }
    else {
        std::cout << "Incorrect choice\n";
        }
    system("pause");
    return 0;
}
