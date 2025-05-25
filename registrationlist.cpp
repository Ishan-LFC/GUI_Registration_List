#include "registrationlist.h"
#include "registration.h"
#include "studentregistration.h"
#include "guestregistration.h"

#include <QMetaObject>
#include <QString>

RegistrationList::~RegistrationList()
{
    qDeleteAll(m_AttendeeList);
}

bool RegistrationList::addRegistration(Registration *r)
{
    foreach (Registration *reg, m_AttendeeList) {
        if(r -> getAttendee().getEmail() == reg -> getAttendee().getEmail() &&
            r -> getAttendee().getName() == reg -> getAttendee().getName())
            return false;
    }

    m_AttendeeList.append(r);

    return true;
}

bool RegistrationList::isRegistered(QString n) const
{
    foreach (Registration *r, m_AttendeeList) {
        if(r -> getAttendee().getName() == n)
            return true;
    }

    return false;
}

double RegistrationList::totalFee(QString t) const
{
    double total = 0;
    QString className;

    foreach (Registration *r, m_AttendeeList) {
        className = r -> metaObject() -> className();

        if(t == "All" || t == className)
            total += r -> calculateFee();
    }

    return total;
}

int RegistrationList::totalRegistrations(QString a) const
{
    int total = 0;
    QString className;

    foreach (Registration *r, m_AttendeeList) {
        className = r -> metaObject() -> className();

        if(a == "All" || a == className)
            total++;
    }

    return total;
}


QList<Registration *> RegistrationList::getRegistrationList() const
{
    return m_AttendeeList;
}
