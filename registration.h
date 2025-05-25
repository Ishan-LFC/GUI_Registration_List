#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "person.h"
#include <QDate>
#include <QObject>

class Registration : public QObject
{
    Q_OBJECT
public:
    Registration(Person a);
    void setBookingDate(QDate date);
    Person getAttendee() const;
    QDate getBookingDate() const;
    virtual double calculateFee() const;
    virtual QString toString() const;
    virtual QString getAdditionalInfo() const;

    static const double STANDARD_FEE;

private:
    Person m_Attendee;
    QDate m_BookingDate;
};

#endif // REGISTRATION_H
