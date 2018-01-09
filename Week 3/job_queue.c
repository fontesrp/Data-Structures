#include <stdio.h>
#include <stdlib.h>

struct SJob {
    long m_lIdx;
    unsigned long long m_ullProcessTime;
    long m_lThread;
    unsigned long long m_ullStartTime;
    unsigned long long m_ullPriority;
};

void printResults(struct SJob * jbAux, long lLen) {

    long lIdx;

    for (lIdx = 0; lIdx < lLen; lIdx++) {
        printf("%ld %llu\n", jbAux[lIdx].m_lThread, jbAux[lIdx].m_ullStartTime);
    }
}

void swap(struct SJob * jbA, struct SJob * jbB) {

    if (jbA == jbB) {
        return;
    }

    struct SJob jbTmp = *jbA;

    *jbA = *jbB;
    *jbB = jbTmp;
}

long lRightChild(long lIdx) {
    return lIdx * 2 + 2;
}

long lLeftChild(long lIdx) {
    return lIdx * 2 + 1;
}

int bHasChildren(long lIdx, long lHeapSize) {

    long lLc = lLeftChild(lIdx);
    long lRc = lRightChild(lIdx);

    return (lLc < lHeapSize || lRc < lHeapSize);
}

void siftDown(struct SJob * heap, long heapSize, long elementIdx) {

    long lc;
    long rc;
    long swapChild;

    while (bHasChildren(elementIdx, heapSize)) {

        lc = lLeftChild(elementIdx);
        rc = lRightChild(elementIdx);

        if (lc >= heapSize) {
            lc = elementIdx;
        }

        if (rc >= heapSize) {
            rc = elementIdx;
        }

        if (heap[lc].m_ullPriority < heap[elementIdx].m_ullPriority || heap[rc].m_ullPriority < heap[elementIdx].m_ullPriority) {
            if (heap[lc].m_ullPriority < heap[rc].m_ullPriority) {
                swapChild = lc;
            } else if (heap[lc].m_ullPriority > heap[rc].m_ullPriority) {
                swapChild = rc;
            } else if (heap[lc].m_lThread < heap[rc].m_lThread) {
                swapChild = lc;
            } else {
                swapChild = rc;
            }
            swap(&heap[elementIdx], &heap[swapChild]);
            elementIdx = swapChild;
        } else if ((lc != elementIdx && heap[lc].m_ullPriority == heap[elementIdx].m_ullPriority && heap[lc].m_lThread < heap[elementIdx].m_lThread) || (rc != elementIdx && heap[rc].m_ullPriority == heap[elementIdx].m_ullPriority && heap[rc].m_lThread < heap[elementIdx].m_lThread)) {
            if (lc == elementIdx || heap[lc].m_ullPriority > heap[elementIdx].m_ullPriority) {
                swapChild = rc;
            } else if (rc == elementIdx || heap[rc].m_ullPriority > heap[elementIdx].m_ullPriority) {
                swapChild = lc;
            } else if (heap[lc].m_lThread < heap[rc].m_lThread) {
                swapChild = lc;
            } else {
                swapChild = rc;
            }
            swap(&heap[elementIdx], &heap[swapChild]);
            elementIdx = swapChild;
        } else {
            elementIdx = heapSize;
        }
    }
}

void arr2heap(struct SJob * arr, long len) {

    long lIdx;

    for (lIdx = len - 1; lIdx >= 0; lIdx--) {

        arr[lIdx].m_lThread = lIdx;
        arr[lIdx].m_ullStartTime = 0;
        arr[lIdx].m_ullPriority = arr[lIdx].m_ullStartTime + arr[lIdx].m_ullProcessTime;

        siftDown(arr, len, lIdx);
    }
}

void processJobs(struct SJob * jobs, long lJobQtt, long lThreadQtt, struct SJob * jobsAux) {

    // Priority queue
    arr2heap(jobs, lThreadQtt);

    long lIdx;

    for (lIdx = lThreadQtt; lIdx < lJobQtt; lIdx++) {
        jobsAux[jobs[0].m_lIdx] = jobs[0];
        jobs[lIdx].m_lThread = jobs[0].m_lThread;
        jobs[lIdx].m_ullStartTime = jobs[0].m_ullPriority;
        jobs[lIdx].m_ullPriority = jobs[lIdx].m_ullStartTime + jobs[lIdx].m_ullProcessTime;
        jobs[0] = jobs[lIdx];
        siftDown(jobs, lThreadQtt, 0);
    }

    long heapSize;

    for (lIdx = 0; lIdx < lThreadQtt; lIdx++) {
        heapSize = lThreadQtt - lIdx;
        jobsAux[jobs[0].m_lIdx] = jobs[0];
        jobs[0] = jobs[heapSize - 1];
        siftDown(jobs, heapSize, 0);
    }
}

void getJobs(struct SJob * jobs, long lJobQtt, struct SJob * jobsAux) {

    long lIdx;

    for (lIdx = 0; lIdx < lJobQtt; lIdx++) {

        jobs[lIdx].m_lIdx = lIdx;

        // Invalid thread
        jobs[lIdx].m_lThread = -1;

        // Maximum value for unsigned long
        jobs[lIdx].m_ullStartTime = -1;
        jobs[lIdx].m_ullPriority = -1;

        // Read input
        scanf("%llu", &(jobs[lIdx].m_ullProcessTime));

        // Copy to aux
        jobsAux[lIdx] = jobs[lIdx];
    }
}

int main(int argc, const char ** argv) {

    long lThreadQtt;
    scanf("%ld", &lThreadQtt);

    long lJobQtt;
    scanf("%ld", &lJobQtt);

    // If there are more more threads than jobs, the program will assign each job to
    // one thread. There is no need to keep track of the excess.
    if (lThreadQtt > lJobQtt) {
        lThreadQtt = lJobQtt;
    }

    struct SJob * jobs = (struct SJob *) malloc(lJobQtt * sizeof (struct SJob));
    struct SJob * jobsAux = (struct SJob *) malloc(lJobQtt * sizeof (struct SJob));

    getJobs(jobs, lJobQtt, jobsAux);
    processJobs(jobs, lJobQtt, lThreadQtt, jobsAux);
    printResults(jobsAux, lJobQtt);

    free(jobs);
    free(jobsAux);

    return 0;
}
