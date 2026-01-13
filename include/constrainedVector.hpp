#ifndef CONSTRAINEDVECTOR
#define CONSTRAINEDVECTOR
#include <array>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

template <typename N, size_t maxSize>
class ConstrainedVector{
    int size;
    std::array<N,maxSize> array;

    public:
    ConstrainedVector(int width, N fillValue): size(width) {
        if(width > maxSize){
            throw std::invalid_argument("width provided is greater than the maxSize of the ConstrainedVector.");
        }
        for(int i = 0; i < size; i++){
            array[i] = fillValue;
        }
    }

    ConstrainedVector(std::initializer_list<N> l): size(l.size()) {
        if(l.size() > maxSize)
            throw std::invalid_argument("Initializer list size greater than the maxSize of the ConstrainedVector");
        std::copy(l.begin(),l.end(),array.begin());
    }

    ConstrainedVector(int size, std::array<N,maxSize> array): size(size), array(array) { }

    N operator[](int index) const{
        if(index >= size || index < 0)
            throw std::invalid_argument("illegal index.");
        return array[index];
    }
    N &operator[](int index){
        if(index >= size || index < 0)
            throw std::invalid_argument("Illegal index.");
        return array[index];
    }

    bool operator==(const ConstrainedVector<N,maxSize> other) const{
        if(this->size != other.size)
            return false;
        for(int i = 0; i < this->size; i++){
            if(this->array[i] != other.array[i])
                return false;
        }
        return true;
    }
};

#endif