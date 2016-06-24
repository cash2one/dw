/// cache_auto_assistant.cpp
/// Generate by DB CACHE tools, PLEASE DO NOT EDIT IT!

#include "cache_auto_assistant.h"

namespace dbass {

bool Assaccount::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO account SET ";
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
        query << ",";
        query << "sum_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.sum_id());
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
} /// Assaccount::insert

bool Assaccount::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,user_name,batch_num,language_type,login_type,channel_type,create_tim,sum_id FROM account WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find account<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_user_name(res.at(0).at(1));
        obj_.set_batch_num(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
        obj_.set_language_type(::boost::lexical_cast< ::boost::int32_t >(res.at(0).at(3)));
        obj_.set_login_type(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(4)));
        obj_.set_channel_type(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(5)));
        obj_.set_create_tim(res.at(0).at(6));
        obj_.set_sum_id(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(7)));
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
} /// Assaccount::load

bool Assaccount::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE account SET ";
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
        query << ",";
        query << "sum_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.sum_id());
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
} /// Assaccount::update

bool Assaccount::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM account WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
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
} /// Assaccount::remove

bool Assrole_server::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO role_server SET ";
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
} /// Assrole_server::insert

bool Assrole_server::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,area_id,gs_id,user_id,first_enter_time,last_enter_time FROM role_server WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find role_server<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
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
} /// Assrole_server::load

bool Assrole_server::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE role_server SET ";
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
} /// Assrole_server::update

bool Assrole_server::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM role_server WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
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
} /// Assrole_server::remove

bool Asssummoner::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO summoner SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "account_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.account_id());
        query << ",";
        query << "sum_name=" << ::mysqlpp::quote << this->obj_.sum_name();
        query << ",";
        query << "match_status=" << ::boost::lexical_cast< ::std::string >(this->obj_.match_status());
        query << ",";
        query << "gate_guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.gate_guid());
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
} /// Asssummoner::insert

bool Asssummoner::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,account_id,sum_name,match_status,gate_guid FROM summoner WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find summoner<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_account_id(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(1)));
        obj_.set_sum_name(res.at(0).at(2));
        obj_.set_match_status(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(3)));
        obj_.set_gate_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(4)));
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
} /// Asssummoner::load

bool Asssummoner::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE summoner SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "account_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.account_id());
        query << ",";
        query << "sum_name=" << ::mysqlpp::quote << this->obj_.sum_name();
        query << ",";
        query << "match_status=" << ::boost::lexical_cast< ::std::string >(this->obj_.match_status());
        query << ",";
        query << "gate_guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.gate_guid());
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
} /// Asssummoner::update

bool Asssummoner::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM summoner WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
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
} /// Asssummoner::remove

bool Assfriend_player_info::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO friend_player_info SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "player_name=" << ::mysqlpp::quote << this->obj_.player_name();
        query << ",";
        query << "vr_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.vr_id());
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
} /// Assfriend_player_info::insert

bool Assfriend_player_info::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,player_name,vr_id FROM friend_player_info WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find friend_player_info<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        obj_.set_player_name(res.at(0).at(1));
        obj_.set_vr_id(::boost::lexical_cast< ::boost::uint32_t >(res.at(0).at(2)));
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
} /// Assfriend_player_info::load

bool Assfriend_player_info::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE friend_player_info SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        query << "player_name=" << ::mysqlpp::quote << this->obj_.player_name();
        query << ",";
        query << "vr_id=" << ::boost::lexical_cast< ::std::string >(this->obj_.vr_id());
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
} /// Assfriend_player_info::update

bool Assfriend_player_info::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM friend_player_info WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
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
} /// Assfriend_player_info::remove

bool Assfriend_player_relation::insert(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "INSERT INTO friend_player_relation SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.apply_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.apply_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "apply_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.black_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.black_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "black_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.friend_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.friend_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "friend_list=" << ::mysqlpp::quote << ssm.str();
        }
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
} /// Assfriend_player_relation::insert

bool Assfriend_player_relation::load(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "SELECT guid,apply_list,black_list,friend_list FROM friend_player_relation WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        const ::mysqlpp::StoreQueryResult& res = query.store();
        if (res.size() == 0) {
             er_code_ = CAE_NOT_FOUND;
             er_str_  = "Cannot find friend_player_relation<<" + ::boost::lexical_cast< ::std::string >(this->obj_.guid()) + ">>";
             return false;
        }
        obj_.set_guid(::boost::lexical_cast< ::boost::uint64_t >(res.at(0).at(0)));
        if (!res.at(0).at(1).is_null()) {
            string temp(res.at(0).at(1).data(), res.at(0).at(1).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_apply_list(v);
            }
	    }
        if (!res.at(0).at(2).is_null()) {
            string temp(res.at(0).at(2).data(), res.at(0).at(2).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_black_list(v);
            }
	    }
        if (!res.at(0).at(3).is_null()) {
            string temp(res.at(0).at(3).data(), res.at(0).at(3).length());
            stringstream ssm(temp);
            ::boost::uint32_t size = 0;
            ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            ::boost::uint64_t v;
            for (::boost::uint32_t i = 0; i < size; i++) {
                ssm.read(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
                this->obj_.add_friend_list(v);
            }
	    }
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
} /// Assfriend_player_relation::load

bool Assfriend_player_relation::update(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query.clear();
        query << "UPDATE friend_player_relation SET ";
        query << "guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.apply_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.apply_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "apply_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.black_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.black_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "black_list=" << ::mysqlpp::quote << ssm.str();
        }
        query << ",";
        {
            ::boost::uint32_t size = this->obj_.friend_list_size();
            stringstream ssm;
            ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
            for (::boost::uint32_t i = 0; i < size; i++) {
                ::boost::uint64_t v = this->obj_.friend_list(i);
                ssm.write(reinterpret_cast<char*>(&v), sizeof(::boost::uint64_t));
            }
            query << "friend_list=" << ::mysqlpp::quote << ssm.str();
        }
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
} /// Assfriend_player_relation::update

bool Assfriend_player_relation::remove(::mysqlpp::Query& query) {
    using namespace ::std;
    try {
        query << "DELETE FROM friend_player_relation WHERE guid=" << ::boost::lexical_cast< ::std::string >(this->obj_.guid());
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
} /// Assfriend_player_relation::remove

} /// namespace dbass
