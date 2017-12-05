/**
 * @file atomicvar.h
 * @author yanshi
 * @date 2015-12-31
 */

#ifndef __ATOMICVAR_H__
#define __ATOMICVAR_H__

namespace MYFRAMECOMMON{

template <typename T, typename T2 = T>
class CAtomicVar
{
public:
	explicit CAtomicVar(T2 nInitValue = 0) : m_nVar(nInitValue) {}
	T FetchAdd(T2 nValue) { return __sync_fetch_and_add(&m_nVar, nValue); }
	T FetchSub(T2 nValue) { return __sync_fetch_and_sub(&m_nVar, nValue); }
	T FetchOr(T2 nValue) { return __sync_fetch_and_or(&m_nVar, nValue); }
	T FetchAnd(T2 nValue) { return __sync_fetch_and_and(&m_nVar, nValue); } 
	T FetchXor(T2 nValue) { return __sync_fetch_and_xor(&m_nVar, nValue); }
	//compile error: ‘void __sync_fetch_and_nand(...)’ changed semantics in GCC 4.4
	//T fetchNand(T2 nValue) { return __sync_fetch_and_nand(&m_nVar, nValue); }
	T AddFetch(T2 nValue) { return __sync_add_and_fetch(&m_nVar, nValue); }
	T SubFetch(T2 nValue) { return __sync_sub_and_fetch(&m_nVar, nValue); }
	T OrFetch(T2 nValue) { return __sync_or_and_fetch(&m_nVar, nValue); }
	T AndFetch(T2 nValue) { return __sync_and_and_fetch(&m_nVar, nValue); }
	T XorFetch(T2 nValue) { return __sync_xor_and_fetch(&m_nVar, nValue); }
	//compile error: ‘void __sync_fetch_and_nand(...)’ changed semantics in GCC 4.4	
	//T nandFetch(T nValue) { return __sync_nand_and_fetch(&m_nVar, nValue); }
	T GetVar() const { return m_nVar; }		
protected:
	volatile T m_nVar;
};

template <typename T, typename T2>
T FetchAdd(T *pVar, T2 nValue)
{
	return __sync_fetch_and_add(pVar, nValue);
}

template <typename T, typename T2>
T FetchSub(T *pVar, T2 nValue) 
{ 
	return __sync_fetch_and_sub(pVar, nValue); 
}

template <typename T, typename T2>
T FetchOr(T *pVar, T2 nValue)
{
	return __sync_fetch_and_or(pVar, nValue);
}

template <typename T, typename T2>
T FetchAnd(T *pVar, T2 nValue)
{
	return __sync_fetch_and_and(pVar, nValue);
}

template <typename T, typename T2>
T FetchXor(T *pVar, T2 nValue)
{
	return __sync_fetch_and_xor(pVar, nValue);
}

template <typename T, typename T2>
T AddFetch(T *pVar, T2 nValue)
{
	return __sync_add_and_fetch(pVar, nValue);
}

template <typename T, typename T2>
T SubFetch(T *pVar, T2 nValue)
{
	return __sync_sub_and_fetch(pVar, nValue);
}

template <typename T, typename T2>
T OrFetch(T *pVar, T2 nValue)
{
	return __sync_or_and_fetch(pVar, nValue);
}

template <typename T, typename T2>
T AndFetch(T *pVar, T2 nValue)
{
	return __sync_and_and_fetch(pVar, nValue);
}

template <typename T, typename T2>
T XorFetch(T *pVar, T2 nValue)
{
	return __sync_xor_and_fetch(pVar, nValue);
}


}

#endif // __ATOMICVAR_H__

