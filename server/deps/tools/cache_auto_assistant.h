/// cache_auto_assistant.h
/// Generate by DB CACHE tools, PLEASE DO NOT EDIT IT!

#ifndef __CACHE_AUTO_ASSISTANT__
#define __CACHE_AUTO_ASSISTANT__

#include <strstream>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include "mysql++.h"
#include "mysqld_error.h"
#include "google/protobuf/message.h"

#include "entity/Account.pb.h"

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

class AssAccount : public CacheAssistant {
public:
    AssAccount(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::Account obj_;
}; /// class AssAccount

class AssRoleServer : public CacheAssistant {
public:
    AssRoleServer(::boost::int64_t guid = 0) {
        obj_.set_guid(guid);
        is_temp_ = false;
    }
    virtual ::google::protobuf::Message* data(void) { return &obj_; }
    virtual bool load(::mysqlpp::Query& query);
    virtual bool update(::mysqlpp::Query& query);
    virtual bool insert(::mysqlpp::Query& query);
    virtual bool remove(::mysqlpp::Query& query);
private:
    Entity::RoleServer obj_;
}; /// class AssRoleServer

} /// namespace dbass

#endif /// __CACHE_AUTO_ASSISTANT__
