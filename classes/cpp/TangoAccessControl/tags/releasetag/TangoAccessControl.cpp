static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         TangoAccessControl.cpp
//
// description :  C++ source for the TangoAccessControl and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                TangoAccessControl are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================



//===================================================================
//
//	The following table gives the correspondance
//	between commands and method's name.
//
//  Command's name        |  Method's name
//	----------------------------------------
//  State                 |  dev_state()
//  Status                |  dev_status()
//  AddAddressForUser     |  add_address_for_user()
//  AddDeviceForUser      |  add_device_for_user()
//  CloneUser             |  clone_user()
//  GetAccess             |  get_access()
//  GetAddressByUser      |  get_address_by_user()
//  GetAllowedCommands    |  get_allowed_commands()
//  GetDeviceByUser       |  get_device_by_user()
//  GetDeviceClass        |  get_device_class()
//  GetUsers              |  get_users()
//  RemoveAddressForUser  |  remove_address_for_user()
//  RemoveDeviceForUser   |  remove_device_for_user()
//  RemoveUser            |  remove_user()
//  RegisterService       |  register_service()
//  UnregisterService     |  unregister_service()
//
//===================================================================


#include <tango.h>
#include <TangoAccessControl.h>
#include <TangoAccessControlClass.h>

namespace TangoAccessControl_ns
{

//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::TangoAccessControl(string &s)
// 
// description : 	constructor for simulated TangoAccessControl
//
// in : - cl : Pointer to the DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------
TangoAccessControl::TangoAccessControl(Tango::DeviceClass *cl,string &s)
:AccessControl_ns::AccessControl(cl,s.c_str())
{
	init_device();
}

TangoAccessControl::TangoAccessControl(Tango::DeviceClass *cl,const char *s)
:AccessControl_ns::AccessControl(cl,s)
{
	init_device();
}

TangoAccessControl::TangoAccessControl(Tango::DeviceClass *cl,const char *s,const char *d)
:AccessControl_ns::AccessControl(cl,s,d)
{
	init_device();
}
//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::delete_device()
// 
// description : 	will be called at device destruction or at init command.
//
//-----------------------------------------------------------------------------
void TangoAccessControl::delete_device()
{
	//	Delete device's allocated object
}

//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::init_device()
// 
// description : 	will be called at device initialization.
//
//-----------------------------------------------------------------------------
void TangoAccessControl::init_device()
{
	INFO_STREAM << "TangoAccessControl::TangoAccessControl() create device " << device_name << endl;

	// Initialise variables to default values
	//--------------------------------------------
	mysql_connection();

	set_state(Tango::ON);
	set_status("Device is OK");
}

//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------
void TangoAccessControl::always_executed_hook()
{
	
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::add_address_for_user
 *
 *	description:	method to execute "AddAddressForUser"
 *	Add an address for the specified user..
 *
 * @param	argin	user name, address
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::add_address_for_user(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "TangoAccessControl::add_address_for_user(): entering... !" << endl;
	if (argin->length() < 2)
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)"Needs at least 2 input arguments",
					  (const char *)"AccessControl::add_address_for_user()");
	int	x = 0;
	string	username((*argin)[x++]);
	string	ip_add  ((*argin)[x++]);

	//	Check before if already exists
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT address FROM access_address WHERE "  <<
				"user=\""    << username << "\" AND  "    <<
				"address=\"" << ip_add   << "\"";

	DEBUG_STREAM << "add_address_for_user(): sql_query " << tms.str() << endl;
	MYSQL_RES *check = query(tms.str(), "add_address_for_user()");
	int	n_rows = mysql_num_rows(check);
	bool	already_exists = (n_rows > 0);
	mysql_free_result(check);
	if (already_exists)
		Tango::Except::throw_exception((const char *)AC_AlreadyExists,
	   				  (const char *)"This record already exists in database",
					  (const char *)"AccessControl::add_address_for_user()");


	//	If not exists, insert it
	tms.str("");
	tms << "INSERT INTO access_address SET user=\"" << username
										<< "\",address=\""      << ip_add
										<< "\"";
	DEBUG_STREAM << "AccessControl::add_address_for_user(): sql_query " << tms.str() << endl;
	simple_query(tms.str(),"add_address_for_user()");
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::add_device_for_user
 *
 *	description:	method to execute "AddDeviceForUser"
 *	Add a device and rights for the specified user..
 *
 * @param	argin	user name, device adn value
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::add_device_for_user(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "TangoAccessControl::add_device_for_user(): entering... !" << endl;

	if (argin->length() < 3)
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)"Needs at least 3 input arguments",
					  (const char *)"AccessControl::ac_add_device_for_user()");
	int	x = 0;
	string	username((*argin)[x++]);
	string	device  ((*argin)[x++]);
	string	rights  ((*argin)[x++]);

	//	Check before if already exists
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT device FROM access_device WHERE "  <<
				"user=\""    << username << "\" AND  "    <<
				"device=\""  << device   << "\" AND  "    <<
				"rights=\""  << rights   << "\"";

	DEBUG_STREAM << "ac_add_device_for_user(): sql_query " << tms.str() << endl;
	MYSQL_RES *check = query(tms.str(), "ac_add_device_for_user()");
	int	n_rows = mysql_num_rows(check);
	bool	already_exists = (n_rows > 0);
	mysql_free_result(check);
	if (already_exists)
		Tango::Except::throw_exception((const char *)AC_AlreadyExists,
	   				  (const char *)"This record already exists in database",
					  (const char *)"AccessControl::ac_add_device_for_user()");


	//	If not exists, insert it
	tms.str("");
	tms << "INSERT INTO access_device SET user=\"" << username
										<< "\",device=\""       << device
										<< "\",rights=\""       << rights
										<< "\"";
	DEBUG_STREAM << "AccessControl::ac_add_device_for_user(): sql_query " << tms.str() << endl;
	simple_query(tms.str(),"ac_add_device_for_user()");
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::clone_user
 *
 *	description:	method to execute "CloneUser"
 *	Copy addresses and devices from source user to target user.
 *
 * @param	argin	[0] - source user name.\n[1] - target user name.
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::clone_user(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "TangoAccessControl::clone_user(): entering... !" << endl;

	if (argin->length() < 2)
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)"Needs at least 2 input arguments",
					  (const char *)"AccessControl::clone_user()");	
	int	x = 0;
	string	src_user((*argin)[x++]);
	string	new_user((*argin)[x++]);

	//	remove new user if already exists
	remove_user((char *)new_user.c_str());

	//	Get address for source user
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT address FROM access_address WHERE user"
			<< "=\"" << src_user <<  "\" ORDER BY address";
	DEBUG_STREAM << "AccessControl::clone_user(): sql_query " << tms.str() << endl;
	
	MYSQL_RES *result = query(tms.str(), "clone_user()");
	int	n_rows = mysql_num_rows(result);
	vector<string>	v_add;
	if (n_rows > 0)
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				string	s(row[0]);
				v_add.push_back(s);
			}
		}
	mysql_free_result(result);

	//	Write with new user.
	int	i;
	for (i=0 ; i<v_add.size() ; i++)
	{
		tms.str("");
		tms << "INSERT INTO access_address SET user=\"" << new_user
								<< "\",address=\""      << v_add[i]
								<< "\"";
		DEBUG_STREAM << "AccessControl::clone_user(): sql_query " << tms.str() << endl;
		simple_query(tms.str(),"clone_user()");
	}


	//	Get Device for source user
	tms.str("");
	tms << "SELECT DISTINCT device, rights FROM access_device WHERE user"
			<< "=\"" << src_user <<  "\" ORDER BY device";
	DEBUG_STREAM << "AccessControl::clone_user(): sql_query " << tms.str() << endl;
	
	result = query(tms.str(), "clone_user()");

		n_rows = mysql_num_rows(result);
	vector<string>	v_dev;
	if (n_rows > 0)
		for (i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				string	device(row[0]);
				string	rights(row[1]);
				v_dev.push_back(device);
				v_dev.push_back(rights);
			}
		}
	mysql_free_result(result);

	//	Write with new user	
	for (i=0 ; i<v_dev.size()/2 ; i++)
	{
		tms.str("");
		tms << "INSERT INTO access_device SET user=\"" << new_user
								<< "\",device=\""      << v_dev[2*i]
								<< "\",rights=\""      << v_dev[2*i+1]
								<< "\"";
		DEBUG_STREAM << "AccessControl::clone_user(): sql_query " << tms.str() << endl;
		simple_query(tms.str(),"clone_user()");
	}
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::get_access
 *
 *	description:	method to execute "GetAccess"
 *	Check access for specified user, device, address and returns access (read or write).
 *
 * @param	argin	[0] - User name\n[1] - Device\n[2] - IP Address
 * @return	cess for specified inputs.
 *
 */
//+------------------------------------------------------------------
Tango::DevString TangoAccessControl::get_access(const Tango::DevVarStringArray *argin)
{
	if (argin->length() < 3)
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)"Needs at least 3 input arguments",
					  (const char *)"AccessControl::get_access()");	
	int	x = 0;
	string	user((*argin)[x++]);
	string	ip_add((*argin)[x++]);
	string	device((*argin)[x++]);

	//	First pass, Check if User,address is defined (autorized)
	vector<AccessStruct>	as_read =
				get_access_for_user_address(user, ip_add);;

	//	Check if first pass has results
	if (as_read.size()==0)
	{
		char	*argout;
		argout = CORBA::string_dup("read");
		return argout;
	}

	DEBUG_STREAM << "First pass results :" << endl;
	int	i;
	for (i=0 ; i<as_read.size() ; i++)
		DEBUG_STREAM << "	object["<< i << "] : " 
					 << as_read[i].user << "|" << as_read[i].address << endl;

	//	Couple: user, address is autorized  -->
	//	second pass, Check if User,device is defined
	//	First time check if user '*' found
	bool	found = false;
	for (i=0 ; i<as_read.size() && !found ; i++)
		if (as_read[i].user == "*")
		{
			//	if found test rights for user "*"
			found = true;
			string	result = get_access_for_user_device(as_read[i].user, device);;

			if (result=="write")
			{
				DEBUG_STREAM << "Second pass result is " << result <<
						" for user \"*\"" << endl;
				char	*argout;
				argout = CORBA::string_dup(result.c_str());
				return argout;
			}
		}

	//	If not found try check for real user
	string	result = get_access_for_user_device(user, device);;
	DEBUG_STREAM << "Second pass result is " << result << endl;

	char	*argout;
	argout = CORBA::string_dup(result.c_str());
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::get_address_by_user
 *
 *	description:	method to execute "GetAddressByUser"
 *	Returns address list  found for the specified user.
 *
 * @param	argin	user name.
 * @return	Addresses found for the specified user.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *TangoAccessControl::get_address_by_user(Tango::DevString argin)
{
	string	username(argin);
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT address FROM access_address WHERE user"
			<< "=\"" << username <<  "\" ORDER BY address";
	DEBUG_STREAM << "AccessControl::get_address_by_user(): sql_query " << tms.str() << endl;
	
	MYSQL_RES *result = query(tms.str(), "get_address_by_user()");
	int	n_rows = mysql_num_rows(result);

	Tango::DevVarStringArray *argout = new Tango::DevVarStringArray;
	int		nb_col = 1;
	if (n_rows > 0)
	{
		argout->length(n_rows*nb_col);
		int		nb=0;
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				for (int c=0 ; c <nb_col ; c++)
					(*argout)[nb++]   = CORBA::string_dup(row[c]);
			}
		}
	}
	else
		argout->length(0);
	mysql_free_result(result);

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::get_allowed_commands
 *
 *	description:	method to execute "GetAllowedCommands"
 *	Returns allowed command list found in database for specified device
 *	It search the class of the specified device and then uses the class property <b>AllowedAccessCmd</b>
 *
 * @param	argin	Device name
 * @return	Allowed commands found in database for specified device
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *TangoAccessControl::get_allowed_commands(Tango::DevString argin)
{
	DEBUG_STREAM << "AccessControl::get_allowed_commands(): entering... !" << endl;
	string	classname(get_device_class(argin));
	
	TangoSys_MemStream	tms;
	tms << "SELECT value FROM property_class WHERE class = \"" 
			<< classname.c_str() << "\" AND name =\"AllowedAccessCmd\"";
	DEBUG_STREAM << "AccessControl::get_allowed_commands(): sql_query " << tms.str() << endl;
	   
	MYSQL_RES	*result = query(tms.str(),"get_allowed_commands()");
	int	n_rows = mysql_num_rows(result);

	Tango::DevVarStringArray *argout = new Tango::DevVarStringArray;
	argout->length(n_rows);
	if (n_rows > 0)
	{
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				DEBUG_STREAM << "AccessControl::get_allowed_commands(): property[ "<< i << "] " << row[0] << endl;
				(*argout)[i] = CORBA::string_dup(row[0]);
			}
		}
	}
	mysql_free_result(result);

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::get_device_by_user
 *
 *	description:	method to execute "GetDeviceByUser"
 *	Returns devices and rights found for the specified user.
 *
 * @param	argin	user name.
 * @return	devices and rights found for the specified user.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *TangoAccessControl::get_device_by_user(Tango::DevString argin)
{
	DEBUG_STREAM << "AccessControl::get_device_by_user(): entering... !" << endl;
	string	username(argin);
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT device,rights FROM access_device WHERE user"
			<< "=\"" << username <<  "\" ORDER BY device";
	DEBUG_STREAM << "AccessControl::get_device_by_user(): sql_query " << tms.str() << endl;
	
	MYSQL_RES *result = query(tms.str(), "get_device_by_user()");
	int	n_rows = mysql_num_rows(result);

	Tango::DevVarStringArray *argout = new Tango::DevVarStringArray;
	int		nb_col = 2;
	if (n_rows > 0)
	{
		argout->length(n_rows*nb_col);
		int		nb=0;
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				for (int c=0 ; c <nb_col ; c++)
					(*argout)[nb++]   = CORBA::string_dup(row[c]);
			}
		}
	}
	else
		argout->length(0);
	mysql_free_result(result);

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::get_device_class
 *
 *	description:	method to execute "GetDeviceClass"
 *	Returns class for specified device.
 *
 * @param	argin	Device name
 * @return	Class found in database for specified device
 *
 */
//+------------------------------------------------------------------
Tango::DevString TangoAccessControl::get_device_class(Tango::DevString argin)
{
	DEBUG_STREAM << "AccessControl::get_device_class(): entering... !" << endl;
	//	Get class for device
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT class FROM device WHERE name=\""
				<< argin <<  "\"";
	DEBUG_STREAM << "AccessControl::get_device_class(): sql_query " << tms.str() << endl;
	
	Tango::DevString	argout;
	MYSQL_RES *result = query(tms.str(), "get_device_class()");
	int	n_rows = mysql_num_rows(result);
	if (n_rows==0)
	{
		TangoSys_MemStream	tms;
		tms << "Class not found for " << argin;
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)tms.str().c_str(),
					  (const char *)"AccessControl::get_device_class()");	
	}

	if (n_rows > 0)
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				argout = CORBA::string_dup(row[0]);
			}
		}
	mysql_free_result(result);

	DEBUG_STREAM << "Class " << argout << " found for " << argin << endl;
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::get_users
 *
 *	description:	method to execute "GetUsers"
 *	Returns user list found in table access_address.
 *
 * @return	Users find in table access_address.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *TangoAccessControl::get_users()
{
	DEBUG_STREAM << "AccessControl::get_users(): entering... !" << endl;
	//	Search in address table
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT user FROM access_address "
			<< " WHERE user like \"%\" ORDER BY user";
	DEBUG_STREAM << "AccessControl::get_users(): sql_query " << tms.str() << endl;
	
	MYSQL_RES *result = query(tms.str(), "get_users()");
	int	n_rows = mysql_num_rows(result);

	vector<string>	users;
	if (n_rows > 0)
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				string	s(row[0]);
				users.push_back(s);
			}
		}
	mysql_free_result(result);

	//	Search in device table
	tms.str("");
	tms << "SELECT DISTINCT user FROM access_device "
			<< " WHERE user like \"%\" ORDER BY user";
	DEBUG_STREAM << "AccessControl::get_users(): sql_query " << tms.str() << endl;
	
	result = query(tms.str(), "get_users()");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				string	s(row[0]);
				//	Check if exists
				bool exists = false;
				for (int j=0 ; !exists && j<users.size() ; j++)
					exists = (users[j]==s);
				if (!exists)
					users.push_back(s);
			}
		}
	mysql_free_result(result);

	Tango::DevVarStringArray *argout = new Tango::DevVarStringArray;
	argout->length(users.size());
	for (int i=0 ; i<users.size() ; i++)
		(*argout)[i] = CORBA::string_dup(users[i].c_str());
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::remove_address_for_user
 *
 *	description:	method to execute "RemoveAddressForUser"
 *	Remove an address for the specified user..
 *
 * @param	argin	user name, address
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::remove_address_for_user(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "TangoAccessControl::remove_address_for_user(): entering... !" << endl;

	if (argin->length() < 2)
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)"Needs at least 2 input arguments",
					  (const char *)"AccessControl::remove_address_for_user()");	
	int	x = 0;
	string	username((*argin)[x++]);
	string	ip_add  ((*argin)[x++]);

	TangoSys_MemStream	tms;
	tms << "DELETE FROM access_address WHERE user=\"" << username
							<< "\" AND address=\""     << ip_add
							<< "\"";
	DEBUG_STREAM << "AccessControl::remove_address_for_user(): sql_query " << tms.str() << endl;
	simple_query(tms.str(),"remove_address_for_user()");
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::remove_device_for_user
 *
 *	description:	method to execute "RemoveDeviceForUser"
 *	Remove a device and its rights for the specified user..
 *
 * @param	argin	user name, device and value
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::remove_device_for_user(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "TangoAccessControl::remove_device_for_user(): entering... !" << endl;

	if (argin->length() < 3)
		Tango::Except::throw_exception((const char *)AC_IncorrectArguments,
	   				  (const char *)"Needs at least 3 input arguments",
					  (const char *)"AccessControl::remove_device_for_user()");	
	int	x = 0;
	string	username((*argin)[x++]);
	string	device  ((*argin)[x++]);
	string	rights  ((*argin)[x++]);

	TangoSys_MemStream	tms;
	tms << "DELETE FROM access_device WHERE user=\"" << username
							<< "\" AND device=\""    << device
							<< "\" AND rights=\""    << rights
							<< "\"";
	DEBUG_STREAM << "AccessControl::remove_device_for_user(): sql_query " << tms.str() << endl;
	simple_query(tms.str(),"remove_device_for_user()");
}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::remove_user
 *
 *	description:	method to execute "RemoveUser"
 *	Remove all records for specified user.
 *
 * @param	argin	user name
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::remove_user(Tango::DevString argin)
{
	DEBUG_STREAM << "TangoAccessControl::remove_user(): entering... !" << endl;

	//	remove user if already exists in address table
	TangoSys_MemStream	tms;
	tms << "DELETE FROM access_device WHERE user=\"" << argin
										<< "\" AND device LIKE \"%\""
										<<   " AND rights LIKE \"%\"";
	DEBUG_STREAM << "AccessControl::clone_user(): sql_query " << tms.str() << endl;
	simple_query(tms.str(),"clone_user()");

	//	remove user if already exists in device table
	tms.str("");
	tms << "DELETE FROM access_address WHERE user=\"" << argin
										<< "\" AND address LIKE \"%\"";
	DEBUG_STREAM << "AccessControl::clone_user(): sql_query " << tms.str() << endl;
	simple_query(tms.str(),"clone_user()");

}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::register_service
 *
 *	description:	method to execute "RegisterService"
 *	Register device as a TANGO service.
 *
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::register_service()
{
	DEBUG_STREAM << "TangoAccessControl::register_service(): entering... !" << endl;

	register_service(ServiceName, InatanceName, device_name);

}

//+------------------------------------------------------------------
/**
 *	method:	TangoAccessControl::unregister_service
 *
 *	description:	method to execute "UnregisterService"
 *	Unregister device as a TANGO service.
 *
 *
 */
//+------------------------------------------------------------------
void TangoAccessControl::unregister_service()
{
	DEBUG_STREAM << "TangoAccessControl::unregister_service(): entering... !" << endl;

	unregister_service(ServiceName, InatanceName, device_name);

}

}	//	namespace
