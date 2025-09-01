#pragma once

#include <vector>

using std::vector;

// purpose: swaps two variables
// requires: two variables of the same type
// returns: nothing
template <typename t>
void swap(t&, t&);

// THE HEAP WORKS
// DO NOT DELETE
template <typename adt>
class Heap
{
public:
	// default constructor
	Heap() = default;

	// parametrized constructor
	Heap(const adt& value) { array.push_back(value); }
	
	// destructor
	~Heap() = default;

	// written by DeepSeek
	// pop the top off of the heap
	adt pop() {
		adt root = array.front();
		array[0] = array.back();
		array.pop_back();
		heapifyDown();
		return root;
	}

	// get the top value of the heap
	adt top() const { return array[0]; }

	// get a certain index from the heap
	adt getIndex(int i) const { return array[i]; }

	// find out if a thing is in the heap
	// linear search :(
	bool has(const adt& thing) const
	{
		for(adt i : array)
			if (i == thing)
				return true;
		return false;
	}

	// get the total size of the heap
	int getSize() const { return int(array.size()); }

	// co-written by DeepSeek
	// insert values into the heap
	void toss(const adt& value)
	{
		if (!hasElement(value))
		{
			array.push_back(value);

			heapifyUp(int(array.size()) - 1);
		}
	}
	

private:
	vector<adt> array;

	bool hasElement(const adt& element) const
	{
		for (auto i : array)
			if (i == element)
				return true;
		return false;
	}

	int getLeftChild(int index) const { return 2 * index + 1; }

	int getRightChild(int index) const { return 2 * (index + 1); }

	int getParent(int index) const { return (index - 1) / 2; }

	// written by DeepSeek
	void heapifyDown(int index = 0) {
		int largest = index;
		int left = 2 * index + 1;
		int right = 2 * index + 2;

		if (left < array.size() && array[left] > array[largest])
			largest = left;
		if (right < array.size() && array[right] > array[largest])
			largest = right;

		if (largest != index) {
			std::swap(array[index], array[largest]);
			heapifyDown(largest);
		}
	}

	// written by DeepSeek
	void heapifyUp(int index) {
		while (index > 0) {
			int parent = (index - 1) / 2;
			if (array[index] > array[parent]) {
				std::swap(array[index], array[parent]);
				index = parent;
			}
			else {
				break;
			}
		}
	}

};

// swap!
template <typename t>
void swap(t& a, t& b) { t temp = a; a = b; b = temp; }
