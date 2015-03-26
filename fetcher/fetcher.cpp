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

