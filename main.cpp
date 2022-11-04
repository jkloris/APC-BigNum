

#pragma once
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <stdexcept>

#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
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
   /* BigNum& operator=(const BigNum& rhs) {
        number = rhs.number;
        negativ = rhs.negativ;

        return *this;
    };*/
    
    
    
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
    BigNum& operator-=(const BigNum& rhs);
    BigNum& operator*=(const BigNum& rhs);


#if SUPPORT_DIVISION == 1
    BigNum& operator/=(const BigNum& rhs); // bonus
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
};

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
    BigNum result;
    result.number.erase(result.number.begin());

    int buf = 0;
    uint8_t ltem = 0, rtem = 0, offset =  '0';
    if ((!rhs.negativ && !lhs.negativ) || (rhs.negativ && lhs.negativ)) {
        // + (+) +  and - (+) -
        for (; ri >= 0 || ni >= 0 || buf > 0; ri--, ni--) {
            ltem = (ni >= 0 ? lhs.number[ni] : offset);
            rtem = (ri >= 0 ? rhs.number[ri] : offset);
            result.number.insert(result.number.begin(), offset + ((ltem + rtem + buf - 2*offset) % 10));
            buf = (ltem + rtem + buf - 2*offset) / 10;
        }
        result.negativ = rhs.negativ;

    }
    else {
        //TODO treba porovnavanie number nie BigNum
        // + (+) - 
        if (lhs > rhs) {
            for (; ri >= 0 || ni >= 0; ri--, ni--) {
                ltem = (ni >= 0 ? lhs.number[ni] : offset);
                rtem = (ri >= 0 ? rhs.number[ri] : offset);
                result.number.insert(result.number.begin(), offset + ((10 + ltem - rtem - buf) % 10));
                buf = 1 - ((10 + ltem - rtem - buf) / 10);
            }
        }
        else {
            std::cout << "<=";
            for (; ri >= 0 || ni >= 0; ri--, ni--) {
                ltem = (ni >= 0 ? lhs.number[ni] : offset);
                rtem = (ri >= 0 ? rhs.number[ri] : offset);
                result.number.insert(result.number.begin(), offset + ((10 + rtem - ltem - buf) % 10));
                buf = 1 - ((10 + rtem - ltem  - buf) / 10);

            }
        }

    }



    return result;
}
BigNum operator-(BigNum lhs, const BigNum& rhs);
BigNum operator*(BigNum lhs, const BigNum& rhs);
#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
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

    BigNum b(-110);
    BigNum c(b);
    BigNum d("120");
    BigNum e("+0");
    std::cout << b+d << "\n";

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
