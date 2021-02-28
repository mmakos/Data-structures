#include <chrono>
#include <iostream>

#include "Vector.h"
#include "LinkedList.h"

int main()
{
	aisdi::Vector< int > collectionVector;
	aisdi::LinkedList< int > collectionList;

	/*------------------------ PREPEND WHEN X ELEMENTS ------------------------*/

	std::cout << std::endl << std::endl << " Prepend in Vector:" << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Prepend when no element:      " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionVector.getSize() - 1 << " elements:      " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8; ++i ) collectionVector.append( 1 );
	
	start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionVector.getSize() - 1 << " elements:     " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89; ++i ) collectionVector.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionVector.getSize() - 1 << " elements:    " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 899; ++i ) collectionVector.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionVector.getSize() - 1 << " elements:   " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8999; ++i ) collectionVector.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionVector.getSize() - 1 << " elements:  " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89999; ++i ) collectionVector.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionVector.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionVector.getSize() - 1 << " elements: " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	/* -------------------------------------------------------------------- */

	std::cout << std::endl;
	std::cout << std::endl << std::endl << " Prepend in LinkedList:" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when no element:      " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionList.getSize() - 1 << " elements:      " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8; ++i ) collectionList.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionList.getSize() - 1 << " elements:     " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89; ++i ) collectionList.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionList.getSize() - 1 << " elements:    " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 899; ++i ) collectionList.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionList.getSize() - 1 << " elements:   " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8999; ++i ) collectionList.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionList.getSize() - 1 << " elements:  " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89999; ++i ) collectionList.append( 1 );

	start = std::chrono::high_resolution_clock::now();
	collectionList.prepend( 1 );
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "Prepend when " << collectionList.getSize() - 1 << " elements: " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	/*------------------------ POPPING FIRST ------------------------*/

	collectionVector.popLast();

	std::cout << std::endl << std::endl << "PopFirst in Vector:" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	collectionVector.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionVector.getSize() + 1 << " elements: " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89999; ++i ) collectionVector.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionVector.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionVector.getSize() + 1 << " elements:  " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8999; ++i ) collectionVector.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionVector.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionVector.getSize() + 1 << " elements:   " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 899; ++i ) collectionVector.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionVector.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionVector.getSize() + 1 << " elements:    " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89; ++i ) collectionVector.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionVector.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionVector.getSize() + 1 << " elements:     " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8; ++i ) collectionVector.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionVector.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionVector.getSize() + 1 << " elements:      " << elapsed_seconds.count() * 1000000 << " us" << std::endl;
	
	/* ------------------------------------------------------------------------------------ */

	collectionList.popLast();

	std::cout << std::endl << std::endl << "PopFirst in LinkedList:" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	collectionList.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionList.getSize() + 1 << " elements: " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89999; ++i ) collectionList.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionList.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionList.getSize() + 1 << " elements:  " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8999; ++i ) collectionList.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionList.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionList.getSize() + 1 << " elements:   " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 899; ++i ) collectionList.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionList.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionList.getSize() + 1 << " elements:    " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 89; ++i ) collectionList.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionList.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionList.getSize() + 1 << " elements:     " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	for( auto i = 0; i < 8; ++i ) collectionList.popLast();

	start = std::chrono::high_resolution_clock::now();
	collectionList.popFirst();
	end = std::chrono::high_resolution_clock::now();
	elapsed_seconds = end - start;
	std::cout << "PopFirst when " << collectionList.getSize() + 1 << " elements:      " << elapsed_seconds.count() * 1000000 << " us" << std::endl;

	return 0;
}