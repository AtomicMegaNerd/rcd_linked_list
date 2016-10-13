#ifndef RCD_LIST_H
#define RCD_LIST_H

#include <cstddef>
#include <ostream>
#include <stdexcept>

//
// TODO items
// ============================================================
// 1. Add support for use with STL / iterators
// 2. Make use of cppunit or something and figure out a way to write
//    a unit test.
//

namespace rcd {

template<typename T> class List;

/*
 * Node object for our singly linked list.  The list will
 * consist of these Node objects.
 */
template<typename T> class Node {

	// Let List objects of the same generic type as the Node
	// access the members of the Node and construct Node objects
	friend class List<T> ;

	// operator<< needs access to the members of the Node class
	// to print out our linked lists.
	template<class E> friend std::ostream& operator<<(std::ostream&,
			const List<E>&);

	// Constructor
	Node(const T& d, Node* p=0, Node* n=0) : data(d), prev(p), next(n) { }

	T data; // This is the data we are storing

	Node<T>* prev; // The previous node in the list
	Node<T>* next; // The next node in the list

};

/*
 * This class represents our doubly linked list
 */
template<typename T> class List {

	template<class E> friend std::ostream& operator<<(std::ostream&,
			const List<E>&);

public:

	typedef std::size_t size_type;
	typedef const T* const_iterator;
	typedef T* iterator;

	List() : head(0), tail(0), num_nodes(0) { }
	List(const List& cpy) : head(0), tail(0), num_nodes(0) {
		copy_nodes(cpy);
	}
	List& operator=(const List& rhs);
	~List() { destroy_list(); }


	// List modifier functions
	void push_front(const T& data);
	void push_back(const T& data);
	void insert(const size_type index, const T& data);
	void erase(const T& data);
	void erase(const size_type index);

	// List accessor/info functions
	bool find(const T& data) const;
	const T& operator[](const size_type index) const {
		return at(index);
	}

	T& begin() { return head->data; }
	const T& begin() const { return head->data; }

	T& end() { return tail->data; }
	const T& end() const { return tail->data; }

	size_type size() const { return num_nodes; }

	bool empty() const { return (num_nodes == 0); }

private:

	const T& at(const size_type index) const;

	// Utility functions
	void copy_nodes(const List& cpy);
	void destroy_list();
	void check_range(size_type index) const {
		if (index < 0 || index >= num_nodes) {
			throw new std::out_of_range("invalid index specified");
		}
	}

	// Class members
	Node<T>* head; // First item in the list
	Node<T>* tail; // Last item in the list
	size_type num_nodes; // Number of nodes in the list

};

template<typename T> List<T>& List<T>::operator=(const List& rhs) {
	if (this != &rhs) {
		destroy_list();
		copy_nodes(rhs);
	}
	return *this;
}

/*
 * Add a node to the front of the list
 */
template<typename T> void List<T>::push_front(const T& data) {
	if (empty()) {
		head = tail = new Node<T>(data);
	} else {
		Node<T>* new_node = new Node<T>(data, 0, head);
		head = new_node;
		head->prev = 0;
	}
	++num_nodes;
}

/*
 * Add a node to the back of the list
 */
template<typename T> void List<T>::push_back(const T& data) {
	if (empty()) {
		head = tail = new Node<T>(data);
	} else {
		Node<T>* new_node = new Node<T> (data, tail);
		tail->next = new_node;
		tail = new_node;
	}
	++num_nodes;
}

/*
 * Add a node at the specified position in the list
 */
template<typename T> void List<T>::insert(const size_type index, const T& data) {

	if (empty() && index == 0) { // Special case if the list is empty
		head = tail = new Node<T>(data);
	} else {

		/*
		 * If the list is not empty, check the supplied index to
		 * make sure it is valid, and then find out where
		 * we need to add the new node.
		 */
		check_range(index);

		// We only construct the new node if the range
		// check passed... otherwise we'd get a leak.
		Node<T>* new_node = 0;
		Node<T>* curr = head;

		// Get to the index position that we want
		for (size_type i = 0; curr != 0; curr = curr->next) {
			if (index == i++) {
				break;
			}
		}

		if (curr != 0) {
			if (curr->prev == 0) { // Same as push_front, insert at the beginning
				new_node = new Node<T> (data, 0, head);
				head = new_node;
			} else if (curr->next == 0) { // Same as push back, insert at end
				new_node = new Node<T> (data, tail);
				tail->next = new_node;
				tail = new_node;
			} else { // Insert somewhere in the middle
				new_node = new Node<T> (data, curr->prev, curr);
				curr->prev->next = new_node;
			}
		}
	}
	++num_nodes;
}

/*
 * Remove the first node in the list that contains
 * the supplied data.
 */
template<typename T> void List<T>::erase(const T& data) {

	Node<T>* curr = head;
	for (; curr != 0; curr = curr->next) {
		if (curr->data == data) {
			break;
		}
	}

	if (curr != 0) {
		if (curr->prev == 0) {
			head = curr->next;
		} else {
			curr->prev->next = curr->next;
		}

		if (curr->next == 0) {
			tail = curr->prev;
		} else {
			curr->next->prev = curr->prev;
		}

		delete(curr);
		--num_nodes;
	}
}

/*
 * Remove the node at the specified position
 */
template<typename T> void List<T>::erase(const size_type index) {

	check_range(index);
	Node<T>* curr = head;

	for (size_type i = 0; curr != 0; curr = curr->next) {
		if (index == i++) {
			break;
		}
	}

	if (curr != 0) {
		if (curr->prev == 0) {
			head = curr->next;
		} else {
			curr->prev->next = curr->next;
		}

		if (curr->next == 0) {
			tail = curr->prev;
		} else {
			curr->next->prev = curr->prev;
		}

		delete(curr);
		--num_nodes;
	}

}

/*
 * return the item contained within the list at the specified index
 */
template<typename T> const T& List<T>::at(const size_type index) const {
	check_range(index);
	Node<T> *n = head;
	for (size_type i = 0; i != index; n = n->next, ++i)
		; // Do nothing
	return n->data;
}

/*
 * Search the list for the specified data.  Return true if the
 * item is in the list, otherwise return false.
 */
template<typename T> bool List<T>::find(const T& data) const {
	for (Node<T> *prev = 0, *curr = head; curr != 0; prev = curr, curr
			= curr->next) {
		if (curr->data == data) {
			return true;
		}
	}
	return false;
}

/*
 * Destroys all nodes in the list... freeing the memory properly
 */
template<typename T> void List<T>::destroy_list() {
	while (!empty()) {
		erase(0);
	}
}

/*
 * Utility function for copy control, copies all the nodes
 * from an external List to this List.
 */
template<typename T> void List<T>::copy_nodes(const List& cpy) {
	for (Node<T> *node = cpy.head; node != 0; node = node->next) {
		push_back(node->data);
	}
}

/*
 * Output the contents of the List.
 */
template<typename T> std::ostream& operator<<(std::ostream& out, const List<T>& l) {
	out << "< ";
	for (Node<T>* node = l.head; node != 0; node = node->next) {
		out << node->data;
		// If this is not the last item in the list
		// print a separating comma and space...
		if (node->next != 0) out << ", ";
	}
	out << " >";
	return out;
}

}

#endif /* RCD_LIST_H */
