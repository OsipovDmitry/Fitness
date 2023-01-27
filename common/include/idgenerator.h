#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <qglobal.h>

template <typename IdType>
class IdGenerator
{
public:
    IdGenerator() : m_freeId(static_cast<IdType>(0)) {}
    IdType generate() { return m_freeId++; }
private:
    IdType m_freeId;
};

#endif // IDGENERATOR_H
