#include <iostream>
#include <list>
#include <cstdint>
#include <iostream>

class hashTable {
protected:
    static uint32_t hash(uint32_t a) {
        a = (a + 0x7ed55d16) + (a << 12);
        a = (a ^ 0xc761c23c) ^ (a >> 19);
        a = (a + 0x165667b1) + (a << 5);
        a = (a + 0xd3a2646c) ^ (a << 9);
        a = (a + 0xfd7046c5) + (a << 3);
        a = (a ^ 0xb55a4f09) ^ (a >> 16);
        return a;
    }
    static uint16_t hash0(const std::string &s) {
        uint32_t h = 0;
        for (char c: s) {
            h = hash(h + c);
        }
        return h;
    }
    static uint16_t hash1(const std::string &s) {
        uint32_t h = 1;
        for (char c: s) {
            h = hash(h + c);
        }
        return h * 2 + 1;
    }
    static uint16_t gcd(uint16_t a, uint16_t b) {
        return b ? gcd(b, a % b) : a;
    }
public:
    virtual void insert(const std::string &s) = 0;
    virtual bool contains(const std::string &s) const = 0;
    virtual void remove(const std::string &s) = 0;
};
class hashTableList : hashTable {
    std::list<std::string> *_vals;
    //вместо списка здесь может быть любой контейнер, в т.ч. собственный, но это имеет мало значения, т.к. он нужен только для обработки коллизий
public:
    hashTableList() {
        _vals = new std::list<std::string>[UINT16_MAX + 1];
        for (uint32_t i = 0; i <= UINT16_MAX; i++) {
            _vals[i] = {};
        }
    }
    ~hashTableList() {
        delete[] _vals;
    }
    void insert(const std::string &s) override {
        uint16_t p = hash0(s);
        _vals[p].push_back(s);
    }
    bool contains(const std::string &s) const override {
        uint16_t p = hash0(s);
        for (std::string &s_: _vals[p]) {
            if (s_ == s) {
                return true;
            }
        }
        return false;
    }
    void remove(const std::string &s) override {
        uint16_t p = hash0(s);
        _vals[p].remove(s);
    }
};
class hashTableDouble : public hashTable {
    std::string **_vals;
public:
    hashTableDouble() {
        _vals = new std::string *[UINT16_MAX + 1];
        for (uint32_t i = 0; i <= UINT16_MAX; i++) {
            _vals[i] = 0;
        }
    }
    ~hashTableDouble() {
        for (uint32_t i = 0; i <= UINT16_MAX; i++) {
            delete _vals[i];
        }
        delete[] _vals;
    }
    void insert(const std::string &s) override {
        uint16_t p = hash0(s), q = hash1(s), p0 = p;
        q /= gcd(q, p);
        do {
            if (_vals[p]) {
                if (*(_vals[p]) == s) {
                    return;
                } else {
                    p += q;
                }
            } else {
                _vals[p] = new std::string(s);
                return;
            }
        } while (p != p0);
        throw std::exception();
    }
    bool contains(const std::string &s) const override {
        uint16_t p = hash0(s), q = hash1(s), p0 = p;
        q /= gcd(q, p);
        do {
            if (_vals[p]) {
                if (*(_vals[p]) == s) {
                    return true;
                } else {
                    p += q;
                }
            } else {
                break;
            }
        } while (p != p0);
        return false;
    }
    void remove(const std::string &s) override {
        uint16_t p = hash0(s), q = hash1(s), p0 = p;
        q /= gcd(q, p);
        do {
            if (_vals[p]) {
                if (*(_vals[p]) == s) {
                    delete _vals[p];
                    _vals[p] = 0;
                } else {
                    p += q;
                }
            } else {
                break;
            }
        } while (p != p0);
    }
};

template<class T>
class vector {
    const uint32_t allocDelta = 5;

    uint32_t _size, _allocated;
    T *_values;
    void realloc(uint32_t allocatedNew) {
        T *values_ = new T[allocatedNew];
        for (uint32_t i = 0; i < allocatedNew; i++) {
            values_[i] = _values[i];
        }
        _allocated = allocatedNew;
    }
public:
    vector() {
        _values = new T[_allocated = allocDelta];
        _size = 0;
    }
    ~vector() {
        delete[] _values;
    }
    uint32_t size() const {
        return _size;
    }
    T &operator[](uint32_t i) {
        if (i < _size) {
            return _values[i];
        } else {
            throw std::out_of_range("_size: " + std::to_string(_size) + ", i:" + std::to_string(i));
        }
    }
    const T &operator[](uint32_t i) const {
        if (i < _size) {
            return _values[i];
        } else {
            throw std::out_of_range("_size: " + std::to_string(_size) + ", i:" + std::to_string(i));
        }
    }
    void pushBack(const T &value) {
        if (_size == _allocated) {
            realloc(_allocated + allocDelta);
        }
        _values[_size++] = value;
    }
    void removeBack() {
        if (_size) {
            if (_allocated > allocDelta && _size - 1 <= _allocated - allocDelta) {
                realloc(_allocated - allocDelta);
            }
            _size--;
        }
    }
    friend std::ostream &operator<<(std::ostream &out, const vector<T> &v) {
        uint32_t s = v._size;
        for (int i = 0; i < s; i++) {
            out << v[i] << '\n';
        }
        return out;
    }
};

int main() {

    //hashTableList table = hashTableList();
    //hashTableDouble table = hashTableDouble();

//    printf(">>> %d %d %d %d\n", table.contains("abc"), table.contains("123"), table.contains(".akhbc"), table.contains(""));
//    table.insert("abc");
//    printf(">>> %d %d %d %d\n", table.contains("abc"), table.contains("123"), table.contains(".akhbc"), table.contains(""));
//    table.insert("123");
//    printf(">>> %d %d %d %d\n", table.contains("abc"), table.contains("123"), table.contains(".akhbc"), table.contains(""));
//    table.remove("123");
//    printf(">>> %d %d %d %d\n", table.contains("abc"), table.contains("123"), table.contains(".akhbc"), table.contains(""));
//    table.insert("123");
//    printf(">>> %d %d %d %d\n", table.contains("abc"), table.contains("123"), table.contains(".akhbc"), table.contains(""));

    vector<std::string> v;
    v.pushBack("abc");
    v.pushBack("123");
    v.pushBack("42");
    std::cout << v;
    v.removeBack();
    std::cout << v;
    return 0;
}
