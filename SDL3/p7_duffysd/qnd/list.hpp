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
        bool checkInBounds(Uint32 index) const;
    public:
        void add(T & item);
        T get(Uint32 index);
        T operator[](Uint32 index);
        Uint32 size() const;

    };
}
#endif //LIST_HPP
