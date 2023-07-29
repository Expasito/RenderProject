#pragma once

#include "Render/Headers.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
* FillerArray is similar to a dynamic array, but it has 2 new features.
* 1. when removing an element, the last element goes in to fill its spot, so all elements are sequentail
* 2. there is a hash table for finding the index of elements efficiently when they get moved.
* Goal time complexity:
* Add: O(1): just add to the end of the array
* Get: O(1): get the index from the hash table and then from the array
* Remove: O(1): remove the element, fill with the last element and update hashtable
*
* All elements are sequential so we do not need to put the correct elements in a new array for graphics
*/

/*
* How it works:
* When adding an element: FillerArray's dynamic array returns a key to get that element back
* This key is added to the Hashtable which gives the index in the array.
*
*/
class FillerArray {
public:

	// this is each element in a FillerArray
	struct Element {
		int key;
		//data goes bellow
		glm::vec3 translations;
		glm::vec3 rotations;
		glm::vec3 scalations;
		glm::vec3 colors;
	};

	// this will hold the keys and indexes
	// Note:  a returned Element object with index of -1 means the get/remove function failed to find since index of -1 is not legal
	class HashTable {
	public:

		struct Element {
			bool used;
			int key;
			int index;
		};

		HashTable::Element* arr;
		int size;
		int elements;
		float load;
		HashTable(int size_) {
			arr = new HashTable::Element[size_];
			size = size_;

			init();

			elements = 0;
			load = 0;

		}

		~HashTable() {
			delete[] arr;
		}

		void see() {
			std::cout << "Load: " << load << "\n";
			for (int i = 0; i < size; i++) {
				HashTable::Element temp = arr[i];
				if (temp.used == 1) {
					std::cout << i << ": (" << temp.key << ", " << temp.index << ")\n";
				}
				else {
					std::cout << i << ":\n";

				}

			}
			std::cout << "\n";
		}

		void add(int key, int index) {
			int index_ = _hash(key);

			while (true) {
				Element temp = arr[index_];
				// 1 means used
				if (temp.used == 0) {
					//arr[index_] = { 1,key,index };
					arr[index_].used = 1;
					arr[index_].key = key;
					arr[index_].index = index;
					break;
				}
				else {
					index_++;
					if (index_ > size) {
						index_ = 0;
					}
				}
			}

			elements++;
			load = (float)elements / size;

		}


		// this returns a reference for editing
		HashTable::Element* get(int key) {
			int index_ = _hash(key);
			int index__ = index_;

			while (true) {
				HashTable::Element* temp = &arr[index_];
				if (temp->key == key) {
					//arr[index_] = { 1,key,index };
					return temp;
					break;
				}
				else {
					index_++;
					if (index_ > size) {
						index_ = 0;
					}
					// if we do not get a match
					if (index_ == index__) {
						return NULL;
					}
				}
			}
		}

		// this returns a copy, not a reference
		HashTable::Element remove(int key) {
			int index_ = _hash(key);
			int index__ = index_;

			while (true) {
				HashTable::Element temp = arr[index_];
				if (temp.key == key) {
					//arr[index_] = { 1,key,index };
					arr[index_].used = 0;
					elements--;
					load = (float)elements / size;
					return temp;
					break;
				}
				else {
					index_++;
					if (index_ > size) {
						index_ = 0;
					}
					// if we do not get a match
					if (index_ == index__) {
						return { 0,-1,-1 };
					}
				}
			}
		}

		int _hash(int key) {
			return key % size;
		}

	private:

		void init() {
			for (int i = 0; i < size; i++) {
				arr[i].key = -1;
				arr[i].index = -1;
				arr[i].used = 0;
			}
		}

	};

	// this will hold the actual elements, do the resizing, etc
	class DynamicArray {
	public:
		FillerArray::Element* arr;
		int size;
		int elements;
		DynamicArray(int size_) {
			arr = new FillerArray::Element[size_];
			size = size_;
			elements = 0;
		}

		~DynamicArray() {
			delete[] arr;
		}

		void add(long key, glm::vec3 trans, glm::vec3 rot, glm::vec3 scal, glm::vec3 color) {
			arr[elements] = { key, trans,rot,scal,color };

			elements++;

			if (elements == size) {
				resize(size * 2);
			}

		}

		FillerArray::Element* get(int index) {
			return &arr[index];
		}

		// remove auto puts the last element in the slot to fill the spot
		FillerArray::Element remove(int index) {

			FillerArray::Element out = arr[index];
			arr[index] = arr[elements - 1];
			elements--;

			return out;
		}

		void resize(int size_) {
			FillerArray::Element* temp = new FillerArray::Element[size_];
			for (int i = 0; i < size; i++) {
				temp[i] = arr[i];
			}

			delete[] arr;
			arr = temp;
			size = size_;

		}

		void see() {
			std::cout << "Elements: " << elements << " Size: " << size << "\n";
			for (int i = 0; i < elements; i++) {
				std::cout << i << ": Key: " << arr[i].key << " :: " << arr[i].translations.x << " " << arr[i].translations.y << " " << arr[i].translations.z << "\n";
			}
			std::cout << "\n";
		}

	private:
	};


	// 
	// 
	// 
	// NOTE: Or just use the binary heap. It is a priority queue and is linear.
	// Does not have O(1) add/remove, but is alreayd prebuilt
	// 
	// 
	// 
	// IPQueue is short of 'In Place Queue'
	// It is a queue that is fast(all O(1)) but also takes up sequential memory slots
	// Basically, same as the DynamicArray class
	/*
	* Enqueue an element: put on the back of the array, nextIndex is -1(meaning none)
	* Dequeue an element: take the first element off. Put the last element there and update the refernce
	*
	*/
	class IPQueue {
	public:
		// last and next give the index of the previous and next element in the queue
		// key is the actual key
		struct Element {
			int last;
			int next;
			int key;
		};

	private:
	};


	// Time for the real meat of the FillerArray class

	HashTable* ht;
	DynamicArray* da;
	long key;
	// size1_ is the base size for the HashTable and size2_ is the base size for the Dynamic array
	FillerArray(int size1_, int size2_) {
		ht = new HashTable(size1_);
		da = new DynamicArray(size2_);
		key = 10;

	}

	~FillerArray() {
		delete ht;
		delete da;
	}

	long add(glm::vec3 trans_, glm::vec3 rot_, glm::vec3 scal_, glm::vec3 color_) {
		long newkey = getNextKey();

		ht->add(newkey, da->elements);
		da->add(newkey, trans_, rot_, scal_, color_);

		if (ht->load > .75) {
			// now we need to resize and rehash each key
			HashTable* temp = new HashTable(ht->size * 2);
			for (int i = 0; i < ht->size; i++) {
				HashTable::Element temp_ = ht->arr[i];
				if (ht->arr[i].used == true) {
					//std::cout << " Key: " << ht->arr[i].key << " Index: " << ht->arr[i].index << "\n";
					temp->add(ht->arr[i].key, ht->arr[i].index);
				}
				
			}
			//std::cout << "Start\n";
			//ht->see();
			//std::cout << "Middle\n";
			//temp->see();
			//std::cout << "End\n";
			delete ht;
			ht = temp;
		}

		return newkey;
	}

	FillerArray::Element* get(long key) {
		int index = ht->get(key)->index;
		return da->get(index);

	}

	// pay close attention to this method and make sure it works well
	FillerArray::Element remove(long key) {
		HashTable::Element* temp = ht->get(key);
		int index = temp->index;
		std::cout << "removing at index: " << index << "\n";
		FillerArray::Element out = da->remove(index);
		//ht->get(da->elements + 1)->index = index;
		ht->remove(key);

		// update the hashtable for the last element of the array
		FillerArray::Element* new_ = da->get(da->elements);
		ht->get(new_->key)->index = index;
		//ht->get(key)->index = index;
		//std::cout << ht->get(key)->key << "\n";

		return out;
	}

	// time to add the method for getting keys
	long getNextKey() {
		return key++;
	}

	void see() {
		std::cout << "View Filler Array: \n";
		std::cout << "   HashTable:\n";
		ht->see();
		std::cout << "   DynamicArray:\n";
		da->see();
	}

private:

};