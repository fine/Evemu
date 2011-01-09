/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Bloody.Rabbit
*/

#include "EVEServerPCH.h"
#include "inventory/AttributeEnum.h"
#include "../eve-common/utils/EvilNumber.h"

/*
 * ShipTypeData
 */
ShipTypeData::ShipTypeData( uint32 weaponTypeID, uint32 miningTypeID, uint32 skillTypeID) : mWeaponTypeID(weaponTypeID),
    mMiningTypeID(miningTypeID), mSkillTypeID(skillTypeID) {}
/*
 * ShipType
 */
 ShipType::ShipType(
    uint32 _id,
    // ItemType stuff:
    const ItemGroup &_group,
    const TypeData &_data,
    // ShipType stuff:
    const ItemType *_weaponType,
    const ItemType *_miningType,
    const ItemType *_skillType,
    const ShipTypeData &stData)
: ItemType(_id, _group, _data),
  m_weaponType(_weaponType),
  m_miningType(_miningType),
  m_skillType(_skillType)
 {
    // data consistency checks:
    if(_weaponType != NULL)
        assert(_weaponType->id() == stData.mWeaponTypeID);
    if(_miningType != NULL)
        assert(_miningType->id() == stData.mMiningTypeID);
    if(_skillType != NULL)
        assert(_skillType->id() == stData.mSkillTypeID);
}

ShipType *ShipType::Load(ItemFactory &factory, uint32 shipTypeID)
{
    return ItemType::Load<ShipType>( factory, shipTypeID );
}

template<class _Ty>
_Ty *ShipType::_LoadShipType(ItemFactory &factory, uint32 shipTypeID,
    // ItemType stuff:
    const ItemGroup &group, const TypeData &data,
    // ShipType stuff:
    const ItemType *weaponType, const ItemType *miningType, const ItemType *skillType, const ShipTypeData &stData)
{
    // we have all the data, let's create new object
    return new ShipType(shipTypeID, group, data, weaponType, miningType, skillType, stData );
}

/*
 * Ship
 */
Ship::Ship(
    ItemFactory &_factory,
    uint32 _shipID,
    // InventoryItem stuff:
    const ShipType &_shipType,
    const ItemData &_data)
: InventoryItem(_factory, _shipID, _shipType, _data) {}

ShipRef Ship::Load(ItemFactory &factory, uint32 shipID)
{
    ShipRef sShipRef = InventoryItem::Load<Ship>( factory, shipID );

    // Load dynamic attributes into new AttributeMap:
    // NOTE: these are set to maximum or default, but they should be saved/loaded to/from the 'entity_attributes' table
    sShipRef.get()->mAttributeMap.SetAttribute(AttrShieldCharge,sShipRef.get()->GetAttribute(AttrShieldCapacity),true);     // Shield Charge
    sShipRef.get()->mAttributeMap.SetAttribute(AttrArmorDamage,sShipRef.get()->GetAttribute(AttrArmorHP),true);             // Armor Damage
    sShipRef.get()->mAttributeMap.SetAttribute(AttrMass,EvilNumber(sShipRef.get()->type().attributes.mass()),true);         // Mass
    sShipRef.get()->mAttributeMap.SetAttribute(AttrRadius,EvilNumber(sShipRef.get()->type().attributes.radius()),true);     // Radius
    sShipRef.get()->mAttributeMap.SetAttribute(AttrInertia,EvilNumber(sShipRef.get()->type().attributes.Inertia()),true);   // Inertia

   return sShipRef;//InventoryItem::Load<Ship>( factory, shipID );   // <--- This used to be here, but was moved at the top of the function
}

template<class _Ty>
RefPtr<_Ty> Ship::_LoadShip(ItemFactory &factory, uint32 shipID,
    // InventoryItem stuff:
    const ShipType &shipType, const ItemData &data)
{
    // we don't need any additional stuff
    return ShipRef( new Ship( factory, shipID, shipType, data ) );
}

ShipRef Ship::Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data
) {
    uint32 shipID = Ship::_Spawn( factory, data );
    if( shipID == 0 )
        return ShipRef();
    return Ship::Load( factory, shipID );
}

uint32 Ship::_Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data
) {
    // make sure it's a ship
    const ShipType *st = factory.GetShipType(data.typeID);
    if(st == NULL)
        return 0;

    // store item data
    uint32 shipID = InventoryItem::_Spawn(factory, data);
    if(shipID == 0)
        return 0;

    // nothing additional

    return shipID;
}

bool Ship::_Load()
{
    // load contents
    if( !LoadContents( m_factory ) )
        return false;

    return InventoryItem::_Load();
}

void Ship::Delete()
{
    // delete contents first
    DeleteContents( m_factory );

    InventoryItem::Delete();
}

double Ship::GetCapacity(EVEItemFlags flag) const
{
    switch( flag ) {
        /*case flagCargoHold:     return capacity();
        case flagDroneBay:      return droneCapacity();
        case flagShipHangar:    return shipMaintenanceBayCapacity();
        case flagHangar:        return corporateHangarCapacity();*/

        // the .get_float() part is a evil hack.... as this function should return a EvilNumber.
        case flagCargoHold:     return GetAttribute(AttrCapacity).get_float();
        case flagDroneBay:      return GetAttribute(AttrDroneCapacity).get_float();
        case flagShipHangar:    return GetAttribute(AttrShipMaintenanceBayCapacity).get_float();
        case flagHangar:        return GetAttribute(AttrCorporateHangarCapacity).get_float();
        default:                return 0.0;
    }
}

void Ship::ValidateAddItem(EVEItemFlags flag, InventoryItemRef item, Client *c)
{
	CharacterRef character = c->GetChar();
	
	if( flag == flagDroneBay )
    {
        if( item->categoryID() != EVEDB::invCategories::Drone )
            //Can only put drones in drone bay
            throw PyException( MakeUserError( "ItemCannotBeInDroneBay" ) );
    }
    else if( flag == flagShipHangar )
    {
		if( c->GetShip()->GetAttribute(AttrHasShipMaintenanceBay ) != 0)
            // We have no ship maintenance bay
			throw PyException( MakeCustomError( "%s has no ship maintenance bay.", item->itemName().c_str() ) );
        if( item->categoryID() != EVEDB::invCategories::Ship )
            // Only ships may be put here
            throw PyException( MakeCustomError( "Only ships may be placed into ship maintenance bay." ) );
    }
    else if( flag == flagHangar )
    {
		if( c->GetShip()->GetAttribute(AttrHasCorporateHangars ) != 0)
            // We have no corporate hangars
            throw PyException( MakeCustomError( "%s has no corporate hangars.", item->itemName().c_str() ) );
    }
    else if( flag == flagCargoHold )
	{
		//get all items in cargohold
		EvilNumber capacityUsed(0);
		std::vector<InventoryItemRef> items;
		c->GetShip()->FindByFlag(flag, items);
		for(uint32 i = 0; i < items.size(); i++){
			capacityUsed += items[i]->GetAttribute(AttrVolume);
		}
		if( capacityUsed + item->GetAttribute(AttrVolume) > c->GetShip()->GetAttribute(AttrCapacity) )
			throw PyException( MakeCustomError( "Not enough cargo space!") );

	}
	else if( flag > flagLowSlot0  &&  flag < flagHiSlot7 )
	{
		if(!Skill::FitModuleSkillCheck(item, character))
			throw PyException( MakeCustomError( "You do not have the required skills to fit this \n%s", item->itemName().c_str() ) );
		if(!ValidateItemSpecifics(c,item))
			throw PyException( MakeCustomError( "Your ship cannot equip this module" ) );
		if(item->categoryID() == EVEDB::invCategories::Charge) {
			InventoryItemRef module;
			c->GetShip()->FindSingleByFlag(flag, module);
			if(module->GetAttribute(AttrChargeSize) != item->GetAttribute(AttrChargeSize) )
				throw PyException( MakeCustomError( "The charge is not the correct size for this module." ) );
			if(module->GetAttribute(AttrChargeGroup1) != item->groupID())
				throw PyException( MakeCustomError( "Incorrect charge type for this module.") );
		}
	}
	else if( flag > flagRigSlot0  &&  flag < flagRigSlot7 )
	{
		if(!Skill::FitModuleSkillCheck(item, character))
			throw PyException( MakeCustomError( "You do not have the required skills to fit this \n%s", item->itemName().c_str() ) );
		if(c->GetShip()->GetAttribute(AttrRigSize) != item->GetAttribute(AttrRigSize))
			throw PyException( MakeCustomError( "Your ship cannot fit this size module" ) );
		if( c->GetShip()->GetAttribute(AttrUpgradeLoad) + item->GetAttribute(AttrUpgradeCost) > c->GetShip()->GetAttribute(AttrUpgradeCapacity) )
			throw PyException( MakeCustomError( "Your ship cannot handle the extra calibration" ) );
	}
	else if( flag > flagSubSystem0  &&  flag < flagSubSystem7 )
	{
		if(!Skill::FitModuleSkillCheck(item, character))
			throw PyException( MakeCustomError( "You do not have the required skills to fit this \n%s", item->itemName().c_str() ) );
	}
	
}

PyObject *Ship::ShipGetInfo()
{
    if( !LoadContents( m_factory ) )
    {
        codelog( ITEM__ERROR, "%s (%u): Failed to load contents for ShipGetInfo", itemName().c_str(), itemID() );
        return NULL;
    }

    Rsp_CommonGetInfo result;
    Rsp_CommonGetInfo_Entry entry;

    //first populate the ship.
    if( !Populate( entry ) )
        return NULL;    //print already done.

    //hacking:
    //maximumRangeCap
    entry.attributes[ 797 ] = new PyFloat( 250000.000000 );

    result.items[ itemID() ] = entry.Encode();

    //now encode contents...
    std::vector<InventoryItemRef> equipped;
	std::vector<InventoryItemRef> integrated;
    //find all the equipped items and rigs
    FindByFlagRange( flagLowSlot0, flagFixedSlot, equipped );
	FindByFlagRange( flagRigSlot0, flagRigSlot7, integrated );
	//append them into one list
	equipped.insert(equipped.end(), integrated.begin(), integrated.end() );
    //encode an entry for each one.
    std::vector<InventoryItemRef>::iterator cur, end;
    cur = equipped.begin();
    end = equipped.end();
    for(; cur != end; cur++)
    {
        if( !(*cur)->Populate( entry ) )
        {
            codelog( ITEM__ERROR, "%s (%u): Failed to load item %u for ShipGetInfo", itemName().c_str(), itemID(), (*cur)->itemID() );
        }
        else
            result.items[ (*cur)->itemID() ] = entry.Encode();
    }

    return result.Encode();
}

void Ship::AddItem(InventoryItemRef item)
{
    InventoryEx::AddItem( item );

    if( item->flag() >= flagSlotFirst && 
        item->flag() <= flagSlotLast && 
        item->categoryID() != EVEDB::invCategories::Charge)
    {
        // make singleton
        item->ChangeSingleton( true );
    }
}

bool Ship::ValidateBoardShip(ShipRef ship, CharacterRef character)
{
	SkillRef requiredSkill;

	//Primary Skill
	if(ship->GetAttribute(AttrRequiredSkill1) != 0)
	{
        // the .get_int() part is a hack...
		requiredSkill = character->GetSkill( ship->GetAttribute(AttrRequiredSkill1).get_int() );
		if( !requiredSkill )
			return false;

		if( ship->GetAttribute(AttrRequiredSkill1Level) > requiredSkill->GetAttribute(AttrSkillLevel) )
			return false;
	}

	//Secondary Skill
	if(ship->GetAttribute(AttrRequiredSkill2) != 0)
	{
		requiredSkill = character->GetSkill( ship->GetAttribute(AttrRequiredSkill2).get_int() );
		if( !requiredSkill )
			return false;

		if( ship->GetAttribute(AttrRequiredSkill2Level) > requiredSkill->GetAttribute(AttrSkillLevel) )
			return false;
	}
	
	//Tertiary Skill
	if(ship->GetAttribute(AttrRequiredSkill3) != 0)
	{
		requiredSkill = character->GetSkill( ship->GetAttribute(AttrRequiredSkill3).get_int() );
		if( !requiredSkill )
			return false;

		if( ship->GetAttribute(AttrRequiredSkill3Level) > requiredSkill->GetAttribute(AttrSkillLevel) )
			return false;
	}
	
	//Quarternary Skill
	if(ship->GetAttribute(AttrRequiredSkill4) != 0)
	{
		requiredSkill = character->GetSkill( ship->GetAttribute(AttrRequiredSkill4).get_int() );
		if( !requiredSkill )
			return false;

		if( ship->GetAttribute(AttrRequiredSkill4Level) > requiredSkill->GetAttribute(AttrSkillLevel) )
			return false;
	}
	
	//Quinary Skill
	if(ship->GetAttribute(AttrRequiredSkill5) != 0)
	{
		requiredSkill = character->GetSkill( ship->GetAttribute(AttrRequiredSkill5).get_int() );
		if( !requiredSkill )
			return false;

		if( ship->GetAttribute(AttrRequiredSkill5Level) > requiredSkill->GetAttribute(AttrSkillLevel) )
			return false;
	}
	
	//Senary Skill
	if(ship->GetAttribute(AttrRequiredSkill6) != 0)
	{
		requiredSkill = character->GetSkill( ship->GetAttribute(AttrRequiredSkill6).get_int() );
		if( !requiredSkill )
			return false;

		if( ship->GetAttribute(AttrRequiredSkill6Level) > requiredSkill->GetAttribute(AttrSkillLevel) )
			return false;
	}

	return true;
}

bool Ship::ValidateItemSpecifics(Client *c, InventoryItemRef equip) {

	//declaring explicitly as int...not sure if this is needed or not
	int groupID = c->GetShip()->groupID();
	int typeID = c->GetShip()->typeID();
	EvilNumber canFitShipGroup1 = equip->GetAttribute(AttrCanFitShipGroup1);
	EvilNumber canFitShipGroup2 = equip->GetAttribute(AttrCanFitShipGroup2);
	EvilNumber canFitShipGroup3 = equip->GetAttribute(AttrCanFitShipGroup3);
	EvilNumber canFitShipGroup4 = equip->GetAttribute(AttrCanFitShipGroup4);
	EvilNumber canFitShipType1 = equip->GetAttribute(AttrCanFitShipType1);
	EvilNumber canFitShipType2 = equip->GetAttribute(AttrCanFitShipType2);
	EvilNumber canFitShipType3 = equip->GetAttribute(AttrCanFitShipType3);
	EvilNumber canFitShipType4 = equip->GetAttribute(AttrCanFitShipType4);

	if( canFitShipGroup1 != 0 )
		if( canFitShipGroup1 != groupID )
			return false;

	if( canFitShipGroup2 != 0 )
		if( canFitShipGroup2 != groupID )
			return false;

	if( canFitShipGroup3 != 0 )
		if( canFitShipGroup3 != groupID )
			return false;

	if( canFitShipGroup4 != 0 )
		if( canFitShipGroup4 != groupID )
			return false;

	if( canFitShipType1 != 0 )
		if( canFitShipType1 != typeID )
			return false;

	if( canFitShipType2 != 0 )
		if( canFitShipType2 != typeID )
			return false;

	if( canFitShipType3 != 0 )
		if( canFitShipType3 != typeID )
			return false;

	if( canFitShipType4 != 0 )
		if( canFitShipType4 != typeID )
			return false;

	return true;

}