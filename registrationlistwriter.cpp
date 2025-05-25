#include "registrationlistwriter.h"
#include "registration.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>

RegistrationListWriter::RegistrationListWriter()
{}

void RegistrationListWriter::writeToFIle(const QList<Registration *> list) const
{
    QFile xmlfile(QFileDialog::getSaveFileName(nullptr,"Save XML file","","XML file (*.xml)"));

    if (!xmlfile.open(QIODevice::WriteOnly))
    {
        QMessageBox info;

        info.setText("Unable to write to " + xmlfile.fileName() + ", an error occured");
        info.setWindowTitle("Error! saving file");
        info.setStandardButtons(QMessageBox::Ok);
        info.setDefaultButton(QMessageBox::Ok);
        info.exec();

        return;
    }

    QDomDocument doc;

    QDomElement registrationList = doc.createElement("registrationList");

    foreach (const Registration *reg, list) {
        QDomElement registration = doc.createElement("registration");
        QString registrationType = reg -> metaObject() -> className();
        registration.setAttribute("type", registrationType);

        QDomElement attendee = doc.createElement("attendee");

        QDomElement name = doc.createElement("name");
        name.appendChild(doc.createTextNode(reg->getAttendee().getName()));

        QDomElement affliation = doc.createElement("affliation");
        affliation.appendChild(doc.createTextNode(reg->getAttendee().getAffiliation()));

        QDomElement email = doc.createElement("email");
        email.appendChild(doc.createTextNode(reg->getAttendee().getEmail()));

        attendee.appendChild(name);
        attendee.appendChild(affliation);
        attendee.appendChild(email);

        QDomElement bookingDate = doc.createElement("bookingdate");
        bookingDate.appendChild(doc.createTextNode(reg->getBookingDate().toString("yyyy/MM/dd")));

        QDomElement registrationFee = doc.createElement("registrationfee");
        registrationFee.appendChild(doc.createTextNode(QString::number(reg->calculateFee(),'f',2)));

        registration.appendChild(attendee);
        registration.appendChild(bookingDate);
        registration.appendChild(registrationFee);

        if(registrationType == "StudentRegistration")
        {
            QDomElement qualification = doc.createElement("qualification");
            qualification.appendChild(doc.createTextNode(reg->getAdditionalInfo()));
            registration.appendChild(qualification);
        }
        if(registrationType == "GuestRegistration")
        {
            QDomElement category = doc.createElement("category");
            category.appendChild(doc.createTextNode(reg->getAdditionalInfo()));
            registration.appendChild(category);
        }

        registrationList.appendChild(registration);
    }

    doc.appendChild(registrationList);

    QTextStream xmlOut(&xmlfile);
    xmlOut << doc.toString();
    xmlfile.close();

    QMessageBox info;

    info.setText(xmlfile.fileName() + " was saved succefully");
    info.setWindowTitle("XML file saved");
    info.setStandardButtons(QMessageBox::Ok);
    info.setDefaultButton(QMessageBox::Ok);
    info.exec();
}
