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
 uint64_t size() const {
        return amount;
    }
    bool empty() const {
        return !(head->getNext());
    }
    bool coin() const {
        return (((float) rand()) / RAND_MAX) < chance;
    }
    void insert(T value) {
        //printf("(%d)", value);
        node<T> **lastOnLvls = new node<T> *[maxHeight];
        for (int8_t i = 0; i < maxHeight; i++) {
            lastOnLvls[i] = head;
        }
        node<T> *P = head, *P_;
        for (int8_t L = height - 1; L >= 0; L--) {
            P_ = P->getNext(L);
            //                               cmp
            while (P_ != nullptr && cmp(P_->value, value) < 0) {
                //printf("->%d", P_->value);
                P = P_;
                P_ = P->getNext(L);
            }
            lastOnLvls[L] = P;
            //printf(" V ");
        }
        //printf("\n");
        node<T> *newNode = new node<T>(value, maxHeight);
        node<T>::connect(lastOnLvls[0], newNode, lastOnLvls[0]->getNext(0), 0);
        for (int8_t i = 1; (i < maxHeight) && coin(); i++) {
            node<T>::connect(lastOnLvls[i], newNode, lastOnLvls[i]->getNext(i), i);
            if (i + 1 > height)height = i + 1;
        }
        amount++;
        if (lastOnLvls[0] == tail)tail = P;
        delete[] lastOnLvls;
        //print();
    }
    void print() {
        for (int8_t l = maxHeight - 1; l >= 0; l--) {
            printf("%d: ", l);
            node<T> *P = head->getNext(l);
            while (P) {
                std::cout << P->value << ' ';
                P = P->getNext(l);
            }
            printf("\n");
        }
        printf("\n");
    }
    void clear() {
        delete head;
        tail = head = new node<T>(-1, maxHeight);
        height = 1;
        amount = 0;
    }

    skipListIterator<T> begin() {
        return skipListIterator<T>(head->getNext());
    }
    skipListIterator<T> end() {
        return skipListIterator<T>(nullptr);
    }
    skipListIterator<T> find(T value) {
        node<T> *P = head, *P_;
        for (int8_t L = height - 1; L >= 0; L--) {
            P_ = P->getNext(L);
            while (P_ != nullptr) {
                if (cmp(P_->value, value) < 0) {
                    P = P_;
                    P_ = P->getNext(L);
                } else if (cmp(P_->value, value) == 0) {
                    return skipListIterator<T>(P_);
                } else {
                    break;
                }
            }
        }
        return end();
    }
    skipListIterator<T> lower_bound(T value) {
        node<T> *P = head, *P_;
        for (int8_t L = height - 1; L >= 0; L--) {
            P_ = P->getNext(L);
            while (P_ != nullptr) {
                if (cmp(P_->value, value) < 0) {
                    P = P_;
                    P_ = P->getNext(L);
                } else {
                    break;
                }
            }
        }
        return skipListIterator<T>(P_);
    }
    skipListIterator<T> upper_bound(T value) {
        node<T> *P = head, *P_;
        for (int8_t L = height - 1; L >= 0; L--) {
            P_ = P->getNext(L);
            while (P_ != nullptr) {
                if (cmp(P_->value, value) <= 0) {
                    P = P_;
                    P_ = P->getNext(L);
                } else {
                    break;
                }
            }
        }
        return skipListIterator<T>(P_);
    }
    void erase(skipListIterator<T> N_) {
        node<T> *N = ~N_;
        for (int8_t i = 0; i < maxHeight; i++) {
            node<T>::connect(N->getPrev(i), N->getNext(i), i);
            N->next[i] = nullptr;
        }
        delete N;
    }
    void erase(skipListIterator<T> from, skipListIterator<T> to) {
        skipListIterator<T> to_ = to;
        ++to_;
        while (from != to_) {
            skipListIterator<T> temp = from;
            ++temp;
            erase(from);
            from = temp;
        }
    }
    void erase(std::pair<skipListIterator<T>, skipListIterator<T>> zone) {
        erase(zone.first, zone.second);
    }
    uint64_t count(T value) {
        skipListIterator<T> Nl = find(value);
        if (Nl == end())return 0;
        uint64_t C = 1;
        skipListIterator<T> Nr = Nl, Nt = Nr;
        while (*++Nt == *Nr) {
            Nr = Nt;
            C++;
        }
        Nt = Nl;
        while (*--Nt == *Nl) {
            Nl = Nt;
            C++;
        }
        return C;
    }
    std::pair<skipListIterator<T>, skipListIterator<T>> equal_range(T value) {
        skipListIterator<T> Nl = find(value);
        if (Nl == end()) std::make_pair(Nl, Nl);
        skipListIterator<T> Nr = Nl, Nt = Nr;
        while (*++Nt == *Nr) {
            Nr = Nt;
        }
        Nt = Nl;
        while (*--Nt == *Nl) {
            Nl = Nt;
        }
        return std::make_pair(Nl, Nr);
    }
};


int main() {
    skipList<int> list(4, 0.25, [](int a, int b) { return a - b; });
    {
        list.insert(7);
        list.insert(8);
        list.insert(2);
        list.insert(3);
        list.insert(5);
        list.insert(6);
        list.insert(11);
        list.insert(4);
        list.insert(9);
        list.insert(21);
        list.insert(22);
        list.insert(23);
        list.insert(8);
        list.insert(16);
        list.insert(16);
        list.insert(1);
        list.insert(13);
        list.insert(12);
        list.insert(13);
        list.insert(14);
        list.insert(15);
        list.insert(16);
        list.insert(17);
        list.insert(11);
        list.insert(19);
        list.insert(20);
    }
    list.print();
    printf(">%lu\n", list.count(16));
    list.erase(list.equal_range(11));
    list.print();
    list.erase(list.equal_range(16));
    list.print();

    printf("from 11: ");
    for (auto x = list.upper_bound(11); x != list.end(); ++x) {
        std::cout << *x << " ";
    }
    printf("\n(for :) ");
    for (auto x: list) {
        std::cout << x << " ";
    }
    return 0;
}
