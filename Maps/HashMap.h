#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#define HASHMAP_DEF_SIZE 10

#include <list>
#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <functional>




namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
	using key_type = KeyType;
	using mapped_type = ValueType;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = std::size_t;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using Type = std::list< value_type >;

	class ConstIterator;
	class Iterator;
	using iterator = Iterator;
	using const_iterator = ConstIterator;

protected:
	Type* m_array;
	size_type m_size;

public:
	HashMap() : m_size( 0 ){
		m_array = new Type[ HASHMAP_DEF_SIZE + 1 ];		//last element is end
	}

	HashMap( std::initializer_list<value_type> list ) : HashMap()
	{
		for( auto it = list.begin(); it != list.end(); ++it ){
			append( *it );
		}
	}

	HashMap( const HashMap& other ) : HashMap()
	{
		for( auto it = other.begin(); it != other.end(); ++it ){
			append( *it );
		}
	}

	HashMap( HashMap&& other ) : m_array( other.m_array ), m_size( other.m_size )
	{
		other.m_array = nullptr;
		other.m_size = 0;
	}

	~HashMap()
	{
		delete[] m_array;
	}

	HashMap& operator=( const HashMap& other )
	{
		if( &other == this ) return *this;
		deleteHashMap();
		for( auto it = other.begin(); it != other.end(); ++it ){
			append( *it );
		}
		m_size = other.m_size;
		return *this;
	}

	HashMap& operator=( HashMap&& other )
	{
		if( &other == this ) return *this;
		deleteHashMap();
		m_array = other.m_array;
		m_size = other.m_size;
		other.m_array = nullptr;
		other.m_size = 0;
		return *this;
	}

	bool isEmpty() const
	{
		return m_size == 0;
	}

	mapped_type& operator[]( const key_type& key )
	{
		iterator it = find( key );
		if( it == end() )
			it = append( std::make_pair( key, mapped_type{} ) );
		return ( *it ).second;
	}

	const_iterator append( value_type item )
	{
		m_array[ hash( item.first ) ].push_back( item );
		++m_size;
		auto it = m_array[ hash( item.first ) ].end();
		return const_iterator( --it, m_array + hash( item.first ), this );
	}

	const mapped_type& valueOf( const key_type& key ) const
	{
		auto it = find( key );
		if( it == end() ) throw std::out_of_range( "" );
		return (*it).second;
	}

	mapped_type& valueOf( const key_type& key )
	{
		return valueOf( key );
	}

	const_iterator find( const key_type& key ) const
	{
		auto current = m_array + hash( key );
		for( auto it = current->begin(); it != current->end(); ++it ){
			if( (*it).first == key )
				return const_iterator( it, current, this );
		}
		return cend();
	}

	iterator find( const key_type& key )
	{
		auto current = m_array + hash( key );
		for( auto it = current->begin(); it != current->end(); ++it ){
			if( ( *it ).first == key )
				return iterator( it, current, this );
		}
		return end();
	}

	void remove( const key_type& key )
	{
		( void ) key;
		throw std::runtime_error( "TODO" );
	}

	void remove( const const_iterator& it )
	{
		( void ) it;
		throw std::runtime_error( "TODO" );
	}

	size_type getSize() const
	{
		return m_size;
	}

	bool operator==( const HashMap& other ) const
	{
		return m_array == other.m_array;
	}

	bool operator!=( const HashMap& other ) const
	{
		return !( *this == other );
	}

	iterator begin()
	{
		return iterator( cbegin() );
	}

	iterator end()
	{
		return iterator( cend() );
	}

	const_iterator cbegin() const
	{
		auto temp = m_array;
		while( temp->empty() ){
			temp += 1;
			if( temp == m_array + HASHMAP_DEF_SIZE ) return cend();
		}
		return const_iterator( temp->begin(), temp, this );
	}

	const_iterator cend() const
	{
		return const_iterator( m_array[ HASHMAP_DEF_SIZE ].end(), m_array + HASHMAP_DEF_SIZE, this );
	}

	const_iterator begin() const
	{
		return cbegin();
	}

	const_iterator end() const
	{
		return cend();
	}

protected:
	size_type hash( const key_type& key ) const
	{
		return std::hash< key_type >()( key ) % HASHMAP_DEF_SIZE;
	}

	void deleteHashMap()
	{
		for( auto i = 0; i < HASHMAP_DEF_SIZE + 1; ++i )
			m_array[ i ].clear();
	}
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename HashMap::const_reference;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename HashMap::value_type;
	using pointer = const typename HashMap::value_type*;
	using Type = typename HashMap::Type;

protected:
	typename Type::iterator m_it;
	Type* m_ptr;
	const HashMap *m_hashMap;

public:
	explicit ConstIterator( typename Type::iterator it, Type* ptr, const HashMap* hashMap ) : m_it( it ), m_ptr( ptr ), m_hashMap( hashMap )
	{}

	ConstIterator( const ConstIterator& other ) : m_it( other.m_it ), m_ptr( other.m_ptr ), m_hashMap( other.m_hashMap )
	{}

	ConstIterator& operator++()
	{
		if( m_ptr == m_hashMap->m_array + HASHMAP_DEF_SIZE ) throw std::out_of_range( "" );
		if( ++m_it == m_ptr->end() ){
			do{
				m_ptr += 1;
				if( m_ptr == m_hashMap->m_array + HASHMAP_DEF_SIZE ) break;
			} while( m_ptr->empty() );
			m_it = m_ptr->begin();
		}
		return *this;

	}

	ConstIterator operator++( int )
	{
		auto temp = *this;
		++( *this );
		return temp;
	}

	ConstIterator& operator--()
	{
		if( m_it == m_ptr->begin() ){
			do{
				m_ptr -= 1;
				if( m_ptr == m_hashMap->m_array && m_ptr->empty() ) throw std::out_of_range( "" );
			} while( m_ptr->empty() );
			m_it = m_ptr->end();
		}
		--m_it;
		return *this;
	}

	ConstIterator operator--( int )
	{
		auto temp = *this;
		++( *this );
		return temp;
	}

	reference operator*() const
	{
		if( !(m_ptr + 1) || m_it == m_ptr->end() ) throw std::out_of_range( "" );
		return *m_it;
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	bool operator==( const ConstIterator& other ) const
	{
		return ( m_ptr == other.m_ptr && m_it == other.m_it );
	}

	bool operator!=( const ConstIterator& other ) const
	{
		return !( *this == other );
	}
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename HashMap::reference;
	using pointer = typename HashMap::value_type*;
	using Type = typename HashMap::Type;

	explicit Iterator( typename Type::iterator it, Type* ptr, HashMap* hashMap ) : ConstIterator( it, ptr, hashMap )
	{}

	Iterator( const ConstIterator& other )
		: ConstIterator( other )
	{}

	Iterator& operator++()
	{
		ConstIterator::operator++();
		return *this;
	}

	Iterator operator++( int )
	{
		auto result = *this;
		ConstIterator::operator++();
		return result;
	}

	Iterator& operator--()
	{
		ConstIterator::operator--();
		return *this;
	}

	Iterator operator--( int )
	{
		auto result = *this;
		ConstIterator::operator--();
		return result;
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	reference operator*() const
	{
		// ugly cast, yet reduces code duplication.
		return const_cast< reference >( ConstIterator::operator*() );
	}
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
