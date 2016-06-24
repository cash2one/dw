/// cache_auto_assistant.h
/// Generate by DB CACHE tools, PLEASE DO NOT EDIT IT!

#ifndef __CACHE_AUTO_ASSISTANT__
#define __CACHE_AUTO_ASSISTANT__

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include <strstream>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include "mysql++.h"
#include "mysqld_error.h"
#include "google/protobuf/message.h"

#include "account.pb.h"

namespace dbass {

class CacheAssistant {
public:
    enum CACHE_ASSISTANT_ERROR {
        CAE_UNKNOWN = 0,
        CAE_NOT_FOUND,
        CAE_DUP_KEY,
        CAE_CANNOT_DEL,
        CAE_INTERNAL
    };
	
	virtual ~CacheAssistant()
	{}
    ///
    virtual ::google::protobuf::Message* data(void) = 0;
    virtual bool load(::mysqlpp::Query& query) = 0;
    virtual bool update(::mysqlpp::Query& query) = 0;
    virtual bool insert(::mysqlpp::Query& query) = 0;
    virtual bool remove(::mysqlpp::Query& query) = 0;
    size_t length(void) { return(data() ? data()->ByteSize() : 0); }
    CACHE_ASSISTANT_ERROR last_error(void) { return er_code_; }
    ::std::string what(void) { return er_str_; }
    bool is_temp(void) { return is_temp_; }
  
protected:
    CACHE_ASSISTANT_ERROR er_code_;
    ::std::string er_str_;
    bool is_temp_;
}; /// class CacheAssistant
typedef ::std::auto_ptr<CacheAssistant> CacheAssistantSPtr;

class Assaccount : public CacheAssistant {
public:
    Assaccount(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::account obj_;
}; /// class Assaccount

class Assrole_server : public CacheAssistant {
public:
    Assrole_server(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::role_server obj_;
}; /// class Assrole_server

class Asssummoner : public CacheAssistant {
public:
    Asssummoner(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::summoner obj_;
}; /// class Asssummoner

class Assfriend_player_info : public CacheAssistant {
public:
    Assfriend_player_info(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::friend_player_info obj_;
}; /// class Assfriend_player_info

class Assfriend_player_relation : public CacheAssistant {
public:
    Assfriend_player_relation(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::friend_player_relation obj_;
}; /// class Assfriend_player_relation

} /// namespace dbass

#endif /// __CACHE_AUTO_ASSISTANT__
