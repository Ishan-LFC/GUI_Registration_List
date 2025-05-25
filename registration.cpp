#include "registration.h"

Registration::Registration(Person a)
    :m_Attendee(a), m_BookingDate(QDate::currentDate())
{
}

void Registration::setBookingDate(QDate date)
{
    m_BookingDate = date;
}

Person Registration::getAttendee() const
{
    return m_Attendee;
}

QDate Registration::getBookingDate() const
{
    return m_BookingDate;
}

double Registration::calculateFee() const
{
    return STANDARD_FEE;
}

QString Registration::toString() const
{
    QString details = "";

    details += "Registration type: Standard\n";
    details += "Attendee Details\n" + m_Attendee.toString();
    details += "Fee: " + QString::number(STANDARD_FEE); + "\n";
    details += "Registered On: " + m_BookingDate.toString() +"\n";

    return details;
}

QString Registration::getAdditionalInfo() const
{
    return "N/A";
}
