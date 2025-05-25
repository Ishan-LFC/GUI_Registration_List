#include "registrationtablemodel.h"
#include "registration.h"

RegistrationTableModel::RegistrationTableModel(QObject *parent)
    : QStandardItemModel{parent}
{
    setHorizontalHeaderLabels({"Name", "Affiliation", "Email", "Type", "Date", "Fee", "Qualification", "Category"});
}

void RegistrationTableModel::addRegistration(const Registration *r)
{
    QList<QStandardItem*> registrationValues;

    QString regType = "Standard", qualification = "N/A", category = "N/A", className = r->metaObject()->className();

    if(className == "StudentRegistration")
    {
        regType = "Student";
        qualification = r -> getAdditionalInfo();
    }
    else if(className == "GuestRegistration")
    {
        regType = "Guest";
        category = r ->getAdditionalInfo();
    }

    registrationValues.append(new QStandardItem(r -> getAttendee().getName()));
    registrationValues.append(new QStandardItem(r -> getAttendee().getAffiliation()));
    registrationValues.append(new QStandardItem(r -> getAttendee().getEmail()));
    registrationValues.append(new QStandardItem(regType));
    registrationValues.append(new QStandardItem(r->getBookingDate().toString("yyyy/MM/dd")));
    registrationValues.append(new QStandardItem(QString::number(r->calculateFee(), 'f', 2)));
    registrationValues.append(new QStandardItem(qualification));
    registrationValues.append(new QStandardItem(category));

    appendRow(registrationValues);
}
