#ifndef REGISTRATIONLISTREADER_H
#define REGISTRATIONLISTREADER_H

#include <QList>

class QXmlStreamReader;
class Registration;
class QMessageBox;
class QTextStream;
class QFileDialog;
class RegistrationFactory;

class RegistrationListReader
{
public:
    RegistrationListReader();
    QList<Registration *> getRegistrationListFromFile();
};

#endif // REGISTRATIONLISTREADER_H
