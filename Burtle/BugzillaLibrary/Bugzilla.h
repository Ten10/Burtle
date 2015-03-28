#pragma once
#include "TimXmlRpc.h"
#include "MethodParams.h"
#include "MethodReturnValues.h"

#ifdef USE_EXPERIMENTAL
#define EXPERIMENTAL(function) function
#else
#define EXPERIMENTAL(function)
#endif

namespace Bugzilla
{
	class CServer;
	class CProduct
	{
		CProductInfo m_ProductInfo;
		CServer& m_Server;
	public:
		CProduct(CServer& Server, const CProductInfo& ProductInfo);
		operator bool() const;

		int CreateBug(const stringA& Component, const stringA& Summary, const stringA& Version, const stringA& Assignee);
	};

	class CUser
	{
		CUserInfo m_UserInfo;
		CServer& m_Server;
	public:
		CUser(CServer& Server, const CUserInfo& BugInfo);
		operator bool() const;
	};

	class CBug
	{
		CBugInfo m_BugInfo;
		CServer& m_Server;
	public:
		CBug(CServer& Server, const CBugInfo& BugInfo);

		bool AppendComment(const stringA& Comment, double Worktime = 0, bool Private = false);
		stringA Update(const BugFunctionParams::CUpdateBugsParam& UpdateBugsParam);
	};

	class CServer
	{
		stringA m_Version;
		time_t m_Time;
		time_t m_DatabaseTime;
		time_t m_BuildTime;
		EXPERIMENTAL(time_t m_LastAuditTime;)

		CActiveLogin m_Login;

		XmlRpcClient m_Connection;

		::std::string GetVersionRpc();
		bool GetTimeRpc(tm& BugzillaServerTime, tm& DatabaseServerTime);

		static void XmlRpcCallback(void* context, char* status);
		XmlRpcValue CallRemoteFunction(const char* pFunction, XmlRpcValue& Param, bool bRequireLogin = false);

		friend class CBug;
		friend class CUser;
		friend class CProduct;
		friend XmlRpcValue Execute(CServer& Server, const char* FunctionName, XmlRpcValue& Param, bool bRequireLogin);

	public:
		CServer(const ::std::string& BugzillaURI, bool bInitialize = false);
		CServer(const CServer& Server) = delete;
		~CServer();

		typedef ::std::function<void(const ::std::string& ReportProgress)> ProgressCallback;
		ProgressCallback Callback;

		bool Initialize();

		::std::string GetVersion() const;
		bool GetTime(time_t& BugzillaServerTime, time_t& DatabaseServerTime) const;

		bool IsLoggedIn() const;
		void Login(const ::std::string& Username, const ::std::string& Password);
		void Logout();

		CBugInfo GetBug(int BugID);
		CBugInfos GetBugs(const BugFunctionParams::CBugSearchParam& BugSearchParam);
		CBugFields GetBugFields(const BugFunctionParams::CGetBugFieldParam& BugFieldParam);
		int CreateBug(const BugFunctionParams::CCreateBugParam& CreateBugParam);

		EXPERIMENTAL(CProductIDs GetSelectableProductIDs();)
		EXPERIMENTAL(CProductIDs GetEnterableProductIDs();)
		EXPERIMENTAL(CProductInfos GetProducts(CProductIDs ProductIDs));

		CUserInfos GetUsers(const ::std::string& Match = ".*@.*");
	};
};