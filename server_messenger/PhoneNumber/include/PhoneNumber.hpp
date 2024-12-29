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
    
    string phoneNumber_ = "NULL";
    bool isValid_ = false;

    uint16_t countryCode_       = 0;
    uint16_t operatorCode_      = 0;
    uint32_t subscriberNumber_  = 0;
};