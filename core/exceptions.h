#ifndef GYS_EXCEPTIONS_H
#define GYS_EXCEPTIONS_H

#include <QString>

namespace GYS
{

class Exception
{
public:
    Exception(const QString &str)
        :m_str(str)
    { }

    virtual ~Exception()
    {

    }

    virtual QString what() const
    {
        return m_str;
    }
protected:
    QString m_str;
};

class NotImplemented : public GYS::Exception
{
public:
    NotImplemented(const QString &str)
        :Exception("Funtion is not implemented. ")
    {
        m_str += str;
    }

    virtual ~NotImplemented()
    {

    }
};

}


#endif // GYS_EXCEPTIONS_H
