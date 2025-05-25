#ifndef REGISTRATIONTABLEMODEL_H
#define REGISTRATIONTABLEMODEL_H

#include <QStandardItemModel>

class Registration;

class RegistrationTableModel : public QStandardItemModel
{
public:
    explicit RegistrationTableModel(QObject *parent = nullptr);
    void addRegistration(const Registration *r);
};

#endif // REGISTRATIONTABLEMODEL_H
