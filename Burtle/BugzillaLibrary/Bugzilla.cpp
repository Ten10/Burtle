#include "stdafx.h"
#include "MethodParams.h"
#include "MethodReturnValues.h"
#include "Bugzilla.h"

namespace Bugzilla
{
	using namespace UserFunctionParams;
	using namespace BugFunctionParams;

	template <typename T>
	static typename enable_if<is_convertible<T, XmlRpcValue>::value, XmlRpcValue>::type ToXmlRpcValue(T& Value)
	{
		return static_cast<XmlRpcValue&>(Value);
	}

	template <typename T>
	static typename enable_if<!is_convertible<T, XmlRpcValue>::value, XmlRpcValue>::type ToXmlRpcValue(T& Value)
	{
		XmlRpcValue value;
		CXmlRpcFieldSerializer visitor(value, true);
		Value.Accept(visitor);
		return value;
	}

	template <typename T>
	static typename enable_if<is_convertible<T, XmlRpcValue>::value, void>::type FromXmlRpcValue(T& Object, XmlRpcValue&& Value)
	{
		static_cast<XmlRpcValue&>(Object) = move(Value);
	}

	template <typename T>
	static typename enable_if<!is_convertible<T, XmlRpcValue>::value, void>::type FromXmlRpcValue(T& Object, XmlRpcValue&& Value)
	{
		try
		{
			CXmlRpcFieldSerializer visitor(Value, false);
			Object.Accept(visitor);
		}
		catch (CVisitException& e)
		{
			throw CBugzillaException(stringA(e.what()));
		}
	}

	static XmlRpcValue Execute(CServer& Server, const char* FunctionName, XmlRpcValue& Param, bool bRequireLogin = false)
	{
		return Server.CallRemoteFunction(FunctionName, Param, bRequireLogin);
	}

	static void Execute(CServer& Server, const char* FunctionName, bool bRequireLogin = false)
	{
		XmlRpcValue emptyParam;
		Execute(Server, FunctionName, emptyParam);
	}

	template<typename TResult>
	static TResult Execute(CServer& Server, const char* FunctionName, bool bRequireLogin = false)
	{
		XmlRpcValue emptyParam;
		XmlRpcValue result = Execute(Server, FunctionName, emptyParam, bRequireLogin);
		CXmlRpcFieldSerializer resultSerializer(result, false);
		TResult object;
		object.Accept(resultSerializer);
		return object;
	}

	template<typename TParam>
	static void Execute(CServer& Server, const char* FunctionName, TParam& Param, bool bRequireLogin = false)
	{
		XmlRpcValue param = ToXmlRpcValue(Param);
		Execute(Server, FunctionName, param, bRequireLogin);
	}

	template<typename TParam, typename TResult>
	static void Execute(CServer& Server, const char* FunctionName, TParam& Param, TResult& Result, bool bRequireLogin = false)
	{
		XmlRpcValue param = ToXmlRpcValue(Param);
		XmlRpcValue result = Execute(Server, FunctionName, param, bRequireLogin);
		FromXmlRpcValue(Result, move(result));
	}

	template<typename TParam>
	static void Execute(CServer& Server, const char* FunctionName, TParam& Param, CBugInfos& Result)
	{
		XmlRpcValue param = ToXmlRpcValue(Param);
		XmlRpcValue result = Execute(Server, FunctionName, param);
		XmlRpcValue::ValueArray& bugs = result["bugs"];
		Result.Objects.Container.reserve(bugs.size());
		for (int index = 0; index < bugs.size(); ++index)
			Result.Objects.Container.emplace_back(move(bugs[index]));
	}

	CProduct::CProduct(CServer& Server, const CProductInfo& ProductInfo)
		: m_Server(Server),
		m_ProductInfo(ProductInfo)
	{
	}

	CProduct::operator bool() const
	{
		return m_ProductInfo.ObjectID.ID && (m_ProductInfo.Name.length() > 0);
	}

	int CProduct::CreateBug(const string& Component, const string& Summary, const string& Version, const string& Assignee)
	{
		CCreateBugParam param;
		param.CommonParameters.Product = m_ProductInfo.Name;
		param.CommonParameters.Component = Component;
		param.CommonParameters.Summary = Summary;
		param.CommonParameters.Version = Version;
		param.CommonParameters.AssignedTo = Assignee;
		return m_Server.CreateBug(param);
	}

	CBug::CBug(CServer& Server, const CBugInfo& BugInfo)
		: m_Server(Server),
		m_BugInfo(BugInfo)
	{
	}

	CBugzillaID CBug::AppendComment(const string& Comment, double Worktime, bool bPrivate)
	{
		CBugzillaID commentID;
		CBugAddCommentParam param;
		param.BugID = m_BugInfo.BugInfo["id"];
		param.Comment.Text = Comment;
		param.Comment.bPrivate = bPrivate;
		param.WorkTime = Worktime;
		Execute(m_Server, "Bug.add_comment", param, commentID);
		return commentID;
	}

	void CBug::Update(const CUpdateBugsParam& UpdateBugsParam)
	{
		stringA sResult;
		auto param = UpdateBugsParam;
		Execute(m_Server, "Bug.update", param, true);
	}

	CServer::CServer(const string& BugzillaURI, bool bInitialize)
		: m_Connection(BugzillaURI.c_str())
	{
		if (bInitialize)
			Initialize();
	}

	CServer::~CServer()
	{
		try
		{
			if (IsLoggedIn())
				Logout();
		}
		catch (CBugzillaException Exception)
		{
		}
	}

	string CServer::GetVersionRpc()
	{
		CGetVersionInfo info;
		Execute(*this, "Bugzilla.version", info);
		return info.Version;
	}

	bool CServer::GetTimeRpc(tm& BugzillaServerTime, tm& DatabaseServerTime)
	{
		bool bSuccess = false;
		CGetTimeInfo info;
		try
		{
			Execute(*this, "Bugzilla.time", info);
			BugzillaServerTime = info.WebTime;
			DatabaseServerTime = info.DatabaseTime;
			bSuccess = true;
		}
		catch (CBugzillaException Exception)
		{
		}
		return bSuccess;
	}

	void CServer::XmlRpcCallback(void* context, char* status)
	{
		CServer* pServer = reinterpret_cast<CServer*>(context);
		if (pServer && pServer->Callback)
			pServer->Callback(status);
	}

	XmlRpcValue CServer::CallRemoteFunction(const char* pFunctionName, XmlRpcValue& Param, bool bRequireLogin)
	{
		XmlRpcValue result;
		if (bRequireLogin)
		{
			if (!IsLoggedIn())
				throw CBugzillaException(string("not logged in"));
			Param["token"] = m_Login.Token;
		}
		if (!m_Connection.execute(pFunctionName, Param, result))
		{
			CBugzillaException exception;
			if (result.valid())
			{
				CXmlRpcFieldSerializer serializer(result, false);
				exception.Accept(serializer);
			}
			else
				exception.ErrorMessage = m_Connection.getError();
			throw exception;
		}
		return result;
	}

	bool CServer::Initialize()
	{
		bool bInitialized = true;
		try
		{
			m_Connection.setCallback(&XmlRpcCallback, this);
			m_Version = GetVersionRpc();
		}
		catch (CBugzillaException Exception)
		{
			bInitialized = false;
		}
		return bInitialized;
	}

	string CServer::GetVersion() const
	{
		return m_Version;
	}

	bool CServer::GetTime(time_t& BugzillaServerTime, time_t& DatabaseServerTime) const
	{
		bool bValid = m_BuildTime != time_t();
		if (bValid)
		{
			time_t now = time(nullptr);
			auto span = now - m_BuildTime;
			BugzillaServerTime = m_Time + span;
			DatabaseServerTime = m_DatabaseTime + span;
		}
		return bValid;
	}

	bool CServer::IsLoggedIn() const
	{
		return m_Login.UserID ? true : false;
	}

	void CServer::Login(const string& Username, const string& Password)
	{
		if (m_Login.UserID)
			Logout();
		CLoginParam loginParam;
		loginParam.Username = Username;
		loginParam.Password = Password;
		Execute(*this, "User.login", loginParam, m_Login);
	}

	void CServer::Logout()
	{
		Execute(*this, "User.logout");
		m_Login = CActiveLogin();
	}

	CBugInfo CServer::GetBug(int BugID)
	{
		CBugInfo bugInfo;
		CGetBugParam param;
		param.BugIDs.Container.push_back(BugID);
		Execute(*this, "Bug.get", param, bugInfo);
		return bugInfo;
	}

	CBugInfos CServer::GetBugs(const CBugSearchParam& BugSearchParam)
	{
		CBugInfos bugInfos;
		auto param = BugSearchParam;
		Execute(*this, "Bug.search", param, bugInfos);
		return bugInfos;
	}

	CBugFields CServer::GetBugFields(const CGetBugFieldParam& BugFieldParam)
	{
		auto param = BugFieldParam;
		CBugFields fields;
		Execute(*this, "Bug.fields", param, fields);
		return fields;
	}

	int CServer::CreateBug(const CCreateBugParam& CreateBugParam)
	{
		auto createParam = CreateBugParam;
		CBugzillaID createdBugID;
		Execute(*this, "Bug.create", createParam, createdBugID);
		return createdBugID.ID;
	}

	EXPERIMENTAL(CProductIDs CServer::GetSelectableProductIDs()
	{
		CProductIDs productIDs;
		Execute(*this, "Product.get_selectable_products", productIDs);
		return productIDs;
	})

	EXPERIMENTAL(CProductIDs CServer::GetEnterableProductIDs()
	{
		CProductIDs productIDs;
		Execute(*this, "Product.get_enterable_products", productIDs);
		return productIDs;
	})

	EXPERIMENTAL(CProductInfos CServer::GetProducts(CProductIDs ProductIDs)
	{
		CProductInfos infos;
		Execute(*this, "Product.get", ProductIDs, infos);
		return infos;
	})

	CUserInfos CServer::GetUsers(const string& Match)
	{
		CUserInfos userInfos;
		CUserSearchParam param;
		param.Match.Container.push_back(Match);
		Execute(*this, "User.get", param, userInfos);
		return userInfos;
	}
}