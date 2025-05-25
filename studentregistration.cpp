#include "studentregistration.h"

StudentRegistration::StudentRegistration(Person a, QString q)
    :Registration(a), m_Qualification(q)
{
}

double StudentRegistration::calculateFee() const
{
    return STANDARD_FEE/2;
}

QString StudentRegistration::toString() const
{
    QString details ="";

    details += "Registration type: Student\n";
    details += "Qualification: " + m_Qualification + "";
    details += "Student Details\n" + Registration::getAttendee().toString();
    details += "Fee: " + QString::number(calculateFee()); + "\n";
    details += "Registered On: " + Registration::getBookingDate().toString() +"\n";

    return details;
}

QString StudentRegistration::getAdditionalInfo() const
{
    return m_Qualification;
}
