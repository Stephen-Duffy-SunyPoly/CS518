#ifndef LIST_HPP
#define LIST_HPP

#include <SDL3/SDL_stdinc.h>

namespace qnd {
    template<typename T>
    class List {
        T * data = nullptr;
        Uint32 dataSize = 0;
        Uint32 allocated = 0;
        void increaseAllocated();
        [[nodiscard]] bool checkInBounds(Uint32 index) const;
    public:
        void add(T & item);
        T get(Uint32 index);
        T operator[](Uint32 index);
        [[nodiscard]] Uint32 size() const;
        ~List();
    };
}

template <typename T>
void qnd::List<T>::increaseAllocated() {
    if (allocated == 0) {
        data = new T[4];
        allocated = 4;
    } else {
        T * newData = new T[allocated*2];
        SDL_memcpy(newData, data, sizeof(T)*allocated);
        delete [] data;
        data = newData;
        allocated *= 2;
    }
}

template <typename T>
bool qnd::List<T>::checkInBounds(const Uint32 index) const {
    return index < dataSize;
}

template <typename T>
void qnd::List<T>::add(T& item) {
    //check if more space is required
    if (dataSize >= allocated) {
        increaseAllocated();
    }
    this->data[dataSize] = item;
    dataSize++;
}

template <typename T>
T qnd::List<T>::get(Uint32 index) {
    if (!checkInBounds(index)) {
        throw nullptr;
    }
    return data[index];
}

template <typename T>
T qnd::List<T>::operator[](Uint32 index) {
    return get(index);
}

template <typename T>
Uint32 qnd::List<T>::size() const {
    return dataSize;
}

template <typename T>
qnd::List<T>::~List() {
    delete[] data;
}
#endif //LIST_HPP
