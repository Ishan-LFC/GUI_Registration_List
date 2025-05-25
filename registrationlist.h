#ifndef REGISTRATIONLIST_H
#define REGISTRATIONLIST_H


#include <QList>

class QString;
class Registration;
class QMetaObject;

class RegistrationList
{
public:
    ~RegistrationList();
    bool addRegistration(Registration *r);
    bool isRegistered(QString n) const;
    double totalFee(QString t) const;
    int totalRegistrations(QString a) const;
    QList<Registration *> getRegistrationList() const;

private:
    QList<Registration *> m_AttendeeList;
};

#endif // REGISTRATIONLIST_H
