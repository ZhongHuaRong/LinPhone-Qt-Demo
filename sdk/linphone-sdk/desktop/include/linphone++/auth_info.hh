/*
Copyright (C) 2017 Belledonne Communications SARL

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _LINPHONE_AUTH_INFO_HH
#define _LINPHONE_AUTH_INFO_HH

#include <string>
#include <list>
#include "object.hh"




struct _LinphoneAuthInfo;


namespace linphone {

	class AuthInfo;

	/**
	 * @brief Object holding authentication information. 
	 *
	 * In most case, authentication information consists of a username and password.
	 * If realm isn't set, it will be deduced automatically from the first
	 * authentication challenge as for the hash algorithm. Sometimes, a userid is
	 * required by the proxy and then domain can be useful to discriminate different
	 * credentials. You can also use this object if you need to use a client
	 * certificate.
	 * Once created and filled, a @ref AuthInfo must be added to the @ref Core in
	 * order to become known and used automatically when needed. Use
	 * Core::addAuthInfo() for that purpose.
	 * The @ref Core object can take the initiative to request authentication
	 * information when needed to the application through the
	 * authentication_requested() callback of it's LinphoneCoreCbs.
	 * The application can respond to this information request later using
	 * Core::addAuthInfo(). This will unblock all pending authentication transactions
	 * and retry them with authentication headers. 
	 */
	class AuthInfo: public Object {
	
		public:

			AuthInfo(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneAuthInfo *cPtr() {return (_LinphoneAuthInfo *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Gets the algorithm. 
			 *
			 * @return The algorithm.   
			 */
			LINPHONECXX_PUBLIC std::string getAlgorithm() const;
			
			/**
			 * @brief Sets the algorithm to use. 
			 *
			 * @param algorithm The algorithm.   
			 */
			LINPHONECXX_PUBLIC void setAlgorithm(const std::string & algorithm);
			
			/**
			 * @brief Gets all available algorithms. 
			 *
			 * @return A list of available algorithms.    
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getAvailableAlgorithms() const;
			
			/**
			 * @brief Sets the available algorithms list without testing unicity. 
			 *
			 * @param algorithms The available algorithms list.    
			 */
			LINPHONECXX_PUBLIC void setAvailableAlgorithms(const std::list<std::string> & algorithms);
			
			/**
			 * @brief Gets the domain. 
			 *
			 * @return The domain.   
			 */
			LINPHONECXX_PUBLIC std::string getDomain() const;
			
			/**
			 * @brief Sets the domain for which this authentication is valid. 
			 *
			 * @param domain The domain.   This should not be necessary because realm is
			 * supposed to be unique and sufficient. However, many SIP servers don't set realm
			 * correctly, then domain has to be used to distinguish between several SIP
			 * account bearing the same username. 
			 */
			LINPHONECXX_PUBLIC void setDomain(const std::string & domain);
			
			/**
			 * @brief Gets the ha1. 
			 *
			 * @return The ha1.   
			 */
			LINPHONECXX_PUBLIC std::string getHa1() const;
			
			/**
			 * @brief Sets the ha1. 
			 *
			 * @param ha1 The ha1.   
			 */
			LINPHONECXX_PUBLIC void setHa1(const std::string & ha1);
			
			/**
			 * @brief Gets the password. 
			 *
			 * @return The password.   
			 */
			LINPHONECXX_PUBLIC std::string getPassword() const;
			
			/**
			 * @brief Sets the password. 
			 *
			 * @param password The password.   
			 */
			LINPHONECXX_PUBLIC void setPassword(const std::string & password);
			
			/**
			 * @brief Gets the realm. 
			 *
			 * @return The realm.   
			 */
			LINPHONECXX_PUBLIC std::string getRealm() const;
			
			/**
			 * @brief Sets the realm. 
			 *
			 * @param realm The realm.   
			 */
			LINPHONECXX_PUBLIC void setRealm(const std::string & realm);
			
			/**
			 * @brief Gets the TLS certificate. 
			 *
			 * @return The TLS certificate.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsCert() const;
			
			/**
			 * @brief Sets the TLS certificate. 
			 *
			 * @param tlsCert The TLS certificate.   
			 */
			LINPHONECXX_PUBLIC void setTlsCert(const std::string & tlsCert);
			
			/**
			 * @brief Gets the TLS certificate path. 
			 *
			 * @return The TLS certificate path.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsCertPath() const;
			
			/**
			 * @brief Sets the TLS certificate path. 
			 *
			 * @param tlsCertPath The TLS certificate path.   
			 */
			LINPHONECXX_PUBLIC void setTlsCertPath(const std::string & tlsCertPath);
			
			/**
			 * @brief Gets the TLS key. 
			 *
			 * @return The TLS key.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsKey() const;
			
			/**
			 * @brief Sets the TLS key. 
			 *
			 * @param tlsKey The TLS key.   
			 */
			LINPHONECXX_PUBLIC void setTlsKey(const std::string & tlsKey);
			
			/**
			 * @brief Gets the TLS key path. 
			 *
			 * @return The TLS key path.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsKeyPath() const;
			
			/**
			 * @brief Sets the TLS key path. 
			 *
			 * @param tlsKeyPath The TLS key path.   
			 */
			LINPHONECXX_PUBLIC void setTlsKeyPath(const std::string & tlsKeyPath);
			
			/**
			 * @brief Gets the user id. 
			 *
			 * @return The user id.   
			 */
			LINPHONECXX_PUBLIC std::string getUserid() const;
			
			/**
			 * @brief Sets the user ID. 
			 *
			 * @param userId The userid.   
			 */
			LINPHONECXX_PUBLIC void setUserid(const std::string & userId);
			
			/**
			 * @brief Gets the username. 
			 *
			 * @return The username.   
			 */
			LINPHONECXX_PUBLIC std::string getUsername() const;
			
			/**
			 * @brief Sets the username. 
			 *
			 * @param username The username.   
			 */
			LINPHONECXX_PUBLIC void setUsername(const std::string & username);
			
			/**
			 * @brief Add an unique algorithm in the the available algorithms list :
			 * Algorithms that already exist will not be added. 
			 *
			 * @param algorithm The algorithm to add.   
			 */
			LINPHONECXX_PUBLIC void addAvailableAlgorithm(const std::string & algorithm);
			
			/**
			 * @brief Remove all algorithms from the available algorithms list. 
			 *
			 */
			LINPHONECXX_PUBLIC void clearAvailableAlgorithms();
			
			/**
			 * @brief Instantiates a new auth info with values from source. 
			 *
			 * @return The newly created @ref AuthInfo object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::AuthInfo> clone() const;
			
			/**
			 * @brief Check if Authinfos are the same without taking account algorithms. 
			 *
			 * @param authInfo2 The second @ref AuthInfo object.   
			 * @return true if all fields (Username, UserId, Realm, Domain) are the same. 
			 */
			LINPHONECXX_PUBLIC bool isEqualButAlgorithms(const std::shared_ptr<const linphone::AuthInfo> & authInfo2) const;
			
			
	};

};

#endif // _LINPHONE_AUTH_INFO_HH
