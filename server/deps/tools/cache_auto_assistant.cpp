/// cache_auto_assistant.cpp
/// Generate by DB CACHE tools, PLEASE DO NOT EDIT IT!

#include "cache_auto_assistant.h"

namespace dbass {

bool AssAccount::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO Account SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "user_name=" << ::mysqlpp::quote << this->obj_.user_name();
        query << ",";
        query << "batch_num=" << ::boost::lexical_cast< ::std::string >(this->obj_.batch_num());
        query << ",";
        query << "language_type=" << ::boost::lexical_cast< ::std::string >(this->obj_.language_type());
        query << ",";
        query << "login_type=" << ::boost::lexical_cast< ::std::string >(this->obj_.login_type());
        query << ",";
        query << "channel_type=" << ::boost::lexical_cast< ::std::string >(this->obj_.channel_type());
        query << ",";
        query << "create_tim=" << ::mysqlpp::quote << this->obj_.create_tim();
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssAccount::insert

bool AssAccount::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,user_name,batch_num,language_type,login_type,channel_type,create_tim FROM Account WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find Account<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_user_name(res.at(0).at(1));
        obj_.set_batch_num(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_language_type(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(3)));
        obj_.set_login_type(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(4)));
        obj_.set_channel_type(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(5)));
        obj_.set_create_tim(res.at(0).at(6));
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssAccount::load

bool AssAccount::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE Account SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "user_name=" << ::mysqlpp::quote << this->obj_.user_name();
        query << ",";
        query << "batch_num=" << ::boost::lexical_cast< ::std::string >(this->obj_.batch_num());
        query << ",";
        query << "language_type=" << ::boost::lexical_cast< ::std::string >(this->obj_.language_type());
        query << ",";
        query << "login_type=" << ::boost::lexical_cast< ::std::string >(this->obj_.login_type());
        query << ",";
        query << "channel_type=" << ::boost::lexical_cast< ::std::string >(this->obj_.channel_type());
        query << ",";
        query << "create_tim=" << ::mysqlpp::quote << this->obj_.create_tim();
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssAccount::update

bool AssAccount::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM Account WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssAccount::remove

bool AssRoleServer::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO RoleServer SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "area_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.area_id());
        query << ",";
        query << "gs_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.gs_id());
        query << ",";
        query << "user_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.user_id());
        query << ",";
        query << "first_enter_time=" << ::mysqlpp::quote << this->obj_.first_enter_time();
        query << ",";
        query << "last_enter_time=" << ::mysqlpp::quote << this->obj_.last_enter_time();
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        if (ER_DUP_ENTRY == er.errnum())
            er_code_ = CAE_DUP_KEY;
        else
            er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRoleServer::insert

bool AssRoleServer::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,area_id,gs_id,user_id,first_enter_time,last_enter_time FROM RoleServer WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find RoleServer<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_area_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(1)));
        obj_.set_gs_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_user_id(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(3)));
        obj_.set_first_enter_time(res.at(0).at(4));
        obj_.set_last_enter_time(res.at(0).at(5));
        return true;
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRoleServer::load

bool AssRoleServer::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE RoleServer SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "area_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.area_id());
        query << ",";
        query << "gs_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.gs_id());
        query << ",";
        query << "user_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.user_id());
        query << ",";
        query << "first_enter_time=" << ::mysqlpp::quote << this->obj_.first_enter_time();
        query << ",";
        query << "last_enter_time=" << ::mysqlpp::quote << this->obj_.last_enter_time();
        query << " WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRoleServer::update

bool AssRoleServer::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM RoleServer WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        return query.exec();
    }
    catch (const ::mysqlpp::BadQuery& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
    catch (const ::mysqlpp::Exception& er) {
        er_code_ = CAE_INTERNAL;
        er_str_  = er.what();
        return false;
    }
} /// AssRoleServer::remove

} /// namespace dbass
