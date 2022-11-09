

#pragma once
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <stdexcept>

#define SUPPORT_DIVISION 1 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1

class BigNum final
{
public:
    // constructors
    BigNum() {
        number.push_back(static_cast<uint8_t>('0'));
    }
    

    BigNum(int64_t n) {
        if (n < 0) {
            negativ = true;
            n *= -1;
        }

        number.insert(number.begin(), static_cast<uint8_t>('0' + (n % 10)));
        n /= 10;
        while (n > 0) {
            number.insert(number.begin(), static_cast<uint8_t>('0' + (n % 10)));
            n /= 10;
        }
    }

    explicit BigNum(const std::string& str) {

        if (str.size() <= 0 ) {
            throw(std::invalid_argument("Zero size"));
        }
        size_t i = 0;
        

        //check for +,-
        if (str[0] == '-') {
            negativ = true;
            i++;
        }
        
        if( str[0] == '+')
            i++;

        //0s deleting
        for (; i < str.size(); i++) {
            if (!isdigit(str[i]))
                throw(std::invalid_argument("Non digit value"));
            if (str[i] != '0') {
                break;
            }
                
        }
        
        for (; i < str.size(); i++) {
            if (!isdigit(str[i])) 
                throw(std::invalid_argument("Non difit value"));
            number.push_back( static_cast<uint8_t>(str[i]));
        }

        if (number.size() == 0)
            number.push_back(static_cast<uint8_t>('0'));
    }
    
    
    
    // copy
    BigNum(const BigNum& other) {
        number = other.number;  
        negativ = other.negativ;

    };

    //neviem ci treba
    BigNum& operator=(const BigNum& rhs) {
        number = rhs.number;
        negativ = rhs.negativ;

        return *this;
    };
    
    
    
    // unary operators
    const BigNum& operator+() const {
        return *this;
    };
    BigNum operator-() const {
        BigNum x;
        x.negativ = !negativ;
        x.number = number;
        return x;
    };
    
    
    
    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs) {
      
        * this = *this + rhs;
        return *this;
    }
    BigNum& operator-=(const BigNum& rhs) {
        *this = *this - rhs;
        return *this;
    }
    BigNum& operator*=(const BigNum& rhs) {
        *this = *this * rhs;
        return *this;
    }


#if SUPPORT_DIVISION == 1

    BigNum& operator/=(const BigNum& rhs) {
        *this = *this / rhs;
        return  *this;
    }

    BigNum& operator%=(const BigNum& rhs); // bonus
#endif
private:
    // here you can add private data and members, but do not add stuff to 
    // public interface, also you can declare friends here if you want
    std::vector<uint8_t> number;
    bool negativ = false;
    friend  std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);
    friend bool operator==(const BigNum& lhs, const BigNum& rhs);
    friend bool operator<(const BigNum& lhs, const BigNum& rhs);
    friend BigNum operator+(BigNum lhs, const BigNum& rhs);
    friend BigNum operator-(BigNum lhs, const BigNum& rhs);
    friend BigNum operator*(BigNum lhs, const BigNum& rhs);
    friend BigNum operator/(BigNum lhs, const BigNum& rhs);
};

bool vectorIsLess(const std::vector<uint8_t> lhs, const std::vector<uint8_t> rhs) {
    if (lhs.size() != rhs.size()) {
        return (lhs.size() < rhs.size());
    }
    for (size_t i = 0; i < lhs.size(); i++) {
        if (lhs[i] != rhs[i])
            return (lhs[i] < rhs[i]);
    }
    return false;
}

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigNum& lhs, const BigNum& rhs);
// idea is, that all comparison should work, it is not important how you do it
bool operator==(const BigNum& lhs, const BigNum& rhs) {
    if (lhs.number.size() != rhs.number.size() || lhs.negativ != rhs.negativ)
        return false;
    for (size_t i = 0; i < lhs.number.size(); i++) {
        if (lhs.number[i] != rhs.number[i])
            return false;
    }
    return true;
}
bool operator!=(const BigNum& lhs, const BigNum& rhs) {
    return !(lhs == rhs);
}
bool operator<(const BigNum& lhs, const BigNum& rhs) {
    if (lhs.negativ != rhs.negativ)
        return lhs.negativ;
    if (lhs.number.size() != rhs.number.size()) {
        return (lhs.number.size() < rhs.number.size()) ^ lhs.negativ;
    }

    for (size_t i = 0; i < lhs.number.size(); i++) {
        if (lhs.number[i] != rhs.number[i])
            return (lhs.number[i] < rhs.number[i]) ^ lhs.negativ;
    }
    return false;
}
bool operator>=(const BigNum& lhs, const BigNum& rhs) {
    return !(lhs < rhs);
}
bool operator>(const BigNum& lhs, const BigNum& rhs) {
    return (lhs >= rhs) && (lhs != rhs);
}
bool operator<=(const BigNum& lhs, const BigNum& rhs) {
    return !(lhs > rhs);
}

BigNum operator+(BigNum lhs, const BigNum& rhs) {
    int ri = static_cast<int>(rhs.number.size() - 1), ni = static_cast<int>(lhs.number.size() - 1);
    std::string res = "";
    bool neg = false;
    //result.number.erase(result.number.begin());

    
    int buf = 0;
    uint8_t ltem = 0, rtem = 0, offset =  '0';
    if ((!rhs.negativ && !lhs.negativ) || (rhs.negativ && lhs.negativ)) {
        // + (+) +  and - (+) -
        for (; ri >= 0 || ni >= 0 || buf > 0; ri--, ni--) {
            ltem = (ni >= 0 ? lhs.number[ni] : offset);
            rtem = (ri >= 0 ? rhs.number[ri] : offset);
            res.insert(res.begin(), offset + ((ltem + rtem + buf - 2 * offset) % 10));
            buf = (ltem + rtem + buf - 2*offset) / 10;
        }
        neg = rhs.negativ;

    }
    else {

        // + (+) - 
        if (!vectorIsLess(lhs.number, rhs.number)) {
            for (; ri >= 0 || ni >= 0; ri--, ni--) {
                ltem = (ni >= 0 ? lhs.number[ni] : offset);
                rtem = (ri >= 0 ? rhs.number[ri] : offset);
                res.insert(res.begin(), offset + ((10 + ltem - rtem - buf) % 10));
                buf = 1 - ((10 + ltem - rtem - buf) / 10);
            }
            neg = lhs.negativ;
        }
        else {
            //std::cout << "<=";
            for (; ri >= 0 || ni >= 0; ri--, ni--) {
                ltem = (ni >= 0 ? lhs.number[ni] : offset);
                rtem = (ri >= 0 ? rhs.number[ri] : offset);
                res.insert(res.begin(), offset + ((10 + rtem - ltem - buf) % 10));
                buf = 1 - ((10 + rtem - ltem  - buf) / 10);
            }
            neg = rhs.negativ;
        }

    }

    BigNum result(res);

    if (result != 0) {
        result.negativ = neg;
    }
    return result;
}
BigNum operator-(BigNum lhs, const BigNum& rhs) {
    return (lhs + ( - rhs));
}

BigNum operator*(BigNum lhs, const BigNum& rhs) {
    std::string res(lhs.number.size() + rhs.number.size() + 2, '0');
    int ri = static_cast<int>(rhs.number.size() - 1), li = static_cast<int>(lhs.number.size() - 1);
    int buf = 0, ltem = 0, rtem = 0;
    size_t akti = lhs.number.size() + rhs.number.size() + 1, negakti=0;
    char tmpres = '0';

    for (; ri >= 0 ; ri--) {

        for (li = static_cast<int>(lhs.number.size() - 1); li >=0 || buf > 0; li--) {
            //std::cout << rhs.number[ri] << "*" << lhs.number[li] << '\n';
            ltem = (li >= 0 ? (lhs.number[li] - '0') : 0);
            rtem = (ri >= 0 ? (rhs.number[ri] - '0') : 0);
            tmpres = '0' + ((buf + ltem * rtem + res[akti] - '0') % 10);
            buf = (buf + ltem * rtem + res[akti] - '0') / 10;
            res[akti] = tmpres;
            akti--;
            negakti++;
        }

        akti += negakti -1;
        negakti = 0;

    }

    BigNum result(res);
    if ((rhs.negativ ^ lhs.negativ) && result != 0)
        result.negativ = true;

    return result;
}

#if SUPPORT_DIVISION == 1


BigNum operator/(BigNum lhs, const BigNum& rhs) {
    std::vector<uint8_t> devv = {lhs.number[0]}, res;
    size_t i = 1;
 
    if (rhs == 0) {
        std::cout << "div  by 0\n";
        throw(std::logic_error("Division by zero"));
    }
    if (vectorIsLess(lhs.number, rhs.number)) {
        return BigNum();
    }
    
    while (vectorIsLess(devv, rhs.number)) {
        devv.push_back(lhs.number[i]);
        i++;
    }

    BigNum devisor, buff, rhsTemp;
    devisor.number = devv;
    devisor.negativ = false;
    buff.negativ = false;
    buff.number = rhs.number;
    rhsTemp = buff;
    size_t mark = i;

    while (1) {
        i = 0;
        buff.number = rhs.number;
        while (buff <= devisor) {
            buff += rhsTemp;
            i++;
        }
        buff -= rhsTemp;
        res.push_back(static_cast<uint8_t>('0' + i));
        if (mark >= lhs.number.size())
            break;
        devisor -= buff;

        devisor = devisor * 10 + (lhs.number[mark]-'0');
        mark++;
    }

    devisor.number = res;
    devisor.negativ = lhs.negativ ^ rhs.negativ;

    return devisor;
}
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif



std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs) {
    if (rhs.negativ)
        lhs << (uint8_t)'-';

    for (size_t i = 0; i < rhs.number.size(); i++) {
        lhs << rhs.number[i];
    }
    return lhs;
};

#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif



int main()
{

    BigNum b("2");
    BigNum c(b);
    BigNum d("-4378");
    BigNum e("+0");

    d /= 2;
    std::cout << d << '\n';
    b-= d;
    c-=d + b;
    c + b;
    std::cout << b-d <<" " << d - b << " "<<  c - b << "\n";

     std::cout << (c==b) << (c<=b) << (c>=b) << (c<b) << (c>b) << '\n';
    std::cout << (c == d) << (c <= d) << (c >= d) << (c < d) << (c > d) << '\n';
    std::cout << (c == e) << (c <= e) << (c >= e) << (c < e) << (c > e)<< '\n';
    std::cout << +c << " " << -c;
    //BigNum c("0");      
    //BigNum d("-0");
    //BigNum f("-012");
    //BigNum g("+100");
    //BigNum h("-182220");
    //BigNum i("00012");

    //std::cout <<b << " " << c << " " << d << " " << e << " " << f << " " << h << '\n';
    //try
    //{
    //    BigNum x("");

    //    BigNum j("00012a");
    //    BigNum k("a00012");
    //    BigNum l("00 012");
    //    BigNum m("0001-2");
    //}
    //catch (const std::exception& e)
    //{
    //    std::cout << e.what();
    //}


	return 0;
}
