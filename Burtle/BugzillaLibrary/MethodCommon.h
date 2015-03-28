#pragma once
#include "Serialization.h"

namespace Bugzilla
{
	struct CXmlRpcWrapper : public CVisitable
	{
		XmlRpcValue& Value;
		CXmlRpcWrapper(XmlRpcValue& XmlRpcValue);

		void Accept(CVisitor& Visitor) override;
	};

	struct CXmlRpcArrayWrapper : public CVisitableVectorBase
	{
		XmlRpcValue& Value;
		CXmlRpcArrayWrapper(XmlRpcValue& XmlRpcValue);

		int GetSize() const override;
		void SetSize(int Size) override;

		void Accept(CElementVisitor& Visitor) override;
	};

	struct CXmlRpcStructWrapper : public CVisitableStruct
	{
		XmlRpcValue& Value;
		CXmlRpcStructWrapper(XmlRpcValue& XmlRpcValue);
		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugzillaException : public CVisitableStruct, public ::std::exception
	{
		int ErrorCode;
		stringA ErrorMessage;

		CBugzillaException()
		{
		}

		CBugzillaException(stringA& Message) 
			: ::std::exception(Message.c_str()),
			ErrorMessage(Message)
		{
		}

		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugzillaID : public CVisitableStruct
	{
		int ID;

		CBugzillaID();
		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugzillaIDs : public CVisitableStruct
	{
		CVisitableVector<int> IDs;

		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugzillaObject : public CVisitableStruct
	{
		CBugzillaID ObjectID;

		void Accept(CFieldVisitor& Visitor) override;
	};

	template <typename TBugzillaObject>
	struct CBugzillaObjects : public CVisitableStruct
	{
		CVisitableVector<TBugzillaObject> Objects;

		void Accept(CFieldVisitor& Visitor) override
		{
			static_assert("override using specialization", false);
		};
	};

	struct CCommonBugParamaters : public CVisitableStruct
	{
		stringA Product;
		stringA Component;
		stringA Summary;
		stringA Version;
		stringA Description;
		stringA OperatingSystem;
		stringA Platform;
		stringA Priority;
		stringA Severity;
		stringA Alias;
		stringA AssignedTo;
		stringA QAContact;
		stringA Status;
		stringA TargetMilestone;

		void Accept(CFieldVisitor& Visitor) override;
	};
}