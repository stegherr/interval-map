// interval-map.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <map>
#include <iostream>
#include <cassert>
template<typename K, typename V>
class interval_map {
	void printIntervalMap(interval_map<int, char>& ivm_mapOfInts);
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

		// empty interval, do nothing
		if (!(keyBegin < keyEnd))
			return;

		// save last value of given range in lastVal, initialize with begin value
		// TODO: increase efficiency (no copying of val)
		V lastVal = m_valBegin;

		// get iterator for key greater than keyBegin
		auto it = m_map.upper_bound(keyBegin);

		// is keyBegin before first element?
		if (it == m_map.begin()) {
			// if first entry in m_map is not the same value as m_valBegin
			// check for canonicity consecutive map entries can't have same val
			// here it is first entry so check only entry after key
			if ((val != m_valBegin) && (it->second != val))
				// add key/value pair for keyBegin with val
				m_map.insert_or_assign(keyBegin, val);
		}
		else {
			it--;
			// before inserting key, check key before and after for value
			if ((it->second != val) && ((++it)->second != val))
				// add key/value pair for keyBegin with val
				m_map.insert_or_assign(keyBegin, val);
		}

		// set iterator to the next key after keyBegin
		it = m_map.upper_bound(keyBegin);

		// while iterator before keyEnd or end of map
		while ((it != m_map.end()) && (it->first < keyEnd))
		{
			// save value of last pair in range
			lastVal = it->second;
			// delete key/value pairs in map until keyEnd
			it = m_map.erase(it);
		}
		
		// first check if any entries left
		
		// check for canonicity consecutive map entries can't have same val
		// before inserting key, check key before for value

		// set iterator to the previous key before keyEnd
		it = m_map.lower_bound(keyBegin);
		if (it->second != lastVal) {
			it++;
			// if at end add key/value pair
			// TODO simplify these clauses
			if (it == m_map.end())
				// add key/value pair for keyEnd with val
				m_map.insert_or_assign(keyEnd, lastVal);
			else
				// if it not at end also check after key
				if(it->second != lastVal)
					// add key/value pair for keyEnd with val
					m_map.insert_or_assign(keyEnd, lastVal);
		}

	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		// upper_bound returns it with key of element greater than key variable
		auto it=m_map.upper_bound(key);
		// if key not found
		if(it==m_map.begin()) {
			// return the standard value
			return m_valBegin;
		} else {
			// else iterate backwards and return the key
			return (--it)->second;
		}
	}
};

void printIntervalMap(interval_map<int, char>& ivm_mapOfInts)
{
	for (int i = -10; i < 10; i++)
		std::cout << "key = " << i << ',' << "val = " << ivm_mapOfInts[i] << '\n';
	std::cout << std::endl;
}

void IntervalMapTest()
{
	// initialize interval map with value 'X'
	interval_map<int, char> ivm_mapOfInts('X');

	// insert test key/value pairs
	std::map<int, char>testKVPairs = { {-3, 'D'}, {1,'B'}, {3, 'A'}, {7, 'C'} };
	ivm_mapOfInts.m_map = testKVPairs;

	// print key/value pairs of interval map from keys -10 to 10
	printIntervalMap(ivm_mapOfInts);

	// TESTS assign test value to range
	// keyBegin > keyEnd should do nothing
	ivm_mapOfInts.assign(6, 5, 'Y');
	printIntervalMap(ivm_mapOfInts);

	// keyBegin == keyEnd should do nothing
	ivm_mapOfInts.assign(1, 1, 'Y');
	printIntervalMap(ivm_mapOfInts);

	// assign Z to keys 2 to 5
	ivm_mapOfInts.assign(2, 5, 'Z');
	printIntervalMap(ivm_mapOfInts);

	// assign L to keys -9 to 4
	ivm_mapOfInts.assign(-9, 4, 'L');
	printIntervalMap(ivm_mapOfInts);

	// assign L to keys -9 to 9
	ivm_mapOfInts.assign(-9, 9, 'L');
	printIntervalMap(ivm_mapOfInts);

	// test with random data
	struct assignData {
		int  keyBegin;
		int  keyEnd;
		char val;
	} testData;

	for (int i = 0; i < 10; i++) {
		testData.keyBegin = -10 + rand() % 10;
		testData.keyEnd = -10 + rand() % 10;
		testData.val = char('A' + rand() % 26);
		std::cout << testData.keyBegin << ", " << testData.keyEnd << ", " << testData.val << '\n';

		ivm_mapOfInts.assign(testData.keyBegin, testData.keyEnd, testData.val);
		printIntervalMap(ivm_mapOfInts);
	}
}

int main()
{
	IntervalMapTest();
}

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
