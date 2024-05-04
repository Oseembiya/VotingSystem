#include <iostream>

template <typename T>
class Vector { // Written by Dumitru Nirca
private:
  T* array;
  size_t capacity;
  size_t size;
  void resize() { // Function to resize the vector
    capacity *= 2;
    T* newElements = new T[capacity];
    for (size_t i = 0; i < size; ++i) {
      newElements[i] = array[i];
    }
    delete[] array;
    array = newElements;
  }
  
public:
  Vector() : array(nullptr), capacity(10), size(0) {
    array = new T[capacity];
  }

  ~Vector() { // Destructor
    delete[] array;
  }
  
  void push_back(const T& element) {
    if (size == capacity) {
      resize();
    }
    array[size++] = element;
  }

  void pop_back() {
    if (size > 0) {
      size--;
    } else {
      throw std::out_of_range("Vector is empty, cannot pop_back");
    }
  }

  T& back() const {
    if (size > 0) {
      return array[size - 1];
    } else {
      throw std::out_of_range("Vector is empty, cannot access last element");
    }

  }

  T* begin () const {
    return array;
  }

  T* end() const {
    return array + size;
  }

  size_t getSize() const { // Function to get the size of the vector.
    return size;
  }

  bool isEmpty() const {
    return size == 0;
  }

  T& operator[] (size_t index) { // Used for modifying
    if (index >= size) {
      throw std::out_of_range("Index out of range");
    }
    return array[index];
  }

  const T& operator[] (size_t index) const{ // Used for working with const.
    if (index >= size) {
      throw std::out_of_range("Index out of range");
    }
    return array[index];
  }
};
