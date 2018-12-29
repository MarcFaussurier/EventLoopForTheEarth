//
// Created by marc on 29/12/18.
//
#ifndef NALP_ASSOCIATIVEARRAY_H
#define NALP_ASSOCIATIVEARRAY_H

#include "AssociativeArray.h"
namespace ipolitic {
    template<class T>
    long AssociativeArray<T>::Size() {
        return stack.size();
    }

    template<class T>
    bool AssociativeArray<T>::IsItem(string name) {
        for (int i = 0; i < Size(); i++) {
            if (stack[i].name == name)
                return true;
        }
        return false;
    }

    template<class T>
    bool AssociativeArray<T>::AddItem(string name, T data) {
        if (IsItem(name))
            return false;
        Data d;
        d.name = name;
        d.data = data;
        stack.push_back(d);
        return true;
    }

    template<class T>
    T &AssociativeArray<T>::operator[](string name) {
        for (int i = 0; i < Size(); i++) {
            if (stack[i].name == name)
                return stack[i].data;
        }
        long idx = Size();
        Data d;
        d.name = name;
        stack.push_back(d);
        return stack[idx].data;
    }

    template<class T>
    string AssociativeArray<T>::GetItemName(long index) {
        if (index < 0)
            index = 0;
        for (int i = 0; i < Size(); i++)
            if (i == index)
                return stack[i].name;
        return "";
    }

    template<class T>
    T &AssociativeArray<T>::operator[](long index) {
        if (index < 0)
            index = 0;
        for (int i = 0; i < Size(); i++) {
            if (i == index)
                return stack[i].data;
        }
        return stack[0].data;
    }
}

#endif //NALP_ASSOCIATIVEARRAY_H

