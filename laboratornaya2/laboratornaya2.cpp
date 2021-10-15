// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
    friend std::ostream& operator<<(std::ostream&, Grid<P> const&);// если чо сюда добавить <T>(Другой)
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

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.




