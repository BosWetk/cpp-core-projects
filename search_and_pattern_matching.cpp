#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
using namespace std;
void menu(){
    cout<<"Choose what you want to do:"<<endl;
    string a[10]={"Exact Substring Search","Fuzzy Search (Landau-Vishkin Algorithm)","Subsequence Check",
        "Common Subsequences / Distance (Wagner-Fischer Algorithm)","Heaviest Increasing Subsequence (HIS)",
        "Maximum Repeated Substring (Naive Algorithm)","Common Elements of Two Arrays","Binary Search",
        "Interpolation Search","Binary Search with Nearest Nodes Detection"};
    for(int i=0;i<10;i++){
        cout<<i+1<<"."<<a[i]<<";"<<endl;
    }
    cout<<"Enter number: ";
}
std::vector<int> substring(string b,string a){
    long long m_len=b.length(), sub_len=a.length(),count=1;
    std::vector<int> ans;
    if(m_len>=sub_len){
        for(int i=0;i<=m_len-sub_len;i++){
            if(b[i]==a[0]){
                for(int j=1;j<sub_len;j++){
                    if(a[j]==b[i+j]){
                        count+=1;
                    }
                    else break;
                }
                if(count==sub_len){
                    ans.push_back(i);
                }
                count=1;
            }
        }
    }
    return ans;
}
std::vector<int> subsequence(string b,string a){
    long long j=0,m_len=b.length(), sub_len=a.length();
    std::vector<int> ans;
    for(int i=0;i<m_len;i++){
        if(b[i]==a[j]){
            ans.push_back(i);
            j++;
        }
        if(j==sub_len){
            break;
        }
    }
    if(ans.size()<sub_len){
        ans={};
    }
    return ans;
}

std::vector<char> common_elements(string a, string b) {
    long long i = 0, j = 0;
    long long n = a.length(), m = b.length();
    std::vector<char> result;
    while(i < n && j < m) {
        if(a[i] == b[j]){
            result.push_back(a[i]);
            i++;
            j++;
        }
        else if(a[i]<b[j]) {
            i++;
        }
        else {
            j++;
        }
    }
    return result;
}
long long binary(string s1, char f) {
    long long start = 0;
    long long end = s1.length() - 1;
    long long middle;
    if (s1[start] <= s1[end]) {
        while (start <= end) {
            middle = start + (end - start) / 2;
            if (s1[middle] == f) return middle;
            if (s1[middle] < f) {
                start = middle + 1;
            } else {
                end = middle - 1;
            }
        }
    }
    else {
        while (start <= end) {
            middle = start + (end - start) / 2;
            if (s1[middle] == f) return middle;
            if (s1[middle] > f) {
                start = middle + 1;
            } else {
                end = middle - 1;
            }
        }
    }
    return -1;
}
long long interpolation(string s, char f) {
    long long lo = 0;
    long long hi = s.length() - 1;
    while (lo <= hi && f >= s[lo] && f <= s[hi]) {
        if (lo == hi) {
            if (s[lo] == f) return lo;
            return -1;
        }
        long long pos = lo + ((double)(hi - lo) / (s[hi] - s[lo]) * (f - s[lo]));
        if (s[pos] == f) return pos;
        if (s[pos] < f) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }
    return -1;
}
int get_common_prefix(string row_p, int i, string text_t, int j) {
    int length = 0;
    while (i < row_p.length() && j < text_t.length() && row_p[i] == text_t[j]) {
        i++; j++; length++;
    }
    return length;
}
bool landau_vishkin(string pattern, string text, int k) {
    int m = pattern.length();
    int n = text.length();
    vector<vector<int>> L(k + 1, vector<int>(2 * k + 1, -2));
    for (int e = 0; e <= k; e++) {
        for (int d = -e; d <= e; d++) {
            int diag_idx = d + k;
            int row = -1;
            if (e > 0) {
                int res_del = L[e - 1][diag_idx - 1] + 1;
                int res_ins = L[e - 1][diag_idx + 1];
                int res_sub = L[e - 1][diag_idx] + 1; 
                row = max({res_del, res_ins, res_sub});
            } else if (d == 0) {
                row = 0;
            }
            if (row >= 0) {
                row += get_common_prefix(pattern, row, text, row + d);
            }
            L[e][diag_idx] = row;
            if (L[e][diag_idx] >= m) return true; 
        }
    }
    return false;
}
int wagnerFischer(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {dp[i][j] = 1 + min({dp[i - 1][j],dp[i][j - 1],dp[i - 1][j - 1]});}
        }
    }
    return dp[m][n];
}
string maxRepeatedSubstring(string s) {
    int n = s.length();
    string maxSub = "";
    for (int len = n - 1; len >= 1; len--) {
        for (int i = 0; i <= n - len; i++) {
            string sub = s.substr(i, len);
            if (s.find(sub, i + 1) != string::npos) {
                return sub;
            }
        }
    }
    return "No repeated substring found";
}
int nearestNodes(vector<double> arr, double value, int old) {
    int n = arr.size();
    if (value < arr[0]) return -1;
    if (value >= arr[n - 1]) return n - 1;
    if (old < 0) old = 0;
    if (old >= n) old = n - 1;
    int left = old, right = old, inc = 1;
    if (value >= arr[old]) {
        while (right < n - 1 && arr[right] <= value) {
            left = right;
            right = min(n - 1, right + inc);
            inc *= 2;
        }
    } else {
        while (left > 0 && arr[left] > value) {
            right = left;
            left = max(0, left - inc);
            inc *= 2;
        }
    }
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (mid + 1 < n && arr[mid] <= value && value < arr[mid + 1]) {
            return mid;
        }
        if (arr[mid] <= value) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return left;
}
void his_logic(vector<int> s, vector<int> w) {
    int n = s.size();
    if (n == 0) return;
    vector<int> max_w(n);
    vector<int> prev_idx(n, -1);
    for (int i = 0; i < n; i++) {
        max_w[i] = w[i];
        for (int j = 0; j < i; j++) {
            if (s[j] < s[i] && max_w[j] + w[i] > max_w[i]) {
                max_w[i] = max_w[j] + w[i];
                prev_idx[i] = j;
            }
        }
    }
    int best_end = 0;
    for (int i = 1; i < n; i++) {
        if (max_w[i] > max_w[best_end]) {
            best_end = i;
        }
    }
    vector<int> path;
    int temp = best_end;
    while (temp != -1) {
        path.push_back(s[temp]);
        temp = prev_idx[temp];
    }
    cout << "Heaviest sequence: ";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << path[i] << (i == 0 ? "" : " -> ");
    }
    cout << "\nTotal weight: " << max_w[best_end] << endl;
}
int main(){
    menu();
    int num_m;
    cin>>num_m;
    cin.ignore();
    if(num_m==1){
        string main,sub;
        std::vector<int> ans;
        cout<<"Enter the string: ";
        cin>>main;
        cout<<"Enter the subsequence: ";
        cin>>sub;
        ans=substring(main,sub);
        if(ans.empty()){
            cout<<"Not Found"<<endl;
        }
        else{
            for(int i=0;i<ans.size();i++){
                cout<<ans[i]<<" ";
            }
        }
    }
    else if(num_m == 2) {
        string text, pattern;
        int k;
        cout << "Enter text: ";
        cin >> text;
        cout << "Enter pattern to find: ";
        cin >> pattern;
        cout << "Enter max allowed differences (k): ";
        cin >> k;
        if (landau_vishkin(pattern, text, k)) {
            cout << "Pattern found with at most " << k << " differences!" << endl;
        } else {
            cout << "Pattern not found within specified differences." << endl;
        }
    }
    else if(num_m==3){
        string main,sub;
        std::vector<int> ans;
        cout<<"Enter the string: ";
        cin>>main;
        cout<<"Enter the substring: ";
        cin>>sub;
        ans=subsequence(main,sub);
        if(ans.empty()){
            cout<<"Not Found"<<endl;
        }
        else{
            for(int i=0;i<ans.size();i++){
                cout<<ans[i]<<" ";
            }
        }
    }
    else if(num_m == 4) {
        string s1, s2;
        cout << "Enter first string: ";
        cin >> s1;
        cout << "Enter second string: ";
        cin >> s2;
        int distance = wagnerFischer(s1, s2);
        cout << "Difference: " << distance << endl;
    }
    else if (num_m == 5) {
        int n;
        cout << "Enter number of elements: ";
        cin >> n;
        vector<int> s(n), w(n);
        cout << "Enter elements (values): ";
        for (int i = 0; i < n; i++) cin >> s[i];
        cout << "Enter weights: ";
        for (int i = 0; i < n; i++) cin >> w[i];
        his_logic(s, w);
    }
    else if(num_m == 6) {
        string main;
        cout << "Enter the string to find repeated fragments: ";
        cin >> main;
        string repeated = maxRepeatedSubstring(main);
        if (repeated != "No repeated substring found") {
            cout << "Maximum repeated substring: " << repeated << endl;
            cout << "Length: " << repeated.length() << endl;
        } else {
            cout << repeated << endl;
        }
    }
    else if(num_m==7){
        string s1,s2;
        cout<<"Enter the first array: ";
        cin>>s1;
        cout<<"Enter the second array: ";
        cin>>s2;
        std::vector<char>ans=common_elements(s1, s2);
        if(ans.empty()){
            cout<<"Not found";
        }
        else{
            cout<<"Common elements: ";
            for(int i = 0; i < ans.size(); i++){
                cout << ans[i] << " ";
            }
        }
    }
    else if(num_m == 8) {
        string s1;
        char find;
        cout << "Enter the sorted array: ";
        cin >> s1;
        cout << "What we need to find: ";
        cin >> find;
        long long result = binary(s1, find);
        if (result != -1) {
            cout << "Element found at index: " << result << endl;
        } else {
            cout << "Element not found!" << endl;
        }
    }
    else if(num_m == 9) {
        string s1;
        char find;
        cout << "Enter the sorted array: ";
        cin >> s1;
        cout << "What we need to find: ";
        cin >> find;
        long long result = interpolation(s1, find);
        if (result != -1) {
            cout << "Found at index: " << result << endl;
        } else {
            cout << "Not found!" << endl;
        }
    }
    else if (num_m == 10) {
        int n, old;
        double val;
        
        cout << "Enter number of nodes: ";
        cin >> n;
        vector<double> nodes(n);
        
        cout << "Enter sorted nodes: ";
        for (int i = 0; i < n; i++) {
            cin >> nodes[i];
        }
        
        cout << "Enter value to find: ";
        cin >> val;
        cout << "Enter starting index: ";
        cin >> old;

        int res = nearestNodes(nodes, val, old);

        if (res == -1) {
            cout << "Value is below the first node." << endl;
        } else if (res == n - 1) {
            cout << "Value is at or above the last node." << endl;
        } else {
            cout << "Value is between index " << res << " and " << res + 1 << endl;
            cout << "Interval: [" << nodes[res] << ", " << nodes[res + 1] << ")" << endl;
        }
    }
    else{
        cout<<"You entered incorrect number!";
    }
}
