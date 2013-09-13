#include "BigInteger.h"

int PositiveIntegerCompare(const TCHAR * s0, const TCHAR * s1)
{
	size_t size0 = STRLEN(s0);
	size_t size1 = STRLEN(s1);

	if(size0 != size1)
	{
		return size0 > size1 ? 1 : -1;			
	}
	return _tcscmp(s0, s1) ;
}

static int __StrCmp(const TCHAR * s0, const TCHAR * s1, size_t size)
{
	for(size_t i=0;i<size;++i)
	{
		if(s0[i]<s1[i]) return -1;
		if(s0[i]>s1[i]) return 1;
	}

	return 0;
}

static void __Format(TCHAR value[])
{
	if(value[0] == TZERO) return;

	TCHAR * tempValue = value[0] == TNEGATIVE ? value +1 : value;
	TCHAR * tempValue2 = tempValue;

	while(*tempValue != TZERO && *tempValue == TNUMBER0) ++tempValue;			

	while(*tempValue != TZERO) 
	{ 
		*tempValue2++ = *tempValue++;  				
	}

	*tempValue2 = TZERO;

	if(value[0] == TZERO)
	{
		value[0] = TNUMBER0;
		value[1] = TZERO;
	}
}

static void __Reverse(TCHAR value[], size_t size = 0)
{
	if(size == 0) size = STRLEN(value);

	for(size_t i=0;i<size/2;++i)
	{
		TCHAR c = value[i];
		value[i] = value[size-i-1];
		value[size-i-1]=c;
	}
}


CBigInteger::~CBigInteger(void)
{
	if(m_szValue != NULL)
	{
		delete [] m_szValue;
		m_szValue = NULL;
	}
}

CBigInteger::CBigInteger(const CBigInteger & i)
{
	m_szValue = NULL;

	*this = i.m_szValue;	
}

CBigInteger::CBigInteger(const TCHAR * other)
{
	if(other == NULL )
	{
		m_szValue = new TCHAR[1];
		memset(m_szValue, 0 ,sizeof(TCHAR) * (1) );
		m_szValue[0] = TNUMBER0;
	}
	else
	{
		m_szValue = NULL;
		SetString(other);		
	}
}


CBigInteger::CBigInteger(INT64 i)
{
	TCHAR v[64]={ TZERO };
	_i64tot_s(i, v, 64, 10);

	m_szValue = NULL;
	SetString(v);	
}

CBigInteger::CBigInteger(INT32 i)
{
	TCHAR v[64]={ TZERO };
	_itot_s(i,v,64, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger::CBigInteger(INT16 i)
{
	TCHAR v[64]={ TZERO };
	_itot_s(i,v,64, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger::CBigInteger(INT8 i)
{
	TCHAR v[64]={ TZERO };
	_itot_s(i,v,64, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger::CBigInteger(UINT64 i)
{
	TCHAR v[64]={ TZERO };
	_ui64tot_s(i,v,60, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger::CBigInteger(UINT32 i)
{
	TCHAR v[64]={ TZERO };
	_ui64tot_s(i,v,60, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger::CBigInteger(UINT16 i)
{
	TCHAR v[64]={ TZERO };
	_ui64tot_s(i,v,60, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger::CBigInteger(UINT8 i)
{
	TCHAR v[64]={ TZERO };
	_ui64tot_s(i,v,60, 10);

	m_szValue = NULL;
	SetString(v);	
}


CBigInteger & CBigInteger::operator = (const CBigInteger & other) 
{		
	if(&other == this) return *this;

	SetString(other.m_szValue);
	return *this;
}

CBigInteger & CBigInteger::operator = (const TCHAR * value)
{
	SetString(value);
	return *this;
}

CBigInteger & CBigInteger::Add(const CBigInteger & other)
{
	if(ExceptAdd(other)!= NULL) return *this;	

	if(other.IsNegative() == IsNegative()) // 做加法
	{
		const TCHAR * value0 = m_szValue;
		const TCHAR * value1 = other.m_szValue;

		if(IsNegative())
		{
			++value0;
			++value1;
		}

		size_t size0 = STRLEN(value0);
		size_t size1 = STRLEN(value1);

		size_t maxSize = max(size0, size1);

		TCHAR * newValue = new TCHAR[maxSize + 3];
		memset(newValue,0, sizeof(TCHAR) * (maxSize +3));

		int carry = 0;

		size_t i = 0;
		for(i=0;i<maxSize;++i)
		{
			int x0 =  size0 > i ?  value0[size0 - 1 - i] : TNUMBER0;
			int x1 =  size1 > i ?  value1[size1 - 1 - i] : TNUMBER0;

			x0 = x0-TNUMBER0;
			x1 = x1-TNUMBER0;

			int x = x0 + x1 + carry;
			carry = x /10;
			x = x % 10;

			newValue[i] = x + TNUMBER0;
		}

		if(carry != 0) newValue[i++] = carry + TNUMBER0;

		if(IsNegative()) newValue[i++]= TNEGATIVE;

		size_t size = i ;

		__Reverse(newValue, size);

		delete this->m_szValue ;
		this->m_szValue = newValue;		
	}
	else // 做减法
	{
		const TCHAR * positive = IsNegative() ? other.m_szValue : m_szValue;
		const TCHAR * negative = IsNegative() ? m_szValue : other.m_szValue;

		size_t size0 = STRLEN(positive);
		size_t size1 = STRLEN(negative);

		int cmp = size0== size1-1 ? _tcscmp(positive, negative +1) :
			(( size0 > size1 -1) ? 1 : -1);

		if(cmp==0)
		{
			*this = ZERO;;
		}
		else
		{
			const TCHAR * maxValue = cmp > 0 ? positive : negative +1;
			const TCHAR * minValue = cmp > 0 ? negative +1 : positive;

			size_t s0 = cmp > 0 ? size0 : size1-1;
			size_t s1 = cmp > 0 ? size1-1 : size0;

			size_t maxSize = max(s0,s1);

			TCHAR * newValue = new TCHAR[maxSize +2];
			memset(newValue,0, sizeof(TCHAR) *(maxSize +2));

			size_t i = 0;
			int borrow = 0;

			TCHAR * temp = newValue;

			for(i=0;i<maxSize;++i)
			{
				int x0 =  s0 > i ?  maxValue[s0 - 1 - i] : TNUMBER0;
				int x1 =  s1 > i ?  minValue[s1 - 1 - i] : TNUMBER0;

				x0 = x0-TNUMBER0;
				x1 = x1-TNUMBER0;

				int x = 10 + x0 - borrow - x1;
				borrow = x < 10 ? 1 : 0;
				x = x % 10;

				if(i != maxSize -1 || x != 0)
					*newValue++ = x + TNUMBER0;
			}

			if(cmp == -1) *newValue++ = TNEGATIVE;


			size_t size = newValue - temp ;
			newValue = temp;

			__Reverse(newValue, size);

			__Format(newValue);

			delete this->m_szValue ;
			this->m_szValue = newValue;	
		}
	}
	return *this;
}


CBigInteger & CBigInteger::Multi(const CBigInteger & other)
{
	if(ExceptMulti(other)!= NULL) return *this;

	const TCHAR * value0 = IsNegative() ? m_szValue + 1 : m_szValue;
	const TCHAR * value1 = other.IsNegative() ? other.m_szValue + 1 : other.m_szValue;

	size_t size0 = STRLEN(value0);
	size_t size1 = STRLEN(value1);

	TCHAR * newValue = new TCHAR [size0 + size1 + 2];
	memset(newValue, 0, sizeof(TCHAR) * (size0 + size1 + 2) );

	for(size_t i=0;i<size0;++i)
	{
		int c0 = value0[ size0 -i -1] - TNUMBER0;	

		if(c0==0) continue;

		for(size_t j=0;j<size1; ++j)
		{
			int c1 = value1[ size1 -j -1] - TNUMBER0;
			if(c1==0) continue;

			int c = c0 * c1 ;
			int carry = c / 10;
			c = c % 10;

			int k = i+j;
			do
			{
				int vk= newValue[k];
				vk = vk==0 ? 0 : vk - TNUMBER0;

				vk = vk + c;
				c = carry + vk / 10;
				newValue[k] = (vk % 10) + TNUMBER0;
				++k;
				carry = 0;

			}while(c !=0 );		
		}
	}

	size_t count =0;
	for(size_t i=size0 + size1 + 1; i>=0;--i)
	{
		if(newValue[i]!=TNUMBER0 && newValue[i] != TZERO)
		{
			count = i+1;
			break;
		}
	}

	for(size_t i=0;i<count;++i)
	{
		if(newValue[i] == TZERO ) newValue[i] = TNUMBER0;
	}

	if( IsNegative() != other.IsNegative())
	{
		newValue[count] = TNEGATIVE;
		++count;
	}


	__Reverse(newValue, count);

	delete this->m_szValue ;
	this->m_szValue = newValue;	

	return *this;
}

static  int __SampleDiv(const TCHAR src[],
						size_t srcSize,
						const TCHAR sub[],
						size_t subSize,
						TCHAR residue[],
						void * tempBuffer)
{
	int srcBegin = srcSize > subSize ? ( src[0] - TNUMBER0 ) * 10 + (src[1] - TNUMBER0 ) : src[0] - TNUMBER0 ;
	int subBegin = sub[0] - TNUMBER0;

	int begin = srcBegin / (subBegin +1);
	int end = srcBegin / subBegin + 1;


	TCHAR * lastMulti = (TCHAR *)tempBuffer;
	memset(lastMulti, 0, sizeof(TCHAR) * srcSize);
	TCHAR * thisMulti = lastMulti + srcSize + 1;

	for(int i = begin;i<= end; ++i)
	{
		memset(thisMulti, 0, sizeof(TCHAR) * srcSize);

		TCHAR * temp = thisMulti;
		int carry = 0;
		for(int j=subSize -1; j >= 0; --j)
		{
			int x =  sub[j] - TNUMBER0 ;		

			x = x * i + carry;
			carry = x /10;
			x = x % 10;

			*temp++ = (TCHAR)x + TNUMBER0;			
		}

		if(carry != 0)
			*temp++ = carry + TNUMBER0;

		size_t len = temp - thisMulti;

		for(size_t j=len;j<srcSize;++j)
		{
			*temp++ = TNUMBER0;
		}

		__Reverse(thisMulti, srcSize);

		if(len < srcSize )
		{
			memset(lastMulti, 0, sizeof(TCHAR) * srcSize);			
			memcpy(lastMulti, thisMulti, sizeof(TCHAR)* srcSize );
			lastMulti[srcSize] = TZERO;
			continue;
		}

		int cmp = __StrCmp(thisMulti, src , srcSize) ;

		if(cmp < 0)
		{
			memset(lastMulti, 0, sizeof(TCHAR) * srcSize);
			memcpy(lastMulti, thisMulti, sizeof(TCHAR)* srcSize );
			lastMulti[srcSize] = TZERO;
			continue;

		}
		else
		{
			TCHAR * multi = cmp == 0 ? thisMulti : lastMulti;

			int borrow = 0;
			for(size_t j = 0; j < srcSize;++j)
			{
				int x0 =  src[srcSize - 1 - j] - TNUMBER0;
				int x1 =  multi[srcSize - 1 - j] - TNUMBER0;

				int x = 10 + x0 - borrow - x1;
				borrow = x < 10 ? 1 : 0;
				x = x % 10;

				residue[j] = x + TNUMBER0;
			}	

			if(borrow != 0) residue[0] = TNUMBER0;

			__Reverse(residue, 0);

			return cmp == 0? i : i-1;
		}
	}	

	return 0;
}

CBigInteger &  CBigInteger::DivEx(const CBigInteger & other, CBigInteger * pResidue)
{
	CBigInteger * exceptResult = ExceptDiv(other);
	if(exceptResult != NULL) return *exceptResult;


	TCHAR * value0 = IsNegative() ? m_szValue + 1 : m_szValue;
	const TCHAR * value1 = other.IsNegative() ? other.m_szValue + 1 : other.m_szValue;

	size_t size0 = STRLEN(value0);
	size_t size1 = STRLEN(value1);

	// 临时的余数
	TCHAR * subResidue = new TCHAR [ size1 + 2];

	// 临时的空间
	TCHAR * temp = new TCHAR [size1 *2 + 10];
	memset(temp,0, sizeof(TCHAR) * (size1 * 2 + 10));

	// 结果
	TCHAR * result = new TCHAR[size0 + 2];
	memset(result, 0, sizeof(TCHAR)*(size0 + 2));

	TCHAR * tempResult = result;

	if(IsNegative() && !other.IsNegative()
		|| !IsNegative() && other.IsNegative())
	{
		*result++ = TNEGATIVE;
	}

	size_t lastIndex = 0;
	for(size_t i=0;i<size0;++i)
	{
		size_t len = i - lastIndex + 1;
		if(size1 > len )
		{
			*result++ =  TNUMBER0;
			continue;
		}

		if(size1 == len && __StrCmp(value0 + lastIndex, value1, len) < 0) 
		{
			*result++ = TNUMBER0;
			continue;
		}

		memset(subResidue, 0 ,sizeof(TCHAR) * (size1 + 2));
		int v = __SampleDiv(value0 + lastIndex, len, value1, size1, subResidue, temp);

		*result++ = v + TNUMBER0;

		memset(value0, 0, sizeof(TCHAR) * len);

		__Format(subResidue);

		size_t subResidueLen = STRLEN(subResidue);

		lastIndex = lastIndex + len - subResidueLen;

		for(size_t j = 0; j< subResidueLen; ++j)
		{
			value0[lastIndex + j] = subResidue[j];
		}
	}

	if(pResidue != NULL) 
	{
		TCHAR * temp = value0 + lastIndex;
		__Format(temp);
		*pResidue = temp;

	}

	delete [] m_szValue;

	__Format(tempResult);
	m_szValue = tempResult;

	delete [] subResidue;
	delete [] temp;

	return *this;

}


CBigInteger & CBigInteger::operator %= (const CBigInteger & i)
{
	CBigInteger i2 = *this;
	i2.DivEx(i,this);

	return *this;
}

CBigInteger CBigInteger::operator % (const CBigInteger & i) const
{
	CBigInteger i2 = *this;
	CBigInteger i3(0.0);
	i2.DivEx(i, &i3);

	return i3;
}

int CBigInteger::Compare(const CBigInteger & other) const
{
	if(other.IsNegative() && IsNegative())
	{
		return -PositiveIntegerCompare(m_szValue + 1,other.m_szValue + 1);		
	}
	else if(other.IsNegative())
	{
		return 1;
	}
	else if(IsNegative())
	{
		return -1;
	}
	else
	{
		return PositiveIntegerCompare(m_szValue, other.m_szValue);
	}
}

CBigInteger::operator INT64() const
{
	return _ttoi64(m_szValue);
}

CBigInteger::operator INT32() const
{
	return _ttoi(m_szValue);
}

CBigInteger::operator INT16() const
{
	return (INT16)_ttoi(m_szValue);
}

CBigInteger::operator INT8() const
{
	return (INT8)_ttoi(m_szValue);
}
