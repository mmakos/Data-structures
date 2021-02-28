#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

protected:
    size_type m_size;
    size_type m_allocSize;
    Type *m_array;

    void realloc( size_type s )
    {
        if( !s ){
            m_allocSize = 0;
            delete [] m_array;
            m_array = nullptr;
            return;
        }
        if( !m_allocSize ){
            m_allocSize = s;
            m_array = new Type[ s ];
        }
        else{
            Type *array = new Type[ s ];
            size_type sizeMax = std::min( s, m_size );
            for( size_type i = 0; i < sizeMax; ++i ) //std::copy();
                array[ i ] = m_array[ i ];
            m_allocSize = s;
            delete [] m_array;
            m_array = array;
        }
    }

public:
    Vector() : m_size( 0 ), m_allocSize( 0 ), m_array( nullptr )
    {}

    Vector(std::initializer_list<Type> l) : Vector()
    {
        int i = 0;
        realloc( l.size() );
		for (auto it = l.begin(); it != l.end(); ++it, ++i) {
			m_array[i] = *it;
			++m_size;
		}
    }

    Vector(const Vector& other) : Vector()
    {
        *this = other;
    }

	Vector(Vector&& other) : m_size( other.m_size ), m_allocSize( other.m_allocSize ), m_array( other.m_array )
    {
		other.m_array = nullptr;
		other.m_size = 0;
		other.m_allocSize = 0;
    }

    ~Vector()
    {
        delete [] m_array;
    }

    Vector& operator=(const Vector& other)
    {
		if ( &other == this ) return *this;
        realloc( 0 );
        auto size = other.getSize();
        realloc( size );
        for( size_type i = 0; i < size; ++i )
            m_array[ i ] = other.m_array[ i ];
        m_size = size;
        return *this;
    }

    Vector& operator=(Vector&& other)
    {
		if( &other == this ) return *this;

		delete[] m_array;
        m_array = other.m_array;
        m_size = other.m_size;
        m_allocSize = other.m_allocSize;

        other.m_array = NULL;
        other.m_size = 0;
        other.m_allocSize = 0;
        return *this;
    }

	reference operator[]( unsigned int index )
	{
		if( index >= m_size ) throw std::out_of_range( "" );
		return m_array[ index ];
	}

    bool isEmpty() const
    {
        return m_size == 0;
    }

    size_type getSize() const
    {
        return m_size;
    }

    void append(const Type& item)       //push_back
    {
        if( m_size >= m_allocSize )
            realloc( m_allocSize ? m_allocSize * 2 : 1 );
        m_array[ m_size ] = item;
        ++m_size;
    }

    void prepend(const Type& item)      //push_first
    {
        if( m_size >= m_allocSize )
			realloc(m_allocSize ? m_allocSize * 2 : 1);
        for( auto i = m_size; i > 0; --i )
            m_array[ i ] = m_array[ i - 1 ];
        m_array[ 0 ] = item;
        ++m_size;
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
		auto index = insertPosition.m_pointer - begin().m_pointer;
        if( m_size >= m_allocSize )
			realloc(m_allocSize ? m_allocSize * 2 : 1);
		auto insertPos = begin() + index;
		for ( auto it = end(); it != insertPos; --it )
			*it.m_pointer = *( it.m_pointer - 1 );
		*insertPos.m_pointer = item;
        ++m_size;
    }

    Type popFirst()
    {
        if( isEmpty() )
            throw std::logic_error( "Empty" );
        Type first = m_array[ 0 ];
        for( size_type i = 0; i < m_size - 1; ++i )
            m_array[ i ] = m_array[ i + 1 ];
        --m_size;
        return first;
    }

    Type popLast()
    {
        if( isEmpty() )
            throw std::logic_error( "Empty" );
        return m_array[ --m_size ];
    }

    void erase(const const_iterator& possition)
    {
		if( !m_size ) throw std::out_of_range( "" );
        for( auto it = possition; it != end() - 1; ++it )
            *it.m_pointer = *( it.m_pointer + 1 );
        --m_size;
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        for( auto itBeg = firstIncluded, itEnd = lastExcluded; itEnd != end(); ++itBeg, ++itEnd ){
            *itBeg.m_pointer = *( itEnd.m_pointer );
        }
        m_size = m_size - ( lastExcluded.m_pointer - firstIncluded.m_pointer );
    }

    iterator begin()
    {
        return Iterator( m_array, this );
    }

    iterator end()
    {
        return Iterator( m_array + m_size, this );
    }

    const_iterator cbegin() const
    {
        return ConstIterator( m_array, this );
    }

    const_iterator cend() const
    {
        return ConstIterator( m_array + m_size, this );
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
protected:
    Type *m_pointer{ nullptr };
	const Vector *m_vector;

    friend void Vector < Type >::erase( const const_iterator& );
    friend void Vector < Type >::erase( const const_iterator&, const const_iterator& );
    friend void Vector < Type >::insert( const const_iterator&, const Type& );

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;

    explicit ConstIterator( Type *pointer = nullptr, const Vector *vector = nullptr ) : m_pointer( pointer ), m_vector( vector )
    {}

    reference operator*() const
    {
		if( !m_vector->m_array || m_pointer == m_vector->m_array + m_vector->m_size ) throw std::out_of_range( "" );
        return *m_pointer;
    }

    ConstIterator& operator++()
    {
		if ( !m_vector->m_array || m_pointer == m_vector->m_array + m_vector->m_size ) throw std::out_of_range( "" );
        ++m_pointer;
        return *this;
    }

    ConstIterator operator++(int)
    {
        auto temp = *this;
        ++( *this );
        return temp;
    }

    ConstIterator& operator--()
    {
		if( !m_vector->m_array || m_pointer == m_vector->m_array ) throw std::out_of_range( "" );
        --m_pointer;
        return *this;
    }

    ConstIterator operator--(int)
    {
        auto temp = *this;
        --( *this );
        return temp;
    }

    ConstIterator operator+(difference_type d) const
    {
		if ( !m_vector->m_array || m_pointer + d > m_vector->m_array + m_vector->m_size ) throw std::out_of_range( "" );
		ConstIterator it(m_pointer + d, m_vector);
			return it;
    }

    ConstIterator operator-(difference_type d) const
    {
		ConstIterator it(m_pointer - d, m_vector);
		return it;
    }

    bool operator==(const ConstIterator& other) const
    {
        return m_pointer == other.m_pointer;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return m_pointer != other.m_pointer;
    }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;

    explicit Iterator( Type *pointer, Vector *vector ) : ConstIterator( pointer, vector )
    {}

    Iterator(const ConstIterator& other)
        : ConstIterator(other)
    {}

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
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

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    Iterator operator+(difference_type d) const
    {
        return ConstIterator::operator+(d);
    }

    Iterator operator-(difference_type d) const
    {
        return ConstIterator::operator-(d);
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif // AISDI_LINEAR_VECTOR_H
