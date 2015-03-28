#pragma once
#include "Visitation.h"
#include "MethodCommon.h"
#include "TimXmlRpc.h"
#include <memory>
#include <functional>

namespace Bugzilla
{
	template <typename T>
	struct CVisitableVector : public CVisitableVectorBase
	{
		::std::vector<T> Container;

		int GetSize() const { return static_cast<int>(Container.size()); };
		void SetSize(int Size) { Container.resize(Size); };

		void Accept(CElementVisitor& Visitor) override
		{
			for (auto it = Container.begin(); it != Container.end(); ++it)
				Visitor.Visit(static_cast<int>(distance(Container.begin(), it)), *it);
		}

		operator bool() const { return Container.size() ? true : false; }
	};

	struct CXmlRpcSerializerBase
	{
		bool bSave;
		XmlRpcValue& XmlValue;

		CXmlRpcSerializerBase(XmlRpcValue& Value, bool bSave)
			: XmlValue(Value),
			bSave(bSave)
		{
		}
	};

	struct CXmlRpcSerializer : public CXmlRpcSerializerBase, public CVisitor
	{
		CXmlRpcSerializer(XmlRpcValue& Value, bool bSave);

		void Visit(bool& Value) override;
		void Visit(int& Value) override;
		void Visit(double& Value) override;
		void Visit(stringA& Value) override;
		void Visit(tm& Value) override;
	};

	struct CXmlRpcFieldSerializer : public CXmlRpcSerializerBase, public CFieldVisitor
	{
		CXmlRpcFieldSerializer(XmlRpcValue& Value, bool bSave);

		bool CanVisit(const char* Name, bool bValid = true) const override;

		void Visit(const char* Name, bool& Value) override;
		void Visit(const char* Name, int& Value) override;
		void Visit(const char* Name, double& Value) override;
		void Visit(const char* Name, stringA& Value) override;
		void Visit(const char* Name, tm& Value) override;

		void Visit(const char* Name, CVisitable& Visitable) override;
		void Visit(const char* Name, CVisitableStruct& Visitable) override;
		void Visit(const char* Name, CVisitableVectorBase& Visitable) override;
	};

	struct CXmlRpcElementSerializer : public CXmlRpcSerializerBase, public CElementVisitor
	{
		CXmlRpcElementSerializer(XmlRpcValue& Value, bool bSave);

		void Visit(int nElement, bool& Value) override;
		void Visit(int nElement, int& Value) override;
		void Visit(int nElement, double& Value) override;
		void Visit(int nElement, stringA& Value) override;
		void Visit(int nElement, tm& Value) override;

		void Visit(int nElement, CVisitable& Visitable) override;
		void Visit(int nElement, CVisitableStruct& Visitable) override;
		void Visit(int nElement, CVisitableVectorBase& Visitable) override;
	};

	template <typename T>
	void Visit(CVisitor& Visitor, T& Value)
	{
		Visitor.Visit(Value);
	};

	template <typename T>
	void Visit(CFieldVisitor& Visitor, const char* Name, T& Value)
	{
		Visitor.Visit(Name, Value);
	};

	template <typename T>
	void Visit(CElementVisitor& Visitor, int nElement, T& Value)
	{
		Visitor.Visit(nElement, Value);
	};

	template <typename T>
	void Visit(CFieldVisitor& Visitor, const char* Name, T& Value, bool bMandatory)
	{
		if (Visitor.CanVisit(Name, Value ? true : false))
			Visitor.Visit(Name, Value);
		else if (bMandatory)
			throw new Bugzilla::CBugzillaException(string("Could not access mandatory element named ") + Name);
	};

	template <typename T>
	void Visit(CFieldVisitor& Visitor, const char* Name, T& Value, bool bMandatory, bool bValidField)
	{
		if (Visitor.CanVisit(Name, bValidField))
			Visitor.Visit(Name, Value);
		else if (bMandatory)
			throw new Bugzilla::CBugzillaException(string("Could not access mandatory element named ") + Name);
	};

	template <>
	void Visit(CFieldVisitor& Visitor, const char* Name, stringA& Value, bool bMandatory);
};