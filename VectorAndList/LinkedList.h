#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
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
    class Node
    {
    public:
        Node *m_next;
        Node *m_previous;
        value_type m_data;
        Node( value_type data = Type{}, Node *next = nullptr, Node *previous = nullptr )
        {
            m_data = data;
            m_next = next;
            m_previous = previous;
        }
        ~Node() {}
    };
    Node *m_first;
    Node *m_last;          //straznik
    size_type m_size;

public:
    LinkedList() : m_first( nullptr ), m_last( nullptr ), m_size( 0 ){}      //done

    LinkedList(std::initializer_list<Type> l) : LinkedList()  //done
    {
        for( auto it = l.begin(); it != l.end(); ++it ){
            append( *it );
        }
    }

    LinkedList(const LinkedList& other) : LinkedList()      //done
    {
        for( auto it = other.begin(); it != other.end(); ++it ){
            append( *it );
        }
    }

    LinkedList(LinkedList&& other) : m_first( other.m_first ), m_last( other.m_last ), m_size( other.m_size )
    {
        other.m_first = nullptr;
        other.m_last = nullptr;
        other.m_size = 0;
    }

    ~LinkedList()       //done
    {
        erase( begin(), end() );
        delete m_first;
    }

    LinkedList& operator=(const LinkedList& other)      //done
    {
        if( &other == this ) return *this;
        erase( begin(), end() );
        for( auto it = other.begin(); it != other.end(); ++it ){
            append( *it );
        }
        m_size = other.m_size;
        return *this;
    }

    LinkedList& operator=(LinkedList&& other)       //done
    {
        erase( begin(), end() );
        m_first = other.m_first;
        m_last = other.m_last;
        m_size = other.m_size;
        other.m_first = nullptr;
        other.m_last = nullptr;
        other.m_size = 0;
        return *this;
    }

    bool isEmpty() const        //done
    {
        return m_size == 0;
    }

    size_type getSize() const       //done
    {
        return m_size;
    }

    void append(const Type& item)   //done
    {
        insert( end(), item );
    }

    void prepend(const Type& item)  //done
    {
        insert( begin(), item );
    }

    void insert(const const_iterator& insertPosition, const Type& item)     //done - doesnt work
    {
        if( !m_size ){
            m_last = new Node( Type{}, nullptr, nullptr );
            m_first = m_last->m_previous = new Node( item, m_last, nullptr );
        }
        else if( insertPosition == end() ){     //last
            m_last->m_previous = new Node( item, m_last, m_last->m_previous );
            m_last->m_previous->m_previous->m_next = m_last->m_previous;
        }
        else if( insertPosition == begin() ){   //first
            m_first->m_previous = new Node( item, m_first, nullptr );
            m_first = m_first->m_previous;
        }
        else{
			insertPosition.m_pointer->m_previous = insertPosition.m_pointer->m_previous->m_next = new Node(item, insertPosition.m_pointer, insertPosition.m_pointer->m_previous);
        }
        ++m_size;
    }

    Type popFirst()     //done
    {
		if (!m_size) throw std::logic_error("");
        auto data = m_first->m_data;
        erase( begin() );
        return data;
    }

    Type popLast()      //done
    {
		if ( !m_size ) throw std::logic_error("");
        auto data = m_last->m_previous->m_data;
        erase( end() - 1 );
        return data;
    }

    void erase(const const_iterator& possition)     //done
    {
        if( !m_size || possition == end() ) throw std::out_of_range( "" );

        if( m_size == 1 ){      //only one
            delete m_first;
            m_last->m_previous = nullptr;
            m_first = m_last;
        }
        else if( possition == end() - 1 ){   //last
            m_last->m_previous = m_last->m_previous->m_previous;
            m_last->m_previous->m_next = m_last;
            delete possition.m_pointer;
        }
        else if( possition == begin() ){    //first
            m_first = possition.m_pointer->m_next;
            m_first->m_previous = nullptr;
            delete possition.m_pointer;
        }
        else{
            possition.m_pointer->m_previous->m_next = possition.m_pointer->m_next;
            possition.m_pointer->m_next->m_previous = possition.m_pointer->m_previous;
            delete possition.m_pointer;
        }
        --m_size;
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        if( firstIncluded == end() ) return;
        for( auto it = firstIncluded; it != lastExcluded; ){
            auto it2 = it + 1;
            erase( it );
            it = it2;
        }
    }

    iterator begin()            //done
    {
        return Iterator( m_first );
    }

    iterator end()          //done
    {
        return Iterator( m_last );
    }

    const_iterator cbegin() const       //done
    {
        return ConstIterator( m_first );
    }

    const_iterator cend() const     //done
    {
        return ConstIterator( m_last );
    }

    const_iterator begin() const       //done
    {
        return cbegin();
    }

    const_iterator end() const      //done
    {
        return cend();
    }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
protected:
    Node *m_pointer;

    friend void LinkedList < Type >::insert( const const_iterator&, const Type& );
	friend void LinkedList < Type >::erase( const const_iterator&, const const_iterator& );
	friend void LinkedList < Type >::erase( const const_iterator& );

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

    explicit ConstIterator( Node *pointer ) : m_pointer( pointer ){}

    reference operator*() const
    {   if( !m_pointer || !m_pointer->m_next ) throw std::out_of_range( "" );
        return m_pointer->m_data;
    }

    ConstIterator& operator++()     //done
    {
        if( !m_pointer || !m_pointer->m_next ) throw std::out_of_range( "" );
        m_pointer = m_pointer->m_next;
        return *this;
    }

    ConstIterator operator++(int)       //done
    {
        auto temp = *this;
        ++( *this );
        return temp;
    }

    ConstIterator& operator--()     //done
    {
        if( !m_pointer || !m_pointer->m_previous ) throw std::out_of_range( "" );
        m_pointer = m_pointer->m_previous;
        return *this;
    }

    ConstIterator operator--(int)       //done
    {
        auto temp = *this;
        --( *this );
        return temp;
    }

    ConstIterator operator+(difference_type d) const        //done
    {
        auto temp = *this;
        for( auto i = d; i > 0; --i )
            ++temp;
        return temp;
    }

    ConstIterator operator-(difference_type d) const        //done
    {
        auto temp = *this;
        for( auto i = d; i > 0; --i )
            --temp;
        return temp;
    }

    bool operator==(const ConstIterator& other) const       //done
    {
        return m_pointer == other.m_pointer;
    }

    bool operator!=(const ConstIterator& other) const       //done
    {
        return m_pointer != other.m_pointer;
    }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

    explicit Iterator( Node *pointer ) : ConstIterator( pointer ){}

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

#endif // AISDI_LINEAR_LINKEDLIST_H
