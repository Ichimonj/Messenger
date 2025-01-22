#pragma once
#include "Preprocessor.hpp"
#include <string>
#include <stdint.h>

class PhoneNumber
{
public:
    PhoneNumber();
    PhoneNumber(const string &phoneNumber);
    PhoneNumber(const char *phoneNumber);
    PhoneNumber(const PhoneNumber &other);
    PhoneNumber(PhoneNumber &&ex) noexcept;

    ~PhoneNumber();

public:
    friend ostream  &operator<<(ostream &os, const PhoneNumber &ex);
    PhoneNumber     &operator=(const PhoneNumber &ex);

public:
    void     setNumber(string phoneNumber);

    string   getNumber()const;
    uint16_t getCountryCode()const;
    uint16_t getOperatorCode()const;
    uint32_t getSubscriberNumber()const;

    const bool isValid() const;

private:
    void checkNumber();
    void setCodes();
    
    string  phone_number = "NULL";
    bool    is_valid     = false;

    uint16_t country_code       = 0;
    uint16_t operator_code      = 0;
    uint32_t subscriber_number  = 0;
};