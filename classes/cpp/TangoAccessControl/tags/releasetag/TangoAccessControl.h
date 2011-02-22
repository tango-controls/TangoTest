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
// $Revision$
//
// $Log$
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
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
#define	CtrlSystem					"ControlSystem"
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

/**
 * Class Description:
 * This class defines how to manage the TANGO access control.
 *	It implements commands for tool to defines access for users, devices and IP addresses.
 *	It implements also commands used by client API to check access for specified user, device and address.
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
 *	@param	argin	Device name
 *	@return	Allowed commands found in database for specified device
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_allowed_commands(Tango::DevString);
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
	void           mysql_connection();
	void           simple_query(string sql_query,const char *method);
	MYSQL_RES     *query(string sql_query,const char *method);
	vector<string> get_dev_members(string &devname);
	vector<string> get_ip_add_members(string &devname);

	vector<AccessStruct>
		get_access_for_user_address(string &user, string &ip_add);
	string get_access_for_user_device(string &user, string &device);

	void register_service(string servicename, string instname, string devname);
	void unregister_service(string servicename, string instname, string devname);

protected :	
	MYSQL mysql; 

};

}	// namespace_ns

#endif	// _TANGOACCESSCONTROL_H
