#include "guestregistration.h"

GuestRegistration::GuestRegistration(Person a, QString q)
    :Registration(a), m_Category(q)
{
}

double GuestRegistration::calculateFee() const
{
    return STANDARD_FEE/10;
}

QString GuestRegistration::toString() const
{
    QString details ="";

    details += "Registration type: Guest\n";
    details += "Category: " + m_Category+ "";
    details += "Guest Details\n" + Registration::getAttendee().toString();
    details += "Fee: " + QString::number(calculateFee()); + "\n";
    details += "Registered On: " + Registration::getBookingDate().toString() +"\n";

    return details;
}

QString GuestRegistration::getAdditionalInfo() const
{
    return m_Category;
}
