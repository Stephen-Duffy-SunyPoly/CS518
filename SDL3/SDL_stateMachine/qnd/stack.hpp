
#ifndef QND_STACK_HPP
#define QND_STACK_HPP
#include <SDL3/SDL_stdinc.h>

namespace qnd {
    template<typename T>
    class Stack {
        T * data = nullptr;
        Uint32 dataSize = 0;
        Uint32 allocated = 0;
        void increaseAllocated() {
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
        [[nodiscard]] bool checkInBounds(Uint32 index) const{
            return index < dataSize;
        }
    public:
        void push(T & item) {
            //check if more space is required
            if (dataSize >= allocated) {
                increaseAllocated();
            }
            this->data[dataSize] = item;
            dataSize++;
        }
        T get(Uint32 index){
            if (!checkInBounds(index)){
                throw nullptr;
            }
            return data[index];
        }
        T operator[](Uint32 index) {
            return get(index);
        }
        T peak() {
            return get(dataSize-1);
        }
        [[nodiscard]] Uint32 size() const {
            return dataSize;
        }
        T pop() {
            if (dataSize ==0) {
                throw nullptr;
            }
            T item = data[dataSize-1];
            dataSize--;
            return item;
        }
        ~Stack() {
            delete [] data;
        }
    };
}
#endif //QND_STACK_HPP
