#include "fetcher.h"

Fetcher::Fetcher(QObject *parent)
    :QObject(parent)
{

}

Fetcher::~Fetcher()
{

}

void Fetcher::start()
{
    this->handleStart();
}

void Fetcher::process(QSqlRecord rec)
{
    this->handleProcess(rec);
}

void Fetcher::complete()
{
    this->handleComplete();
}
