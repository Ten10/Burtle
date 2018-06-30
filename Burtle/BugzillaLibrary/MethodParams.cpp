#include "stdafx.h"
#include "MethodParams.h"

namespace Bugzilla
{
	namespace UserFunctionParams
	{
		void CLoginParam::Accept(CFieldVisitor& Visitor)
		{
			Bugzilla::TryVisit(Visitor, "login", Username);
			Bugzilla::TryVisit(Visitor, "password", Password);
			Bugzilla::TryVisit(Visitor, "restrict_login", bRestrictLogin);
		}

		void CUserSearchParam::Accept(CFieldVisitor& Visitor)
		{
			Bugzilla::TryVisit(Visitor, "ids", IDs);
			Bugzilla::TryVisit(Visitor, "names", Names);
			Bugzilla::TryVisit(Visitor, "match", Match);
		}
	};

	namespace BugFunctionParams
	{

		void CGetBugParam::Accept(CFieldVisitor& Visitor)
		{
			Bugzilla::TryVisit(Visitor, "ids", BugIDs);
		}

		void CGetBugFieldParam::Accept(CFieldVisitor& Visitor)
		{
			if (FieldIDs.Container.size())
				Bugzilla::TryVisit(Visitor, "ids", FieldIDs);
			if (FieldNames.Container.size())
				Bugzilla::TryVisit(Visitor, "names", FieldNames);
		}

		void CBugComment::Accept(CFieldVisitor& Visitor)
		{
			Bugzilla::TryVisit(Visitor, "comment", Text);
			Bugzilla::TryVisit(Visitor, "is_private", bPrivate);
		}

		void CBugAddCommentParam::Accept(CFieldVisitor& Visitor)
		{
			Comment.Accept(Visitor);
			Bugzilla::TryVisit(Visitor, "id", BugID, BugID > 0);
			Bugzilla::TryVisit(Visitor, "work_time", WorkTime);
		}

		void CCreateBugParam::Accept(CFieldVisitor& Visitor)
		{
			CommonParameters.Accept(Visitor);
			Bugzilla::TryVisit(Visitor, "cc", Cc);
		}

		void CUpdateBugsParam::Accept(CFieldVisitor& Visitor)
		{
			CXmlRpcStructWrapper wrapper(UpdatedFields);
			wrapper.Accept(Visitor);
		}

		CBugSearchParam::CBugSearchParam()
			: ID(-1),
			Limit(-1),
			Offset(-1)
		{
			Created = { 0 };
			Changed = { 0 };
		}

		bool CBugSearchParam::operator==(const CBugSearchParam& Other) const
		{
			return ID == Other.ID
				&& Alias == Other.Alias
				&& AssigneeName == Other.AssigneeName
				&& Component == Other.Component
				&& Creator == Other.Creator
				&& OperatingSystem == Other.OperatingSystem
				&& Platform == Other.Platform
				&& Resolution == Other.Resolution
				&& Severity == Other.Severity
				&& Status == Other.Status
				&& Summary == Other.Summary
				&& TargetMilestone == Other.TargetMilestone
				&& QAContact == Other.QAContact
				&& Version == Other.Version
				&& Whiteboard == Other.Whiteboard
				&& memcmp(&Created, &Other.Created, sizeof(Created))
				&& memcmp(&Changed, &Other.Changed, sizeof(Changed));
		}

		void CBugSearchParam::Accept(CFieldVisitor& Visitor)
		{
			Bugzilla::TryVisit(Visitor, "id", ID, ID > 0);
			Bugzilla::TryVisit(Visitor, "limit", Limit, Limit >= 0);
			Bugzilla::TryVisit(Visitor, "offset", Offset, Offset >= 0);
		
			pair<string, string*> Fields [] = 
			{ 
				make_pair("alias", &Alias),
				make_pair("assigned_to", &AssigneeName),
				make_pair("component", &Component),
				make_pair("creator", &Creator),
				make_pair("op_sys", &OperatingSystem),
				make_pair("platform", &Platform),
				make_pair("resolution", &Resolution),
				make_pair("severity", &Severity),
				make_pair("status", &Status),
				make_pair("summary", &Summary),
				make_pair("target_milestone", &TargetMilestone),
				make_pair("qa_contact", &QAContact),
				make_pair("version", &Version),
				make_pair("whiteboard", &Whiteboard),
			};

			for (auto& fieldPair : Fields)
				Bugzilla::TryVisit(Visitor, fieldPair.first.c_str(), *fieldPair.second);

			Bugzilla::TryVisit(Visitor, "creation_time", Created, Created.tm_year ? true : false);
			Bugzilla::TryVisit(Visitor, "last_change_time", Changed, Changed.tm_year ? true : false);
		}
	};
}