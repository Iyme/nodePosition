#include "nodeiterator.h"

NodeIterator::NodeIterator()
{
}

NodeIterator::NodeIterator(QDomNode root)
{
    this->root = root;
    current = root;
}

bool NodeIterator::hasNext(){
    if (!root.isNull()) {
        //是否当前节点有子节点
        if (!current.isNull() && current.hasChildNodes()) {
            //将第一个子节点变成当前节点
            current = current.firstChildElement();
            isHasNext = true;
        } else if (!current.isNull()&& !current.nextSiblingElement().isNull()) {
            current = current.nextSiblingElement();
            isHasNext = true;
        } else if (!current.isNull()) {
            while (!current.isNull() && current != root && current.nextSiblingElement().isNull()) {
                current = current.parentNode();
            }
            if (!current.isNull() && current != root) {
                current = current.nextSiblingElement();
                isHasNext = true;
            } else {
                isHasNext = false;
            }
        } else {
            isHasNext = false;
        }
    } else {
        isHasNext = false;
    }
    return isHasNext;
}

QDomNode NodeIterator::next() {
    return current;
}
