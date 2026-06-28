#include <bits/stdc++.h>
#include <iostream>
using namespace std;

#define fastio do { ios::sync_with_stdio(false); cin.tie(nullptr); } while (0)
#define ll long long
#define all(x) ((x).begin()), ((x).end())
typedef vector<ll> vll;

const ll MOD = 1000000007;

// -------- GCD (Euclidean Algorithm) --------
ll gcd(ll a, ll b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// -------- LCM --------
ll lcm(ll a, ll b) {
    return a / gcd(a, b) * b;
}

// -------- Primality Test (O(sqrt(n))) --------
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// -------- Sieve of Eratosthenes --------
vector<bool> sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }
    return is_prime;
}

// -------- Fast Power (modular exponentiation) --------
ll modpow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

// -------- Modular Inverse --------
ll modinv(ll a) {
    return modpow(a, MOD - 2);
}

// -------- Factorials (call init_factorials manually when needed) --------
vector<ll> fact, invfact;

void init_factorials(int n) {
    fact.resize(n + 1);
    invfact.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = (fact[i - 1] * i) % MOD;
    invfact[n] = modinv(fact[n]);
    for (int i = n - 1; i >= 0; i--) invfact[i] = (invfact[i + 1] * (i + 1)) % MOD;
}

// -------- nCr --------
ll nCr(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n - r] % MOD;
}

// -------- nPr --------
ll nPr(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[n - r] % MOD;
}

// -------- Print vector<long long> --------
void print_vector(const vll &v) {
    for (ll x : v) cout << x << " ";
    cout << '\n';
}

void solve() {
    cout << "Hello word " << endl;
}

int main() {
    fastio;

    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}