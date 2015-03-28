#pragma once
#include <map>
#include <list>

template <typename THandleType, typename TObjectType>
class CHandleMap
{
	friend struct CTempCleaner;

	typedef ::std::map<THandleType, TObjectType*> HandleToObject;
public:
	struct CTempMapHelper
	{
		CHandleMap<THandleType, TObjectType>* pHandleMap;
		CTempMapHelper(const CTempCleaner&) = delete;
		CTempMapHelper(CTempCleaner&&) = delete;
		CTempMapHelper& operator=(const CTempMapHelper& Other) = delete;
		CTempMapHelper& operator=(CTempMapHelper&& Other)
		{
			swap(pHandleMap, Other.pHandleMap);
			return *this;
		}
		CTempMapHelper(CHandleMap<THandleType, TObjectType>* pHandleMap = nullptr)
			: pHandleMap(pHandleMap)
		{
			if (pHandleMap)
				pHandleMap->m_TemporaryMaps.push_back(HandleToObject());
		}

		~CTempMapHelper()
		{
			if (!pHandleMap)
				return;

			auto& tempMap = pHandleMap->m_TemporaryMaps.back();
			for (auto it = tempMap.begin(); it != tempMap.end(); ++it)
			{
				it->second->SetHandle(nullptr);
				delete it->second;
			}
			pHandleMap->m_TemporaryMaps.pop_back();
		}
	};

private:
	HandleToObject m_PermenentMap;
	::std::list<HandleToObject> m_TemporaryMaps;
	CTempMapHelper m_TempMapHandler;
public:
	CHandleMap()
		: m_TempMapHandler(this)
	{
	}

	~CHandleMap()
	{
	}

	void Add(const ::std::pair<THandleType, TObjectType*>& Pair)
	{
		m_PermenentMap.insert(Pair);
	}

	void Remove(THandleType Handle)
	{
		m_PermenentMap.erase(Handle);
	}

	TObjectType* FromHandle(THandleType Handle)
	{
		if (Handle == nullptr)
			return nullptr;

		auto it = m_PermenentMap.find(Handle);
		if (it != m_PermenentMap.end())
			return it->second;
		
		auto& tempMap = m_TemporaryMaps.back();
		TObjectType*& pObject = tempMap[Handle];
		if (!pObject)
			pObject = new TObjectType();
		pObject->SetHandle(Handle);
		return pObject;
	}

	TObjectType* FromPermenantHandle(THandleType Handle)
	{
		if (Handle == nullptr)
			return nullptr;

		auto it = m_PermenentMap.find(Handle);
		if (it == m_PermenentMap.end())
			return nullptr;

		return it->second;
	}

	CTempMapHelper GetTempMapHelper()
	{
		return CTempMapHelper(this);
	}
};