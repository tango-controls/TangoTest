//=============================================================================
//
// file :        TangoAccessControl.h
//
// description : Include for the TangoAccessControl class.
//
// project :	Tango Access Control Management
//
// $Author$
//
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
// $Revision$
//
// $Log$
// Revision 1.9  2009/04/06 12:20:20  taurel
// - Fix compilation problem with Solaris CC
//
// Revision 1.8  2009/04/04 17:42:36  taurel
// Device now inherits from Device_4Impl.
// Environment variable got from Tango library (To manage tangorc files)
//
// Revision 1.7  2009/02/06 08:07:19  pascal_verdier
// Running on same host than mysql test removed.
//
// Revision 1.6  2008/10/08 09:28:22  pascal_verdier
// GetAllowedCommandClassList command added.
//
// Revision 1.5  2006/09/28 08:59:58  pascal_verdier
// Check of host where running added.
//
// Revision 1.4  2006/09/21 09:20:15  pascal_verdier
// *** empty log message ***
//
// Revision 1.3  2006/09/20 07:22:49  pascal_verdier
// Another bug fixed in GetAccess command.
//
// Revision 1.2  2006/09/15 12:02:17  pascal_verdier
// Class descrition modified.
//
// Revision 1.1.1.1  2006/09/15 11:55:43  pascal_verdier
// Initial Revision
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================
#ifndef _TANGOACCESSCONTROL_H
#define _TANGOACCESSCONTROL_H

#include <tango.h>
#include <AccessControl.h>
#include <mysql.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

 //	Add your own constants definitions here.
 //-----------------------------------------------
#define	CONTROL_SYSTEM				"CtrlSystem"
#define SERVICE_PROP_NAME			"Services"
#define ServiceName					"AccessControl"
#define InatanceName				"tango"
 
#define AC_SQLError 				"AC_SQLError"
#define AC_IncorrectArguments		"AC_IncorrectArguments"
#define AC_AlreadyExists			"AC_AlreadyExists"

#define	STARTER_DEVNAME_HEADER	"tango/admin/"

typedef struct {
	string	user;
	string	device;
	string	address;
	string	rights;
}
AccessStruct;

//	Define time measuremnt type (depends on OS)
#ifndef WIN32

#	define	TimeVal	struct timeval
#	define	GetTime(t)	gettimeofday(&t, NULL);
#	define	Elapsed(before, after)	\
		1000.0*(after.tv_sec-before.tv_sec) + \
		((double)after.tv_usec-before.tv_usec) / 1000

#else

static LARGE_INTEGER	cpu_freq;
#	define	TimeVal	LARGE_INTEGER
#	define	GetTime(t)	w_gettimeofday(&t);
#	define	Elapsed(before, after)		\
		(cpu_freq.QuadPart==0) ? 0.0 :	\
			(double) (after.QuadPart - before.QuadPart)/cpu_freq.QuadPart * 1000;

#endif	/*	WIN32		*/



namespace TangoAccessControl_ns
{

class DummyDev: public Tango::Connection
{
public:
	DummyDev():Tango::Connection(true) {};

	virtual string get_corba_name(bool) {string str;return str;}
	virtual string build_corba_name() {string str;return str;}
	virtual int get_lock_ctr() {return 0;}
	virtual void set_lock_ctr(int) {};

	virtual string dev_name() {string str;return str;}

	int get_env_var(const char *cc,string &str_ref) {return Tango::Connection::get_env_var(cc,str_ref);}
};

/**
 * Class Description:
 * This class is a conceate class inherited from AccessControl abstract class.<Br>
 *	<Br>
 *	This class defines how to manage the TANGO access control.<Br>
 *	It implements commands for tool to defines access for users, devices and IP addresses.<Br>
 *	It implements also commands used by client API to check access for specified user, device and address.<Br>
 *	And it implements register and unregister it as TANGO service.
 */

/*
 *	Device States Description:
*  Tango::ON :     The MySql database handle is OK.
*  Tango::FAULT :  The MySql database handle is not OK.
 */


class TangoAccessControl: public AccessControl_ns::AccessControl	//	Inherited from class Device_3Impl
{
public :
	//	Add your own data members here
	//-----------------------------------------


	//	Here is the Start of the automatic code generation part
	//-------------------------------------------------------------	
/**
 *	@name attributes
 *	Attributs member data.
 */
//@{
//@}

/**
 *	@name Device properties
 *	Device properties member data.
 */
//@{
//@}

/**@name Constructors
 * Miscellaneous constructors */
//@{
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	TangoAccessControl(Tango::DeviceClass *cl,string &s);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	TangoAccessControl(Tango::DeviceClass *cl,const char *s);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device name
 *	@param d	Device description.
 */
	TangoAccessControl(Tango::DeviceClass *cl,const char *s,const char *d);
//@}

/**@name Destructor
 * Only one desctructor is defined for this class */
//@{
/**
 * The object desctructor.
 */	
	~TangoAccessControl() {delete_device();};
/**
 *	will be called at device destruction or at init command.
 */
	void delete_device();
//@}

	
/**@name Miscellaneous methods */
//@{
/**
 *	Initialize the device
 */
	virtual void init_device();
/**
 *	Always executed method befor execution command method.
 */
	virtual void always_executed_hook();

//@}

/**
 * @name TangoAccessControl methods prototypes
 */

//@{
/**
 *	Execution allowed for AddAddressForUser command.
 */
	virtual bool is_AddAddressForUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for AddDeviceForUser command.
 */
	virtual bool is_AddDeviceForUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for CloneUser command.
 */
	virtual bool is_CloneUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetAccess command.
 */
	virtual bool is_GetAccess_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetAddressByUser command.
 */
	virtual bool is_GetAddressByUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetAllowedCommands command.
 */
	virtual bool is_GetAllowedCommands_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetAllowedCommandClassList command.
 */
	virtual bool is_GetAllowedCommandClassList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetDeviceByUser command.
 */
	virtual bool is_GetDeviceByUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetDeviceClass command.
 */
	virtual bool is_GetDeviceClass_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetUsers command.
 */
	virtual bool is_GetUsers_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for RemoveAddressForUser command.
 */
	virtual bool is_RemoveAddressForUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for RemoveDeviceForUser command.
 */
	virtual bool is_RemoveDeviceForUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for RemoveUser command.
 */
	virtual bool is_RemoveUser_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for RegisterService command.
 */
	virtual bool is_RegisterService_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for UnregisterService command.
 */
	virtual bool is_UnregisterService_allowed(const CORBA::Any &any);
/**
 * Add an address for the specified user..
 *	@param	argin	user name, address
 *	@exception DevFailed
 */
	void	add_address_for_user(const Tango::DevVarStringArray *);
/**
 * Add a device and rights for the specified user..
 *	@param	argin	user name, device adn value
 *	@exception DevFailed
 */
	void	add_device_for_user(const Tango::DevVarStringArray *);
/**
 * Copy addresses and devices from source user to target user.
 *	@param	argin	[0] - source user name.\n[1] - target user name.
 *	@exception DevFailed
 */
	void	clone_user(const Tango::DevVarStringArray *);
/**
 * Check access for specified user, device, address and returns access (read or write).
 *	@param	argin	[0] - User name\n[1] - Device\n[2] - IP Address
 *	@return	cess for specified inputs.
 *	@exception DevFailed
 */
	Tango::DevString	get_access(const Tango::DevVarStringArray *);
/**
 * Returns address list  found for the specified user.
 *	@param	argin	user name.
 *	@return	Addresses found for the specified user.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_address_by_user(Tango::DevString);
/**
 * Returns allowed command list found in database for specified device
 *	It search the class of the specified device and then uses the class property <b>AllowedAccessCmd</b>
 *	@param	argin	Device name OR Device Class name
 *	@return	Allowed commands found in database for specified device
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_allowed_commands(Tango::DevString);
/**
 * Returns the class names which have AllowedAccessCmd property defined.
 *	@return	Class names which have AllowedAccessCmd property defined.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_allowed_command_class_list();
/**
 * Returns devices and rights found for the specified user.
 *	@param	argin	user name.
 *	@return	devices and rights found for the specified user.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_device_by_user(Tango::DevString);
/**
 * Returns class for specified device.
 *	@param	argin	Device name
 *	@return	Class found in database for specified device
 *	@exception DevFailed
 */
	Tango::DevString	get_device_class(Tango::DevString);
/**
 * Returns user list found in table access_address.
 *	@return	Users find in table access_address.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_users();
/**
 * Remove an address for the specified user..
 *	@param	argin	user name, address
 *	@exception DevFailed
 */
	void	remove_address_for_user(const Tango::DevVarStringArray *);
/**
 * Remove a device and its rights for the specified user..
 *	@param	argin	user name, device and value
 *	@exception DevFailed
 */
	void	remove_device_for_user(const Tango::DevVarStringArray *);
/**
 * Remove all records for specified user.
 *	@param	argin	user name
 *	@exception DevFailed
 */
	void	remove_user(Tango::DevString);
/**
 * Register device as a TANGO service.
 *	@exception DevFailed
 */
	void	register_service();
/**
 * Unregister device as a TANGO service.
 *	@exception DevFailed
 */
	void	unregister_service();

/**
 *	Read the device properties from database
 */
	 void get_device_property();
//@}

	//	Here is the end of the automatic code generation part
	//-------------------------------------------------------------

protected :	
	MYSQL mysql;

	string         removeFQDN(string s);
	void           mysql_connection();
	void           simple_query(string sql_query,const char *method);
	MYSQL_RES     *query(string sql_query,const char *method);
	vector<string> get_dev_members(string &devname);
	vector<string> get_ip_add_members(string &devname);

	vector<AccessStruct>
		get_access_for_user_address(string &user, string &ip_add);
	string get_access_for_user_device(string &user, string &device);
	int    nb_chars(string s, char c);
	int    mini_nb_stars(vector<AccessStruct> v);

	void register_service(string servicename, string instname, string devname);
	void unregister_service(string servicename, string instname, string devname);

};

}	// namespace_ns

#endif	// _TANGOACCESSCONTROL_H
