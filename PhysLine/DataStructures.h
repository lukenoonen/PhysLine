#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#ifndef NULL
#define NULL nullptr
#endif // NULL

//===========================================================================================
// CNode
//===========================================================================================

template <class T>
class CNode
{
public:
	CNode( T &tValue );
	CNode( CNode<T> *pNext, T &tValue );
	
	CNode<T> *m_pNext;
	T m_tValue;
};

template <class T>
CNode<T>::CNode( T &tValue ) : m_tValue( tValue )
{
	m_pNext = NULL;
}

template <class T>
CNode<T>::CNode(CNode *pNext, T &tValue) : m_tValue( tValue )
{
	m_pNext = pNext;
}

template <class T>
class CIterator
{
public:
	CIterator( CNode<T> *pFirst );
	
	bool Iterating( void );
	T &Iterate( void );
	int Iteration( void );

private:
	CNode<T> *m_pCurrent;
	int m_iIteration;
};

template <class T>
CIterator<T>::CIterator( CNode<T> *pFirst )
{
	m_pCurrent = pFirst;
	m_iIteration = 0;
}

template <class T>
bool CIterator<T>::Iterating( void )
{
	return m_pCurrent != NULL;
}

template <class T>
T &CIterator<T>::Iterate( void )
{
	T &tOutput = m_pCurrent->m_tValue;
	m_pCurrent = m_pCurrent->m_pNext;
	m_iIteration++;
	return tOutput;
}

template <class T>
int CIterator<T>::Iteration( void )
{
	return m_iIteration;
}

//===========================================================================================
// CLinkedList
//===========================================================================================

template <class T>
class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();
	
	bool			AddToHead( T &tValue );
	bool			AddToTail( T &tValue );
	bool			Add( T &tValue, int iIndex );
	
	bool			RemoveFromHead( void );
	bool			RemoveFromTail( void );
	bool			Remove( int iIndex );
	void			RemoveAll( void );
	void			RemoveAllNoDelete( void );
	
	T 				&GetHead( void );
	const T 		&GetHead( void ) const;
	T 				&GetTail( void );
	const T 		&GetTail( void ) const;
	T				&Get( int iIndex ); // DANGEROUS
	const T			&Get( int iIndex ) const; // DANGEROUS
	bool			Get( int iIndex, T &tValue );
	bool			Get( int iIndex, const T &tValue ) const;

	int				Find( const T &tValue );
	
	bool			Swap( int iIndex1, int iIndex2 );
	bool			Move( int iIndexFrom, int iIndexTo );
	
	bool			IsEmpty( void ) const;
	int				Count( void ) const;

	CIterator<T>	StartIterating( void ) const;
	CIterator<T>	StartIterating( void );

	void			CopyOther( CLinkedList<T> &other );
	void			SetShouldDeleteOnDestruction( bool bDeleteOnDestruction );
	
private:
	bool			IsValidIndex( int iIndex ) const;
	CNode<T>		*GetNodeFromIndex( int iIndex );
	const CNode<T>	*GetNodeFromIndex( int iIndex ) const;
	
	// Data stuff
	CNode<T> *m_pFirst;
	CNode<T> *m_pLast;
	int m_iCount;

	bool m_bDeleteOnDestruction;
};

template <class T>
CLinkedList<T>::CLinkedList()
{
	m_pFirst = m_pLast = NULL;
	m_iCount = 0;
	m_bDeleteOnDestruction = true;
}

template <class T>
CLinkedList<T>::~CLinkedList()
{
	if (m_bDeleteOnDestruction)
		RemoveAll();
}

template <class T>
bool CLinkedList<T>::AddToHead( T &tValue )
{
	return Add( tValue, 0 );
}

template <class T>
bool CLinkedList<T>::AddToTail( T &tValue )
{
	return Add( tValue, m_iCount );
}

template <class T>
bool CLinkedList<T>::Add( T &tValue, int iIndex )
{
	if (iIndex < 0 || iIndex > m_iCount)
		return false;
	
	// Do the first one quickly
	if (iIndex == 0)
	{
		m_pFirst = new CNode<T>( m_pFirst, tValue );
		if (!m_pLast)
			m_pLast = m_pFirst;
	}
	else if (iIndex == m_iCount)
	{
		m_pLast->m_pNext = new CNode<T>( tValue );
		m_pLast = m_pLast->m_pNext;
	}
	else
	{
		CNode<T> *pSearch = GetNodeFromIndex( iIndex - 1 );
		pSearch->m_pNext = new CNode<T>( pSearch->m_pNext, tValue );
	}
	m_iCount++;
	return true;
}

template <class T>
bool CLinkedList<T>::RemoveFromHead( void )
{
	return Remove( 0 );
}

template <class T>
bool CLinkedList<T>::RemoveFromTail( void )
{
	return Remove( m_iCount - 1 );
}

template <class T>
bool CLinkedList<T>::Remove( int iIndex )
{
	if (!IsValidIndex( iIndex ))
		return false;
	
	// Do the first one quickly, again
	if (iIndex == 0)
	{
		CNode<T> *pOldFirst = m_pFirst;
		m_pFirst = m_pFirst->m_pNext;
		delete pOldFirst;
		if (!m_pFirst)
			m_pLast = m_pFirst;

		m_iCount--;
		return true;
	}
	
	// Since it doesn't store the second to last node, we removing from the end has to traverse everything just like everyone else... sigh
	CNode<T> *pSearch = GetNodeFromIndex( iIndex - 1 );
	CNode<T> *pToDelete = pSearch->m_pNext;
	pSearch->m_pNext = pToDelete->m_pNext;
	delete pToDelete;

	if (iIndex == m_iCount - 1)
		m_pLast = pSearch;

	m_iCount--;
	return true;
}

template <class T>
void CLinkedList<T>::RemoveAll( void )
{
	CNode<T> *pSearch = m_pFirst;
	while (pSearch)
	{
		CNode<T> *pToDelete = pSearch;
		pSearch = pSearch->m_pNext;
		delete pToDelete;
	}
	RemoveAllNoDelete();
}

template <class T>
void CLinkedList<T>::RemoveAllNoDelete( void )
{
	m_pFirst = m_pLast = NULL;
	m_iCount = 0;
}

template <class T>
T &CLinkedList<T>::GetHead( void )
{
	return m_pFirst->m_tValue;
}

template <class T>
const T &CLinkedList<T>::GetHead( void ) const
{
	return m_pFirst->m_tValue;
}

template <class T>
T &CLinkedList<T>::GetTail( void )
{
	return m_pLast->m_tValue;
}

template <class T>
const T &CLinkedList<T>::GetTail( void ) const
{
	return m_pLast->m_tValue;
}

template <class T>
T &CLinkedList<T>::Get( int iIndex )
{
	return GetNodeFromIndex( iIndex )->m_tValue;
}

template <class T>
const T &CLinkedList<T>::Get( int iIndex ) const
{
	return GetNodeFromIndex( iIndex )->m_tValue;
}

template <class T>
bool CLinkedList<T>::Get( int iIndex, T &tValue )
{
	if (!IsValidIndex( iIndex ))
		return false;
	
	tValue = GetNodeFromIndex( iIndex )->m_tValue;
	return true;
}

template <class T>
bool CLinkedList<T>::Get( int iIndex, const T &tValue ) const
{
	if (!IsValidIndex( iIndex ))
		return false;
		
	tValue = GetNodeFromIndex( iIndex )->m_tValue;
	return true;
}

template <class T>
int	CLinkedList<T>::Find( const T &tValue )
{
	int i = 0;
	CIterator<T> iterator = StartIterating();
	while (iterator.Iterating())
	{
		if (tValue == iterator.Iterate())
			return i;

		i++;
	}
	return -1;
}

template <class T>
bool CLinkedList<T>::Swap( int iIndex1, int iIndex2 )
{
	if (iIndex1 == iIndex2)
		return false;
	
	if (!IsValidIndex( iIndex1 ) || !IsValidIndex( iIndex2 ))
		return false;
	
	CNode<T> *pNode1 = GetNodeFromIndex( iIndex1 );
	CNode<T> *pNode2 = GetNodeFromIndex( iIndex2 );
	T tTemp = pNode1->m_tValue;
	pNode1->m_tValue = pNode2->m_tValue;
	pNode2->m_tValue = tTemp;
	return true;
}

template <class T>
bool CLinkedList<T>::Move( int iIndexFrom, int iIndexTo )
{
	if (iIndexFrom == iIndexTo)
		return false;
	
	if (!IsValidIndex( iIndexFrom ) || !IsValidIndex( iIndexTo ))
		return false;

	if (iIndexFrom == 0)
	{
		CNode<T> *pNodeTo = GetNodeFromIndex( iIndexTo );

		pNodeTo->m_pNext = m_pFirst;
		m_pFirst = m_pFirst->m_pNext;
	}
	else
	{
		CNode<T> *pNodeTo = GetNodeFromIndex( iIndexTo );
		CNode<T> *pNodeBeforeFrom = GetNodeFromIndex( iIndexFrom - 1 );
		CNode<T> *pNodeFrom = pNodeBeforeFrom->m_pNext;

		pNodeBeforeFrom->m_pNext = pNodeBeforeFrom->m_pNext->m_pNext;
		pNodeFrom->m_pNext = pNodeTo->m_pNext;
		pNodeTo->m_pNext = pNodeFrom;

		if (iIndexFrom == m_iCount - 1)
			m_pLast = pNodeBeforeFrom;
	}

	return true;
}

template <class T>
bool CLinkedList<T>::IsEmpty( void ) const
{
	return m_iCount == 0;
}

template <class T>
int CLinkedList<T>::Count( void ) const
{
	return m_iCount;
}

template <class T>
CIterator<T> CLinkedList<T>::StartIterating( void )
{
	CIterator<T> iterator( m_pFirst );
	return iterator;
}

template <class T>
CIterator<T> CLinkedList<T>::StartIterating( void ) const
{
	CIterator<T> iterator( m_pFirst );
	return iterator;
}

template <class T>
bool CLinkedList<T>::IsValidIndex( int iIndex ) const
{
	return iIndex >= 0 && iIndex < m_iCount;
}

template <class T>
void CLinkedList<T>::CopyOther( CLinkedList<T> &other )
{
	RemoveAll();

	m_pFirst = other.m_pFirst;
	m_pLast = other.m_pLast;
	m_iCount = other.m_iCount;
}

template <class T>
void CLinkedList<T>::SetShouldDeleteOnDestruction( bool bDeleteOnDestruction )
{
	m_bDeleteOnDestruction = bDeleteOnDestruction;
}

template <class T>
CNode<T> *CLinkedList<T>::GetNodeFromIndex( int iIndex )
{
	CNode<T> *pSearch = m_pFirst;
	for (int i = 0; i < iIndex; i++)
		pSearch = pSearch->m_pNext;
	
	return pSearch;
}

template <class T>
const CNode<T> *CLinkedList<T>::GetNodeFromIndex( int iIndex ) const
{
	CNode<T> *pSearch = m_pFirst;
	for (int i = 0; i < iIndex; i++)
		pSearch = pSearch->m_pNext;
	
	return pSearch;
}

#endif // DATASTRUCTURES_H