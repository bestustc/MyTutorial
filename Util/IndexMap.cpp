#ifndef __UTIL__INDEX_MAP__H__
#define __UTIL__INDEX_MAP__H__

#pragma once

#include "UtilHelper.h"

template<typename TKey,typename TValue>
class CIndexMapT
{
	struct Value
	{
		int position;
		TValue value;

		Value(int position, const TValue & value)
		{
			this->position = position;
			this->value = value;
		}

		Value()
		{
		}
	};

	vector<TKey> m_keys;

	std::map<TKey, Value> m_map;

public:

	typedef typename map<TKey, Value>::const_iterator const_iterator;
	typedef typename map<TKey, Value>::iterator iterator;

public:

	const_iterator begin() const
	{
		return m_map.begin();
	}

	iterator begin()
	{
		return m_map.begin();
	}

	const_iterator end() const
	{
		return m_map.end();
	}

	iterator end()
	{
		return m_map.end();
	}

	size_t size() const
	{
		return m_map.size();
	}

public:

	const_iterator Begin() const
	{
		return m_map.begin();
	}

	iterator Begin()
	{
		return m_map.begin();
	}

	const_iterator End() const
	{
		return m_map.end();
	}

	iterator End()
	{
		return m_map.end();
	}

	const_iterator find(const TKey & key) const
	{
		return m_map.find(key);
	}

	iterator find(const TKey & key) 
	{
		return m_map.find(key);
	}

public:

	int Count() const
	{	
		return (int)m_keys.size();
	}

	bool Contains(const TKey & key) const
	{
		return m_map.find(key) != m_map.end();
	}

	void Set(const TKey & key, const TValue & value)
	{
		if(Contains(key))
		{
			m_map[key].value = value;
		}
		else
		{
			m_keys.push_back(key);

			m_map[key] = Value( (int)m_keys.size() -1, value);
		}
	}

	TValue Get(const TKey & key) const
	{		
		if(Contains(key))
		{
			return m_map[key].value;
		}
		else
		{
			return TValue();
		}
		
	}

	TValue & Get(const TKey & key)
	{		
		if(Contains(key))
		{
			return m_map[key].value ;
		}
		else
		{
			m_keys.push_back(key);

			m_map[key] = Value( (int)m_keys.size() -1 , TValue());
			return m_map[key].value;
		}		
	}

	TValue operator [](const TKey & key) const
	{
		return Get(key);
	}

	TValue & operator [](const TKey & key)
	{
		return Get(key);
	}

	const vector<TKey> & GetKeys() const
	{
		return m_keys;
	}

	const TValue & GetAt(int index) const
	{
		const TKey & key =  m_keys[index];
		return const_cast<CIndexMapT<TKey, TValue> *>(this)->m_map[key].value;
	}

	TValue & GetAt(int index) 
	{
		const TKey & key =  m_keys[index];
		return const_cast<CIndexMapT<TKey, TValue> *>(this)->m_map[key].value;
	}


	const TKey & GetKeyAt(int index) const
	{
		return m_keys[index];
	}

	void Remove(const TKey & key)
	{
		map<TKey, Value>::iterator it = m_map.find(key);

		if(it!= m_map.end())
		{
			int position = it->second.position;
			m_keys.erase(m_keys.begin() + position);
			m_map.erase(it);

			for(int i=position;i< (int)m_keys.size();i++)
			{
				m_map[ m_keys[i] ].position = i;
			}
		}
	}

	void RemoveAt(int index)
	{
		TKey key = m_keys[index];
		m_keys.erase ( m_keys.begin() + index );

		m_map.erase( m_map.find(key) );
	}

	void MoveTo(int srcIndex,int destIndex)
	{
		TKey key = m_keys[srcIndex];
		m_keys.erase( m_keys.begin() + srcIndex );
		m_keys.insert(m_keys.begin() + destIndex,key);
	}

	void Clear()
	{
		m_map.clear();
		m_keys.clear();
	}

	int IndexOf(const TKey & key)
	{
		const_iterator it = m_map.find(key);
		if(it == m_map.end()) return -1;

		return it->second.position;
	}

public:

	CIndexMapT(void)
	{
	}

	~CIndexMapT(void)
	{
		Clear();
	}
};

typedef CIndexMapT<String , String> String2StringMap;

typedef CIndexMapT<String , LONG> String2LongMap;

#endif