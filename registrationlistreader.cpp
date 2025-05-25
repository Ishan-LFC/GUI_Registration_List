#include "registrationlistreader.h"
#include "registration.h"
#include "studentregistration.h"
#include "guestregistration.h"
#include "registrationfactory.h"

#include <QXmlStreamReader>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

RegistrationListReader::RegistrationListReader()
{

}

QList<Registration *> RegistrationListReader::getRegistrationListFromFile()
{
    RegistrationFactory *factory = RegistrationFactory::getInstance();
    QList<Registration *> list;

    QFile xmlfile(QFileDialog::getOpenFileName(nullptr,"Open XML file","","XML file (*.xml)"));

    if (!xmlfile.open(QIODevice::ReadOnly))
    {
        QMessageBox info;

        info.setText("Unable to Open " + xmlfile.fileName() + " file, an error occured");
        info.setWindowTitle("Error! opening file");
        info.setStandardButtons(QMessageBox::Ok);
        info.setDefaultButton(QMessageBox::Ok);
        info.exec();

        return list;
    }

    QXmlStreamReader doc;

    doc.setDevice(&xmlfile);

    while(!doc.atEnd() && !doc.hasError())
    {
        if(doc.isStartElement() && doc.name().toString() == "registration")
        {
            QString name, affliation, email, date, fee, categoryOrQualification, className = doc.attributes().value("type").toString();

            if(doc.readNextStartElement() && doc.name().toString() == "attendee")
            {
                if(doc.readNextStartElement() && doc.name().toString() == "name")
                    name = doc.readElementText();
                else continue;

                if(doc.readNextStartElement() && doc.name().toString() == "affliation")
                    affliation = doc.readElementText();
                else continue;

                if(doc.readNextStartElement() && doc.name().toString() == "email")
                    email = doc.readElementText();
                else continue;
            }
            else continue;

            doc.readNextStartElement();

            if(doc.readNextStartElement() && doc.name().toString() == "bookingdate")
                date = doc.readElementText();
            else continue;

            if(doc.readNextStartElement() && doc.name().toString() == "registrationfee")
                fee = doc.readElementText();
            else continue;

            if(className == "StudentRegistration")
                if(doc.readNextStartElement() && doc.name().toString() == "qualification")
                    categoryOrQualification = doc.readElementText();
                else continue;

            if(className == "GuestRegistration")
                if(doc.readNextStartElement() && doc.name().toString() == "category")
                    categoryOrQualification = doc.readElementText();
                else continue;


            Person attendee(name,affliation,email);
            Registration *reg;

            if(className == "Registration") reg = factory->createRegistration("Standard", attendee, categoryOrQualification);
            else if(className == "StudentRegistration") reg = factory->createRegistration("Student", attendee, categoryOrQualification);
            else reg = factory->createRegistration("Guest", attendee, categoryOrQualification);

            reg->setBookingDate(QDate::fromString(date,"yyyy/MM/dd"));

            list.append(reg);
        }

        doc.readNext();
    }

    xmlfile.close();

    QMessageBox info;

    info.setText(QString::number(list.size()) + " registrations were succesfully read from file: " + xmlfile.fileName());
    info.setWindowTitle("XML file read");
    info.setStandardButtons(QMessageBox::Ok);
    info.setDefaultButton(QMessageBox::Ok);
    info.exec();

    delete factory;
    return list;
}
