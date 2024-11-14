#include "PhoneNumber.hpp"
// constructors
PhoneNumber::PhoneNumber()
    : isValid_(false), phoneNumber_("NULL")
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber()");
}

PhoneNumber::PhoneNumber(const string &phoneNumber)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(const string& phoneNumber)");
    this->phoneNumber_ = phoneNumber;
    checkNumber();
    if (isValid_)
        setCodes();
}
PhoneNumber::PhoneNumber(const char *phoneNumber)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(const char* phoneNumber)");
    this->phoneNumber_ = string(phoneNumber);
    checkNumber();
    if (isValid_)
        setCodes();
}
PhoneNumber::PhoneNumber(const PhoneNumber &other)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(const PhoneNumber& ex)");
    this->phoneNumber_      = other.phoneNumber_;
    this->isValid_          = other.isValid_;

    this->countryCode_      = other.countryCode_;
    this->operatorCode_     = other.operatorCode_;
    this->subscriberNumber_ = other.subscriberNumber_;
}
PhoneNumber::PhoneNumber(PhoneNumber &&other) noexcept
    : phoneNumber_(move(other.phoneNumber_)), isValid_(other.isValid_)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(PhoneNumber&& ex)");
    this->countryCode_      = other.countryCode_;
    this->operatorCode_     = other.operatorCode_;
    this->subscriberNumber_ = other.subscriberNumber_;
}

// destructor
PhoneNumber::~PhoneNumber()
{
    phDEBUG_LOG("DEBUG_phone", "~PhoneNumber()");
}

// operators
ostream &operator<<(ostream &os, const PhoneNumber &ex)
{
    os << ex.phoneNumber_;
    return os;
}
PhoneNumber &PhoneNumber::operator=(const PhoneNumber &other)
{
    phDEBUG_LOG("DEBUG_phone", "operator=");
    if (this != &other)
    {
        phDEBUG_LOG("DEBUG_phone", "assignment");
        this->phoneNumber_      = other.phoneNumber_;
        this->isValid_          = other.isValid_;

        this->countryCode_      = other.countryCode_;
        this->operatorCode_     = other.operatorCode_;
        this->subscriberNumber_ = other.subscriberNumber_;
    }
    else
    {
        phDEBUG_LOG("DEBUG_phone", "not assignment");
    }
    return *this;
}

void PhoneNumber::setNumber(string phoneNumber)
{
    this->phoneNumber_ = phoneNumber;
    checkNumber();
    if (isValid_)
        setCodes();
}
string PhoneNumber::getNumber()const             { return this->phoneNumber_; }
uint16_t PhoneNumber::getCountryCode()const      { return this->countryCode_; }
uint16_t PhoneNumber::getOperatorCode()const     { return this->operatorCode_; }
uint32_t PhoneNumber::getSubscriberNumber()const { return this->subscriberNumber_;}

const bool PhoneNumber::isValid() const { return isValid_; }

void PhoneNumber::checkNumber()
{
    phDEBUG_LOG("DEBUG_phone", "Start checkNumber()");

    if (phoneNumber_.size() < MIN_NUMBER_LENGTH || phoneNumber_.size() > MAX_NUMBER_LENGTH)
    {
        phDEBUG_LOG("DEBUG_phone", "End checkNumber() FALSE");
        isValid_ = false;
        phoneNumber_ = "NULL";
        return;
    }
    else
    {
        for (auto &ex : phoneNumber_)
        {
            if (!isdigit(ex))
            {
                phDEBUG_LOG("DEBUG_phone", "End checkNumber() FALSE");
                phoneNumber_ = "NULL";
                isValid_ = false;
                return;
            }
        }
        phDEBUG_LOG("DEBUG_phone", "End checkNumber() TRUE");
        isValid_ = true;
        return;
    }
}

void PhoneNumber::setCodes()
{
    this->countryCode_      = stoi(phoneNumber_.substr(1, phoneNumber_.size()   - 10));
    this->operatorCode_     = stoi(phoneNumber_.substr(phoneNumber_.size()      - 10, 3));
    this->subscriberNumber_ = stoi(phoneNumber_.substr(phoneNumber_.size()      - 7));
}
