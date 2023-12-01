#ifndef HEAP_HEADER
#define HEAP_HEADER

typedef struct Node {
    char data;
    int frequency;
    struct Node *left, *right;
} Node;

typedef struct MinHeap {
    int size;
    int capacity;
    Node** pQueue;
} MinHeap;

Node* newNode(char data, unsigned frequency) {
    Node* node = malloc(sizeof(Node));
    node->left = node->right = NULL;
    node->data = data;
    node->frequency = frequency;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = malloc(sizeof(MinHeap));
    minHeap -> capacity = capacity;
    minHeap -> size = 0;
    minHeap -> pQueue = malloc(capacity * sizeof(Node*));
    return minHeap;
}

void swapMinHeapNode(Node** a, Node** b) {
    Node* t = *b;
    *b = *a;
    *a = t;
}

void minHeapify(MinHeap* minHeap, int index) {
    int smallest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;

    if (left < minHeap->size &&
        minHeap->pQueue[left]->frequency < minHeap->pQueue[smallest]->frequency) {
            smallest = left;
        }
    if (right < minHeap->size &&
        minHeap->pQueue[right]->frequency < minHeap->pQueue[smallest]->frequency) {
            smallest = right;
        }
    if (smallest != index) {
        swapMinHeapNode(&minHeap->pQueue[smallest], &minHeap->pQueue[index]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

//start from the last non leaf node (n-1)/2
void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size-1;
    for (int i = (n-1)/2; i >= 0; i--) {
        minHeapify(minHeap, i);
    }
}

//function to find the least frequency node from heap
//present always as the root (because minHeap)
Node* extractMin(MinHeap* minHeap) {
    Node* temp = minHeap->pQueue[0];
    minHeap->size--;
    minHeap->pQueue[0] = minHeap->pQueue[minHeap->size];
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, Node* node) {
    minHeap->size++;
    int i = minHeap->size -1;
    minHeap->pQueue[i] = node;
    buildMinHeap(minHeap);
} 

int isLeaf(Node* node) {
    return !(node->left) && !(node->right);
}

MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; i++) {
        minHeap->pQueue[i] = newNode(data[i], freq[i]);
    }
    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

#endif