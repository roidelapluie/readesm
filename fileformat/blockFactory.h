#ifndef BLOCKFACTORY_H
#define BLOCKFACTORY_H

#include "Block.h"
#include <QSharedPointer>

QSharedPointer<Block> blockFactory(const constDataPointer& start);

#endif
