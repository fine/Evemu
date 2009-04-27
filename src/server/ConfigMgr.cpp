/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2009 The EVEmu Team
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
	Author:		Captnoord
*/

#include "EvemuPCH.h"

initialiseSingleton( ConfigMgr );

ConfigMgr::ConfigMgr()
{
	init();
}

ConfigMgr::~ConfigMgr() {}

bool ConfigMgr::init()
{
	initServiceInfo();
	initBulkData();

	return true;
}

void ConfigMgr::initServiceInfo()
{
	PyDict * serviceInfo = new PyDict();
	/* its a fact that this list should be generated dynamically */
	serviceInfo->set_item("stationSvc",		&PyNone);
	serviceInfo->set_item("objectCaching",	&PyNone);
	serviceInfo->set_item("invbroker",		&PyNone);
	serviceInfo->set_item("map",			&PyNone);
	serviceInfo->set_item("beyonce",		&PyNone);
	serviceInfo->set_item("standing2",		&PyNone);
	serviceInfo->set_item("ram",			&PyNone);
	serviceInfo->set_item("DB",				&PyNone);
	serviceInfo->set_item("posMgr",			&PyNone);
	serviceInfo->set_item("voucher",		&PyNone);
	serviceInfo->set_item("entity",			&PyNone);
	serviceInfo->set_item("damageTracker",	&PyNone);
	serviceInfo->set_item("agentMgr",		&PyNone);
	serviceInfo->set_item("dogmaIM",		&PyNone);
	serviceInfo->set_item("machoNet",		&PyNone);
	serviceInfo->set_item("dungeonExplorationMgr", &PyNone);
	serviceInfo->set_item("watchdog",		&PyNone);
	serviceInfo->set_item("ship",			&PyNone);
	serviceInfo->set_item("DB2",			&PyNone);
	serviceInfo->set_item("market",			&PyNone);
	serviceInfo->set_item("surveilance",	&PyNone);
	serviceInfo->set_item("dungeon",		&PyNone);
	serviceInfo->set_item("aggressionMgr",	&PyNone);
	serviceInfo->set_item("sessionMgr",		&PyNone);
	serviceInfo->set_str ("LSC",			"location");
	serviceInfo->set_item("allianceRegistry",&PyNone);
	serviceInfo->set_item("npcSvc",			&PyNone);
	serviceInfo->set_item("cache",			&PyNone);
	serviceInfo->set_item("character",		&PyNone);
	serviceInfo->set_item("factory",		&PyNone);
	serviceInfo->set_item("facWarMgr",		&PyNone);
	serviceInfo->set_item("corpStationMgr", &PyNone);
	serviceInfo->set_item("authentication", &PyNone);
	serviceInfo->set_str ("station",		"station");
	serviceInfo->set_str ("slash",			"location");
	serviceInfo->set_item("charmgr",		&PyNone);
	serviceInfo->set_item("BSD",			&PyNone);
	serviceInfo->set_item("languageSvc",	&PyNone);
	serviceInfo->set_str ("config",			"locationPreferred");
	serviceInfo->set_item("billingMgr",		&PyNone);
	serviceInfo->set_item("billMgr",		&PyNone);
	serviceInfo->set_item("lookupSvc",		&PyNone);
	serviceInfo->set_item("emailreader",	&PyNone);
	serviceInfo->set_item("lootSvc",		&PyNone);
	serviceInfo->set_item("http",			&PyNone);
	serviceInfo->set_item("gagger",			&PyNone);
	serviceInfo->set_item("dataconfig",		&PyNone);
	serviceInfo->set_item("lien",			&PyNone);
	serviceInfo->set_item("i2",				&PyNone);
	serviceInfo->set_str ("wormholeMgr",	"location");
	serviceInfo->set_item("alert",			&PyNone);
	serviceInfo->set_item("director",		&PyNone);
	serviceInfo->set_item("dogma",			&PyNone);
	serviceInfo->set_item("pathfinder",		&PyNone);
	serviceInfo->set_item("corporationSvc", &PyNone);
	serviceInfo->set_item("clones",			&PyNone);
	serviceInfo->set_item("jumpCloneSvc",	&PyNone);
	serviceInfo->set_item("effectCompiler", &PyNone);
	serviceInfo->set_item("corpmgr",		&PyNone);
	serviceInfo->set_item("warRegistry",	&PyNone);
	serviceInfo->set_item("corpRegistry",	&PyNone);
	serviceInfo->set_str ("account",		"location");
	serviceInfo->set_str ("gangSvc",		"location");
	serviceInfo->set_item("userSvc",		&PyNone);
	serviceInfo->set_item("counter",		&PyNone);
	serviceInfo->set_item("petitioner",		&PyNone);
	serviceInfo->set_str ("keeper",			"solarsystem");
	serviceInfo->set_str ("bookmark",		"location");
	serviceInfo->set_item("jumpbeaconsvc",	&PyNone);
	serviceInfo->set_str ("scanMgr",		"solarsystem");
	serviceInfo->set_item("debug",			&PyNone);
	serviceInfo->set_item("skillMgr",		&PyNone);
	serviceInfo->set_item("onlineStatus",	&PyNone);
	serviceInfo->set_item("LPSvc",			&PyNone);

	mMachoNetServiceInfo = (PyObject*)serviceInfo;
	sObjectCachingSvc.CacheObject((PyObject*)serviceInfo, "machoNet.serviceInfo");
}

void ConfigMgr::initBulkData()
{
	QueryResult* result = StaticDatabase.QueryNA("SELECT * FROM invcategories");

	SafeDelete(result);
}