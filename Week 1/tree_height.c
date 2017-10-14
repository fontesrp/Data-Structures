#include <stdio.h>
#include <stdlib.h>

#define QUEUE_DATA_TYPE long

struct SQueue {

    long m_lHeadPtr;
    long m_lTailPtr;
    char m_bEmpty;
    long m_lSize;

    QUEUE_DATA_TYPE * m_pElements;

    long (* m_lEnqueue)(struct SQueue * this, QUEUE_DATA_TYPE item);
    QUEUE_DATA_TYPE (* m_dequeue)(struct SQueue * this);
    void (* m_destruct)(struct SQueue * this);
    void (* m_constructor)(struct SQueue * this, long lQueueSize);
};

QUEUE_DATA_TYPE queueDequeue(struct SQueue * this) {

    QUEUE_DATA_TYPE item;

    if (this->m_bEmpty) {
        // Return random data
        return item;
    }

    item = this->m_pElements[this->m_lHeadPtr];

    this->m_lHeadPtr++;

    if (this->m_lHeadPtr >= this->m_lSize) {
        this->m_lHeadPtr -= this->m_lSize;
    }

    if (this->m_lHeadPtr == this->m_lTailPtr) {
        this->m_bEmpty = 1;
    }

    return item;
}

long queueEnqueue(struct SQueue * this, QUEUE_DATA_TYPE item) {

    if (!this->m_bEmpty && this->m_lHeadPtr == this->m_lTailPtr) {
        // overflow
        return -1;
    }

    this->m_pElements[this->m_lTailPtr] = item;
    this->m_bEmpty = 0;
    this->m_lTailPtr++;

    if (this->m_lTailPtr >= this->m_lSize) {
        this->m_lTailPtr -= this->m_lSize;
    }

    return this->m_lTailPtr;
}

void queueDestruct(struct SQueue * this) {

    this->m_lHeadPtr = 0;
    this->m_lTailPtr = 0;
    this->m_bEmpty = 1;
    this->m_lSize = 0;

    free(this->m_pElements);
}

void queueConstructor(struct SQueue * this, long lQueueSize) {
    this->m_lHeadPtr = 0;
    this->m_lTailPtr = 0;
    this->m_bEmpty = 1;
    this->m_lSize = lQueueSize;
    this->m_pElements = (long *) malloc(lQueueSize * sizeof (long));
    this->m_lEnqueue = queueEnqueue;
    this->m_dequeue = queueDequeue;
    this->m_destruct = queueDestruct;
}

struct SQueue newQueue(long lQueueSize) {

    struct SQueue q;

    q.m_constructor = queueConstructor;
    q.m_constructor(&q, lQueueSize);

    return q;
}

struct SNode {

    long m_lIndex;
    long m_lParentIndex;
    long m_lChildrenQtt;
    long m_lLevel;

    struct SNode * m_pnParent;
    struct SNode ** m_prgpnChildren;

    void (* m_setParent)(struct SNode * this, struct SNode * pnParent);
    void (* m_addChild)(struct SNode * this, struct SNode * pnChild);
    void (* m_setLevel)(struct SNode * this);
    void (* m_destruct)(struct SNode * this);
    void (* m_constructor)(struct SNode * this, long lNodeIdx);
};

void nodeSetParent(struct SNode * this, struct SNode * pnParent) {

    this->m_pnParent = pnParent;
    this->m_lLevel = pnParent->m_lLevel + 1;

    pnParent->m_addChild(pnParent, this);
}

void nodeAddChild(struct SNode * this, struct SNode * pnChild) {

    this->m_lChildrenQtt++;

    this->m_prgpnChildren = (struct SNode **) realloc(this->m_prgpnChildren, this->m_lChildrenQtt * sizeof (struct SNode *));

    this->m_prgpnChildren[this->m_lChildrenQtt - 1] = pnChild;
}

void nodeSetLevel(struct SNode * this) {

    this->m_lLevel = (this->m_pnParent == NULL) ? 1 : this->m_pnParent->m_lLevel + 1;
}

void nodeDestruct(struct SNode * this) {

    this->m_lIndex = 0;
    this->m_lChildrenQtt = 0;
    this->m_lLevel = 0;
    this->m_pnParent = NULL;

    if (this->m_prgpnChildren != NULL) {
        free(this->m_prgpnChildren);
    }

    this->m_prgpnChildren = NULL;
}

void nodeConstructor(struct SNode * this, long lNodeIdx) {

    this->m_lIndex = lNodeIdx;
    this->m_lParentIndex = 0;
    this->m_lChildrenQtt = 0;
    this->m_lLevel = 0;

    this->m_pnParent = NULL;
    this->m_prgpnChildren = NULL;

    this->m_setParent = nodeSetParent;
    this->m_addChild = nodeAddChild;
    this->m_setLevel = nodeSetLevel;
    this->m_destruct = nodeDestruct;
}

struct SNode newNode(long lNodeIdx) {

    struct SNode n;

    n.m_constructor = nodeConstructor;
    n.m_constructor(&n, lNodeIdx);

    return n;
}

long getTreeHeight(struct SNode * prgnTree, long lNodeQtt) {

    struct SQueue qNode = newQueue(lNodeQtt);
    long lIdx;
    long lRoot;

    for (lIdx = 0; lIdx < lNodeQtt; lIdx++) {

        if (prgnTree[lIdx].m_lParentIndex == -1) {
            lRoot = lIdx;
        } else {
            prgnTree[lIdx].m_setParent(&(prgnTree[lIdx]), &(prgnTree[prgnTree[lIdx].m_lParentIndex]));
        }
    }

    long lNodeIdx;
    long lHeight = 0;

    qNode.m_lEnqueue(&qNode, lRoot);

    while (!qNode.m_bEmpty) {

        lNodeIdx = qNode.m_dequeue(&qNode);

        prgnTree[lNodeIdx].m_setLevel(&(prgnTree[lNodeIdx]));

        if (prgnTree[lNodeIdx].m_lLevel > lHeight) {
            lHeight = prgnTree[lNodeIdx].m_lLevel;
        }

        for (lIdx = 0; lIdx < prgnTree[lNodeIdx].m_lChildrenQtt; lIdx++) {
            qNode.m_lEnqueue(&qNode, prgnTree[lNodeIdx].m_prgpnChildren[lIdx]->m_lIndex);
        }
    }

    qNode.m_destruct(&qNode);

    return lHeight;
}

void getTreeInput(struct SNode * prgnTree, long lNodeQtt) {

    long lIdx;
    struct SNode n;

    for (lIdx = 0; lIdx < lNodeQtt; lIdx++) {

        n = newNode(lIdx);

        scanf("%ld", &(n.m_lParentIndex));

        prgnTree[lIdx] = n;
    }
}

int main(int argc, char const * argv[]) {

    long lNodeQtt;
    scanf("%ld", &lNodeQtt);

    struct SNode * prgnTree = (struct SNode *) malloc(lNodeQtt * sizeof (struct SNode));
    getTreeInput(prgnTree, lNodeQtt);

    long lHeight = getTreeHeight(prgnTree, lNodeQtt);
    printf("%ld", lHeight);

    while (lNodeQtt > 0) {

        prgnTree[lNodeQtt - 1].m_destruct(&(prgnTree[lNodeQtt - 1]));

        lNodeQtt--;
    }

    free(prgnTree);

    return 0;
}
