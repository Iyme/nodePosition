#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <QtCore>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "nodeiterator.h"

struct Node
{
    //节点标识
    QString name;

    //根据page7定义的，逻辑运算相关字段
    QString parent;
    QString firstChild;
    QString leftSibling;
    QString rightSibling;
    QString leftNeighbor;
    float x;
    float y;
    float prelim;
    float modifier;

    //译者用于计算左邻居的辅助参数
    int column;
    int row;
};


class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject();
    Q_INVOKABLE void start();

signals:
    void createNode(QString name, int x, int y);
    void drowLine(int x_pre, int y_pre, int x_new, int y_new);

private:
    Node getLeftMost(Node node, int level, int depth);
    void apportion(QString name);
    void firstWalk(int level, QString name);
    void secondWalk(int level, int modSom, QString name);
    QString getLeftNeighbor(int row, int column);

    void convertNode(int level, QDomNode node);

private:
    QMap<QString, Node> nodeList;
    int space = 40;
    int nodeSize = 20;
};

#endif // MYOBJECT_H
