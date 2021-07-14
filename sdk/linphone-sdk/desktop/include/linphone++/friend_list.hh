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

#ifndef _LINPHONE_FRIEND_LIST_HH
#define _LINPHONE_FRIEND_LIST_HH

#include <string>
#include <list>
#include "friend_list.hh"
#include "object.hh"




struct _LinphoneFriendList;


namespace linphone {

	class Core;
	class Friend;
	class Address;
	class PresenceModel;
	class FriendListListener;

	/**
	 * @brief This object representing a list of @ref Friend. 
	 *
	 * You can use it to store contacts locally or synchronize them through CardDAV
	 * protocol. 
	 */
	class FriendList: public MultiListenableObject {
	
		public:
			/**
			* Enum describing the status of a CardDAV synchronization. 
			*
			*/
			enum class SyncStatus {
				/**
				* Synchronization started. 
				*/
				Started,
				/**
				* Synchronization finished successfuly. 
				*/
				Successful,
				/**
				* Synchronization failed. 
				*/
				Failure
			};

			/**
			* Enum describing the status of a LinphoneFriendList operation. 
			*
			*/
			enum class Status {
				/**
				* Operation went fine. 
				*/
				OK,
				/**
				* @ref Friend wasn't found in the @ref FriendList 
				*/
				NonExistentFriend,
				/**
				* @ref Friend is already present in a @ref FriendList 
				*/
				InvalidFriend
			};


			FriendList(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneFriendList *cPtr() {return (_LinphoneFriendList *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<FriendListListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<FriendListListener> &listener);
	
		public:
			
		
			/**
			 * @brief Returns the @ref Core object attached to this LinphoneFriendList. 
			 *
			 * @return a @ref Core object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> getCore() const;
			
			/**
			 * @brief Get the display name of the friend list. 
			 *
			 * @return The display name of the friend list.   
			 */
			LINPHONECXX_PUBLIC std::string getDisplayName() const;
			
			/**
			 * @brief Set the display name of the friend list. 
			 *
			 * @param displayName The new display name of the friend list.   
			 */
			LINPHONECXX_PUBLIC void setDisplayName(const std::string & displayName);
			
			/**
			 * @brief Retrieves the list of @ref Friend from this LinphoneFriendList. 
			 *
			 * @return A list of @ref Friend    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Friend>> getFriends() const;
			
			/**
			 * @brief Get wheter the subscription of the friend list is bodyless or not. 
			 *
			 * @return Wheter the subscription of the friend list is bodyless or not. 
			 */
			LINPHONECXX_PUBLIC bool isSubscriptionBodyless();
			
			/**
			 * @brief Get the RLS (Resource List Server) URI associated with the friend list
			 * to subscribe to these friends presence. 
			 *
			 * @return The RLS URI as @ref Address associated with the friend list.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getRlsAddress() const;
			
			/**
			 * @brief Set the RLS (Resource List Server) URI associated with the friend list
			 * to subscribe to these friends presence. 
			 *
			 * @param rlsAddr The RLS URI to associate with the friend list.   
			 */
			LINPHONECXX_PUBLIC void setRlsAddress(const std::shared_ptr<const linphone::Address> & rlsAddr);
			
			/**
			 * @brief Get the RLS (Resource List Server) URI associated with the friend list
			 * to subscribe to these friends presence. 
			 *
			 * @return The RLS URI associated with the friend list.   
			 * @deprecated 27/10/2020. Use getRlsAddress() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::string getRlsUri() const;
			
			/**
			 * @brief Set the RLS (Resource List Server) URI associated with the friend list
			 * to subscribe to these friends presence. 
			 *
			 * @param rlsUri The RLS URI to associate with the friend list.   
			 * @deprecated 27/10/2020. Use setRlsAddress() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED void setRlsUri(const std::string & rlsUri);
			
			/**
			 * @brief Set wheter the subscription of the friend list is bodyless or not. 
			 *
			 * @param bodyless boolean telling if the subscription of the friend list is
			 * bodyless or not. 
			 */
			LINPHONECXX_PUBLIC void setSubscriptionBodyless(bool bodyless);
			
			/**
			 * @brief Gets whether subscription to NOTIFYes of all friends list are enabled or
			 * not. 
			 *
			 * @return Whether subscriptions are enabled or not 
			 */
			LINPHONECXX_PUBLIC bool subscriptionsEnabled();
			
			/**
			 * @brief Enable subscription to NOTIFYes of all friends list. 
			 *
			 * @param enabled should subscription be enabled or not 
			 */
			LINPHONECXX_PUBLIC void enableSubscriptions(bool enabled);
			
			/**
			 * @brief Get the URI associated with the friend list. 
			 *
			 * @return The URI associated with the friend list.   
			 */
			LINPHONECXX_PUBLIC std::string getUri() const;
			
			/**
			 * @brief Set the URI associated with the friend list. 
			 *
			 * @param uri The URI to associate with the friend list.   
			 */
			LINPHONECXX_PUBLIC void setUri(const std::string & uri);
			
			/**
			 * @brief Add a friend to a friend list. 
			 *
			 * If or when a remote CardDAV server will be attached to the list, the friend
			 * will be sent to the server. 
			 * @param linphoneFriend @ref Friend object to add to the friend list.   
			 * @return @ref Status::OK if successfully added, @ref Status::InvalidFriend if
			 * the friend is not valid. 
			 */
			LINPHONECXX_PUBLIC Status addFriend(const std::shared_ptr<linphone::Friend> & linphoneFriend);
			
			/**
			 * @brief Add a friend to a friend list. 
			 *
			 * The friend will never be sent to a remote CardDAV server. Warning!
			 * #LinphoneFriends added this way will be removed on the next synchronization,
			 * and the callback contact_deleted will be called. 
			 * @param linphoneFriend @ref Friend object to add to the friend list.   
			 * @return @ref Status::OK if successfully added, @ref Status::InvalidFriend if
			 * the friend is not valid. 
			 */
			LINPHONECXX_PUBLIC Status addLocalFriend(const std::shared_ptr<linphone::Friend> & linphoneFriend);
			
			/**
			 * @brief Creates and export @ref Friend objects from @ref FriendList to a file
			 * using vCard 4 format. 
			 *
			 * @param vcardFile the path to a file that will contain the vCards   
			 */
			LINPHONECXX_PUBLIC void exportFriendsAsVcard4File(const std::string & vcardFile);
			
			/**
			 * @brief Find a friend in the friend list using a LinphoneAddress. 
			 *
			 * @param address @ref Address object of the friend we want to search for.   
			 * @return A @ref Friend if found, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> findFriendByAddress(const std::shared_ptr<const linphone::Address> & address) const;
			
			/**
			 * @brief Find a friend in the friend list using a phone number. 
			 *
			 * @param phoneNumber a string of the phone number for which we want to find a
			 * friend.   
			 * @return A @ref Friend if found, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> findFriendByPhoneNumber(const std::string & phoneNumber) const;
			
			/**
			 * @brief Find a friend in the friend list using a ref key. 
			 *
			 * @param refKey The ref key string of the friend we want to search for.   
			 * @return A @ref Friend if found, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> findFriendByRefKey(const std::string & refKey) const;
			
			/**
			 * @brief Find a friend in the friend list using an URI string. 
			 *
			 * @param uri A string containing the URI of the friend we want to search for.   
			 * @return A @ref Friend if found, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> findFriendByUri(const std::string & uri) const;
			
			/**
			 * @brief Find all friends in the friend list using a LinphoneAddress. 
			 *
			 * @param address @ref Address object of the friends we want to search for.   
			 * @return A list of @ref Friend if found, nullptr otherwise.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Friend>> findFriendsByAddress(const std::shared_ptr<const linphone::Address> & address) const;
			
			/**
			 * @brief Find all friends in the friend list using an URI string. 
			 *
			 * @param uri A string containing the URI of the friends we want to search for.   
			 * @return A list of @ref Friend if found, nullptr otherwise.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Friend>> findFriendsByUri(const std::string & uri) const;
			
			/**
			 * @brief Creates and adds @ref Friend objects to @ref FriendList from a buffer
			 * that contains the vCard(s) to parse. 
			 *
			 * @param vcardBuffer the buffer that contains the vCard(s) to parse   
			 * @return the amount of linphone friends created 
			 */
			LINPHONECXX_PUBLIC int importFriendsFromVcard4Buffer(const std::string & vcardBuffer);
			
			/**
			 * @brief Creates and adds @ref Friend objects to @ref FriendList from a file that
			 * contains the vCard(s) to parse. 
			 *
			 * @param vcardFile the path to a file that contains the vCard(s) to parse   
			 * @return the amount of linphone friends created 
			 */
			LINPHONECXX_PUBLIC int importFriendsFromVcard4File(const std::string & vcardFile);
			
			/**
			 * @brief Notify our presence to all the friends in the friend list that have
			 * subscribed to our presence directly (not using a RLS). 
			 *
			 * @param presence @ref PresenceModel object.   
			 */
			LINPHONECXX_PUBLIC void notifyPresence(const std::shared_ptr<linphone::PresenceModel> & presence);
			
			/**
			 * @brief Remove a friend from a friend list. 
			 *
			 * @param linphoneFriend @ref Friend object to remove from the friend list.   
			 * @return @ref Status::OK if removed successfully, @ref Status::NonExistentFriend
			 * if the friend is not in the list. 
			 */
			LINPHONECXX_PUBLIC Status removeFriend(const std::shared_ptr<linphone::Friend> & linphoneFriend);
			
			/**
			 * @brief Starts a CardDAV synchronization using value set using
			 * linphone_friend_list_set_uri. 
			 *
			 */
			LINPHONECXX_PUBLIC void synchronizeFriendsFromServer();
			
			/**
			 * @brief Goes through all the @ref Friend that are dirty and does a CardDAV PUT
			 * to update the server. 
			 *
			 */
			LINPHONECXX_PUBLIC void updateDirtyFriends();
			
			/**
			 * @brief Sets the revision from the last synchronization. 
			 *
			 * @param revision The revision 
			 */
			LINPHONECXX_PUBLIC void updateRevision(int revision);
			
			/**
			 * @brief Update presence subscriptions for the entire list. 
			 *
			 * Calling this function is necessary when list subscriptions are enabled, ie when
			 * a RLS presence server is used. 
			 */
			LINPHONECXX_PUBLIC void updateSubscriptions();
			
			
		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_FRIEND_LIST_HH
