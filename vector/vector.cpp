#include <iostream>
#include <list>
#include <cstdint>
#include <iostream>

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

    vector<std::string> v;
    v.pushBack("abc");
    v.pushBack("123");
    v.pushBack("42");
    std::cout << v;
    v.removeBack();
    std::cout << v;
    return 0;
}
