#include "discipline.h"

Discipline::Discipline()
{
    this->m_name = "None";

}

Discipline::Discipline(const QString& name)
{
    this->m_name = name;
}

Discipline::~Discipline()
{

}

void Discipline::rename(QString& newName)
{
    this->m_name = newName;
}

void Discipline::setEnabled(bool enabled)
{
    m_isConductible = enabled;
}

void Discipline::setConductRange(QDate &start, QDate &finish)
{
    m_range.first = start;
    m_range.second = finish;

    validate();
}

void Discipline::addTeachDay(QString& day)
{
    m_teachDays.append(day);
}
void Discipline::removeTeachDay(QString& day)
{
    m_teachDays.removeOne(day);
}
bool Discipline::isEnabled() const
{
    return m_isConductible;
}

bool Discipline::isValid() const
{
    return m_isValid;
}

QString Discipline::getName() const
{
    return this->m_name;
}

const QPair<QDate, QDate> &Discipline::getTeachRange() const
{
    return m_range;
}

const QList<QString> &Discipline::getCourseDays() const
{
    return m_teachDays;
}

bool Discipline::operator<(const Discipline &other) const
{
    return m_name < other.m_name;
}

bool Discipline::operator==(const Discipline &other) const
{
    return m_name == other.m_name;
}

bool Discipline::operator!=(const Discipline &other) const
{
    return m_name != other.m_name;
}

void Discipline::validate()
{
    QDate currentDate = QDate::currentDate();

    if(currentDate.year() > m_range.second.year()){
                m_isValid = false;
                m_isConductible = false;
    }else{
        m_isValid  = true;

        if( currentDate.month() >= 2 && currentDate.month() <= 6)
        {
            if(m_range.second.month() > currentDate.month()+3){
                m_isConductible = false;
            }
            else if(m_range.second.month() == currentDate.month()){
                if(m_range.second.day() <= currentDate.day()){
                    m_isConductible = false;
                }
                else{
                    m_isConductible = true;
                }
            }
            else{
                m_isConductible = true;
            }

        }
        else if(currentDate.month() >= 9 && currentDate.month() <= 12)
        {
            if(m_range.second.month() < currentDate.month()){
                m_isConductible = false;
            }
            else if(m_range.second.month() == currentDate.month()){
                if(m_range.second.day() <= currentDate.day()){
                    m_isConductible = false;
                }
                else{
                    m_isConductible = true;
                }
            }
            else{
                m_isConductible = true;
            }
        }
    }
}


void Discipline::write(QJsonObject &json)const
{

    json["name"] = m_name;
    json["rangeFrom"] = m_range.first.toString("dd.MM.yyyy");
    json["rangeTo"] = m_range.second.toString("dd.MM.yyyy");

    QJsonArray days;
    for(auto&i:m_teachDays)
        days.append(i);

    if(!days.isEmpty())
        json["teachDays"] = days;
}

void Discipline::read(const QJsonObject &json)
{
    if(json.contains("name") && json["name"].isString())
        m_name = json["name"].toString();
    if(json.contains("rangeFrom") && json["rangeFrom"].isString())
        m_range.first = QDate::fromString(json["rangeFrom"].toString(),"dd.MM.yyyy");
    if(json.contains("rangeTo") && json["rangeTo"].isString()){
        m_range.second = QDate::fromString(json["rangeTo"].toString(), "dd.MM.yyyy");
    }

    validate();

    if(json.contains("teachDays") && json["teachDays"].isArray()){
        QJsonArray days = json["teachDays"].toArray();
        for(const auto&i:days)
            m_teachDays.append(i.toString());
    }

}
