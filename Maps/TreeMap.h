#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#define BF_END 23

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>


namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
	using key_type = KeyType;
	using mapped_type = ValueType;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = std::size_t;
	using reference = value_type & ;
	using const_reference = const value_type&;

	class ConstIterator;
	class Iterator;
	using iterator = Iterator;
	using const_iterator = ConstIterator;

protected:
	class Node
	{
	public:
		Node* m_parent;
		Node* m_right;
		Node* m_left;
		value_type m_data;
		int m_bf;			//balance factor

		Node( Node* parent = nullptr, value_type data = value_type{} )
			: m_parent( parent ), m_right( nullptr ), m_left( nullptr ), m_data( data ), m_bf( 0 ){}
		~Node(){}
	};
	Node* m_root;
	size_type m_size;

	Node* m_begin;
	Node* m_end;
	
public:
	TreeMap() : m_root( nullptr ), m_size( 0 ), m_begin( nullptr ), m_end( nullptr ){}

	TreeMap( std::initializer_list<value_type> list ) : TreeMap()	//done
	{
		for( auto it = list.begin(); it != list.end(); ++it ){
			append( *it );
		}
	}

	TreeMap( const TreeMap& other ) : TreeMap()		//done
	{
		for( auto it = other.begin(); it != other.end(); ++it ){
			append( *it );
		}
	}

	TreeMap( TreeMap&& other ) : m_root( other.m_root ), m_size( other.m_size ), m_begin( other.m_begin ), m_end( other.m_end )	//done
	{
		other.m_root = nullptr;
		other.m_size = 0;
		other.m_begin = nullptr;
		other.m_end = nullptr;
	}

	TreeMap& operator=( const TreeMap& other )	//done
	{
		if( &other == this ) return *this;
		deleteTree( m_root );
		for( auto it = other.begin(); it != other.end(); ++it ){
			append( *it );
		}
		m_size = other.m_size;
		return *this;
	}

	TreeMap& operator=( TreeMap&& other )
	{
		if( &other == this ) return *this;
		deleteTree( m_root );
		m_root = other.m_root;
		m_size = other.m_size;
		m_begin = other.m_begin;
		m_end = other.m_end;
		other.m_root = nullptr;
		other.m_size = 0;
		other.m_begin = nullptr;
		other.m_end = nullptr;
		return *this;
	}

	bool isEmpty() const
	{
		return m_size == 0;
	}

	mapped_type& operator[]( const key_type& key )	//done
	{
		auto it = find( key );
		if( it != end() )	//item exist
			return it.m_ptr->m_data.second;
		Node* newNode = append( std::make_pair( key, mapped_type{} ) );
		return newNode->m_data.second;
	}

	const mapped_type& valueOf( const key_type& key ) const	//done
	{
		auto it = find( key );
		if( it == end() ) throw std::out_of_range( "" );
		return it.m_ptr->m_data.second;
	}

	mapped_type& valueOf( const key_type& key )	//done
	{
		auto it = find( key );
		if( it == end() ) throw std::out_of_range( "" );
		return it.m_ptr->m_data.second;
	}

	const_iterator find( const key_type& key ) const	//done
	{
		if( !m_size || key < m_begin->m_data.first || key > m_end->m_parent->m_data.first ) return cend();
		auto temp = m_root;
		while( temp ){
			if( temp->m_data.first == key ) return const_iterator( temp );
			temp = ( key < temp->m_data.first ) ? temp->m_left : temp->m_right;
		}
		return cend();
	}

	iterator find( const key_type& key )	//done
	{
		if( !m_size || key < m_begin->m_data.first || key > m_end->m_parent->m_data.first ) return end();
		auto temp = m_root;
		while( temp ){
			if( temp->m_data.first == key ) return iterator( temp );
			temp = ( key < temp->m_data.first ) ? temp->m_left : temp->m_right;
		}
		return end();
	}

	Node* append( value_type value )
	{
		if( !m_root ){
			m_begin = m_root = new Node( nullptr, value );
			m_end = new Node( m_root, value_type{} );
			m_end->m_bf = BF_END;
			m_root->m_right = m_end;
			++m_size;
			return m_begin;
		}

		auto temp = m_root;
		auto parent = m_root;
		while( temp && temp->m_bf != BF_END ){
			parent = temp;
			if( value.first < temp->m_data.first )
				temp = temp->m_left;
			else
				temp = temp->m_right;
		}													//prev is now parent

		auto newNode = new Node( parent, value );
		if( value.first < parent->m_data.first )					//inserting
			parent->m_left = newNode;
		else
			parent->m_right = newNode;

		/*------BALANCING------*/
		if( parent->m_bf )										//surely tree remains balanced
			parent->m_bf = 0;
		else{
			if( parent->m_left == newNode )
				parent->m_bf = 1;								// 1 if left is higher than right!!!
			else
				parent->m_bf = -1;								//-1 if right is higher than left!!!

			temp = parent->m_parent;
			while( temp ){
				if( temp->m_bf ){								// then balancing and break;
					/*--balancing--*/
					if( temp->m_bf == 1 ){
						if( temp->m_right == parent )
							temp->m_bf = 0;					// branches balanced
						else if( parent->m_bf == -1 )
							rotateLR( temp );
						else
							rotateLL( temp );
					}
					else{
						if( temp->m_left == parent )
							temp->m_bf = 0;					//barnches balanced
						else if( parent->m_bf == 1 )
							rotateRL( temp );
						else
							rotateRR( temp );
					}
					/*------end------*/
					break;
				}
				if( temp->m_left == parent )
					temp->m_bf = 1;
				else
					temp->m_bf = -1;
				parent = temp;
				temp = temp->m_parent;
			}
		}
		if( value.first >= m_end->m_parent->m_data.first ){
			m_end->m_parent = newNode;
			newNode->m_right = m_end;
		}
		if( value.first < m_begin->m_data.first )
			m_begin = newNode;
		++m_size;

		return newNode;
	}

	void remove( const key_type& key )
	{
		auto temp = find( key );
		if( temp == end() ) throw std::out_of_range( "" );
		remove( temp );
	}

	void remove( const const_iterator& it )
	{
		removeNode( it );
		--m_size;
	}

	size_type getSize() const		//done
	{
		return m_size;
	}

	bool operator==( const TreeMap& other ) const	//done
	{
		if( m_size != other.m_size ) return false;
		for( auto it = begin(); it != end(); ++it )
			if( it.m_ptr->m_data.second != other.valueOf( it.m_ptr->m_data.first ) ) return false;
		return true;
	}

	bool operator!=( const TreeMap& other ) const
	{
		return !( *this == other );
	}

	iterator begin()	//done
	{
		return Iterator( m_begin );
	}

	iterator end()	//done
	{
		return Iterator( m_end );
	}

	const_iterator cbegin() const	//done
	{
		return ConstIterator( m_begin );
	}

	const_iterator cend() const	//done
	{
		return ConstIterator( m_end );
	}

	const_iterator begin() const	//done
	{
		return cbegin();
	}

	const_iterator end() const	//done
	{
		return cend();
	}

protected:
	Node* removeNode( const const_iterator& it )
	{
		if( !it.m_ptr || it == end() ) throw std::out_of_range( "" );
		bool nest;
		Node* other = nullptr;
		auto current = it.m_ptr;
		if( current->m_left && current->m_right ){			//both sons
			auto temp = it;
			if( temp != begin() )
				other = removeNode( --temp );
			nest = false;
		}
		else{
			if( current->m_left ){	//has only left son
				other = current->m_left;
				current->m_left = nullptr;
			}
			else{					//has only right son or no sons
				other = current->m_right;
				current->m_right = nullptr;
			}
			current->m_bf = 0;
			nest = true;
		}

		if( other ){
			other->m_parent = current->m_parent;
			other->m_left = current->m_left;
			other->m_right = current->m_right;
			if( other->m_left ) other->m_left->m_parent = other;
			if( other->m_right ) other->m_right->m_parent = other;
			other->m_bf = current->m_bf;
		}

		if( current->m_parent ){
			if( current->m_parent->m_left == current )
				current->m_parent->m_left = other;
			else
				current->m_parent->m_right = other;
		}
		else
			m_root = other;

		if( nest ){
			auto temp = other;
			other = current->m_parent;
			while( other ){
				if( !other->m_bf ){
					if( other->m_left == temp )
						other->m_bf = -1;
					else
						other->m_bf = 1;
					break;
				}
				else{
					if( ( other->m_bf == 1 && other->m_left == temp ) || ( other->m_bf == -1 && other->m_right == temp ) ){
						other->m_bf = 0;
						temp = other;
						other = other->m_parent;
					}
					else{
						Node* temp2;
						if( other->m_left == temp )
							temp2 = other->m_right;
						else
							temp2 = other->m_left;
						if( !temp2->m_bf ){
							if( other->m_bf == 1 ) rotateLL( other );
							else rotateRR( other );
							break;
						}
						else if( other->m_bf == temp2->m_bf ){
							if( other->m_bf == 1 ) rotateLL( other );
							else rotateRR( other );
							temp = temp2;
							other = temp2->m_parent;
						}
						else{
							if( other->m_bf == 1 ) rotateLR( other );
							else rotateRL( other );
							temp = other->m_parent;
							other = temp->m_parent;
						}
					}
				}
			}
		}
		m_end->m_bf = BF_END;
		return current;
	}

	void deleteTree( Node* node ){
		if( node->m_right ) deleteTree( node->m_right );
		if( node->m_left ) deleteTree( node->m_left );
		delete node;
		m_size = 0;
		m_begin = nullptr;
		m_end = nullptr;
		m_root = nullptr;
	}

	void rotateRR( Node* branchA )	// B to A, A to B->left, B->left to A->right
	{
		auto branchB = branchA->m_right;
		auto parentA = branchA->m_parent;

		//switch pointers
		branchA->m_right = branchB->m_left;
		if( branchA->m_right )
			branchA->m_right->m_parent = branchA;
		branchB->m_left = branchA;
		branchB->m_parent = parentA;
		branchA->m_parent = branchB;
		if( parentA ){
			if( parentA->m_left == branchA )
				parentA->m_left = branchB;
			else
				parentA->m_right = branchB;
		}
		else
			m_root = branchB;
		//set bf's
		if( branchB->m_bf == -1 )
			branchA->m_bf = branchB->m_bf = 0;
		else{
			branchA->m_bf = -1;
			branchB->m_bf = 1;
		}
	}

	void rotateLL( Node* branchA )	// B to A, A to B->right, B->right to A->left (symetry to RR)
	{
		auto branchB = branchA->m_left;
		auto parentA = branchA->m_parent;

		//switch pointers
		branchA->m_left = branchB->m_right;
		if( branchA->m_left )
			branchA->m_left->m_parent = branchA;
		branchB->m_right = branchA;
		branchB->m_parent = parentA;
		branchA->m_parent = branchB;
		if( parentA ){
			if( parentA->m_left == branchA )
				parentA->m_left = branchB;
			else
				parentA->m_right = branchB;
		}
		else
			m_root = branchB;
		//set bf's
		if( branchB->m_bf == 1 )
			branchA->m_bf = branchB->m_bf = 0;
		else{
			branchA->m_bf = 1;
			branchB->m_bf = -1;
		}
	}

	void rotateRL( Node* branchA )	// firstly rotate LL and then RR
	{
		auto branchB = branchA->m_right;
		auto branchC = branchB->m_left;
		auto parentA = branchA->m_parent;

		branchB->m_left = branchC->m_right;
		if( branchB->m_left )
			branchB->m_left->m_parent = branchB;
		branchA->m_right = branchC->m_left;
		if( branchA->m_right )
			branchA->m_right->m_parent = branchA;
		branchC->m_left = branchA;
		branchC->m_right = branchB;
		branchC->m_parent = parentA;
		branchA->m_parent = branchB->m_parent = branchC;
		if( parentA ){
			if( parentA->m_left == branchA )
				parentA->m_left = branchC;
			else
				parentA->m_right = branchC;
		}
		else
			m_root = branchC;

		if( branchC->m_bf == -1 )
			branchA->m_bf = 1;
		else
			branchA->m_bf = 0;
		if( branchC->m_bf == 1 )
			branchB->m_bf = -1;
		else
			branchB->m_bf = 0;
		branchC->m_bf = 0;
	}

	void rotateLR( Node* branchA )	// firstly rotate RR and then LL
	{
		auto branchB = branchA->m_left;
		auto branchC = branchB->m_right;
		auto parentA = branchA->m_parent;

		branchB->m_right = branchC->m_left;
		if( branchB->m_right )
			branchB->m_right->m_parent = branchB;
		branchA->m_left = branchC->m_right;
		if( branchA->m_left )
			branchA->m_left->m_parent = branchA;
		branchC->m_left = branchB;
		branchC->m_right = branchA;
		branchC->m_parent = parentA;
		branchA->m_parent = branchB->m_parent = branchC;
		if( parentA ){
			if( parentA->m_left == branchA )
				parentA->m_left = branchC;
			else
				parentA->m_right = branchC;
		}
		else
			m_root = branchC;

		if( branchC->m_bf == 1 )
			branchA->m_bf = -1;
		else
			branchA->m_bf = 0;
		if( branchC->m_bf == -1 )
			branchB->m_bf = 1;
		else
			branchB->m_bf = 0;
		branchC->m_bf = 0;
	}
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
protected:
	Node* m_ptr;

	friend ValueType& TreeMap< KeyType, ValueType >::valueOf( const key_type& key );
	friend const ValueType& TreeMap< KeyType, ValueType >::valueOf( const key_type& key ) const;
	friend ValueType& TreeMap< KeyType, ValueType >::operator[]( const key_type& key );
	friend bool TreeMap< KeyType, ValueType >::operator==( const TreeMap& other ) const;
	friend TreeMap< KeyType, ValueType >::Node* TreeMap< KeyType, ValueType >::removeNode( const const_iterator& it );
	friend void TreeMap< KeyType, ValueType >::deleteTree( Node* node );

public:
	using reference = typename TreeMap::const_reference;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename TreeMap::value_type;
	using pointer = const typename TreeMap::value_type*;

	explicit ConstIterator( Node* ptr ) : m_ptr( ptr ){}

	ConstIterator( const ConstIterator& other ) : m_ptr( other.m_ptr ){}

	ConstIterator& operator++()
	{
		if( !m_ptr || m_ptr->m_bf == BF_END ) throw std::out_of_range( "" );
		if( m_ptr->m_right ){
			m_ptr = m_ptr->m_right;
			while( m_ptr->m_left )
				m_ptr = m_ptr->m_left;
		}
		else{
			auto parent = m_ptr->m_parent;
			while( parent && parent->m_right == m_ptr ){
				m_ptr = parent;
				parent = m_ptr->m_parent;
			}
			if( !parent || parent->m_bf == BF_END ) throw std::out_of_range( "" );
			m_ptr = parent;
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
		if( !m_ptr ) throw std::out_of_range( "" );
		if( m_ptr->m_left ){
			m_ptr = m_ptr->m_left;
			while( m_ptr->m_right )
				m_ptr = m_ptr->m_right;
		}
		else{
			auto parent = m_ptr->m_parent;
			while( parent && parent->m_left == m_ptr ){
				m_ptr = parent;
				parent = m_ptr->m_parent;
			}
			if( !parent ) throw std::out_of_range( "" );
			m_ptr = parent;
		}
		return *this;
	}

	ConstIterator operator--( int )
	{
		auto temp = *this;
		--( *this );
		return temp;
	}

	reference operator*() const
	{
		if( !m_ptr ) throw std::out_of_range( "" );
		return m_ptr->m_data;
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	bool operator==( const ConstIterator& other ) const
	{
		return m_ptr == other.m_ptr;
	}

	bool operator!=( const ConstIterator& other ) const
	{
		return !( *this == other );
	}
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename TreeMap::reference;
	using pointer = typename TreeMap::value_type*;

	explicit Iterator( Node* ptr) : ConstIterator( ptr ){}

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

#endif /* AISDI_MAPS_MAP_H */
