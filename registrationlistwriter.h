#ifndef REGISTRATIONLISTWRITER_H
#define REGISTRATIONLISTWRITER_H

#include <QList>

class QDomDocument;
class Registration;
class QMessageBox;
class QTextStream;
class QFileDialog;

class RegistrationListWriter
{
public:
    RegistrationListWriter();
    void writeToFIle(QList<Registration *> list) const;
};

#endif // REGISTRATIONLISTWRITER_H
