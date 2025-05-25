#ifndef REGISTRATIONFACTORY_H
#define REGISTRATIONFACTORY_H

#include <QString>

class Registration;
class StudentRegistration;
class GuestRegistration;
class Person;

class RegistrationFactory
{
public:
    static RegistrationFactory *getInstance();
    Registration *createRegistration(QString type, Person attendee, QString AdditionalInfo = "");

private:
    RegistrationFactory();
    static RegistrationFactory *instance;
};

#endif // REGISTRATIONFACTORY_H
