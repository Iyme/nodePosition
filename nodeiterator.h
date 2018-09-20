#ifndef NODEITERATOR_H
#define NODEITERATOR_H

#include <QDomDocument>

class NodeIterator
{
public:
    NodeIterator();
    NodeIterator(QDomNode root);
    bool hasNext();
    QDomNode next();
private:
    QDomNode root, current;
    bool isHasNext;
};

#endif // NODEITERATOR_H
