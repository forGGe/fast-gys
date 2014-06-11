#ifndef GYS_EXCEPTIONS_H
#define GYS_EXCEPTIONS_H

#include <QString>

namespace GYS {
class NotImplemented
{
public:
    NotImplemented(const char *str)
        :m_str("Funtion is not implemented. ")
    {
        m_str += str;
    }

    virtual QString what() const noexcept
    {
        return m_str;
    }

private:
    QString m_str;
};
}


#endif // GYS_EXCEPTIONS_H
