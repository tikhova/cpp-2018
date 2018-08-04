#pragma once
#ifndef NODE_H
#define NODE_H

#include <vector>

template <typename T>
class node {
private:
    T value;
    node<T>* parent;
    node<T>* children[2];


public:
    node(): parent(0), children() {}
    node(T const & value_) : value(value_), parent(0), children() {}
    node(T const & value_, node<T>  * parent_, node<T> * left = nullptr, node<T> * right = nullptr):
        value(value_), parent(parent_), children{left, right} {}

    inline void set_parent(node<T> * parent_) { parent = parent_; }

    inline void set_children(node<T> * left, node<T> * right) { children = {left, right}; }
    inline void set_left_child(node<T> * left) { children[0] = left; }
    inline void set_right_child(node<T> * right) { children[1] = right; }

    inline bool is_root() { return !parent; }
    inline bool is_left() { return !is_root() && parent->children[0] == this; }
    inline bool is_right() { return !is_root() && !is_left(); }

    inline T get_value() const { return value; }
    inline node * get_parent() const { return parent; }
    inline node * get_left_child() const { return children[0]; }
    inline node * get_right_child() const { return children[1]; }
};

#endif // NODE_H
