#ifndef CONSTRAINEDVECTOR
#define CONSTRAINEDVECTOR
#include <array>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

template <typename N, size_t maxSize>
class ConstrainedVector{
    int width;
    int front;
    int back;
    std::array<N,maxSize> array;

    public:

    ConstrainedVector(){
        this->width = 0;
        this->front = 0;
        this->back = 1;
        this->array = std::array<N,maxSize>();
    }

    ConstrainedVector(int width, N fillValue): width(width), array(), front(0),back(width) {
        if(width > maxSize){
            throw std::invalid_argument("width provided is greater than the maxSize of the ConstrainedVector.");
        }
        for(int i = 0; i < width; i++){
            array[i] = fillValue;
        }
    }

    ConstrainedVector(std::initializer_list<N> l): width(l.size()), front(0),back(l.size()) {
        if(l.size() > maxSize)
            throw std::invalid_argument("Initializer list size greater than the maxSize of the ConstrainedVector");
        std::copy(l.begin(),l.end(),array.begin());
    }

    ConstrainedVector(int width, std::array<N,maxSize> array, int front, int back): width(width), array(array), front(front),back(back) { }

    ConstrainedVector(int width, std::array<N,maxSize> array): width(width), array(array), front(0),back(width) { }

    N operator[](int index) const{
        if(index >= width || index < 0)
            throw std::invalid_argument("illegal index.");
        return array[(front+index)%maxSize];
    }
    N &operator[](int index){
        if(index >= width || index < 0)
            throw std::invalid_argument("Illegal index.");
        return array[(front+index)%maxSize];
    }

    bool operator==(const ConstrainedVector<N,maxSize> other) const{
        if(this->width != other.width || this->front != other.front || this->back != other.back)
            return false;
        for(int i = 0; i < this->width; i++){
            if(!((*this)[i] == other[i]))
                return false;
        }
        return true;
    }

    int size(){
        return this->width;
    }

    bool push_back(N toAdd){
        if(width == maxSize)
            return false;
        array[back] = toAdd;
        back = (back+1)%maxSize;
        width++;
        return true;
    }

    bool pop_front(){
        if(width == 0)
            return false;
        front = (front+1)%maxSize;
        width--;
        return true;
    }
};

#endif