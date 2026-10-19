/**
 * MyVector.hpp
 * CPSC 131 - Project 1
 *
 * My own version of a vector that uses a dynamic array.
 * It can grow when it gets full and shrink when it gets too empty.
 */

#include <cstddef>
#include <new>
#include <stdexcept>

namespace CPSC131::MyVector
{
	template <typename T>
	class MyVector
	{
		public:
			
			/// Default capacity if the user doesn't give one
			static constexpr size_t DEFAULT_CAPACITY = 64;
			
			/// The array should never be smaller than this
			static constexpr size_t MINIMUM_CAPACITY = 8;
			
			/**
			 * Default constructor
			 * Makes an empty vector with the given capacity
			 * (but never less than the minimum capacity)
			 */
			MyVector(size_t capacity = MyVector::DEFAULT_CAPACITY)
			{
				if (capacity < MINIMUM_CAPACITY) {
					capacity = MINIMUM_CAPACITY;
				}
				
				size_ = 0;
				capacity_ = capacity;
				elements_ = new T[capacity_];
			}
			
			/**
			 * Copy constructor
			 * Makes a deep copy (new array) so we don't share
			 * the same pointer as the other vector
			 */
			MyVector(const MyVector& other)
			{
				size_ = other.size_;
				capacity_ = other.capacity_;
				elements_ = new T[capacity_];
				
				for (size_t i = 0; i < size_; i++) {
					elements_[i] = other.elements_[i];
				}
			}
			
			/**
			 * Destructor
			 * Frees the array. Setting the pointer to nullptr makes it
			 * safe even if the destructor gets called more than once.
			 */
			~MyVector()
			{
				delete[] elements_;
				elements_ = nullptr;
				size_ = 0;
				capacity_ = 0;
			}
			
			/**
			 * Copy assignment operator
			 * Copies everything from rhs into this vector (deep copy)
			 */
			MyVector& operator=(const MyVector& rhs)
			{
				// check for self assignment (v = v)
				if (this == &rhs) {
					return *this;
				}
				
				// make the new array first, then delete the old one
				T* temp = new T[rhs.capacity_];
				for (size_t i = 0; i < rhs.size_; i++) {
					temp[i] = rhs.elements_[i];
				}
				
				delete[] elements_;
				elements_ = temp;
				size_ = rhs.size_;
				capacity_ = rhs.capacity_;
				
				return *this;
			}
			
			/**
			 * Operator [] works the same as at()
			 */
			T& operator[](size_t index) const
			{
				return at(index);
			}
			
			/**
			 * Returns the raw pointer to the internal array
			 */
			T* elements()
			{
				return elements_;
			}
			
			/**
			 * Returns how many elements are in the vector
			 */
			size_t size() const
			{
				return size_;
			}
			
			/**
			 * Returns how big the internal array is
			 */
			size_t capacity() const
			{
				return capacity_;
			}
			
			/**
			 * Returns true if there are no elements
			 */
			bool empty() const
			{
				return (size_ == 0);
			}
			
			/**
			 * Returns the element at the index
			 * Throws out_of_range if the index is bad
			 */
			T& at(size_t index) const
			{
				if (index >= size_) {
					throw std::out_of_range("at(): index is out of range");
				}
				
				return elements_[index];
			}
			
			/**
			 * Makes sure the capacity is at least the given amount
			 * Throws if they try to reserve less than the current size
			 * Does nothing if the capacity is already big enough
			 */
			void reserve(size_t capacity)
			{
				if (capacity < size_) {
					throw std::invalid_argument("reserve(): capacity can't be less than size");
				}
				
				if (capacity > capacity_) {
					changeCapacity(capacity);
				}
			}
			
			/**
			 * Replaces the element at index with a new one
			 * The old element's destructor gets called
			 */
			T& set(size_t index, const T& element)
			{
				if (index >= size_) {
					throw std::out_of_range("set(): index is out of range");
				}
				
				// destroy the old element, then build the new one in the same spot
				elements_[index].~T();
				new (&elements_[index]) T(element);
				
				return elements_[index];
			}
			
			/**
			 * Adds an element to the end
			 * Doubles the capacity if the array is full
			 */
			T& push_back(const T& element)
			{
				if (size_ == capacity_) {
					changeCapacity(capacity_ * 2);
				}
				
				elements_[size_] = element;
				size_++;
				
				return elements_[size_ - 1];
			}
			
			/**
			 * Removes the last element and returns it
			 * Throws underflow_error if the vector is empty
			 */
			T pop_back()
			{
				if (size_ == 0) {
					throw std::underflow_error("pop_back(): vector is empty");
				}
				
				// save a copy so we can return it
				T removed = elements_[size_ - 1];
				
				// call the destructor and put a default object back in that spot
				elements_[size_ - 1].~T();
				new (&elements_[size_ - 1]) T();
				
				size_--;
				shrinkIfNeeded();
				
				return removed;
			}
			
			/**
			 * Inserts an element at index and shifts everything after it right
			 * index == size is allowed (same as push_back)
			 */
			T& insert(size_t index, const T& element)
			{
				if (index > size_) {
					throw std::out_of_range("insert(): index is out of range");
				}
				
				if (size_ == capacity_) {
					changeCapacity(capacity_ * 2);
				}
				
				// shift right, starting from the end
				for (size_t i = size_; i > index; i--) {
					elements_[i] = elements_[i - 1];
				}
				
				elements_[index] = element;
				size_++;
				
				return elements_[index];
			}
			
			/**
			 * Removes the element at index, shifts everything after it left,
			 * and returns the removed element
			 */
			T erase(size_t index)
			{
				if (index >= size_) {
					throw std::out_of_range("erase(): index is out of range");
				}
				
				// save a copy so we can return it
				T removed = elements_[index];
				
				// call the destructor on the erased element
				elements_[index].~T();
				new (&elements_[index]) T();
				
				// shift left
				for (size_t i = index; i < size_ - 1; i++) {
					elements_[i] = elements_[i + 1];
				}
				
				// reset the last spot since it's not used anymore
				elements_[size_ - 1] = T();
				
				size_--;
				shrinkIfNeeded();
				
				return removed;
			}
			
			/**
			 * Removes everything and goes back to the default capacity
			 * Calls the destructor on every element
			 */
			void clear()
			{
				for (size_t i = 0; i < size_; i++) {
					elements_[i].~T();
					new (&elements_[i]) T();
				}
				
				size_ = 0;
				
				if (capacity_ != DEFAULT_CAPACITY) {
					changeCapacity(DEFAULT_CAPACITY);
				}
			}
		
		private:
			
			size_t size_ = 0;
			size_t capacity_ = 0;
			T* elements_ = nullptr;
			
			/**
			 * Helper: makes a new array with the new capacity,
			 * copies the elements over, and deletes the old array
			 */
			void changeCapacity(size_t newCapacity)
			{
				if (newCapacity < MINIMUM_CAPACITY) {
					newCapacity = MINIMUM_CAPACITY;
				}
				
				T* temp = new T[newCapacity];
				for (size_t i = 0; i < size_; i++) {
					temp[i] = elements_[i];
				}
				
				delete[] elements_;
				elements_ = temp;
				capacity_ = newCapacity;
			}
			
			/**
			 * Helper: shrinks the array when it gets too empty
			 * - if empty, go down to the minimum capacity
			 * - if size is less than 1/3 of capacity, cut capacity in half
			 */
			void shrinkIfNeeded()
			{
				if (size_ == 0) {
					if (capacity_ != MINIMUM_CAPACITY) {
						changeCapacity(MINIMUM_CAPACITY);
					}
					return;
				}
				
				if (capacity_ > MINIMUM_CAPACITY && size_ < capacity_ / 3) {
					changeCapacity(capacity_ / 2);
				}
			}
	};
}
