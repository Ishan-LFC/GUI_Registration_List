#include "person.h"

Person::Person(QString n, QString a, QString e)
    :m_Name(n), m_Affiliation(a), m_Email(e)
{

}

QString Person::getName() const
{
    return m_Name;
}

QString Person::getAffiliation() const
{
    return m_Affiliation;
}

QString Person::getEmail() const
{
    return m_Email;
}

QString Person::toString() const
{
    QString details = "";

    details += "Name: \t\t" + m_Name + "\n";
    details += "Affiliation: \t" + m_Affiliation + "\n";
    details += "Email: \t\t" + m_Email + "\n";

    return details;
}
