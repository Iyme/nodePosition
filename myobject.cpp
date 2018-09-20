#include "myobject.h"

Node MyObject::getLeftMost(Node node, int level, int depth)
{
    if(depth <= level)
        return node;
    else if(node.firstChild.isEmpty())
        return Node();
    else
    {
        Node rightMost = nodeList.value(node.firstChild);
        Node leftMost = getLeftMost(rightMost, level + 1, depth);
        while (leftMost.name.isEmpty() && !rightMost.rightSibling.isEmpty()) {
            rightMost = nodeList.value(rightMost.rightSibling);
            leftMost = getLeftMost(rightMost, level + 1, depth);
        }
        return leftMost;
    }
}

//当前节点有子节点，有左节点
void MyObject::apportion(QString name)
{
    Node curNode = nodeList.value(name);
    Node leftMost = nodeList.value(curNode.firstChild);     //当前节点下移一行，取最左节点
    Node neighbor = nodeList.value(leftMost.leftNeighbor);  //下移后左节点的左兄弟，进行比较

    int CompareDepth = 1;
    float leftModSum, rightModSum;
    //计算当前行理论最佳位置

    while (!leftMost.name.isEmpty() && !neighbor.name.isEmpty()) {
        //两个节点都存在，可进行比较时，累加其父节点的modifier(即理想位置)
        leftModSum = 0.0;
        rightModSum = 0.0;
        Node ancestorLeftmost = leftMost;
        Node ancestorNeighbor = neighbor;

        //从子节点往上遍历到当前深度
        for(int i = 0; i < CompareDepth; i++)
        {
            ancestorLeftmost = nodeList.value(ancestorLeftmost.parent);
            ancestorNeighbor = nodeList.value(ancestorNeighbor.parent);

            if(ancestorLeftmost.name.isEmpty() || ancestorNeighbor.name.isEmpty())
                continue;
            rightModSum += ancestorLeftmost.modifier;
            leftModSum += ancestorNeighbor.modifier;
        }

        float moveDistance = (neighbor.prelim + leftModSum + space + nodeSize) - (leftMost.prelim + rightModSum);

        //计算后应向右偏移，当前列所有节点分摊右移
        if(moveDistance > 0.0)
        {
            Node tempPtr = curNode;
            int leftSibling = 0;

            //往左数到子节点邻居的父节点，
            while (!tempPtr.name.isEmpty() && tempPtr.name != ancestorNeighbor.name) {
                leftSibling = leftSibling + 1;
                tempPtr = nodeList.value(tempPtr.leftSibling);
            }

            //从当前节点数到邻居父节点，对所有节点移位
            if(!tempPtr.name.isEmpty())
            {
                int portion = moveDistance / leftSibling;
                tempPtr = curNode;
                while (tempPtr.name != ancestorNeighbor.name) {//此处判定为不等，与原文相反（或者我对原文语言理解错误）
                    tempPtr.prelim = tempPtr.prelim + moveDistance;
                    tempPtr.modifier = tempPtr.modifier + moveDistance;
                    moveDistance = moveDistance - portion;
                    nodeList.insert(tempPtr.name, tempPtr);
                    tempPtr = nodeList.value(tempPtr.leftSibling);
                }
            }
            else
                return;
        }

        CompareDepth += 1;
        if(leftMost.firstChild.isEmpty())
            leftMost = getLeftMost(curNode, 0, CompareDepth);//当前深度，查找第一个有子节点的，取最左子节点
        else
            leftMost = nodeList.value(leftMost.firstChild);

        neighbor = nodeList.value(leftMost.leftNeighbor);

    }
}

void MyObject::firstWalk(int level, QString name)
{
    Node curNode = nodeList.value(name);
    //当前节点为叶节点
    if(curNode.firstChild.isEmpty())
    {
        //有左兄弟prelim计算相对位置
        if(!curNode.leftSibling.isEmpty())
        {
            Node leftSibling = nodeList.value(curNode.leftSibling);
            curNode.prelim = leftSibling.prelim + space + nodeSize;
        }else//没有左兄弟prelim记0
            curNode.prelim = 0;

        nodeList.insert(curNode.name, curNode);

    }
    else //非叶节点计算：
    {
        //先计算所有子节点位置
        int leftPrelim = 0;
        int rightPrelim = 0;
        Node child = nodeList.value(curNode.firstChild);
        firstWalk(level + 1, child.name);
        child = nodeList.value(child.name);
        leftPrelim = child.prelim;
        while (!child.rightSibling.isEmpty()) {
            child = nodeList.value(child.rightSibling);
            firstWalk(level + 1, child.name);
        }
        child = nodeList.value(child.name);
        rightPrelim = child.prelim;

        int midPoint = (leftPrelim + rightPrelim) / 2; //这是子节点集合的中心位置

        //如果当前节点有左兄弟，prelim计算相对位置, 计算modifier
        if(!curNode.leftSibling.isEmpty())
        {
            Node leftSibling = nodeList.value(curNode.leftSibling);
            curNode.prelim = leftSibling.prelim + space + nodeSize;
            curNode.modifier = curNode.prelim - midPoint;
            //计算子树移动距离，当前深度节点分摊偏移
            nodeList.insert(curNode.name, curNode);
            apportion(curNode.name);
        }
        else//没有左兄弟，prelim记子节点集合中间位置
        {
            curNode.prelim = midPoint;
            nodeList.insert(curNode.name, curNode);
        }
    }
    //替换更改后信息
//    nodeList.insert(curNode.name, curNode);
    qDebug() << "name: " << curNode.name << " prelim: " << curNode.prelim << " modifier: " << curNode.modifier;
}

void MyObject::secondWalk(int level, int modSom, QString name)
{
    if(name.isEmpty())
        return;

    //根据prelim和偏移计算x，根据深度计算y
    Node curNode = nodeList.value(name);
    curNode.x = curNode.prelim + modSom;
    curNode.y = level * space;
    nodeList.insert(curNode.name, curNode);

    //遍历子节点, 子节偏移累加父节点prelim
    Node child = nodeList.value(curNode.firstChild);
    secondWalk(level + 1, modSom + curNode.modifier, child.name);
    while (!child.rightSibling.isEmpty()) {
        child = nodeList.value(child.rightSibling);
        secondWalk(level + 1, modSom + curNode.modifier, child.name);
    }
}


QString MyObject::getLeftNeighbor(int row, int column)
{
    //遍历列表，根据行列返回左邻居
    for(int n = 0; n < nodeList.count(); n++)
    {
        Node temp = nodeList.values().at(n);
        if((temp.column == column - 1) && (temp.row == row))
            return temp.name;
    }

    return "";

}

void MyObject::convertNode(int level, QDomNode node)
{
    if(node.isNull())
        return;

    Node temp;
    temp.name = node.toElement().tagName();
    temp.parent = node.parentNode().toElement().tagName();
    temp.firstChild = node.firstChild().toElement().tagName();
    temp.rightSibling = node.nextSibling().toElement().tagName();
    temp.leftSibling = node.previousSibling().toElement().tagName();

    temp.x = 0;
    temp.y = 0;
    temp.prelim = 0;
    temp.modifier = 0;

    temp.row = level;
    //检查是第几行，遍历得到当前行排序
    int column = 0;
    for(int n = 0; n < nodeList.count(); n++)
    {
        if(nodeList.values().at(n).row == level)
            column++;
    }
    temp.column = column;

    temp.leftNeighbor = getLeftNeighbor(temp.row, temp.column);

    nodeList.insert(temp.name, temp);

    //递归查询
    QDomNode child = node.firstChild();
    while (!child.isNull()) {
        convertNode(level + 1, child);
        child = child.nextSibling();
    }

}


MyObject::MyObject()
{


}

void MyObject::start()
{
    QFile file(QStringLiteral(":/node.xml"));
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return ;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        return ;
    }

    file.close();

    QDomElement ele = doc.documentElement();

    nodeList.clear();

    convertNode(0, ele);

    firstWalk(0, ele.tagName());
    secondWalk(0,0,ele.tagName());

    //完成计算后向界面发送信号绘图
    for(int n = 0; n < nodeList.size(); n++)
    {
        Node curNode = nodeList.values().at(n);
        emit createNode(curNode.name,
                        curNode.x,
                        curNode.y);

        Node parent = nodeList.value(curNode.parent);
        if(!parent.name.isEmpty())
            emit drowLine(curNode.x, curNode.y, parent.x, parent.y);


//        qDebug() << "name: " << nodeList.values().at(n).name << " x: " << nodeList.values().at(n).x << " y: " << nodeList.values().at(n).y;
        qDebug() << "_name: " << nodeList.values().at(n).name << " prelim: " << nodeList.values().at(n).prelim << " modifier: " << nodeList.values().at(n).modifier;
    }

}
