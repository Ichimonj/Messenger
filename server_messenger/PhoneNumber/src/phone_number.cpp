#include "phone_number.hpp"
//constructors
PhoneNumber::PhoneNumber()
    : is_valid(false), phone_number("NULL")
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber()");
}
PhoneNumber::PhoneNumber(const string &phoneNumber)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(const string& phoneNumber)");
    this->phone_number = phoneNumber;
    checkNumber();
    if (is_valid)
        setCodes();
}
PhoneNumber::PhoneNumber(const char *phoneNumber)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(const char* phoneNumber)");
    this->phone_number = string(phoneNumber);
    checkNumber();
    if (is_valid)
        setCodes();
}
PhoneNumber::PhoneNumber(const PhoneNumber &other)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(const PhoneNumber& ex)");
    this->phone_number      = other.phone_number;
    this->is_valid          = other.is_valid;

    this->country_code      = other.country_code;
    this->operator_code     = other.operator_code;
    this->subscriber_number = other.subscriber_number;
}
PhoneNumber::PhoneNumber(PhoneNumber &&other) noexcept
    : phone_number(move(other.phone_number)), is_valid(other.is_valid)
{
    phDEBUG_LOG("DEBUG_phone", "PhoneNumber(PhoneNumber&& ex)");
    this->country_code      = other.country_code;
    this->operator_code     = other.operator_code;
    this->subscriber_number = other.subscriber_number;
}

//destructor
PhoneNumber::~PhoneNumber()
{
    phDEBUG_LOG("DEBUG_phone", "~PhoneNumber()");
}

//operators
ostream &operator<<(ostream &os, const PhoneNumber &ex)
{
    os << ex.phone_number;
    return os;
}
PhoneNumber &PhoneNumber::operator=(const PhoneNumber &other)
{
    phDEBUG_LOG("DEBUG_phone", "operator=");
    if (this != &other)
    {
        phDEBUG_LOG("DEBUG_phone", "assignment");
        this->phone_number      = other.phone_number;
        this->is_valid          = other.is_valid;

        this->country_code      = other.country_code;
        this->operator_code     = other.operator_code;
        this->subscriber_number = other.subscriber_number;
    }
    else
    {
        phDEBUG_LOG("DEBUG_phone", "not assignment");
    }
    return *this;
}

//sets
void PhoneNumber::setNumber(string phoneNumber)
{
    this->phone_number = phoneNumber;
    checkNumber();
    if (is_valid)
        setCodes();
}

//gets
string PhoneNumber::getNumber()const             { return this->phone_number; }
uint16_t PhoneNumber::getCountryCode()const      { return this->country_code; }
uint16_t PhoneNumber::getOperatorCode()const     { return this->operator_code; }
uint32_t PhoneNumber::getSubscriberNumber()const { return this->subscriber_number;}

//other member functions
const bool PhoneNumber::isValid() const { return is_valid; }
void PhoneNumber::checkNumber()
{
    phDEBUG_LOG("DEBUG_phone", "Start checkNumber()");

    if (phone_number.size() < MIN_NUMBER_LENGTH || phone_number.size() > MAX_NUMBER_LENGTH)
    {
        phDEBUG_LOG("DEBUG_phone", "End checkNumber() FALSE");
        is_valid = false;
        phone_number = "NULL";
        return;
    }
    else
    {
        for (auto &ex : phone_number)
        {
            if (!isdigit(ex))
            {
                phDEBUG_LOG("DEBUG_phone", "End checkNumber() FALSE");
                phone_number = "NULL";
                is_valid = false;
                return;
            }
        }
        phDEBUG_LOG("DEBUG_phone", "End checkNumber() TRUE");
        is_valid = true;
        return;
    }
}

void PhoneNumber::setCodes()
{
    this->country_code      = stoi(phone_number.substr(1, phone_number.size()   - 10));
    this->operator_code     = stoi(phone_number.substr(phone_number.size()      - 10, 3));
    this->subscriber_number = stoi(phone_number.substr(phone_number.size()      - 7));
}
