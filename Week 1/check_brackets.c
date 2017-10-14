#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_SIZE 100000

struct SBracket {
    char m_cLetter;
    long m_lIndex;
};

#define STACK_DATA_TYPE struct SBracket

struct Stack {

    long m_lHeadPtr;
    long m_lSize;
    STACK_DATA_TYPE * m_pElements;

    STACK_DATA_TYPE (* m_pop)(struct Stack * this);
    long (* m_lPush)(struct Stack * this, STACK_DATA_TYPE newElement);
    void (* m_destruct)(struct Stack * this);
    void (* m_constructor)(struct Stack * this, long lStSize);
};

STACK_DATA_TYPE stPop(struct Stack * this) {

    STACK_DATA_TYPE tmp;

    if (this->m_lHeadPtr < 0) {
        // Return random data
        return tmp;
    }

    this->m_lHeadPtr--;

    return this->m_pElements[this->m_lHeadPtr + 1];
}

long stlPush(struct Stack * this, STACK_DATA_TYPE newElement) {

    this->m_lHeadPtr++;

    if (this->m_lHeadPtr >= this->m_lSize) {
        this->m_lHeadPtr--;
        return -1;
    }

    this->m_pElements[this->m_lHeadPtr] = newElement;

    return this->m_lHeadPtr;
}

void stDestruct(struct Stack * this) {

    this->m_lHeadPtr = -1;
    this->m_lSize = 0;

    free(this->m_pElements);
}

void stConstructor(struct Stack * this, long lStSize) {
    this->m_lHeadPtr = -1;
    this->m_lSize = lStSize;
    this->m_pElements = (STACK_DATA_TYPE *) malloc(lStSize * sizeof (STACK_DATA_TYPE));
    this->m_pop = stPop;
    this->m_lPush = stlPush;
    this->m_destruct = stDestruct;
}

struct Stack newStack(long lStSize) {

    struct Stack stNew;

    stNew.m_constructor = stConstructor;
    stNew.m_constructor(&stNew, lStSize);

    return stNew;
}

char pairBracket(char cBracket) {
    switch (cBracket) {
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    default:
        return -1;
    }
}

int bIsBracket(char cLetter) {
    return ( \
        cLetter == '(' \
        || cLetter == '[' \
        || cLetter == '{' \
        || cLetter == ')' \
        || cLetter == ']' \
        || cLetter == '}' \
    );
}

long lCheckBrackets(char * szUserCode, struct Stack * stBracket) {

    long lIdx;
    struct SBracket bktInput;
    struct SBracket bktOpening;

    for (lIdx = 0; lIdx <= MAX_STR_SIZE && szUserCode[lIdx] != '\0'; lIdx++) {

        if (!bIsBracket(szUserCode[lIdx])) {
            continue;
        }

        bktInput.m_cLetter = szUserCode[lIdx];
        bktInput.m_lIndex = lIdx;

        switch (bktInput.m_cLetter) {
        case '(':
        case '[':
        case '{':
            stBracket->m_lPush(stBracket, bktInput);
            break;
        case ')':
        case ']':
        case '}':

            bktOpening = stBracket->m_pop(stBracket);

            if (bktOpening.m_lIndex < 0 || pairBracket(bktOpening.m_cLetter) != bktInput.m_cLetter) {
                // Unmatched closing bracket
                return lIdx;
            }

            break;
        }
    }

    // No unmatched closing bracket, search for the first unmatched opening bracket

    if (stBracket->m_lHeadPtr >= 0) {
        return stBracket->m_pElements[0].m_lIndex;
    }

    return -1;
}

int main(int argc, char const * argv[]) {

    char szUserCode[MAX_STR_SIZE + 1];
    fgets(szUserCode, MAX_STR_SIZE + 1, stdin);

    struct Stack stBracket = newStack(MAX_STR_SIZE);

    long lUnmatched = lCheckBrackets(szUserCode, &stBracket);

    if (lUnmatched == -1) {
        printf("Success");
    } else {
        printf("%ld", lUnmatched + 1);
    }

    stBracket.m_destruct(&stBracket);

    return 0;
}
