#include <iostream>
#include <vector>
#include <string>



class BigInteger {
private:
    bool sign = 1;
    std::vector<long long> number;
    static const int base = 1e9;
    static const int baseSize = 9;
    bool IsZero() const;
    void DelZero();
    void AddZero(size_t length);
    int BinSearch(const BigInteger& carry, const BigInteger& N);
    void Reverse();
public:
    BigInteger() {}
    BigInteger(const int& value);
    BigInteger(const std::string& value);
    BigInteger(const BigInteger& value);

    size_t GetSize() const;
    bool   GetSign() const;
    std::string toString() const;

    BigInteger& operator=(const BigInteger& N);
    BigInteger& operator+=(const BigInteger& N);
    BigInteger& operator-=(const BigInteger& N);
    BigInteger& operator*=(const BigInteger& N);
    BigInteger& operator/=(const BigInteger& N);
    BigInteger& operator%=(const BigInteger& N);
    friend BigInteger operator-(BigInteger N);
    BigInteger& operator++();
    BigInteger& operator--();
    BigInteger  operator++(int);
    BigInteger  operator--(int);
    explicit operator bool() const;

    BigInteger Karatsuba(BigInteger N, BigInteger M);
    BigInteger Multiplication(BigInteger N, BigInteger M);
};

BigInteger::BigInteger(const std::string& value) {
    sign = (value[0] == '-' ? 0 : 1);
    for( int i = static_cast<int>(value.size()) - 1; i >= 0; i -= baseSize ) {
        long long cell = 0;
        for( int j = (i - baseSize + 1 >= 0 ? i - baseSize + 1 : 0) ; j <= i ; ++j ) {
            if( value[j] != '-' ) {
                cell = cell * 10 + (value[j] - '0');
            }
        }
        number.push_back(cell);
    }
}

BigInteger::BigInteger(const int& value) {
    std::string line = std::to_string(value);
    (*this) = BigInteger(line);
}

BigInteger::BigInteger(const BigInteger& value) {
    sign = value.sign;
    number = value.number;
}

void BigInteger::DelZero() {
    while( number.size() > 1 && number.back() == 0 ) {
        number.pop_back();
    }
}

size_t BigInteger::GetSize() const {
    return number.size();
}

bool BigInteger::GetSign() const {
    return sign;
}

bool BigInteger::IsZero() const {
    return number.size() == 1 && number[0] == 0 ? true : false;
}

BigInteger operator-(BigInteger N) {
    if( !(N.IsZero()) ) {
        N.sign = 1 - N.sign;
    }
    return N;
}

BigInteger abs(const BigInteger& N) {
    BigInteger other = N;
    if( !N.GetSign() ) {
        other = -other;
    }
    return other;
}

bool operator<(const BigInteger& N, const BigInteger& M) {
    if( N.GetSign() == M.GetSign() && N.GetSign() == 1 ) {
        if( N.GetSize() < M.GetSize() ) {
            return true;
        } else if( M.GetSize() < N.GetSize() ) {
            return false;
        } else {
            std::string left = N.toString();
            std::string right = M.toString();
            if( left.size() < right.size() ) {
                return true;
            }
            if( right.size() < left.size() ) {
                return false;
            }
            int i = 0;
            while( i < static_cast<int>(left.size()) - 1 && left[i] == right[i] ) {
                ++i;
            }
            return left[i] < right[i];
        }
    } else if( N.GetSign() == M.GetSign() ) {
        if( N.GetSize() > M.GetSize() ) {
            return true;
        } else if( M.GetSize() > N.GetSize() ) {
            return false;
        } else {
            std::string left = N.toString();
            std::string right = M.toString();
            if( left.size() > right.size() ) {
                return true;
            }
            if( right.size() > left.size() ) {
                return false;
            }
            int i = 0;
            while( i < static_cast<int>(left.size()) - 1 && left[i] == right[i] ) {
                ++i;
            }
            return left[i] > right[i];
        }
    } else if( N.GetSign() == 0 && M.GetSign() == 1 ) {
        return true;
    } else {
        return false;
    }
}

bool operator>(const BigInteger& N, const BigInteger& M)
{
    return M < N;
}

bool operator<=(const BigInteger& N, const BigInteger& M)
{
    return !(N > M);
}

bool operator>=(const BigInteger& N, const BigInteger& M)
{
    return !(N < M);
}

bool operator==(const BigInteger& N, const BigInteger& M)
{
    return (N <= M) && (M <= N);
}

bool operator!=(const BigInteger& N, const BigInteger& M)
{
    return !(N == M);
}

BigInteger& BigInteger::operator=(const BigInteger& N) {
    if( this == &N ) {
        return *this;
    }
    sign = N.sign;
    number = N.number;
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& N) {
    if( N.IsZero() ) {
        return *this;
    }
    if( sign == N.sign ) {
        int addition = 0;
        for( size_t i = 0; i < std::max(number.size(), N.number.size()) || addition; ++i ) {
            if( i == number.size() ) {
                number.push_back(0);
            }
            number[i] += (addition + ( i < N.number.size() ? N.number[i] : 0 ));
            addition = (number[i] >= base);
            if( addition ) {
                number[i] -= base;
            }
        }
    } else {
        (*this) -= (-N);
    }
    DelZero();
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& N) {
    if( N.IsZero() ) {
        return *this;
    }
    if( sign == N.sign ) {
        if( abs(*this) < abs(N) ) {
            BigInteger other = N;
            other -= (*this);
            (*this) = -other;
        } else {
            int addition = 0;
            for( size_t i = 0; i < N.number.size() || addition; ++i ) {
                number[i] -= (addition + (i < N.number.size() ? N.number[i] : 0));
                addition = (number[i] < 0);
                if( addition ) {
                    number[i] += base;
                }
            }
        }
    } else {
        (*this) += (-N);
    }
    DelZero();
    return *this;
}

BigInteger operator+(const BigInteger& N, const BigInteger& M) {
    BigInteger other = N;
    other += M;
    return other;
}

BigInteger operator-(const BigInteger& N, const BigInteger& M) {
    BigInteger other = N;
    other -= M;
    return other;
}

BigInteger operator*(const BigInteger& N, const BigInteger& M) {
    BigInteger other = N;
    other *= M;
    return other;
}

BigInteger operator/(const BigInteger& N, const BigInteger& M) {
    BigInteger other = N;
    other /= M;
    return other;
}

BigInteger operator%(const BigInteger& N, const BigInteger& M) {
    BigInteger other = N;
    other %= M;
    return other;
}

void BigInteger::Reverse() {
    int n = static_cast<int>(number.size());
    for( int i = 0; i < n / 2; ++i ) {
        std::swap(number[i], number[n - i - 1]);
    }
}

void BigInteger::AddZero(size_t length) {
    while( number.size() < length ) {
        number.push_back(0);
    }
}

BigInteger BigInteger::Multiplication(BigInteger N, BigInteger M) {
    std::vector<long long> buffer(N.number.size() + M.number.size());
    for( size_t i = 0; i < N.number.size(); ++i ) {
        long long carry = 0;
        for( size_t j = 0; j < M.number.size() || carry; ++j ) {
            long long current = buffer[i + j] + N.number[i] * (j < M.number.size() ? M.number[j] : 0) + carry;
            buffer[i + j] = current % base;
            carry = current / base;
        }
    }
    std::swap(N.number, buffer);
    N.DelZero();
    return N;
}

BigInteger BigInteger::Karatsuba(BigInteger N, BigInteger M) {
    if( N.number.size() < 32 && M.number.size() < 32 ) {
        return Multiplication(N, M);
    } else {
        size_t length = std::max(N.number.size(), M.number.size());
        N.AddZero(length);
        M.AddZero(length);
        int n = static_cast<int>(N.number.size());
        int m = n / 2;
        BigInteger firstN;
        for( int i = 0; i < n / 2; ++i ) {
            firstN.number.push_back(N.number[i]);
        }
        BigInteger secondN;
        for( int i = n / 2; i < n; ++i ) {
            secondN.number.push_back(N.number[i]);
        }
        BigInteger firstM;
        for( int i = 0; i < n / 2; ++i ) {
            firstM.number.push_back(M.number[i]);
        }
        BigInteger secondM;
        for( int i = n / 2; i < n; ++i ) {
            secondM.number.push_back(M.number[i]);
        }
        BigInteger firstProduct = Karatsuba(firstN, firstM);
        BigInteger secondProduct = Karatsuba(secondN, secondM);
        BigInteger thirdProduct = Karatsuba(firstN + secondN, firstM + secondM);
        BigInteger multiplier = thirdProduct - firstProduct - secondProduct;
        multiplier.Reverse();
        for( int i = 0; i < m; ++i ) {
            multiplier.number.push_back(0);
        }
        multiplier.Reverse();
        secondProduct.Reverse();
        for( int i = 0; i < 2 * m; ++i ) {
            secondProduct.number.push_back(0);
        }
        secondProduct.Reverse();
        BigInteger result = firstProduct + multiplier + secondProduct;
        result.DelZero();
        return result;
    }
}

BigInteger& BigInteger::operator*=(const BigInteger& N) {
    bool valueSign = (sign == N.sign ? 1 : 0);
    (*this) = Karatsuba(abs(*this),abs(N));
    sign = IsZero() ? 1 : valueSign;
    return *this;
}

int BigInteger::BinSearch(const BigInteger& buffer, const BigInteger& N) {
    int left = 0;
    int right = base;
    while( left + 1 < right ) {
        int middle = (left + right) / 2;
        if( middle * N < buffer ) {
            left = middle;
        } else {
            right = middle;
        }
    }
    return (right * N == buffer ? right : left);
}

BigInteger& BigInteger::operator/=(const BigInteger& N) {
    if( abs(*this) < abs(N) ) {
        (*this) = 0;
    } else {
        BigInteger other = *this;
        number.clear();
        bool valueSign = (sign == N.sign ? 1 : 0);
        int index = static_cast<int>(other.GetSize()) - 1;
        BigInteger buffer;
        while( index >= 0 && buffer.number.size() + 1 < N.number.size() ) {
            buffer.number.push_back(other.number[index]);
            --index;
        }
        buffer.Reverse();
        while( index >= 0 ) {
            buffer.Reverse();
            buffer.number.push_back(other.number[index]);
            index--;
            buffer.Reverse();
            buffer.DelZero();
            int bit = BinSearch(buffer, abs(N));
            number.push_back(bit);
            buffer -= bit * abs(N);
        }
        Reverse();
        DelZero();
        sign = IsZero() ? 1 : valueSign;
    }
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& N) {
    (*this) = (*this) - N * ((*this) / N);
    return *this;
}

BigInteger& BigInteger::operator++() {
    return (*this) += 1;
}

BigInteger& BigInteger::operator--() {
    return (*this) -= 1;
}

BigInteger  BigInteger::operator++(int) {
    BigInteger other = *this;
    (*this) += 1;
    return other;
}

BigInteger  BigInteger::operator--(int) {
    BigInteger other = *this;
    (*this) -= 1;
    return other;
}

std::istream& operator>>(std::istream& in, BigInteger& N) {
    std::string value;
    in >> value;
    N = BigInteger(value);
    return in;
}

void ReverseString(std::string& line) {
    int n = static_cast<int>(line.size());
    for( int i = 0; i < n / 2; ++i ) {
        std::swap(line[i], line[n - i - 1]);
    }
}

std::string BigInteger::toString() const {
    std::string value;
    if( !GetSign() ) {
        value.push_back('-');
    }
    int n = static_cast<int>(number.size());
    for( int i = n - 1; i >= 0; --i ) {
        std::string cell = std::to_string(number[i]);
        if( i != n - 1 && cell.size() < baseSize ) {
            ReverseString(cell);
            while( cell.size() < baseSize ) {
                cell.push_back('0');
            }
            ReverseString(cell);
        }
        value += cell;
    }
    return value;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& N) {
    out << N.toString();
    return out;
}

BigInteger::operator bool() const {
    return IsZero() ? false : true;
}

class Rational {
private:
    BigInteger numerator;
    BigInteger denominator;
    void Normalize();
    bool IsZero() const;
public:
    Rational() {};
    Rational(const BigInteger& value);
    Rational(const BigInteger& N, const BigInteger& M);
    Rational(const int& value);
    Rational(const int& a, const int& b);
    Rational(const Rational& value);

    Rational& operator=(const Rational& value);
    Rational& operator+=(const Rational& value);
    Rational& operator-=(const Rational& value);
    Rational& operator*=(const Rational& value);
    Rational& operator/=(const Rational& value);
    friend Rational operator-(Rational value);
    friend bool operator<(const Rational& N, const Rational& M);
    explicit operator double();

    std::string toString();
    std::string asDecimal(size_t precision = 0);
};

Rational::Rational(const int &value) {
    numerator = value;
    denominator = 1;
}

Rational::Rational(const int& a, const int& b) {
    numerator = a;
    denominator = b;
    Normalize();
}

Rational::Rational(const BigInteger& value) {
    numerator = value;
    denominator = 1;
}

Rational::Rational(const BigInteger& N, const BigInteger& M) {
    numerator = N;
    denominator = M;
    Normalize();
}

Rational::Rational(const Rational& value) {
    numerator = value.numerator;
    denominator = value.denominator;
    Normalize();
}

Rational operator-(Rational value) {
    if( !(value.IsZero()) ) {
        value.numerator *= -1;
    }
    return value;
}

void Rational::Normalize() {
    if( denominator < 0 ) {
        numerator *= -1;
        denominator *= -1;
    }
    BigInteger a = abs(numerator);
    BigInteger b = abs(denominator);
    while( b > 0 ) {
        a %= b;
        std::swap(a, b);
    }
    BigInteger gcd = a;
    numerator /= gcd;
    denominator /= gcd;
}

bool Rational::IsZero() const {
    return numerator == 0 && denominator == 1 ? true : false;
}

Rational& Rational::operator=(const Rational& value) {
    if( this == &value ) {
        return *this;
    }
    numerator = value.numerator;
    denominator = value.denominator;
    Normalize();
    return *this;
}

Rational& Rational::operator+=(const Rational& value) {
    numerator = numerator * value.denominator + denominator * value.numerator;
    denominator *= value.denominator;
    Normalize();
    return *this;
}

Rational& Rational::operator-=(const Rational& value) {
    numerator = numerator * value.denominator - denominator * value.numerator;
    denominator *= value.denominator;
    Normalize();
    return *this;
}

Rational& Rational::operator*=(const Rational& value) {
    numerator *= value.numerator;
    denominator *= value.denominator;
    Normalize();
    return *this;
}

Rational& Rational::operator/=(const Rational& value) {
    numerator *= value.denominator;
    denominator *= value.numerator;
    Normalize();
    return *this;
}

Rational operator+(const Rational& N, const Rational& M) {
    Rational other = N;
    other += M;
    return other;
}

Rational operator-(const Rational& N, const Rational& M) {
    Rational other = N;
    other -= M;
    return other;
}

Rational operator*(const Rational& N, const Rational& M) {
    Rational other = N;
    other *= M;
    return other;
}

Rational operator/(const Rational& N, const Rational& M) {
    Rational other = N;
    other /= M;
    return other;
}

bool operator<(const Rational& N, const Rational& M) {
    return N.numerator * M.denominator < N.denominator * M.numerator;
}

bool operator>(const Rational& N, const Rational& M)
{
    return M < N;
}

bool operator<=(const Rational& N, const Rational& M)
{
    return !(N > M);
}

bool operator>=(const Rational& N, const Rational& M)
{
    return !(N < M);
}

bool operator==(const Rational& N, const Rational& M)
{
    return (N <= M) && (M <= N);
}

bool operator!=(const Rational& N, const Rational& M)
{
    return !(N == M);
}

std::string Rational::toString() {
    Normalize();
    std::string value;
    value += numerator.toString();
    if( denominator != 1 ) {
        value.push_back('/');
        value += denominator.toString();
    }
    return value;
}

BigInteger TenPow(int x) {
    std::string s = "1";
    for( int i = 0; i < x; ++i ) {
        s += "0";
    }
    return BigInteger(s);
}

std::string Rational::asDecimal(size_t precision) {
    Normalize();
    std::string value;
    if( numerator == 0 ) {
        value = "0";
        return value;
    }
    BigInteger intPart = numerator / denominator;
    if( numerator < 0 && intPart == 0 && precision != 0 ) {
        value.push_back('-');
    }
    value += intPart.toString();
    if( precision == 0 ) {
        return value;
    }
    value.push_back('.');
    (*this) -= intPart;
    if( numerator < 0 ) {
        numerator *= -1;
    }
    (*this) *= TenPow(static_cast<int>(precision));
    BigInteger fractPart = numerator / denominator;
    std::string line = fractPart.toString();
    if( line.size() != precision ) {
        ReverseString(line);
        while( line.size() != precision ) {
            line.push_back('0');
        }
        ReverseString(line);
    }
    value += line;
    int i = static_cast<int>(value.size()) - 1;
    while( value[i] == '0' ) {
        value.pop_back();
        --i;
    }
    if( value[i] == '.' ) {
        value.pop_back();
    }
    return value;
}

Rational::operator double() {
    std::string current = asDecimal(324);
    double result = std::stod(current);
    return result;
}
