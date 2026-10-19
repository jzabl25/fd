

#ifndef MY_DOUBLY_LINKED_LIST_HPP
#define MY_DOUBLY_LINKED_LIST_HPP


/**
 * TODO: Implement DoublyLinkedList, its Node, and its Iterator!
 * 
 * I've left some methods filled out for you,
 * 	and stubbed out some structure, to reduce difficulty.
 * 
 * You may add helper methods as you see fit,
 * 	as long as you can still pass all unit tests.
 * 
 * Notice we're inside a namespace here.
 * The DLL is inside a namespace called DoublyLinkedList,
 * 	which is itself inside a namespace called CPSC131
 * This means, if you'd like to play around with your class later,
 * 	you'll need to access it like so:
 * ::CPSC131::DoublyLinkedList::DoublyLinkedList<int> list;
 * 
 * Look into main.cpp and CPP_Tests.cpp for examples of using
 * 	the DLL and your BookStore. But don't worry too much, as you
 * 	only need to implement these classes
 * (main and tests are already done for you)
 */


//
#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <utility>


/**
 * Namespace for our classroom and DLL !
 */
namespace CPSC131::DoublyLinkedList
{
	/**
	 * Implement our DoublyLinkedList class !
	 */
	template <class T>
	class DoublyLinkedList
	{
		public:
			
			/**
			 * Node class, representing a single item in our linked list
			 */
			class Node
			{
				public:
					
					/// CTORS
					/// Member initialization lists?
					Node() : element_(), prev_(nullptr), next_(nullptr) {}
					Node(T element) : element_(element), prev_(nullptr), next_(nullptr) {}
					Node(T element, Node* prev, Node* next) : element_(element), prev_(prev), next_(next) {}
					
					/// Set the pointer to the previous element
					void setPrevious(Node* prev) { this->prev_ = prev; }
					
					/// Set the pointer to the previous element
					void setPrev(Node* prev) { this->prev_ = prev; }
					
					/// Get a pointer to the previous element
					Node* getPrevious() { return this->prev_; }
					
					/// Get a pointer to the previous element
					Node* getPrev() { return this->prev_; }
					
					/// Set the pointer to the next node
					void setNext(Node* next) { this->next_ = next; }
					
					/// Get a pointer to the next node
					Node* getNext() { return this->next_; }
					
					/// Set the element this node holds
					void setElement(T element) { this->element_ = element; }
					
					/// Get the element this node holds
					///	YOUR WELCOME
					T& getElement() { return this->element_; }
					
					/// Return a reference to the element
					///	YOUR WELCOME
					T& operator*() { return this->element_; }
					
				private:
					T element_;
					Node* prev_ = nullptr;
					Node* next_ = nullptr;
			};
			
			/**
			 * Nested Iterator class.
			 * This allows user code to refer to the Iterator's type as:
			 * 
			 * CPSC131::DoublyLinkedList::DoublyLinkedList<int>::Iterator
			 * 
			 * (as opposed to specifying the template argument two times)
			 */
			class Iterator
			{
				public:
					
					///	Constructor that does nothing; YOUR WELCOME
					Iterator()
					{
						/// yw
					}
					
					///	Constructor taking a head and tail pointer; YOUR WELCOME
					Iterator(Node* head, Node* tail) : head_(head), tail_(tail)
					{
						this->cursor_ = this->end();	/// yw
					}
					
					///	Constructor taking a head, tail, and cursor pointer; YOUR WELCOME
					Iterator(Node* head, Node* tail, Node* cursor) : head_(head), tail_(tail), cursor_(cursor) {}
					
					///	Copy constructor (explicit, since we define operator=)
					Iterator(const Iterator& other) = default;
					
					///	Get a pointer to the head node, or end() if this list is empty
					Node* begin()
					{
						if ( this->head_ == nullptr ) {
							return this->end();
						}
						return this->head_;
					}
					
					///	Get a node pointer representing "end" (aka "depleted"). Probably want to just use nullptr.
					Node* end()
					{
						return nullptr;
					}
					
					///	Get the node to which this iterator is currently pointing
					Node* getCursor()
					{
						return this->cursor_;
					}
					
					///	Return true if this iterator has hit its end; false otherwise
					/// YOUR WELCOME
					bool isAtEnd()
					{
						return this->cursor_ == nullptr;
					}
					
					/**
					 * Assignment operator
					 * Return a copy of this Iterator, after modification
					 */
					Iterator& operator=(const Iterator& other)
					{
						if ( this != &other ) {
							this->head_ = other.head_;
							this->tail_ = other.tail_;
							this->cursor_ = other.cursor_;
						}
						return *this;
					}
					
					///	Comparison operator
					bool operator==(const Iterator& other)
					{
						return this->cursor_ == other.cursor_;
					}
					///	Inequality comparison operator
					bool operator!=(const Iterator& other)
					{
						return !(this->operator==(other));
					}
					
					/**
					 * Addition operator
					 */
					Iterator operator +(size_t add)
					{
						Iterator result(*this);
						result.stepForward(add);
						return result;
					}
					
					/**
					 * Subtraction operator
					 */
					Iterator operator -(size_t subtract)
					{
						Iterator result(*this);
						result.stepBackward(subtract);
						return result;
					}
					
					/**
					 * Prefix increment operator
					 * Return a reference to this Iterator, after modification
					 */
					Iterator& operator++()
					{
						this->stepForward(1);
						return *this;
					}
					
					/**
					 * Postfix increment
					 * Return a copy of this Iterator, BEFORE it was modified
					 */
					Iterator operator++(int)
					{
						Iterator old(*this);
						this->stepForward(1);
						return old;
					}
					
					/**
					 * Prefix decrement operator
					 * Return a reference to this Iterator, after modification
					 */
					Iterator& operator--()
					{
						this->stepBackward(1);
						return *this;
					}
					
					/**
					 * Postfix decrement operator
					 * Return a copy of this Iterator BEFORE it was modified
					 */
					Iterator operator--(int)
					{
						Iterator old(*this);
						this->stepBackward(1);
						return old;
					}
					
					/**
					 * AdditionAssignment operator
					 * Return a copy of the current iterator, after modification
					*/
					Iterator operator +=(size_t add)
					{
						this->stepForward(add);
						return *this;
					}
					/**
					 * SubtractionAssignment operator
					 * Return a copy of the current iterator, after modification
					 */
					Iterator operator -=(size_t sub)
					{
						this->stepBackward(sub);
						return *this;
					}
					
					/**
					 * AdditionAssignment operator, supporting positive or negative ints
					 */
					Iterator operator +=(int add)
					{
						if ( add >= 0 ) {
							this->stepForward(static_cast<size_t>(add));
						}
						else {
							this->stepBackward(static_cast<size_t>(-static_cast<long long>(add)));
						}
						return *this;
					}
					
					/**
					 * SubtractionAssignment operator, supporting positive or negative ints
					 */
					Iterator operator -=(int subtract)
					{
						if ( subtract >= 0 ) {
							this->stepBackward(static_cast<size_t>(subtract));
						}
						else {
							this->stepForward(static_cast<size_t>(-static_cast<long long>(subtract)));
						}
						return *this;
					}
					
					/**
					 * Dereference operator returns a reference to the ELEMENT contained with the current node
					 */
					T& operator*()
					{
						if ( this->cursor_ == nullptr ) {
							throw std::out_of_range("Cannot dereference an iterator that is at end()");
						}
						return this->cursor_->getElement();
					}
				
				private:
					
					/// Helper: move forward 'count' nodes, stopping at end (nullptr)
					void stepForward(size_t count)
					{
						for ( size_t i = 0; i < count && this->cursor_ != nullptr; i++ ) {
							this->cursor_ = this->cursor_->getNext();
						}
					}
					
					/**
					 * Helper: move backward 'count' nodes.
					 * Stepping back from end() lands on the tail.
					 * Stepping back from the head lands on end().
					 */
					void stepBackward(size_t count)
					{
						for ( size_t i = 0; i < count; i++ ) {
							if ( this->cursor_ == nullptr ) {
								this->cursor_ = this->tail_;
							}
							else {
								this->cursor_ = this->cursor_->getPrev();
							}
							if ( this->cursor_ == nullptr ) {
								break;
							}
						}
					}
					
					/// Pointer to the head node
					Node* head_ = nullptr;
					
					/// Pointer to the tail node
					Node* tail_ = nullptr;
					
					/**
					 * Pointer to the cursor node.
					 */
					Node* cursor_ = nullptr;
				
				friend class DoublyLinkedList;
			};
			
			/// Default constructor
			DoublyLinkedList()
			{
				this->head_ = nullptr;
				this->tail_ = nullptr;
				this->size_ = 0;
			}
			
			///	Copy Constructor
			DoublyLinkedList(const DoublyLinkedList& other)
			{
				this->copyFrom(other);
			}
			
			/// DTOR
			~DoublyLinkedList()
			{
				this->clear();
			}
			
			/**
			 * Clear the list and assign the same value, count times.
			 * 
			 * Example:
			 *   T is an int
			 *   count is 5
			 *   value = 3
			 * 
			 * Our list would become:
			 *   {3, 3, 3, 3, 3}
			 */
			void assign(size_t count, const T& value)
			{
				this->clear();
				for ( size_t i = 0; i < count; i++ ) {
					this->push_back(value);
				}
			}
			
			/**
			 * Clear the list and assign values from another list.
			 * The 'first' iterator points to the first item copied from the other list.
			 * The 'last' iterator points to the last item copied from the other list.
			 * 
			 * Example:
			 * 	Suppose we have a source list like {8, 4, 3, 2, 7, 1}
			 * 	Suppose first points to the 4 node
			 *	Suppose last points to the 7 node
			 * 	We should end up with our list becoming: {4, 3, 2, 7}
			 *
			 * If the user code sends out-of-order iterators,
			 * 	just copy from 'first' to the end of the source list
			 * 
			 * Example:
			 *  If we have the same source list {8, 4, 3, 2, 7, 1},
			 *  and first points to the 7 node,
			 *  and last points to the 4 node,
			 *  we would end up with: {7, 1}
			 */
			void assign(Iterator first, Iterator last)
			{
				//	Copy values into a temporary list first,
				//	in case the source is this same list
				DoublyLinkedList<T> temp;
				Node* stop = last.getCursor();
				for ( Node* node = first.getCursor(); node != nullptr; node = node->getNext() ) {
					temp.push_back(node->getElement());
					if ( node == stop ) {
						break;
					}
				}
				
				//	Move temp's nodes into this list
				this->clear();
				this->head_ = temp.head_;
				this->tail_ = temp.tail_;
				this->size_ = temp.size_;
				temp.head_ = nullptr;
				temp.tail_ = nullptr;
				temp.size_ = 0;
			}
			
			/// Return a pointer to the head node, if any
			Node* head() const
			{
				return this->head_;
			}
			
			/// Return a pointer to the tail node, if any
			Node* tail() const
			{
				return this->tail_;
			}
			
			/**
			 * Return an iterator that points to the head of our list
			 */
			Iterator begin() const
			{
				return Iterator(this->head_, this->tail_, this->head_);
			}
			
			/**
			 * Return an iterator that points to the last element (tail) of our list
			 */
			Iterator last() const
			{
				return Iterator(this->head_, this->tail_, this->tail_);
			}
			
			/**
			 * Should return an iterator that represents being past the end of our nodes,
			 * or just that we are finished.
			 * 
			 * You can make this a nullptr or use some other scheme of your choosing,
			 * 	as long as it works with the logic of the rest of your implementations.
			 */
			Iterator end() const
			{
				return Iterator(this->head_, this->tail_, nullptr);
			}
			
			/**
			 * Returns true if our list is empty
			 */
			bool empty() const
			{
				return this->size_ == 0;
			}
			
			/**
			 * Returns the current size of the list
			 * 
			 * Should finish in constant time!
			 * (keep track of the size elsewhere)
			 */
			size_t size() const
			{
				return this->size_;
			}
			
			/**
			 * Clears our entire list, making it empty
			 */
			void clear()
			{
				Node* node = this->head_;
				while ( node != nullptr ) {
					Node* next = node->getNext();
					delete node;
					node = next;
				}
				this->head_ = nullptr;
				this->tail_ = nullptr;
				this->size_ = 0;
			}
			
			/**
			 * Insert an element after the node pointed to by the pos Iterator
			 * 
			 * If the list is currently empty,
			 * 	ignore the iterator and just make the new node at the head/tail (list of length 1).
			 * 
			 * If the incoming iterator is this->end(),
			 *   insert the element as the new tail
			 * 
			 * Should return an iterator that points to the newly added node
			 */
			Iterator insert_after(Iterator pos, const T& value)
			{
				Node* cursor = pos.getCursor();
				
				//	Empty list, or end() iterator: new tail
				if ( this->empty() || cursor == nullptr ) {
					return this->push_back(value);
				}
				
				//	Inserting after the tail is the same as push_back
				if ( cursor == this->tail_ ) {
					return this->push_back(value);
				}
				
				//	Somewhere in the middle
				Node* next = cursor->getNext();
				Node* node = new Node(value, cursor, next);
				cursor->setNext(node);
				next->setPrev(node);
				this->size_++;
				
				return Iterator(this->head_, this->tail_, node);
			}
			
			/**
			 * Insert a new element after the index pos.
			 * Should work with an empty list.
			 * 
			 * If the user attempts to insert to an index
			 * that is out of range (e.g., size=5 but index=13),
			 * just add to the end of the list.
			 * 
			 * Should return an iterator pointing to the newly created node
			*/
			Iterator insert_after(size_t pos, const T& value)
			{
				if ( pos >= this->size_ ) {
					return this->push_back(value);
				}
				
				return this->insert_after(Iterator(this->head_, this->tail_, this->nodeAt(pos)), value);
			}
			
			/**
			 * Erase the node pointed to by the Iterator's cursor.
			 * 
			 * If the 'pos' iterator does not point to a valid node,
			 * 	throw an std::range_error
			 * 
			 * Return an iterator to the node AFTER the one we erased,
			 * 	or this->end() if we just erased the tail
			 */
			Iterator erase(Iterator pos)
			{
				Node* node = pos.getCursor();
				if ( node == nullptr || this->empty() ) {
					throw std::range_error("Cannot erase: iterator does not point to a valid node");
				}
				
				Node* prev = node->getPrev();
				Node* next = node->getNext();
				
				if ( prev != nullptr ) {
					prev->setNext(next);
				}
				else {
					this->head_ = next;
				}
				
				if ( next != nullptr ) {
					next->setPrev(prev);
				}
				else {
					this->tail_ = prev;
				}
				
				delete node;
				this->size_--;
				
				return Iterator(this->head_, this->tail_, next);
			}
			
			/**
			 * Add an element just after the one pointed to by the 'pos' iterator
			 * 
			 * Should return an iterator pointing to the newly created node
			 */
			Iterator push_after(Iterator pos, const T& value)
			{
				return this->insert_after(pos, value);
			}
			
			/**
			 * Add a new element to the front of our list.
			 */
			void push_front(const T& value)
			{
				Node* node = new Node(value, nullptr, this->head_);
				if ( this->head_ != nullptr ) {
					this->head_->setPrev(node);
				}
				else {
					this->tail_ = node;
				}
				this->head_ = node;
				this->size_++;
			}
			
			/**
			 * Add an element to the end of this list.
			 * 
			 * Should return an iterator pointing to the newly created node.
			 */
			Iterator push_back(const T& value)
			{
				Node* node = new Node(value, this->tail_, nullptr);
				if ( this->tail_ != nullptr ) {
					this->tail_->setNext(node);
				}
				else {
					this->head_ = node;
				}
				this->tail_ = node;
				this->size_++;
				
				return Iterator(this->head_, this->tail_, node);
			}
			
			/**
			 * Remove the node at the front of our list
			 * 
			 * Should throw an exception if our list is empty
			 */
			void pop_front()
			{
				if ( this->empty() ) {
					throw std::range_error("Cannot pop_front from an empty list");
				}
				this->erase(this->begin());
			}
			
			/**
			 * Remove the node at the end of our list
			 * 
			 * Should throw an exception if our list is empty
			 */
			void pop_back()
			{
				if ( this->empty() ) {
					throw std::range_error("Cannot pop_back from an empty list");
				}
				this->erase(this->last());
			}
			
			/**
			 * Return a reference to the element at the front.
			 * 
			 * Throw an exception if the list is empty
			 */
			T& front()
			{
				if ( this->empty() ) {
					throw std::range_error("Cannot call front() on an empty list");
				}
				return this->head_->getElement();
			}
			
			/**
			 * Return a reference to the element at the back.
			 * 
			 * Throw an exception if the list is empty
			 */
			T& back()
			{
				if ( this->empty() ) {
					throw std::range_error("Cannot call back() on an empty list");
				}
				return this->tail_->getElement();
			}
			
			/**
			 * Return the element at an index
			 * 
			 * Should throw a range_error is out of bounds
			 */
			T& at(size_t index)
			{
				if ( index >= this->size_ ) {
					throw std::range_error("Index out of bounds in at()");
				}
				return this->nodeAt(index)->getElement();
			}
			
			/**
			 * Reverse the current list
			 */
			void reverse()
			{
				Node* node = this->head_;
				while ( node != nullptr ) {
					Node* next = node->getNext();
					node->setNext(node->getPrev());
					node->setPrev(next);
					node = next;
				}
				std::swap(this->head_, this->tail_);
			}
			
			/**
			 * I bet you're happy I'm not making you do this.
			 * No tests will be run against this function,
			 * 	but feel free to try it out, as a challenge!
			 * 
			 * If I were doing this and didn't care too much for efficiency,
			 * 	I would probably create an extra helper function to swap two
			 * 	positions in the current list.
			 * Then I would simply sweep through the list and perform
			 *  the bubble-sort algorithm. Perhaps selection sort.
			 * 
			 * If you want a huge challenge, try implementing quicksort.
			 * 
			 * (but again, don't worry about this method; it will not be tested)
			 */
			void sort()
			{
				//	Simple bubble sort, swapping elements between nodes
				if ( this->size_ < 2 ) {
					return;
				}
				bool swapped = true;
				while ( swapped ) {
					swapped = false;
					for ( Node* node = this->head_; node->getNext() != nullptr; node = node->getNext() ) {
						Node* next = node->getNext();
						if ( next->getElement() < node->getElement() ) {
							std::swap(node->getElement(), next->getElement());
							swapped = true;
						}
					}
				}
			}
			
			/**
			 * Assignment operator
			 * 
			 * Clear this list and fill it with the others' values
			 * (by value, not by reference)
			 * 
			 * Return a reference to this list
			 */
			DoublyLinkedList<T>& operator =(const DoublyLinkedList<T>& other)
			{
				if ( this != &other ) {
					this->clear();
					this->copyFrom(other);
				}
				return *this;
			}
			
			/**
			 * Return true if the lists are "equal"
			 * 
			 * "Equal" here is defined as:
			 * - Same size
			 * - Elements at the same indexes would return true for their own comparison operators
			 * 
			 * In other words: "They contain all the same values"
			 * (no need for their pointers or addresses to be the same)
			 */
			bool operator ==(DoublyLinkedList<T>& other)
			{
				if ( this->size_ != other.size_ ) {
					return false;
				}
				
				Node* a = this->head_;
				Node* b = other.head_;
				while ( a != nullptr && b != nullptr ) {
					if ( a->getElement() != b->getElement() ) {
						return false;
					}
					a = a->getNext();
					b = b->getNext();
				}
				
				return true;
			}
			
			/**
			 * Return true if the lists are "not equal"
			 * 
			 * See the operator== stub for definition of "equal"
			 */
			bool operator !=(DoublyLinkedList<T>& other)
			{
				return !(this->operator==(other));
			}
			
		private:
			
			/// Helper: copy all values from another list into this (assumed empty) list
			void copyFrom(const DoublyLinkedList& other)
			{
				this->head_ = nullptr;
				this->tail_ = nullptr;
				this->size_ = 0;
				for ( Node* node = other.head_; node != nullptr; node = node->getNext() ) {
					this->push_back(node->getElement());
				}
			}
			
			/// Helper: return the node at an index (assumes index is valid).
			/// Walks from whichever end is closer.
			Node* nodeAt(size_t index) const
			{
				Node* node;
				if ( index < this->size_ / 2 ) {
					node = this->head_;
					for ( size_t i = 0; i < index; i++ ) {
						node = node->getNext();
					}
				}
				else {
					node = this->tail_;
					for ( size_t i = this->size_ - 1; i > index; i-- ) {
						node = node->getPrev();
					}
				}
				return node;
			}
			
			Node* head_ = nullptr;
			Node* tail_ = nullptr;
			size_t size_ = 0;
	};
}

#endif
