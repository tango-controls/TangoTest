//=============================================================================
//
// file :         StarterClass.h
//
// description :  Include for the StarterClass root class.
//                This class is represents the singleton class for
//                the Starter device class.
//                It contains all properties and methods which the 
//                Starter requires only once e.g. the commands.
//			
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.2  2004/05/19 08:56:32  pascal_verdier
// Bug fixed on service mode.
// CreateProcess used under Windows.
//
// Revision 3.0.1.3  2004/02/27 09:53:02  pascal_verdier
// - The starter device is now warned by Database server when something change on a server.
// It replace the DbGetHostServersInfo polling.
// - HostState, ControlledRunningServers, ontrolledStoppedServers attributes added.
//
// Revision 3.0.1.2  2003/12/08 08:53:52  pascal_verdier
// Cluster (multi-host) control implemented.
// Control of notify daemon implemented but not tested.
//
// Revision 3.0.1.1  2003/09/18 12:02:50  pascal_verdier
// Problem on Windows service startup fixed.
//
// Revision 3.0  2003/06/17 12:06:36  pascal_verdier
// TANGO 3.x server.
// polling bugs fixed.
//
// Revision 2.0  2003/01/09 13:35:50  verdier
// TANGO 2.2
//
// Revision 1.22  2002/12/18 08:09:19  verdier
// omniORB compatibility
//
// Revision 1.6  2002/10/15 18:55:21  verdier
// The host state is now calculated during the State polled command.
//
// Revision 1.5  2002/02/27 15:19:32  verdier
// WinNt services implemented.
//
// Revision 1.4  2001/04/03 09:40:50  verdier
// WInNt version is running.
//
// Revision 1.3  2001/02/12 09:34:21  verdier
// SunOS and Linux OK.
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//             (c) - Pascal Verdier - ESRF
//=============================================================================

#ifndef _STARTERCLASS_H
#define _STARTERCLASS_H

#include <tango.h>


namespace Starter
{

//
// Define classes for commands
//
class NotifyDaemonStateCmd : public Tango::Command
{
public:
	NotifyDaemonStateCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	NotifyDaemonStateCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~NotifyDaemonStateCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class StartPollingThreadCmd : public Tango::Command
{
public:
	StartPollingThreadCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	StartPollingThreadCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~StartPollingThreadCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class UpdateServersInfoCmd : public Tango::Command
{
public:
	UpdateServersInfoCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	UpdateServersInfoCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~UpdateServersInfoCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevReadLogCmd : public Tango::Command
{
public:
	DevReadLogCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevReadLogCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevReadLogCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevStopCmd : public Tango::Command
{
public:
	DevStopCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevStopCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevStopCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevStartCmd : public Tango::Command
{
public:
	DevStartCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevStartCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevStartCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevGetStopServersCmd : public Tango::Command
{
public:
	DevGetStopServersCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevGetStopServersCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevGetStopServersCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevGetRunningServersCmd : public Tango::Command
{
public:
	DevGetRunningServersCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevGetRunningServersCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevGetRunningServersCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevStopAllCmd : public Tango::Command
{
public:
	DevStopAllCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevStopAllCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevStopAllCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevStartAllCmd : public Tango::Command
{
public:
	DevStartAllCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *, Tango::DispLevel);
	DevStartAllCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevStartAllCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};



//
// The StarterClass singleton definition
//

class StarterClass : public Tango::DeviceClass
{
public:
//	properties member data
/**
 *	Period to read database for new info.
 */
	Tango::DevShort	readInfoDbPeriod;
/**
 *	Number of startup levels managed by starter.
 */
	Tango::DevShort	nbStartupLevels;
/**
 *	Timeout value in seconds to stop polling if no command has been received.
 */
	Tango::DevLong	cmdPollingTimeout;
/**
 *	Use events if not null.
 */
	Tango::DevShort	useEvents;

//	add your own data members here
//------------------------------------

public:

//	Method prototypes
	static StarterClass *init(const char *);
	static StarterClass *instance();
	~StarterClass();
	
protected:
	StarterClass(string &);
	static StarterClass *_instance;
	void command_factory();
	void get_class_property();
	void attribute_factory(vector<Tango::Attr *> &);
	void write_class_property();

private:
	void device_factory(const Tango::DevVarStringArray *);
};


}	//	namespace Starter

#endif // _STARTERCLASS_H
