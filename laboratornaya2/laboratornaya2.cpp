// ConsoleApplication1.cpp : ���� ���� �������� ������� "main". ����� ���������� � ������������� ���������� ���������.
//
#include<string>
#include <iostream>
template <typename T>
class Grid {
private:
    T* memory;
    size_t x_size, y_size;
public:
    Grid(size_t x_size, size_t y_size)
    {
        this->x_size = x_size;
        this->y_size = y_size;
        memory = new T[x_size * y_size];
    };
    ~Grid()
    {
        delete [] memory;
    }
    T operator()(size_t x_idx, size_t y_idx) const
    {
        //size_t id;
        //id = x_idx * y_idx;
        //id--;
        //return memory[id];

        //size_t id;
        //id = x_idx * y_idx;
        //id--;

        size_t id;
        id = x_idx - 1;
        id = x_size * id;
        id = id + y_idx - 1;
        return *memory[id];
    };
    T& operator()(size_t x_idx, size_t y_idx)
    {

        size_t id;
        id = x_idx - 1;
        id = x_size * id;
        id = id + y_idx - 1;


        return memory[id];
    };
    size_t get_xsize() const
    {
        return x_size;
    };
    size_t get_ysize() const
    {
        return y_size;
    };
    Grid& operator=(T temp)
    {
        size_t size = x_size * y_size;
        for (size_t i = 0; i < size; i++)
        {
            memory[i] = temp;
        }
        return *this;
    };
    void out()
    {
        size_t size = x_size * y_size;
        for (size_t i = 0; i < size; i++)
        {
            std::cout << memory[i] << std::endl;;
        }
    }
    template <typename P>
    friend std::ostream& operator<<(std::ostream&, Grid<P> const&);// ���� �� ���� �������� <T>(������)
    template <typename P>
    friend std::istream& operator>>(std::istream&, Grid<P>&);
};
template<typename T>
std::ostream& operator << (std::ostream& os, Grid<T> const& grid)
{
    size_t size = grid.x_size * grid.y_size;
    for (size_t i = 0; i < size; i++)
    {
        os << grid.memory[i];
        os << "\n";
    }
    return os;
};
template<typename T>
std::istream& operator>>(std::istream& os, Grid<T>& grid)
{
    size_t size = grid.x_size * grid.y_size;
    for (size_t i = 0; i < size; i++)
    {
        os >> grid.memory[i];
    }
    return os;



};
int main()
{
    Grid <int> a(2,2);
    a = 3;
    a.out();
    a(2, 1) = 2;
    std::cout << std::endl;
    a.out();
    std::cout << a;
    std::cin >> a;
    std::cout << a;


}

// ������ ���������: CTRL+F5 ��� ���� "�������" > "������ ��� �������"
// ������� ���������: F5 ��� ���� "�������" > "��������� �������"

// ������ �� ������ ������
//   1. � ���� ������������ ������� ����� ��������� ����� � ��������� ���.
//   2. � ���� Team Explorer ����� ������������ � ������� ���������� ��������.
//   3. � ���� "�������� ������" ����� ������������� �������� ������ ������ � ������ ���������.
//   4. � ���� "������ ������" ����� ������������� ������.
//   5. ��������������� �������� ������ ���� "������" > "�������� ����� �������", ����� ������� ����� ����, ��� "������" > "�������� ������������ �������", ����� �������� � ������ ������������ ����� ����.
//   6. ����� ����� ������� ���� ������ �����, �������� ������ ���� "����" > "�������" > "������" � �������� SLN-����.




