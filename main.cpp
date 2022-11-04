

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
    
    //zaporne osetrit
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
        //this.negativ = !negativ; ???
        return *this;
    };
    
    
    
    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs) {
        int ri = static_cast<int>(rhs.number.size()-1), ni = static_cast<int>(number.size()-1);
        std::vector<uint8_t> tmp;

        int buf = 0;
        if ((!rhs.negativ && !negativ) || (rhs.negativ && negativ)) {
            // + (+) + 
            for (; ri >= 0 || ni >= 0 || buf > 0; ri--, ni--) {
                tmp.insert(tmp.begin(), '0' + (((ni >= 0 ? number[ni] : '0') + (ri >= 0 ? rhs.number[ri] : '0') + buf - 2 * '0') % 10));
                buf = ((ni >= 0 ? number[ni] : '0') + (ri >= 0 ? rhs.number[ri] : '0') + buf - 2 * '0') / 10;
            
            }
        }
        else {
            //TODO
            // - (+) +
            for (; ri >= 0 || ni >= 0 ; ri--, ni--) {
                tmp.insert(tmp.begin(), '0' + (( 10 + (ni >= 0 ? number[ni] : '0') - (ri >= 0 ? rhs.number[ri] : '0') - buf) % 10));

                std::cout << (10 + (ni >= 0 ? number[ni] : '0')) << ' ' << ((ri >= 0 ? rhs.number[ri] : '0') - buf)  << '\n';
                buf = 1 - ((10 + (ni >= 0 ? number[ni] : '0') - (ri >= 0 ? rhs.number[ri] : '0') - buf ) / 10);

            }
       
        }

        
        number = tmp;
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
};

BigNum operator+(BigNum lhs, const BigNum& rhs);
BigNum operator-(BigNum lhs, const BigNum& rhs);
BigNum operator*(BigNum lhs, const BigNum& rhs);
#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

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
        return ( lhs.number.size() < rhs.number.size() )^lhs.negativ ;
    }

    for (size_t i = 0; i < lhs.number.size(); i++) {
        if (lhs.number[i] != rhs.number[i])
            return (lhs.number[i] < rhs.number[i])^lhs.negativ;
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

    BigNum b(-1392);
    BigNum c(b);
    BigNum d("-12292");
    BigNum e("+0");
    
    
    std::cout << (c==b) << (c<=b) << (c>=b) << (c<b) << (c>b) << '\n';
    std::cout << (c == d) << (c <= d) << (c >= d) << (c < d) << (c > d) << '\n';
    std::cout << (c == e) << (c <= e) << (c >= e) << (c < e) << (c > e)<< '\n';
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
