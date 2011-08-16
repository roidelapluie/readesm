#ifndef BLOCKFACTORY_H
#define BLOCKFACTORY_H

#include "Block.h"

#include <QtCore/QSharedPointer>

QSharedPointer<Block> blockFactory(const constDataPointer& start);

#endif
