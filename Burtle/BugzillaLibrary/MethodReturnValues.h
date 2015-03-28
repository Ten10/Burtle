#pragma once
#include "TimXmlRpc.h"
#include "MethodCommon.h"

namespace Bugzilla
{
	struct CActiveLogin : public CVisitableStruct
	{
		int UserID = 0;
		stringA Token;
		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CGetVersionInfo : public CVisitableStruct
	{
		stringA Version;
		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CGetTimeInfo : public CVisitableStruct
	{
		tm WebTime;
		tm DatabaseTime;

		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugUpdateResult : public CBugzillaObject
	{
		stringA Alias;
		tm LastChangeTime;
		//changes are not implemented

		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugField : public CBugzillaObject
	{
		struct CFieldValue : public CBugzillaObject
		{
			bool bOpen;
			bool bIsActive;

			stringA Name;
			int SortKey;
			stringA Description;
			CVisitableVector<stringA> VisibillityValues;

			struct CStatusOption : public CVisitableStruct
			{
				stringA Name;
				bool bCommentRequired;

				void Accept(CFieldVisitor& Visitor) override;
			};

			CVisitableVector<CStatusOption> CanChangeTo;

			void Accept(CFieldVisitor& Visitor) override;
		};

		enum eType : int
		{
			typeUnknown = 0,
			typeFreeText = 1, 
			typeDropDown = 2,
			typeMultipleSelectionBox = 3,
			typeLargeTextBox = 4,
			typeDate = 5,
			typeBugID = 6,
			typeBugURL = 7
		};

		eType FieldType;
		bool bCustom;
		bool bMandatory;
		bool bInBugEntry;
		stringA Name;
		stringA DisplayName;

		stringA VisibillityField;
		CVisitableVector<stringA> VisibillityValues;

		stringA ValueField;
		CVisitableVector<CFieldValue> Values;

		bool ShouldDisplay() const;
		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CBugInfo : public CVisitableStruct
	{
		XmlRpcValue BugInfo;

		CBugInfo() = default;
		CBugInfo(XmlRpcValue&& Value);

		int GetBugID() const;

		void Accept(CFieldVisitor& Visitor) override;
		operator XmlRpcValue&() { return BugInfo; }
	};

	struct CUserInfo : public CBugzillaObject
	{
		stringA Name;
		stringA Email;
		stringA LoginName;
		bool bCanLogin;
		bool bEmailEnabled;
		stringA LoginDeniedText;

		void Accept(CFieldVisitor& Visitor) override;
	};

	struct CProductInfo : public CBugzillaObject
	{
		stringA Name;
		stringA Description;

		void Accept(CFieldVisitor& Visitor) override;
	};

	typedef CBugzillaObjects<CUserInfo> CUserInfos;
	typedef CBugzillaObjects<CBugField> CBugFields;
	typedef CBugzillaObjects<CBugInfo> CBugInfos;
	typedef CBugzillaObjects<CProductInfo> CProductInfos;
	typedef CBugzillaObjects<CBugUpdateResult> CBugUpdateResults;

	template <>
	void CUserInfos::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "users", Objects);
	};

	template <>
	void CBugFields::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "fields", Objects);
	};

	template <>
	void CBugInfos::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "bugs", Objects);
	};

	template <>
	void CProductInfos::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "products", Objects);
	};

	template <>
	void CBugUpdateResults::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "bugs", Objects);
	};
};