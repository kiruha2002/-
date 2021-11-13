#include <bits/stdc++.h>
#include <functional>

template<class T>
struct node {
    T value;
    node **prev, **next;
    explicit node(T value_, int8_t size) {
        value = value_;
        prev = new node *[size];
        next = new node *[size];
        for (int8_t i = 0; i < size; i++) {
            prev[i] = next[i] = nullptr;
        }
    }
    ~node() {
        delete getNext();
        delete[] prev;
        delete[] next;
    }
    node *getNext(int8_t lvl = 0) const {
        return next[lvl];
    }
    node *getPrev(uint8_t lvl = 0) const {
        return prev[lvl];
    }
    static void connect(node *A, node *B, uint8_t level) {
        if (A) A->next[level] = B;
        if (B) B->prev[level] = A;
    }
    static void connect(node *A, node *B, node *C, uint8_t level) {
        connect(A, B, level);
        connect(B, C, level);
    }
};
template<class T>
class skipList;

template<class T>
class skipListIterator : public std::iterator<std::bidirectional_iterator_tag, skipList<T>> {
    node<T> *current;
public:
    explicit skipListIterator(node<T> *N) {
        current = N;
    }
    node<T> *operator~() {
        return current;
    }
    T &operator*() {
        return current->value;
    }
    const T &operator*() const {
        return current->value;
    }
    skipListIterator<T> &operator++() {
        current = current->getNext();
        return *this;
    }
    skipListIterator<T> &operator--() {
        current = current->getPrev();
        return *this;
    }
    bool operator==(const skipListIterator<T> &other) const {
        return current == other.current;
    }
    bool operator!=(const skipListIterator<T> &other) const {
        return current != other.current;
    }
};
template<typename T>
class skipList {
    std::function<int(T, T)> cmp;
    uint64_t amount = 0;
    int8_t maxHeight{}, height{};
    float chance{};
    node<T> *head, *tail;
public:
    explicit skipList(int8_t maxHeight_ = 4, float chance_ = .5, std::function<int(T, T)> cmp_ = [](T a, T b) { return a - b; }) {
        cmp = cmp_;
        maxHeight = maxHeight_;
        chance = chance_;
        height = 1;
        tail = head = new node<T>(-1, maxHeight);
    }
//skipList(int8_t maxHeight_, float chance, std::iter)
    void copy(const skipList<T> &other) {
        cmp = other.cmp;
        maxHeight = other.maxHeight;
        chance = other.chance;
        height = other.height;
        amount = other.amount;
        tail = other.tail;
        head = new node<T>(-1, maxHeight);
        auto **lastOnLevels = new node<T> *[maxHeight];
        auto **lastOnLevelsOther = new node<T> *[maxHeight];
        for (int i = 0; i < maxHeight; i++) {
            lastOnLevels[i] = head;
            lastOnLevelsOther[i] = other.head;
        }
        node<T> *temp = head;
        node<T> *tempOther = other.head;
        while (tempOther) {
            for (int i = 0; i < maxHeight; i++) {
                if (lastOnLevelsOther[i]->next[i] == tempOther) {
                    node<T>::connect(lastOnLevels[i], temp, i);
                    lastOnLevelsOther[i] = tempOther;
                    lastOnLevels[i] = temp;
                }
            }
            tempOther = tempOther->next[0];
            if (tempOther) {
                temp->next[0] = new node<T>(tempOther->value, maxHeight);
            }
            temp = temp->next[0];
        }
        delete[] lastOnLevels;
        delete[] lastOnLevelsOther;
    }
    skipList(const skipList<T> &other) {
        copy(other);
    }
void move(skipList<T> &&other) {
        maxHeight = other.maxHeight;
        chance = other.chance;
        height = other.height;
        other.height = 0;
        amount = other.amount;
        other.amount = 0;
        head = other.head;
        tail = other.tail;
        other.tail = other.head = nullptr;
        cmp = other.cmp;
    }
    skipList(skipList<T> &&other) noexcept {
        move();
    }
    skipList &operator=(skipList<T> &&other) noexcept {
        if (&other != this) {
            delete[] head;
            move();
        }
        return *this;
    }

    skipList<T> &operator=(const skipList<T> &other) {
        if (this == &other) return *this;
        delete head;
        copy(other);
        return *this;
    }
    ~skipList() {
        delete head;
    }