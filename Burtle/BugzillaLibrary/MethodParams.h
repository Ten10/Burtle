#pragma once
#include "MethodCommon.h"

namespace Bugzilla
{
	namespace UserFunctionParams
	{
		struct CLoginParam : public CVisitableStruct
		{
			stringA Username;
			stringA Password;
			bool bRestrictLogin = false;
			void Accept(CFieldVisitor& Visitor) override;
		};

		struct CUserSearchParam : public CVisitableStruct
		{
			CVisitableVector<int> IDs;
			CVisitableVector<stringA> Names;
			CVisitableVector<stringA> Match;

			void Accept(CFieldVisitor& Visitor) override;
		};
	};

	namespace BugFunctionParams
	{
		struct CGetBugParam : public CVisitableStruct
		{
			CVisitableVector<int> BugIDs;

			void Accept(CFieldVisitor& Visitor) override;
		};

		struct CGetBugFieldParam : public CVisitableStruct
		{
			CVisitableVector<stringA> FieldIDs;
			CVisitableVector<stringA> FieldNames;

			void Accept(CFieldVisitor& Visitor) override;
		};

		struct CBugComment : public CVisitableStruct
		{
			stringA Text;
			bool bPrivate;

			void Accept(CFieldVisitor& Visitor) override;
		};

		struct CBugAddCommentParam : public CVisitableStruct
		{
			int BugID;
			CBugComment Comment;
			double WorkTime;

			void Accept(CFieldVisitor& Visitor) override;
		};

		struct CCreateBugParam : public CVisitableStruct
		{
			CCommonBugParamaters CommonParameters;
			CVisitableVector<stringA> Cc;

			void Accept(CFieldVisitor& Visitor) override;
		};

		struct CUpdateBugsParam : public CVisitableStruct
		{
			XmlRpcValue UpdatedFields;

			void Accept(CFieldVisitor& Visitor) override;

			operator XmlRpcValue&() { return UpdatedFields; }
		};

		struct CBugSearchParam : public CVisitableStruct
		{
			CBugSearchParam();
			bool operator==(const CBugSearchParam& Other) const;

			int ID;
			int Limit;
			int Offset;
        
			stringA Alias;
			stringA AssigneeName;        
			stringA Component;
			stringA Creator;
			stringA OperatingSystem;
			stringA Platform;
			stringA Resolution;
			stringA Severity;
			stringA Status;
			stringA Summary;
			stringA TargetMilestone;
			stringA QAContact;
			stringA Version;    
			stringA Whiteboard;

			tm Created;
			tm Changed;

			void Accept(CFieldVisitor& Visitor) override;
		};
	};
};