//#include <QJsonArray>
//#include <QJsonObject>
//#include <QJsonValue>
//#include <QJsonDocument>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile>


#include "gys_storage.h"
#include "gys_exceptions.h"


GYS::Storage::Storage() noexcept
{
}

GYS::Storage::~Storage()
{
}

void GYS::Storage::addRecords(const GYS::DataTable_Map &records)
{
    //throw GYS::NotImplemented(Q_FUNC_INFO);
    // May be better to use XML here, since its supports streams associated with devices?

    if (QFile::exists("storage_tmp.xml"))
        QFile::remove("storage_tmp.xml");

    QFile outFile("storage_tmp.xml");
    outFile.open(QIODevice::WriteOnly);
    QXmlStreamWriter outStream(&outFile);
    outStream.setAutoFormatting(true);

    outStream.writeStartDocument();
    outStream.writeStartElement("root");

    for (auto it = records.begin(); it != records.end(); ++it) {
        outStream.writeStartElement("client");
        outStream.writeAttribute("name", it.key().second);
        //outStream.writeTextElement("placeholder", "placeholder value");
        outStream.writeEndElement();
    }

    if (QFile::exists("storage.xml"))
    {
        QFile inFile("storage.xml");
        inFile.open(QIODevice::ReadOnly);
        QXmlStreamReader inStream(&inFile);

        // Re-read whole file :(
        while (!inStream.atEnd())
        {
            inStream.readNext();
            qDebug() << inStream.tokenString();
            if (!inStream.error())
            {
                switch (inStream.tokenType())
                {
                case QXmlStreamReader::StartElement:
                case QXmlStreamReader::EndElement:
                    qDebug() << "Elem " << inStream.name().toString();
                    if (inStream.name().toString() != "root")
                    {
                        outStream.writeCurrentToken(inStream);
                    }
                    break;
                case QXmlStreamReader::Characters:
                case QXmlStreamReader::Comment:
                case QXmlStreamReader::DTD:
                    qDebug() << "Chars " << inStream.text();
                    outStream.writeCurrentToken(inStream);
                    break;
                default:
                    break;
                }

            }
            else
            {
                qDebug() << "Error " << inStream.errorString()
                         << "line " << inStream.lineNumber() << "column "
                         << inStream.columnNumber() << "char "
                         << inStream.characterOffset();
            }
        }
        inFile.close();
    }

    outStream.writeEndElement();
    outStream.writeEndDocument();
    outFile.close();

    QFile::remove("storage.xml");
    QFile::rename("storage_tmp.xml", "storage.xml");
}

GYS::DataTable_Map GYS::Storage::getNextRecords(quint64 amount) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

void GYS::Storage::resetGetPosition()
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::addRecord(const GYS::DataItem_Pair   &record,
                             const GYS::DataRow_Vec     &data)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::overwriteRecord(const GYS::DataItem_Pair &record,
                                   const GYS::DataRow_Vec   &data)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataRow_Vec GYS::Storage::getRecordData(const GYS::DataItem_Pair &record) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataTable_Map GYS::Storage::getRecordsData(const GYS::DataRow_Vec &records) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::referRecords(const GYS::DataItem_Pair &record,
                                const QString            &refName,
                                const GYS::DataRow_Vec   &references)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataRow_Vec GYS::Storage::getRefers(const GYS::DataItem_Pair &record,
                                         const QString            &refname) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}
