#include "stdafx.h"
#include "MethodCommon.h"
#include "Serialization.h"
#include <algorithm>
#include "TimXmlRpc.h"

namespace Bugzilla
{
	namespace VisitTemplates
	{
		template <typename T>
		void Visit(CXmlRpcSerializerBase& Serializer, T& Value)
		{
			if (Serializer.bSave)
				Serializer.XmlValue = Value;
			else
				Value = Serializer.XmlValue;
		};

		template <>
		void Visit(CXmlRpcSerializerBase& Serializer, stringA& Value)
		{
			if (Serializer.bSave)
				Serializer.XmlValue = Value;
			else
				Value = Serializer.XmlValue.operator string();
		}
	};

	CXmlRpcSerializer::CXmlRpcSerializer(XmlRpcValue& Value, bool bSave)
		: CXmlRpcSerializerBase(Value, bSave)
	{
	}

	void CXmlRpcSerializer::Visit(bool& Value)
	{
		Bugzilla::VisitTemplates::Visit(*this, Value);
	}

	void CXmlRpcSerializer::Visit(int& Value)
	{
		Bugzilla::VisitTemplates::Visit(*this, Value);
	}

	void CXmlRpcSerializer::Visit(double& Value)
	{
		Bugzilla::VisitTemplates::Visit(*this, Value);
	}

	void CXmlRpcSerializer::Visit(stringA& Value)
	{
		Bugzilla::VisitTemplates::Visit(*this, Value);
	}

	void CXmlRpcSerializer::Visit(tm& Value)
	{
		Bugzilla::VisitTemplates::Visit(*this, Value);
	}

	CXmlRpcFieldSerializer::CXmlRpcFieldSerializer(XmlRpcValue& Value, bool bSave)
		: CXmlRpcSerializerBase(Value, bSave)
	{
	}

	bool CXmlRpcFieldSerializer::CanVisit(const char* Name, bool bValid) const
	{ 
		return (!bSave && XmlValue.hasMember(Name)) || bValid;
	};

	void CXmlRpcFieldSerializer::Visit(const char* Name, bool& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[Name], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, int& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[Name], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, double& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[Name], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, stringA& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[Name], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, tm& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[Name], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, CVisitable& Visitable)
	{
		CXmlRpcSerializer serializer(XmlValue[Name], bSave);
		Visitable.Accept(serializer);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, CVisitableStruct& Visitable)
	{
		CXmlRpcFieldSerializer serializer(XmlValue[Name], bSave);
		Visitable.Accept(serializer);
	}

	void CXmlRpcFieldSerializer::Visit(const char* Name, CVisitableVectorBase& Visitable)
	{
		CXmlRpcElementSerializer serializer(XmlValue[Name], bSave);
		if (serializer.bSave)
			serializer.XmlValue.setSize(Visitable.GetSize());
		else
			Visitable.SetSize(serializer.XmlValue.size());
		Visitable.Accept(serializer);
	}

	CXmlRpcElementSerializer::CXmlRpcElementSerializer(XmlRpcValue& Value, bool bSave)
		: CXmlRpcSerializerBase(Value, bSave)
	{
	}

	void CXmlRpcElementSerializer::Visit(int nElement, bool& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[nElement], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, int& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[nElement], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, double& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[nElement], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, stringA& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[nElement], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, tm& Value)
	{
		CXmlRpcSerializer serializer(XmlValue[nElement], bSave);
		Bugzilla::VisitTemplates::Visit(serializer, Value);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, CVisitable& Visitable)
	{
		CXmlRpcSerializer serializer(XmlValue[nElement], bSave);
		Visitable.Accept(serializer);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, CVisitableStruct& Visitable)
	{
		CXmlRpcFieldSerializer serializer(XmlValue[nElement], bSave);
		Visitable.Accept(serializer);
	}

	void CXmlRpcElementSerializer::Visit(int nElement, CVisitableVectorBase& Visitable)
	{
		CXmlRpcElementSerializer serializer(XmlValue[nElement], bSave);
		if (serializer.bSave)
			serializer.XmlValue.setSize(Visitable.GetSize());
		else
			Visitable.SetSize(serializer.XmlValue.size());
		Visitable.Accept(serializer);
	}

	void Visit(CFieldVisitor& Visitor, const char* Name, stringA& Value, bool bMandatory)
	{
		if (Visitor.CanVisit(Name, !Value.empty()))
			Visitor.Visit(Name, Value);
		else if (bMandatory)
			throw new CBugzillaException(string("Could not access mandatory element named ") + Name);
	};
};