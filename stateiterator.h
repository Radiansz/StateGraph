#ifndef STATEITERATOR_H
#define STATEITERATOR_H
#include <iterator>
#include <algorithm>
#include <vector>
#include <state.h>
#include <StateGraph.h>
namespace States {
template <class V>
class State;

template <class N>
class StateDiagram;

template <class T>
class StateIterator
{
    friend class StateDiagram<T>;
private:
    std::vector< State<T>* > data;
    int index;
    int size;
    StateIterator();
    StateIterator(std::vector<State<T>* > data, int index = 0);
public:
    typedef State<T> value_type;
    typedef State<T>* pointer;
    typedef State<T>& reference;
    typedef std::output_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;

    // конструкторы

    StateIterator(const StateIterator& clone);
    // оператор присваиваний
    StateIterator& operator =(const StateIterator &);

    // сдвиги операторов
    StateIterator& operator ++();    // префиксный инкремент
    StateIterator  operator ++(int); // постфиксная инкремент

    // операторы обращения
    pointer operator *();
    pointer operator ->();

    // операторы сравнения
    bool operator ==(const StateIterator &);
    bool operator !=(const StateIterator &);

    bool isEnd(){
        return index == data.size()? true : false;
    }


};

// конструкторы
template <class T>
StateIterator<T>::StateIterator(){

}

template <class T>
StateIterator<T>::StateIterator(std::vector<State<T>* > data, int index){
    this->data = data;
    this->index = index;

}

template <class T>
StateIterator<T>::StateIterator(const StateIterator& clone){
    this->data = clone.data;
    this->index = clone.index;
}

// оператор присваиваний
template <class T>
StateIterator<T>& StateIterator<T>::operator =(const StateIterator & clone){
       if(this!=&clone){
           data = clone ->data;
           index = clone ->index;
       }
       return *this;
}

// сдвиги операторов
template <class T>
StateIterator<T>& StateIterator<T>::operator ++(){// префиксный инкремент
    index++;
    return *this;
}

template <class T>
StateIterator<T>  StateIterator<T>::operator ++(int){// постфиксная инкремент
    StateIterator<T> tmp(data,index);
    index++;
    return tmp;
}

// операторы обращения
template <class T>
State<T>* StateIterator<T>::operator *(){
    return data[index];
}

template <class T>
State<T>* StateIterator<T>::operator ->(){
    return data[index];
}

// операторы сравнения
template <class T>
bool StateIterator<T>::operator ==(const StateIterator& other){
    return data[index] == other.data[other.index] ? true : false;
}

template <class T>
bool StateIterator<T>::operator !=(const StateIterator& other){
    return data[index] != other.data[other.index] ? true : false;
}
}
#endif // STATEITERATOR_H
