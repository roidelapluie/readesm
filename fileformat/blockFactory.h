#ifndef BLOCKFACTORY_H
#define BLOCKFACTORY_H

#include "TopLevelBlock.h"

#include <QtCore/QSharedPointer>

QSharedPointer<TopLevelBlock> blockFactory(const DataPointer& start);

#endif
