//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Item represents an existing item.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////


#ifndef __OTSERV_ITEM_H__
#define __OTSERV_ITEM_H__

#include "thing.h"
#include "items.h"

#include <iostream>
#include <list>
#include <vector>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

class Creature;
class Player;
class Container;
class Depot;
class Teleport;
class TrashHolder;
class Mailbox;
class Door;
class MagicField;

enum ITEMPROPERTY{
	BLOCKSOLID,
	HASHEIGHT,
	BLOCKPROJECTILE,
	BLOCKPATHFIND,
	PROTECTIONZONE,
	ISVERTICAL,
	ISHORIZONTAL,
};

enum TradeEvents_t{
	ON_TRADE_TRANSFER,
	ON_TRADE_CANCEL,
};

struct LightInfo{
	long level;
	long color;
	LightInfo(){
		level = 0;
		color = 0;
	};
};

/*from iomapotbm.h*/
#pragma pack(1)
struct TeleportDest{
	unsigned short _x;
	unsigned short _y;
	unsigned char	_z;
};
#pragma pack()

enum AttrTypes_t{
	//ATTR_DESCRIPTION = 1,
	//ATTR_EXT_FILE = 2,
	ATTR_TILE_FLAGS = 3,
	ATTR_ACTION_ID = 4,
	ATTR_UNIQUE_ID = 5,
	ATTR_TEXT = 6,
	ATTR_DESC = 7,
	ATTR_TELE_DEST = 8,
	ATTR_ITEM = 9,
	ATTR_DEPOT_ID = 10,
	//ATTR_EXT_SPAWN_FILE = 11,
	ATTR_RUNE_CHARGES = 12,
	//ATTR_EXT_HOUSE_FILE = 13,
	ATTR_HOUSEDOORID = 14,
	ATTR_COUNT = 15
};

class ItemAttributes{
public:
	ItemAttributes();
	virtual ~ItemAttributes();
	
	void setSpecialDescription(const std::string& desc);
	const std::string& getSpecialDescription() const;
	
	void setText(const std::string& text);
	const std::string& getText() const;
	
	void setActionId(unsigned short n);
	unsigned short getActionId() const;

	void setUniqueId(unsigned short n);
	unsigned short getUniqueId() const;
	
private:
	static std::string emptyString;
	
	enum itemAttrTypes{
		ATTR_ITEM_ACTIONID = 1,
		ATTR_ITEM_UNIQUEID = 2,
		ATTR_ITEM_DESC = 4,
		ATTR_ITEM_TEXT = 8,
	};
	
	struct Attribute{
		itemAttrTypes type;
		void* value;
		Attribute* next;
		Attribute(itemAttrTypes _type){
			type = _type;
			value = NULL;
			next = NULL;
		}
	};
	
	uint32_t m_attributes;
	Attribute* m_firstAttr;
	
	const std::string& getStrAttr(itemAttrTypes type) const;
	void setStrAttr(itemAttrTypes type, const std::string& value);
	
	uint32_t getIntAttr(itemAttrTypes type) const;
	void setIntAttr(itemAttrTypes type, uint32_t value);
	
	bool validateIntAttrType(itemAttrTypes type) const;
	bool validateStrAttrType(itemAttrTypes type) const;
	
	void addAttr(Attribute* attr);
	Attribute* getAttr(itemAttrTypes type) const;
	Attribute* getAttr(itemAttrTypes type);
	
	void deleteAttrs(Attribute* attr);
};

class Item : virtual public Thing, public ItemAttributes
{
public:
	//Factory member to create item of right type based on type
	static Item* CreateItem(const unsigned short _type, unsigned short _count = 1);
	static Item* CreateItem(PropStream& propStream);
	static Items items;

	// Constructor for items
	Item(const unsigned short _type);
	Item(const unsigned short _type, unsigned short _count);
	Item();
	Item(const Item &i);

	virtual ~Item();

	virtual Item* getItem() {return this;};
	virtual const Item* getItem()const {return this;};
	virtual Container* getContainer() {return NULL;};
	virtual const Container* getContainer() const {return NULL;};
	virtual Teleport* getTeleport() {return NULL;};
	virtual const Teleport* getTeleport() const {return NULL;};
	virtual TrashHolder* getTrashHolder() {return NULL;};
	virtual const TrashHolder* getTrashHolder() const {return NULL;};
	virtual Mailbox* getMailbox() {return NULL;};
	virtual const Mailbox* getMailbox() const {return NULL;};
	virtual Door* getDoor() {return NULL;};
	virtual const Door* getDoor() const {return NULL;};
	virtual MagicField* getMagicField() {return NULL;};
	virtual const MagicField* getMagicField() const {return NULL;};

	//serialization
	virtual bool unserialize(xmlNodePtr p);
	virtual xmlNodePtr serialize();

	virtual bool readAttr(AttrTypes_t attr, PropStream& propStream);
	virtual bool unserializeAttr(PropStream& propStream);
	virtual bool unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream);
	//virtual bool serializeItemNode();

	virtual bool serializeAttr(PropWriteStream& propWriteStream);

	virtual bool isPushable() const {return !isNotMoveable();};
	virtual int getThrowRange() const {return (isPickupable() ? 15 : 2);};

	virtual std::string getDescription(int32_t lookDistance) const;
	std::string getWeightDescription() const;

	unsigned short getID() const;    // ID as in ItemType
	unsigned short getClientID() const;
	virtual void setID(unsigned short newid);

	WeaponType_t getWeaponType() const;
	Ammo_t	getAmuType() const;
	//subfight_t getSubfightType() const;

	virtual double getWeight() const;
	int getAttack() const;
	int getArmor() const;
	int getDefense() const;
	int getSlotPosition() const;
	int getRWInfo(int& maxlen) const;
	int getWorth() const;
	void getLight(LightInfo& lightInfo);

	bool hasProperty(enum ITEMPROPERTY prop) const;
	bool isBlocking() const;
	bool isStackable() const;
	bool isRune() const;
	bool isFluidContainer() const;
	bool isAlwaysOnTop() const;
	bool isGroundTile() const;
	bool isSplash() const;
	bool isMagicField() const;
	bool isNotMoveable() const;
	bool isPickupable() const;
	bool isWeapon() const;
	bool isUseable() const;
	bool isHangable() const;
	bool isRoteable() const;
	bool isDoor() const;

	bool floorChangeDown() const;
	bool floorChangeNorth() const;
	bool floorChangeSouth() const;
	bool floorChangeEast() const;
	bool floorChangeWest() const;

	const std::string& getName() const;

	// get the number of items
	unsigned short getItemCount() const;
	void setItemCount(uint8_t n);

	unsigned char getItemCountOrSubtype() const;
	void setItemCountOrSubtype(unsigned char n);
	bool hasSubType() const;

	unsigned char getItemCharge() const {return chargecount;};
	void setItemCharge(unsigned char n) {chargecount = n;};

	unsigned char getFluidType() const {return fluid;};
	void setFluidType(unsigned char n) {fluid = n;};
	
	void setUniqueId(unsigned short n);

	long getDecayTime();
	bool canDecay();

	virtual bool canRemove() const {return true;}
	virtual bool onTradeEvent(TradeEvents_t event, Player* owner){return true;};

protected:
	unsigned short id;  // the same id as in ItemType
	unsigned char count; // number of stacked items
	unsigned char chargecount; //number of charges on the item
	unsigned char fluid; //fluid type
};

#endif
