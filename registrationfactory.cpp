#include "registrationfactory.h"
#include "registration.h"
#include "guestregistration.h"
#include "studentregistration.h"

RegistrationFactory *RegistrationFactory::instance = 0;

RegistrationFactory *RegistrationFactory::getInstance()
{
    if(!instance) instance = new RegistrationFactory;

    return instance;
}

Registration *RegistrationFactory::createRegistration(QString type, Person attendee, QString AdditionalInfo)
{
    if(type == "Standard") return new Registration(attendee);
    else if(type == "Student") return new StudentRegistration(attendee, AdditionalInfo);
    else if(type == "Guest") return new GuestRegistration(attendee, AdditionalInfo);
    else return nullptr;
}

RegistrationFactory::RegistrationFactory()
{

}
