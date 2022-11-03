

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
    BigNum(const BigNum& other);
    BigNum& operator=(const BigNum& rhs);
    // unary operators
    const BigNum& operator+() const;
    BigNum operator-() const;
    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs);
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
bool operator==(const BigNum& lhs, const BigNum& rhs);
bool operator!=(const BigNum& lhs, const BigNum& rhs);
bool operator<(const BigNum& lhs, const BigNum& rhs);
bool operator>(const BigNum& lhs, const BigNum& rhs);
bool operator<=(const BigNum& lhs, const BigNum& rhs);
bool operator>=(const BigNum& lhs, const BigNum& rhs);

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
    BigNum a;

    BigNum b(-283092);


    BigNum c("0");      
    BigNum d("-0");
    BigNum e("+0");
    BigNum f("-012");
    BigNum g("+100");
    BigNum h("-182220");
    BigNum i("00012");

    std::cout <<b << " " << c << " " << d << " " << e << " " << f << " " << h << '\n';
    try
    {
        BigNum x("");

        BigNum j("00012a");
        BigNum k("a00012");
        BigNum l("00 012");
        BigNum m("0001-2");
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
	return 0;
}
