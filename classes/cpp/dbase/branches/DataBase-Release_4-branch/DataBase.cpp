static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         DataBase.cpp
//
// description :  C++ source for the DataBase and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                DataBase are implemented in this file.
//
// project :      TANGO Device Servers
//
// author(s) :    A.Gotz, JL Pons, P.Verdier
//
// $Revision$
//
// $Log$
// Revision 2.73.2.12  2008/02/28 17:07:36  taurel
// - Added the StoredProcedureRelease attribute
//
// Revision 2.73.2.11  2008/02/06 09:51:13  jlpons
// Fixed date FORMAT for history
//
// Revision 2.73.2.10  2008/02/06 07:13:18  taurel
// - Fix a bug introduced by the previous bug fix in the DbGetHostServerList
//
// Revision 2.73.2.9  2008/01/25 12:29:50  taurel
// - Add a new command DbDeleteAllDeviceAttributeProperty
// - The DbImportDevice now also returns the device class
// - Fix bug in the DbGetHostServerList command
//
// Revision 2.73.2.8  2008/01/03 08:44:49  taurel
// - Add MySQL connection pool size definition as a command line option
// called "poolSize" with a default value of 20
//
// Revision 2.73.2.7  2007/12/19 11:48:29  taurel
// - Fix several bugs in the connection management in case of error
// - Use the same connection for the get_id() method
// - Re-code the call to the stored procedure as decribed in MySQL doc
//
// Revision 2.73.2.6  2007/12/10 08:21:56  taurel
// - Add test in the DbGetDeviceInfo command to protect the code against server
// or host column being NULL
//
// Revision 2.73.2.5  2007/12/07 09:25:58  taurel
// - Fix a bug in the DbAddServer command
// - Remove some messages print-out
//
// Revision 2.73.2.4  2007/12/06 16:45:03  taurel
// - Fix some bugs in the locking system
//
// Revision 2.73.2.3  2007/12/06 08:34:21  taurel
// - Add table slock to be pretected against threading within a server and againt several servers using the same DB
//
// Revision 2.73.2.2  2007/11/08 10:02:58  taurel
// Several changes to get a fully multi-threaded Database server
// - Add interceptors for thread creation and deletion
// - Use the NO_SYNC Tango serialisation model
// - Add some mutexes to protect miscellaneous pieces of code
// - Re-write the get_id() method
// - Create a MySQL connection pool
//
// Revision 2.73.2.1  2007/11/07 09:50:39  taurel
// - First changes to get a fully-threaded DB server
//
// Revision 2.73  2007/11/06 07:24:05  taurel
// - Added the DbGetDataForServerCache command (with timing stats)
// - Add timing stats for the DbPutClassProperty command
//
// Revision 2.72  2007/06/15 06:24:30  jensmeyer
// Deleted root user for mysql as default.
// No default user is specified. The environment variable
// MYSQL_USER or the mysql configuration file my.cnf has
// to be used.
//
// Revision 2.71  2007/05/23 09:15:23  pascal_verdier
// Memory leak fixed on db_get_alias_device() method.
//
// Revision 2.70  2007/04/25 06:01:02  pascal_verdier
// IOR NULL checked in DbGetDeviceInfo() and DbImportDevice().
//
// Revision 2.69  2007/03/23 14:40:06  pascal_verdier
// Bug on MySql version compiler flag fixed.
//
// Revision 2.68  2007/02/26 13:47:13  jlpons
// Fixed a bug in db_get_class_attribute_list()
//
// Revision 2.67  2007/01/23 10:19:06  pascal_verdier
// Auto-reconnection to MySql for MySql-5 (Thanks to C. Scafuri).
//
// Revision 2.66  2006/11/06 15:31:03  jlpons
// Fixed bug when history_ids reaches 2^32.
//
// Revision 2.65  2006/11/03 16:49:34  jlpons
// Fixed bug with wildcard in db_delete_xxx_property
//
// Revision 2.64  2006/09/28 11:18:20  pascal_verdier
// DbGetClassForDevice and DbGetClassInheritanceForDevice commands added.
//
// Revision 2.63  2006/09/15 12:45:39  jlpons
// Added mysql_options() call.
//
// Revision 2.62  2006/08/29 08:49:19  jlpons
// Moved database purge to the DB server
//
// Revision 2.61  2006/08/23 08:57:34  pascal_verdier
// Header changed.
//
// Revision 2.60  2006/06/22 15:25:32  jlpons
// Added history commands
//
// Revision 2.59  2006/03/27 09:09:19  taurel
// - Fix memory leaks in db_get_device_alias() method and after usage of the
// internal db_get_device_host() method which allocates memory which was never freed. It is called by db_export_device(), db_delete_server() and db_put_server_info()
//
// Revision 2.58  2005/10/20 10:46:23  pascal_verdier
// stringstream replaced by TangoSys_MemStream.
//
// Revision 2.57  2005/10/19 08:47:02  pascal_verdier
// Bug on put_property() for multi lines value fixed.
//
// Revision 2.56  2005/10/05 07:35:54  pascal_verdier
// Bug fixed in Timing_Info attribute read.
// ResetTimingValues command added.
// Windows compatibility added for timing attributes.
//
// Revision 2.55  2005/10/04 06:22:39  pascal_verdier
// Fixe a bug appeared with MySql 5.0
//
// Revision 2.54  2005/10/03 16:27:18  nleclercq
// // sql_query.erase(sql_query.size()) replaced by sql_query.erase() for windows compatibility.
//
// Revision 2.53  2005/09/26 10:40:20  pascal_verdier
// sql_query.clear() replaced by sql_query.erase(sql_query.size()) for windows compatibility.
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
//
//  This file has been partially generated by POGO
//
//                     (c) - Pascal Verdier - ESRF
//
//=============================================================================


//===================================================================
//
//	The following table gives the correspondence
//	between commands and method name.
//
//  Command name                        |  Method name
//	----------------------------------------
//  State                               |  dev_state()
//  Status                              |  dev_status()
//  DbAddDevice                         |  db_add_device()
//  DbAddServer                         |  db_add_server()
//  DbDeleteAttributeAlias              |  db_delete_attribute_alias()
//  DbDeleteClassAttribute              |  db_delete_class_attribute()
//  DbDeleteClassAttributeProperty      |  db_delete_class_attribute_property()
//  DbDeleteClassProperty               |  db_delete_class_property()
//  DbDeleteDevice                      |  db_delete_device()
//  DbDeleteDeviceAlias                 |  db_delete_device_alias()
//  DbDeleteDeviceAttribute             |  db_delete_device_attribute()
//  DbDeleteDeviceAttributeProperty     |  db_delete_device_attribute_property()
//  DbDeleteDeviceProperty              |  db_delete_device_property()
//  DbDeleteProperty                    |  db_delete_property()
//  DbDeleteServer                      |  db_delete_server()
//  DbDeleteServerInfo                  |  db_delete_server_info()
//  DbExportDevice                      |  db_export_device()
//  DbExportEvent                       |  db_export_event()
//  DbGetAliasDevice                    |  db_get_alias_device()
//  DbGetAttributeAlias                 |  db_get_attribute_alias()
//  DbGetAttributeAliasList             |  db_get_attribute_alias_list()
//  DbGetClassAttributeList             |  db_get_class_attribute_list()
//  DbGetClassAttributeProperty         |  db_get_class_attribute_property()
//  DbGetClassAttributeProperty2        |  db_get_class_attribute_property2()
//  DbGetClassAttributePropertyHist     |  db_get_class_attribute_property_hist()
//  DbGetClassForDevice                 |  db_get_class_for_device()
//  DbGetClassInheritanceForDevice      |  db_get_class_inheritance_for_device()
//  DbGetClassList                      |  db_get_class_list()
//  DbGetClassProperty                  |  db_get_class_property()
//  DbGetClassPropertyHist              |  db_get_class_property_hist()
//  DbGetClassPropertyList              |  db_get_class_property_list()
//  DbGetDeviceAlias                    |  db_get_device_alias()
//  DbGetDeviceAliasList                |  db_get_device_alias_list()
//  DbGetDeviceAttributeList            |  db_get_device_attribute_list()
//  DbGetDeviceAttributeProperty        |  db_get_device_attribute_property()
//  DbGetDeviceAttributeProperty2       |  db_get_device_attribute_property2()
//  DbGetDeviceAttributePropertyHist    |  db_get_device_attribute_property_hist()
//  DbGetDeviceClassList                |  db_get_device_class_list()
//  DbGetDeviceDomainList               |  db_get_device_domain_list()
//  DbGetDeviceExportedList             |  db_get_device_exported_list()
//  DbGetDeviceFamilyList               |  db_get_device_family_list()
//  DbGetDeviceInfo                     |  db_get_device_info()
//  DbGetDeviceList                     |  db_get_device_list()
//  DbGetDeviceMemberList               |  db_get_device_member_list()
//  DbGetDeviceProperty                 |  db_get_device_property()
//  DbGetDevicePropertyHist             |  db_get_device_property_hist()
//  DbGetDevicePropertyList             |  db_get_device_property_list()
//  DbGetDeviceServerClassList          |  db_get_device_server_class_list()
//  DbGetExportdDeviceListForClass      |  db_get_exportd_device_list_for_class()
//  DbGetHostList                       |  db_get_host_list()
//  DbGetHostServerList                 |  db_get_host_server_list()
//  DbGetHostServersInfo                |  db_get_host_servers_info()
//  DbGetInstanceNameList               |  db_get_instance_name_list()
//  DbGetObjectList                     |  db_get_object_list()
//  DbGetProperty                       |  db_get_property()
//  DbGetPropertyHist                   |  db_get_property_hist()
//  DbGetPropertyList                   |  db_get_property_list()
//  DbGetServerInfo                     |  db_get_server_info()
//  DbGetServerList                     |  db_get_server_list()
//  DbGetServerNameList                 |  db_get_server_name_list()
//  DbImportDevice                      |  db_import_device()
//  DbImportEvent                       |  db_import_event()
//  DbInfo                              |  db_info()
//  DbPutAttributeAlias                 |  db_put_attribute_alias()
//  DbPutClassAttributeProperty         |  db_put_class_attribute_property()
//  DbPutClassAttributeProperty2        |  db_put_class_attribute_property2()
//  DbPutClassProperty                  |  db_put_class_property()
//  DbPutDeviceAlias                    |  db_put_device_alias()
//  DbPutDeviceAttributeProperty        |  db_put_device_attribute_property()
//  DbPutDeviceAttributeProperty2       |  db_put_device_attribute_property2()
//  DbPutDeviceProperty                 |  db_put_device_property()
//  DbPutProperty                       |  db_put_property()
//  DbPutServerInfo                     |  db_put_server_info()
//  DbUnExportDevice                    |  db_un_export_device()
//  DbUnExportEvent                     |  db_un_export_event()
//  DbUnExportServer                    |  db_un_export_server()
//  ResetTimingValues                   |  reset_timing_values()
//  DbGetDataForServerCache             |  db_get_data_for_server_cache()
//  DbDeleteAllDeviceAttributeProperty  |  db_delete_all_device_attribute_property()
//
//===================================================================


#include <tango.h>
#include <DataBase.h>
#include <DataBaseClass.h>
#include <mysql.h>
#include <stdio.h>


using namespace std;

namespace DataBase_ns {

string DataBase::db_name("sys/database/1");

//+----------------------------------------------------------------------------
//
// method : 		DataBase::DataBase(string &s)
// 
// description : 	constructors for TANGO Database device server
//
// in : - cl : Pointer to the DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------
DataBase::DataBase(Tango::DeviceClass *cl,string &s):Device_3Impl(cl,s.c_str())
{
	init_device();
}

DataBase::DataBase(Tango::DeviceClass *cl,const char *s):Device_3Impl(cl,s)
{
	init_device();
}

DataBase::DataBase(Tango::DeviceClass *cl,const char *s,const char *d)
:Device_3Impl(cl,s,d)
{
	init_device();
}

void DataBase::init_device()
{
	char *mysql_user     = NULL;
	char *mysql_password = NULL;	

	WARN_STREAM << "DataBase::DataBase() create database device " << device_name << endl;
	
//
// Check if we are using the thread safe release of the MySQL library
//

	if (mysql_thread_safe() == 0)
	{
		ERROR_STREAM << "MySQL library used by this process is not tread safe. Please, use libmysqlclient_r" << endl;

		Tango::Except::throw_exception((const char *)DB_MySQLLibNotThreadSafe,
	   				  (const char *)"MySQL library used by this process is not thread safe. Please, use libmysqlclient_r or use DataBase release < 4.x",
					  (const char *)"DataBase::DataBase()");
	}

//
// Get user environment variables if defined
//

	if (getenv("MYSQL_USER") != NULL)
	{
		mysql_user = getenv("MYSQL_USER");
	}
	if (getenv("MYSQL_PASSWORD") != NULL)
	{
		mysql_password = getenv("MYSQL_PASSWORD");
	}
	
//
// Create the connection pool after some initialisation
//

	conn_pool = new DbConnection[conn_pool_size];
	for (int loop = 0;loop < conn_pool_size;loop++)
		conn_pool[loop].db = NULL;
	mysql_svr_version = 0;
	
	create_connection_pool(mysql_user,mysql_password);
	
//
// Do we need to propagate info to Starter
//
	
	try
	{
		//	Check if controlled server list modification must be fired to starter
		Tango::DevVarStringArray *argin = new Tango::DevVarStringArray();
		argin->length(2);
		(*argin)[0] = CORBA::string_dup("Default");
		(*argin)[1] = CORBA::string_dup("FireToStarter");
		Tango::DevVarStringArray *argout = db_get_property(argin);

		if ((*argout)[3] == 0)	//	Not defined
			fireToStarter = true;
		else
		{
			//	Get property value
			string	value((*argout)[4]);
			transform(value.begin(), value.end(), value.begin(), ::tolower);
			if (value=="false")	
				fireToStarter = false;
			else
				fireToStarter = true;
		}
		delete argin;
		delete argout;
	}
	catch(Tango::DevFailed &)
	{
		fireToStarter = true;
	}
	WARN_STREAM << "fireToStarter = " << fireToStarter << endl;

	//	If fire to starter is true
	if (fireToStarter==true)
	{
		//	Build shared data and thread to update Starter in case of
		//	change of controlled servers conditions
		starter_shared = new UpdStarterData();
		upd_starter_thread = new UpdateStarter(starter_shared);
		upd_starter_thread->start();
	}
	
	// Load history depth property
	historyDepth = 10;
	try
	{
		Tango::DevVarStringArray *argin = new Tango::DevVarStringArray();
		argin->length(2);
		(*argin)[0] = CORBA::string_dup(get_name().c_str());
		(*argin)[1] = CORBA::string_dup("historyDepth");
		Tango::DevVarStringArray *argout = db_get_device_property(argin);

		if ((*argout)[3] != 0)
		{
		  if(strcmp((*argout)[4]," ")!=0) {
		    historyDepth = atoi( (*argout)[4] );
		    if( historyDepth == 0 ) {
		      cout << "Warning, Invalid historyDepth property, reseting to default value (10)" << endl;
		      historyDepth = 10;
		    }
		  }
		}
		delete argin;
		delete argout;
	}
	catch(Tango::DevFailed &)
	{}
			
	// Check history tables
	check_history_tables();
	
	init_timing_stats();
	attr_StoredProcedureRelease_read = &stored_release;
	set_state(Tango::ON);
	set_status("Device is OK");
		
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::delete_device()
// 
// description : 	will be called at device destruction or at init command.
//
//-----------------------------------------------------------------------------
void DataBase::delete_device()
{
	//	Delete device's allocated object
	
	for (int loop = 0;loop < conn_pool_size;loop++)
	{
		if (conn_pool[loop].db != NULL)
			mysql_close(conn_pool[loop].db);
	}
	delete [] conn_pool;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------
void DataBase::always_executed_hook()
{

	INFO_STREAM << "In always_executed_hook method" << endl;

}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_attr_hardware
// 
// description : 	Hardware acquisition for attributes.
//
//-----------------------------------------------------------------------------
void DataBase::read_attr_hardware(vector<long> &attr_list)
{
	DEBUG_STREAM << "DataBase::read_attr_hardware(vector<long> &attr_list) entering... "<< endl;
	//	Add your own code here
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_StoredProcedureRelease
// 
// description : 	Extract real attribute values for StoredProcedureRelease acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_StoredProcedureRelease(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_StoredProcedureRelease(Tango::Attribute &attr) entering... "<< endl;

	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_fields, i;

	sql_query_stream << "SHOW PROCEDURE STATUS LIKE \"ds_start\"";

	DEBUG_STREAM << "DataBase::read_StoredProcedureRelease(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"read_StoredProcedureRelease()");

	n_fields = mysql_num_fields(result);
	DEBUG_STREAM << "DataBase::read_StoreProcedureRelease(): mysql_num_fields() " << n_fields << endl;

	if (n_fields == 8)
	{
		unsigned long *lengths;
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			lengths = mysql_fetch_lengths(result);
			stored_release = new char [lengths[7] + 1];
			strcpy(stored_release,row[7]);
		}
		else
		{
	   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"Can't fetch row from the returned result",
					  (const char *)"DataBase::read_StoredProcedureRelease()");
		}
	}
	else
	{
	   	Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"Wrong fields number in the data returned by the query",
					  (const char *)"DataBase::read_StoredProcedureRelease()");
	}
	
	mysql_free_result(result);
	
	attr.set_value(attr_StoredProcedureRelease_read,1,0,true);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_Timing_info
// 
// description : 	Extract real attribute values for Timing_info acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_Timing_info(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_Timing_info(Tango::Attribute &attr) entering... "<< endl;

	char info_str[256];
	char hostname[256];
	Tango::DevString *timing_info = new Tango::DevString[timing_stats_size+4];

	gethostname(hostname, sizeof(hostname));
	sprintf(info_str,"TANGO Database Timing info on host %s",hostname);
	timing_info[0] = CORBA::string_dup(info_str);
// newline
	timing_info[1] = CORBA::string_dup(" ");
	timing_info[2] = CORBA::string_dup("command	average	minimum	maximum	calls");
 	timing_info[3] = CORBA::string_dup(" ");
 
    std::map<std::string,TimingStatsStruct*>::iterator iter;
    int i=0;
    for (iter = timing_stats_map.begin(); iter != timing_stats_map.end(); iter++)
    {
    	sprintf(info_str,"%s\t%6.3f\t%6.3f\t%6.3f\t%.0f",
				iter->first.c_str(), iter->second->average,
    			iter->second->minimum, iter->second->maximum,
				iter->second->calls);
        timing_info[i+4] = CORBA::string_dup(info_str);
        i++;
    }
	attr.set_value(timing_info,timing_stats_size+4,0,true);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_Timing_average
// 
// description : 	Extract real attribute values for Timing_average acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_Timing_average(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_Timing_average(Tango::Attribute &attr) entering... "<< endl;

	std::map<std::string,TimingStatsStruct*>::iterator iter;
	int i=0;

	for (iter = timing_stats_map.begin(); iter != timing_stats_map.end(); iter++)
	{
		timing_stats_average[i] = iter->second->average;
		i++;
	}

	attr.set_value(timing_stats_average, timing_stats_size);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_Timing_minimum
// 
// description : 	Extract real attribute values for Timing_minimum acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_Timing_minimum(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_Timing_minimum(Tango::Attribute &attr) entering... "<< endl;
	std::map<std::string,TimingStatsStruct*>::iterator iter;
	int i=0;

	for (iter = timing_stats_map.begin(); iter != timing_stats_map.end(); iter++)
	{
		timing_stats_minimum[i] = iter->second->minimum;
		i++;
	}

	attr.set_value(timing_stats_minimum, timing_stats_size);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_Timing_maximum
// 
// description : 	Extract real attribute values for Timing_maximum acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_Timing_maximum(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_Timing_maximum(Tango::Attribute &attr) entering... "<< endl;
	std::map<std::string,TimingStatsStruct*>::iterator iter;
	int i=0;

	for (iter = timing_stats_map.begin(); iter != timing_stats_map.end(); iter++)
	{
		timing_stats_maximum[i] = iter->second->maximum;
		i++;
	}

	attr.set_value(timing_stats_maximum, timing_stats_size);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_Timing_calls
// 
// description : 	Extract real attribute values for Timing_calls acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_Timing_calls(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_Timing_calls(Tango::Attribute &attr) entering... "<< endl;
	std::map<std::string,TimingStatsStruct*>::iterator iter;
	int i=0;

	for (iter = timing_stats_map.begin(); iter != timing_stats_map.end(); iter++)
	{
		timing_stats_calls[i] = iter->second->calls;
		i++;
	}

	attr.set_value(timing_stats_calls, timing_stats_size);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::read_Timing_index
// 
// description : 	Extract real attribute values for Timing_index acquisition result.
//
//-----------------------------------------------------------------------------
void DataBase::read_Timing_index(Tango::Attribute &attr)
{
	DEBUG_STREAM << "DataBase::read_Timing_index(Tango::Attribute &attr) entering... "<< endl;

	attr.set_value(timing_stats_index, timing_stats_size);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::state_cmd()
// 
// description : 	command to read the device state
//
// out :		device state
//
//-----------------------------------------------------------------------------
Tango::DevState DataBase::dev_state()
{

	INFO_STREAM << "In DataBase state command" << endl;
	
	return device_state;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::status_cmd()
// 
// description : 	command to read the device status
//
// out :		device status
//
//-----------------------------------------------------------------------------
const char* DataBase::dev_status()
{

	INFO_STREAM << "In DataBase status command" << endl;

	return device_status.c_str();
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_object_list
 *
 *	description:	method to execute "DbGetObjectList"
 *	DataBase methods prototypes
 *
 * @param	argin	wild card
 * @return	list of object names
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_object_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_wildcard;

	INFO_STREAM << "DataBase::db_get_object_list(): object " << wildcard << endl;

	tmp_wildcard = replace_wildcard(wildcard);
	sql_query_stream << "SELECT DISTINCT object FROM property WHERE object LIKE \"" 
	                 << tmp_wildcard << "\" ORDER BY object";

	DEBUG_STREAM << "DataBase::db_get_object_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_object_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_object_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *object_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   object_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
//	         DEBUG_STREAM << "DataBase::db_get_object_list(): object[ "<< i << "] object " << row[0] << endl;
	         (*object_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		object_list->length(0);
	mysql_free_result(result);

	return(object_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_property_list
 *
 *	description:	method to execute "DbGetPropertyList"
 *	DataBase methods prototypes
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_property_list(const Tango::DevVarStringArray *object_wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *object, *wildcard;
	string tmp_wildcard;

	if (object_wildcard->length() != 2)
	{
	   WARN_STREAM << "DataBase::db_get_property_list(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 2 (object,wildcard)",
					  (const char *)"DataBase::GetPropertyList()");
	}
	object = (*object_wildcard)[0];
	wildcard = (*object_wildcard)[1];

	INFO_STREAM << "DataBase::db_get_property_list(): object " << object << endl;

	if (object == NULL)
	{
		sql_query_stream << "SELECT DISTINCT name FROM property WHERE object LIKE \"%\" ORDER BY name";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT name FROM property WHERE object LIKE \"" << object << 
		                    "\" AND name LIKE \"" << tmp_wildcard << "\" ORDER BY name";
	}
	DEBUG_STREAM << "DataBase::db_get_property_list(): sql_query " << sql_query_stream.str() << endl;

    result = query( sql_query_stream.str() , "db_get_property_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_property_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   property_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_property_list(): property[ "<< i << "] property " << row[0] << endl;
	         (*property_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		property_list->length(0);
	mysql_free_result(result);

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_property
 *
 *	description:	method to execute "DbGetProperty"
 *	DataBase methods prototypes
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_property(const Tango::DevVarStringArray *property_names)
{
	TangoSys_MemStream	sql_query_stream;
	char n_properties_str[256];
	char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_object;
	string tmp_name;

	INFO_STREAM << "DataBase::db_get_property(): get " << property_names->length()-1 << " properties for object " << (*property_names)[0] << endl;

	tmp_object = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	property_list->length(2);
	(*property_list)[0] = CORBA::string_dup(tmp_object);
	(*property_list)[1] = CORBA::string_dup(n_properties_str);
        n_props = 2;

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_name = replace_wildcard((*property_names)[i]);
	   sql_query_stream.str("");
	   sql_query_stream << "SELECT count,value,name FROM property WHERE object = \"" << tmp_object << 
	                       "\" AND name LIKE \"" << tmp_name << "\" ORDER BY count";
	   DEBUG_STREAM << "DataBase::db_get_property(): sql_query " << sql_query_stream.str() << endl;
	   
	   result = query(sql_query_stream.str(),"db_get_property()");

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::db_get_property(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup((*property_names)[i]);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {
	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::db_get_property(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
		    	n_props++;
	   	    	property_list->length(n_props);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_properties += n_rows;
	   }
	   else
	   {
	      n_props++;
	      property_list->length(n_props);
	      (*property_list)[n_props-1] = CORBA::string_dup(" ");
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::db_get_property(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_property
 *
 *	description:	method to execute "DbPutProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream	sql_query_stream;
	int n_properties=0, n_rows=0;
	const char *tmp_object;
	int	tmp_count;
	string tmp_name;

	tmp_object = (*property_list)[0];
	sscanf((*property_list)[1],"%d", &n_properties);
	INFO_STREAM << "DataBase::db_put_property(): put " << n_properties << " properties for object " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property WRITE, property_hist WRITE,object_history_id WRITE",this);
		
		int	k = 2;
		for (int i=0; i<n_properties; i++)
		{
			tmp_count = 0;
			tmp_name = (*property_list)[k];
			sscanf((*property_list)[k+1], "%d", &n_rows);

			// first delete the property from the property table
			sql_query_stream.str("");
			sql_query_stream << "DELETE FROM property WHERE object=\"" << tmp_object
											 << "\" AND name=\"" << tmp_name << "\"";
			DEBUG_STREAM  << "DataBase::db_put_property(): sql_query " << sql_query_stream.str() << endl;

			simple_query(sql_query_stream.str(),"db_put_property()",al.get_con_nb());
			unsigned int object_property_hist_id = get_id("object",al.get_con_nb());
	    	for (int j=k+2 ; j<k+n_rows+2 ; j++)
	    	{ 
        	  string tmp_escaped_string = escape_string((*property_list)[j]);
	    	  tmp_count++;

			  // then insert the new value for this tuple
			  sql_query_stream.str("");
			  sql_query_stream << "INSERT INTO property SET object='" << tmp_object
											 << "',name='" << tmp_name
											 << "',count='" << tmp_count
											 << "',value='" << tmp_escaped_string
											 << "',updated=NULL,accessed=NULL";
	    	  DEBUG_STREAM << "DataBase::db_put_property(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_put_property()",al.get_con_nb());

			  // then insert the new value into history table
			  sql_query_stream.str("");
			  sql_query_stream << "INSERT INTO property_hist SET object='" << tmp_object
											 << "',name='" << tmp_name
											 << "',count='" << tmp_count
											 << "',id='" << object_property_hist_id
											 << "',value='" << tmp_escaped_string << "'";
	    	  DEBUG_STREAM << "DataBase::db_put_property(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_put_property()",al.get_con_nb());
			}
			purge_property("property_hist","object",tmp_object,tmp_name.c_str(),al.get_con_nb());
			k = k+n_rows+2;
		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_property
 *
 *	description:	method to execute "DbDeleteProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream	sql_query_stream;
	int n_properties=0, i,j;
	const char *tmp_object;
	string tmp_name;
	MYSQL_RES *result;
	MYSQL_ROW row;

	n_properties = property_list->length() - 1;
	INFO_STREAM << "DataBase::db_delete_property(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property WRITE, property_hist WRITE,object_history_id WRITE",this);
		
		for (i=0; i<n_properties; i++)
		{
	    	  tmp_object = (*property_list)[0];
	    	  tmp_name = replace_wildcard((*property_list)[i+1]);

			  // Is there something to delete ?

        	  sql_query_stream.str("");
	    	  sql_query_stream << "SELECT DISTINCT name FROM property WHERE object=\"" << tmp_object 
		                	   << "\" AND name LIKE \"" << tmp_name << "\"";
			  result = query(sql_query_stream.str(),"db_delete_property()",al.get_con_nb());
			  int count = mysql_num_rows(result);

			  for (j=0;j<count;j++) {

				row = mysql_fetch_row(result);

				// delete the tuple (object,name,count) from the property table

		    	sql_query_stream.str("");
		    	sql_query_stream << "DELETE FROM property WHERE object=\"" << tmp_object 
		                	   << "\" AND name = \"" << row[0] << "\"";
	        	DEBUG_STREAM << "DataBase::db_delete_property(): sql_query " << sql_query_stream.str() << endl;
		    	simple_query(sql_query_stream.str(),"db_delete_property()",al.get_con_nb());

				// Mark this property as deleted

		    	unsigned int object_property_hist_id = get_id("object",al.get_con_nb());
		    	sql_query_stream.str("");
		    	sql_query_stream << "INSERT INTO property_hist SET object='" << tmp_object
											 << "',name='" << row[0]
											 << "',id='" << object_property_hist_id
											 << "',count='0',value='DELETED'";
	        	DEBUG_STREAM << "DataBase::db_put_property(): sql_query " << sql_query_stream.str() << endl;
		    	simple_query(sql_query_stream.str(),"db_delete_property()",al.get_con_nb());

            	purge_property("property_hist","object",tmp_object,row[0],al.get_con_nb());

			  }

  	    	  mysql_free_result(result);

		}
	}

	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_host_server_list
 *
 *	description:	method to execute "DbGetHostServerList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_host_server_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::db_get_host_server_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT server FROM device WHERE host LIKE \"%\" ORDER BY server";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		//	For compatibility reason between before and after Tang-5.2
		//	Will check with and without Fully Qualify Domain Name.
		sql_query_stream << "SELECT DISTINCT server FROM device WHERE (host LIKE \"" << tmp_wildcard 
		                 << "\" or host LIKE \"" << tmp_wildcard << ".%%\") AND name LIKE \"dserver/%%\" ORDER BY server";
	}
	DEBUG_STREAM << "DataBase::db_get_host_server_list(): sql_query " << sql_query_stream.str() << endl;

	result = query(sql_query_stream.str(),"db_get_host_server_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_host_server_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *server_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   server_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_host_server_list(): row[ "<< i << "] alias " << row[0] << endl;
	         (*server_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		server_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetHostServerList");
	return(server_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_host_list
 *
 *	description:	method to execute "DbGetHostList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_host_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::db_get_host_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT host FROM device WHERE host LIKE \"%\" ORDER BY host";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT host FROM device WHERE host LIKE \"" << tmp_wildcard 
		                 << "\" ORDER BY host";
	}
	DEBUG_STREAM << "DataBase::db_get_host_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_host_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_host_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *host_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   host_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_host_list(): host[ "<< i << "] alias " << row[0] << endl;
	         (*host_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		host_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetHostList");
	return(host_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_server_list
 *
 *	description:	method to execute "DbGetServerList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_server_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::db_get_server_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT server FROM device WHERE server LIKE \"%\" ORDER BY server";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT server FROM device WHERE server LIKE \"" 
		                 << tmp_wildcard << "\" ORDER BY server";
	}
	DEBUG_STREAM << "DataBase::db_get_server_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_server_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_server_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *server_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   server_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
//	         DEBUG_STREAM << "DataBase::db_get_server_list(): server[ "<< i << "] alias " << row[0] << endl;
	         (*server_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		server_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetServerList");
	return(server_list);
}

bool nocase_compare(char c1, char c2)
{	
	return toupper(c1) == toupper(c2);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_server_name_list
 *
 *	description:	method to execute "DbGetServerNameList"
 *	Returns the list of server names found for the wildcard specified.
 *	It returns only the server executable name without instance name as DbGetServerList.
 *
 * @param	argin	wildcard for server names.
 * @return	server names found.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_server_name_list(Tango::DevString wildcard)
{
	DEBUG_STREAM << "DataBase::db_get_server_name_list(): entering... !" << endl;
	Tango::DevVarStringArray *server_list = db_get_server_list(wildcard);
  
  int i;
	vector<string>	server_names;

	for (i = 0 ; i<server_list->length() ; i++)
	{
		//	Take only server name
		string	str((*server_list)[i]);
		int	idx;
		if ((idx=str.find("/"))!= string::npos)
		{
			str = str.substr(0, idx);
			//	Search if already in vector
			bool	found = false;
			for(int j=0 ; j<server_names.size() && !found ; j++)
			{
				string	name(server_names[j]);
				//	compare without case sensitive
				found = (name.size() ==  str.size()  &&   //	ensure same size
						equal (name.begin(), name.end(),  //	server name from vector
								str.begin(),              //	server name from db
								nocase_compare));         //	case insensitive
			}
			//	if not already in vector, put in
			if (!found)
				server_names.push_back(str);
		}
	}

	//	copy result vector to arg out.
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray;
	argout->length(server_names.size());
	for (i = 0 ; i<server_names.size() ; i++)
	{
		string	name(server_names[i]);
		(*argout)[i] = CORBA::string_dup(name.c_str());
	}

	delete server_list;
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_instance_name_list
 *
 *	description:	method to execute "DbGetInstanceNameList"
 *	Returns the instance names found for specified server.
 *
 * @param	argin	server name
 * @return	The instance names found for specified server.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_instance_name_list(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_get_instance_name_list(): entering... !" << endl;

	//	Build a wildcard wing arg in
	char	*wildcard = new char[strlen(argin) + 3];
	strcpy(wildcard, argin);
	strcat(wildcard, "/*");
	Tango::DevVarStringArray *server_list = db_get_server_list(wildcard);

  int i;
	vector<string>	instance_names;

	for (i = 0 ; i<server_list->length() ; i++)
	{
		//	Take only server name
		string	str((*server_list)[i]);
		int	idx;
		if ((idx=str.find("/"))!= string::npos)
		{
			str = str.substr(idx+1);
			instance_names.push_back(str);
		}
	}
	//	copy result vector to arg out.
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray;
	argout->length(instance_names.size());
	for (i = 0 ; i<instance_names.size() ; i++)
	{
		string	name(instance_names[i]);
		(*argout)[i] = CORBA::string_dup(name.c_str());
	}
	delete wildcard;
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_property_list
 *
 *	description:	method to execute "DbGetDevicePropertyList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_property_list(const Tango::DevVarStringArray *device_wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *device,*wildcard;
	string tmp_wildcard;

	TimeVal	before, after;
	GetTime(before);


	device = (*device_wildcard)[0];
	wildcard = (*device_wildcard)[1];
	INFO_STREAM << "DataBase::db_get_device_property_list(): device " << device ;
	INFO_STREAM << " wildcard " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT name FROM property_device WHERE device=\"" << device 
		                 << "\" AND name LIKE \"%\" AND count=1 ORDER BY name";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT name FROM property_device WHERE device=\"" << device 
		                 << "\" AND name LIKE \"" << tmp_wildcard << "\" AND count=1 ORDER BY name";
	}
	DEBUG_STREAM << "DataBase::db_get_device_property_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_property_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_property_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   property_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_property_list(): property[ "<< i << "] alias " << row[0] << endl;
	         (*property_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		property_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDevicePropertyList");
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_property_list
 *
 *	description:	method to execute "DbGetClassPropertyList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_property_list(Tango::DevString class_name)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::db_get_class_property_list(): class " << class_name << endl;

	if (class_name == NULL)
	{
		sql_query_stream << "SELECT DISTINCT name FROM property_class WHERE class LIKE \"%\" ORDER BY name";
	}
	else
	{
		sql_query_stream << "SELECT DISTINCT name FROM property_class WHERE class LIKE \"" 
		                 << class_name << "\" ORDER BY name";
	}
	DEBUG_STREAM << "DataBase::db_get_class_property_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_class_property_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_class_property_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   property_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_class_property_list(): property[ "<< i << "] alias " << row[0] << endl;
	         (*property_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		property_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetClassPropertyList");
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_member_list
 *
 *	description:	method to execute "DbGetDeviceMemberList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_member_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::db_get_device_member_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT member FROM device WHERE name LIKE \"%\" ORDER BY member";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT member FROM device WHERE name LIKE \"" 
		                 << tmp_wildcard << "\" OR alias LIKE \"" << tmp_wildcard 
						 << "\" ORDER BY member";
	}
	DEBUG_STREAM << "DataBase::db_get_device_member_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_member_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_member_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *member_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   member_list->length(n_rows);
	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_member_list(): member[ "<< i << "] " << row[0] << endl;
	         (*member_list)[i] = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		member_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceMemberList");
	return(member_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_family_list
 *
 *	description:	method to execute "DbGetDeviceFamilyList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_family_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::db_get_device_family_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT family FROM device WHERE name LIKE \"%\" ORDER BY family";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT family FROM device WHERE name LIKE \"" 
		                 << tmp_wildcard << "\" OR alias LIKE \"" << tmp_wildcard 
						 << "\" ORDER BY family";
	}
	DEBUG_STREAM << "DataBase::db_get_device_family_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_family_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_family_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *family_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   family_list->length(n_rows);
	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_family_list(): family[ "<< i << "] " << row[0] << endl;
	         (*family_list)[i] = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		family_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceFamilyList");
	return(family_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_domain_list
 *
 *	description:	method to execute "DbGetDeviceDomainList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_domain_list(Tango::DevString wildcard)
{
	TangoSys_MemStream	sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::db_get_device_domain_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT domain FROM device WHERE name LIKE \"%\" ORDER BY domain";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT domain FROM device WHERE name LIKE \"" 
		                 << tmp_wildcard << "\" OR alias LIKE \"" << tmp_wildcard 
						 << "\" ORDER BY domain";
	}
	DEBUG_STREAM << "DataBase::db_get_device_domain_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_domain_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_domain_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *domain_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   domain_list->length(n_rows);
	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
//	         DEBUG_STREAM << "DataBase::db_get_device_domain_list(): domain[ "<< i << "] " << row[0] << endl;
	         (*domain_list)[i] = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		domain_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceDomainList");
	return(domain_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_info
 *
 *	description:	method to execute "DbInfo"
 *
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_info()
{
	TangoSys_MemStream	sql_query_stream;
	char info_str[256];
	char info2_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0, n_infos=0;
	Tango::DevVarStringArray *info_list = new Tango::DevVarStringArray;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::db_info(): get general database infos" << endl;

	sprintf(info_str,"TANGO Database %s",DataBase::db_name.c_str());
	n_infos = 1;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// newline

	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(" ");

// get start time of database

	sql_query_stream << "SELECT started FROM device WHERE name = \"" << DataBase::db_name << "\" ";
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query_stream.str() << endl;

    result = query(sql_query_stream.str(),"db_info()");

	sprintf(info_str,"Running since ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): database started " << row[0] << endl;
	      sprintf(info_str,"Running since %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// newline

	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(" ");

// get number of devices defined
	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM device ";
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query_stream.str() << endl;

    result = query(sql_query_stream.str(),"db_info()");

	sprintf(info_str,"Devices defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of devices " << row[0] << endl;
	      sprintf(info_str,"Devices defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of devices exported

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM device WHERE exported = 1 ";
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query_stream.str() << endl;

    result = query(sql_query_stream.str(),"db_info()");

	sprintf(info_str,"Devices exported ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of devices exported " << row[0] << endl;
	      sprintf(info_str,"Devices exported  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of devices servers defined

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM device WHERE class = \"DServer\" ";
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query_stream.str() << endl;

    result = query(sql_query_stream.str(),"db_info()");

	sprintf(info_str,"Device servers defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of device servers defined " << row[0] << endl;
	      sprintf(info_str,"Device servers defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of devices servers exported

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM device WHERE class = \"DServer\" AND exported = 1 ";
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query_stream.str() << endl;

    result = query(sql_query_stream.str(),"db_info()");

	sprintf(info_str,"Device servers exported ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of device servers exported " << row[0] << endl;
	      sprintf(info_str,"Device servers exported  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// newline

	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(" ");

// -------------------------------------------------------------------------------------

// get number of device properties 

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_device ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info_str,"Device properties defined  = %s",row[0]);
	mysql_free_result(result);
	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_device_hist ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info2_str," [History lgth = %s]",row[0]);
	strcat(info_str,info2_str);
	mysql_free_result(result);	
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of class properties 

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_class ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info_str,"Class properties defined  = %s",row[0]);
	mysql_free_result(result);
	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_class_hist ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info2_str," [History lgth = %s]",row[0]);
	strcat(info_str,info2_str);
	mysql_free_result(result);	
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of device attribute properties 

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_attribute_device ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info_str,"Device attribute properties defined  = %s",row[0]);
	mysql_free_result(result);
	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_attribute_device_hist ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info2_str," [History lgth = %s]",row[0]);
	strcat(info_str,info2_str);
	mysql_free_result(result);	
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of class attribute properties 

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_attribute_class ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info_str,"Class attribute properties defined  = %s",row[0]);
	mysql_free_result(result);
	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_attribute_class_hist ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info2_str," [History lgth = %s]",row[0]);
	strcat(info_str,info2_str);
	mysql_free_result(result);	
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of object properties 

	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info_str,"Object properties defined  = %s",row[0]);
	mysql_free_result(result);
	sql_query_stream.str("");
	sql_query_stream << "SELECT COUNT(*) FROM property_hist ";
    result = query(sql_query_stream.str(),"db_info()");
    row = mysql_fetch_row(result);
    sprintf(info2_str," [History lgth = %s]",row[0]);
	strcat(info_str,info2_str);
	mysql_free_result(result);	
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);
	
	DEBUG_STREAM << "DataBase::db_info(): info_list->length() "<< info_list->length() << endl;

	GetTime(after);
	update_timing_stats(before, after, "DbInfo");
	return(info_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_list
 *
 *	description:	method to execute "DbGetDeviceList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_list(const Tango::DevVarStringArray *server_class)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_server;
	string tmp_class;

	if (server_class->length() != 2)
	{
	   WARN_STREAM << "DataBase::db_get_device_list(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 2 (server,class)",
					  (const char *)"DataBase::GetDeviceList()");
	}
		
	tmp_server = replace_wildcard((*server_class)[0]);
	tmp_class = replace_wildcard((*server_class)[1]);

	INFO_STREAM << "DataBase::GetClassList(): server " << tmp_server << endl;

	sql_query_stream << "SELECT DISTINCT name FROM device WHERE server LIKE \"" 
	                 << tmp_server << "\" AND class LIKE \"" << tmp_class << "\" ORDER BY name";
	DEBUG_STREAM << "DataBase::GetDeviceList(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::GetDeviceList(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::GetdeviceList(): row[ "<< i << "] name " << row[0] << endl;
	         (*device_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		device_list->length(0);
	mysql_free_result(result);

	return(device_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_list
 *
 *	description:	method to execute "DbGetClassList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_list(Tango::DevString server)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_server;

	INFO_STREAM << "DataBase::db_get_class_list(): server " << server << endl;

	tmp_server = replace_wildcard(server);
	sql_query_stream << "SELECT DISTINCT class FROM device WHERE class LIKE \"" 
	                 << tmp_server << "\" ORDER BY class";
	DEBUG_STREAM << "DataBase::db_get_class_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_class_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_class_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *class_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   class_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
//	         DEBUG_STREAM << "DataBase::db_get_class_list(): row[ "<< i << "] class " << row[0] << endl;
	         (*class_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		class_list->length(0);
	mysql_free_result(result);

	return(class_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_class_list
 *
 *	description:	method to execute "DbGetDeviceClassList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_class_list(Tango::DevString server)
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::GetDeviceClassList(): server " << server << endl;

	sql_query_stream << "SELECT name,class FROM device WHERE server = \"" 
	                 << server << "\" ORDER BY name"; 
	DEBUG_STREAM << "DataBase::GetDeviceClassList(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_class_list()");

	n_rows = mysql_num_rows(result);
	INFO_STREAM << "DataBase::GetDeviceClassList(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_class_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_class_list->length(n_rows*2);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::GetDeviceClassList(): row[ "<< i << "] name " << row[0] << " class " << row[1] << endl;
	         (*device_class_list)[i*2]   = CORBA::string_dup(row[0]);
	         (*device_class_list)[i*2+1] = CORBA::string_dup(row[1]);
	      }
	   }
	}
	else
		device_class_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceClassList");
	return(device_class_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_property
 *
 *	description:	method to execute "DbGetDeviceProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_property(const Tango::DevVarStringArray *property_names)
{
	TangoSys_MemStream	sql_query_stream;
	char n_properties_str[256];
	char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_device;
	string	tmp_name;
	string	prop_name;

	TimeVal	before, after;
	GetTime(before);

	if (property_names->length() < 2)
	{
	   WARN_STREAM << "DataBase::GetDeviceProperty(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs at least 2 (device,property)",
					  (const char *)"DataBase::GetDeviceProperty()");
	}

	INFO_STREAM << "DataBase::GetDeviceProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[0] = CORBA::string_dup(tmp_device);
	(*property_list)[1] = CORBA::string_dup(n_properties_str);

	for (i=1; i<property_names->length(); i++)
	{
	   prop_name = (*property_names)[i];
	   tmp_name = replace_wildcard((*property_names)[i]);
	   sql_query_stream.str("");
	   sql_query_stream << "SELECT count,value,name FROM property_device WHERE device = \""
	                    << tmp_device << "\" AND name LIKE \"" << tmp_name << "\" ORDER BY count";
	   DEBUG_STREAM << "DataBase::GetDeviceProperty(): sql_query " << sql_query_stream.str() << endl;
	   
	   result = query(sql_query_stream.str(),"db_get_device_property()");

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetDeviceProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(prop_name.c_str());
       (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetDeviceProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
		    	n_props++;
		    	property_list->length(n_props);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_properties += n_rows;
	   }
	   else
	   {
	      n_props++;
	      property_list->length(n_props);
	      (*property_list)[n_props-1] = CORBA::string_dup(" ");
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetDeviceProperty(): property_list->length() "<< property_list->length() << endl;

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceProperty");
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_property
 *
 *	description:	method to execute "DbPutDeviceProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream sql_query_stream;
	char tmp_count_str[256];
	int n_properties=0, n_rows=0, i, j, k;
	const char *tmp_device, *tmp_count;
	string tmp_name;

	TimeVal	before, after;
	GetTime(before);

	sscanf((*property_list)[1],"%d",&n_properties);
	INFO_STREAM << "DataBase::PutDeviceProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_device WRITE, property_device_hist WRITE,device_history_id WRITE",this);
		
		k = 2;
		for (i=0; i<n_properties; i++)
		{
		   tmp_count = 0;
		   tmp_device = (*property_list)[0];
		   tmp_name = (*property_list)[k];

// first delete all tuples (device,name) from the property table
    	   sql_query_stream.str("");
		   sql_query_stream << "DELETE FROM property_device WHERE device LIKE \"" << tmp_device \
													 << "\" AND name LIKE \"" << tmp_name << "\"";
		   DEBUG_STREAM << "DataBase::PutDeviceProperty(): sql_query " << sql_query_stream.str() << endl;

		   simple_query(sql_query_stream.str(),"db_put_device_property()",al.get_con_nb());
		   sscanf((*property_list)[k+1], "%d", &n_rows);
		   unsigned int device_property_hist_id = get_id("device",al.get_con_nb());

		   for (j=k+2; j<k+n_rows+2; j++)
		   {
        	  string tmp_escaped_string = escape_string((*property_list)[j]);
	    	  tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

			// then insert the new value for this tuple
					sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_device SET device='" << tmp_device \
													 << "',name='" << tmp_name \
													 << "',count='" << tmp_count_str \
													 << "',value='" << tmp_escaped_string \
													 << "',updated=NULL,accessed=NULL";
	    	  DEBUG_STREAM << "DataBase::PutDeviceProperty(): sql_query " << sql_query_stream.str() << endl;

			  simple_query(sql_query_stream.str(),"db_put_device_property",al.get_con_nb());

			// insert the new value into the history table
					sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_device_hist SET device='" << tmp_device \
													 << "',id='" << device_property_hist_id \
													 << "',name='" << tmp_name \
													 << "',count='" << tmp_count_str \
													 << "',value='" << tmp_escaped_string \
													 << "'";

	    	  DEBUG_STREAM << "DataBase::PutDeviceProperty(): sql_query " << sql_query_stream.str() << endl;

			  simple_query(sql_query_stream.str(),"db_put_device_property",al.get_con_nb());

		   }
		   purge_property("property_device_hist","device",tmp_device,tmp_name.c_str(),al.get_con_nb());
		   k = k+n_rows+2;
		}
	}

	GetTime(after);
	update_timing_stats(before, after, "DbPutDeviceProperty");
	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_property
 *
 *	description:	method to execute "DbDeleteDeviceProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream	sql_query_stream;
	int n_properties=0, i,j;
	const char *tmp_device;
	string tmp_name;
	MYSQL_RES *result;
	MYSQL_ROW row;

	TimeVal	before, after;
	GetTime(before);

	n_properties = property_list->length() - 1;
	INFO_STREAM << "DataBase::PutDeviceProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_device WRITE, property_device_hist WRITE,device_history_id WRITE",this);
		
		for (i=0; i<n_properties; i++)
		{
	    	  tmp_device = (*property_list)[0];
	    	  tmp_name =   replace_wildcard((*property_list)[i+1]);

			  // Is there something to delete ?

        	  sql_query_stream.str("");
	    	  sql_query_stream << "SELECT DISTINCT name FROM property_device WHERE device=\"" 
		                	   << tmp_device << "\" AND name LIKE \"" << tmp_name << "\"";
			  result = query(sql_query_stream.str(),"db_delete_device_property()",al.get_con_nb());
			  int count = mysql_num_rows(result);

			  for(j=0;j<count;j++) {

  		    	row = mysql_fetch_row(result);

				// delete the tuple (device,name,count) from the property table

            	sql_query_stream.str("");
	        	sql_query_stream << "DELETE FROM property_device WHERE device=\"" 
		                    	 << tmp_device << "\" AND name=\"" << row[0] << "\"";
	        	DEBUG_STREAM << "DataBase::DeleteDeviceProperty(): sql_query " << sql_query_stream.str() << endl;
		    	simple_query(sql_query_stream.str(),"db_delete_device_property()",al.get_con_nb());

				// Mark this property as deleted

	        	unsigned int device_property_hist_id = get_id("device",al.get_con_nb());
		    	sql_query_stream.str("");
		    	sql_query_stream << "INSERT INTO property_device_hist SET device='" 
		                	   << tmp_device << "',id='" << device_property_hist_id << "',name='" 
							   << row[0] << "',count='0',value='DELETED'";
		    	simple_query(sql_query_stream.str(),"db_delete_device_property()",al.get_con_nb());		

            	purge_property("property_device_hist","device",tmp_device,row[0],al.get_con_nb());

        	  }

  	    	  mysql_free_result(result);

		}
	}

	GetTime(after);
	update_timing_stats(before, after, "DbDeleteDeviceProperty");
	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_property
 *
 *	description:	method to execute "DbGetDeviceAttributeProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_property(const Tango::DevVarStringArray *property_names)
{
	TangoSys_MemStream	sql_query_stream;
	char n_attributes_str[256];
	char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_device, *tmp_attribute;

	TimeVal	before, after;
	GetTime(before);


	INFO_STREAM << "DataBase::GetAttributeProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_device);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_attribute = (*property_names)[i];
	   sql_query_stream.str("");
	   sql_query_stream << "SELECT name,value FROM property_attribute_device WHERE device = \"" 
	                    << tmp_device << "\" AND attribute LIKE \"" << tmp_attribute << "\" ";
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
	   
	   result = query(sql_query_stream.str(),"db_get_device_attribute_property()");

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetAttributeProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	   	    	n_props = n_props+2;
	   	    	property_list->length(n_props);
	            (*property_list)[n_props-2] = CORBA::string_dup(row[0]);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_attributes += n_rows;
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetDeviceProperty(): property_list->length() "<< property_list->length() << endl;

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceAttributeProperty");
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_attribute_property
 *
 *	description:	method to execute "DbPutDeviceAttributeProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_attribute_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream sql_query_stream;
	int n_attributes, n_properties=0, n_rows=0, i, j, k;
	const char *tmp_device, *tmp_attribute, *tmp_name;

	TimeVal	before, after;
	GetTime(before);


	sscanf((*property_list)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutAttributeProperty(): put " << n_attributes << " attributes for device " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_attribute_device WRITE, property_attribute_device WRITE,device_attribute_history_id WRITE",this);
		
		k = 2;
		for (i=0; i<n_attributes; i++)
		{
		   tmp_device = (*property_list)[0];
		   tmp_attribute = (*property_list)[k];
		   sscanf((*property_list)[k+1], "%d", &n_properties);
		   for (j=k+2; j<k+n_properties*2+2; j=j+2)
		   {
	    	  tmp_name = (*property_list)[j];
        	  string tmp_escaped_string = escape_string((*property_list)[j+1]);

// first delete the tuple (device,name,count) from the property table
				sql_query_stream.str("");
				sql_query_stream << "DELETE FROM property_attribute_device WHERE device LIKE \"" \
												 << tmp_device << "\" AND attribute LIKE \"" << tmp_attribute \
												 << "\" AND name LIKE \"" << tmp_name << "\"";
				DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
				
				simple_query(sql_query_stream.str(),"db_put_device_attribute_property()",al.get_con_nb());

// then insert the new value for this tuple
				sql_query_stream.str("");
				sql_query_stream << "INSERT INTO property_attribute_device SET device='" << tmp_device \
												 << "',attribute='" << tmp_attribute \
												 << "',name='" << tmp_name \
												 << "',count='1',value='" << tmp_escaped_string \
												 << "',updated=NULL,accessed=NULL";
	      		DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
		  		simple_query(sql_query_stream.str(),"db_put_device_attribute_property()",al.get_con_nb());

// then insert the new value for this tuple into the history table
          		unsigned int device_attribute_property_hist_id = get_id("device_attribute",al.get_con_nb());
				sql_query_stream.str("");
				sql_query_stream << "INSERT INTO property_attribute_device_hist SET device='" << tmp_device \
												 << "',attribute='" << tmp_attribute \
												 << "',name='" << tmp_name \
												 << "',id='" << device_attribute_property_hist_id \
												 << "',count='1',value='" << tmp_escaped_string << "'";
	      		DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
		  		simple_query(sql_query_stream.str(),"db_put_device_attribute_property()",al.get_con_nb());

	      		purge_att_property("property_attribute_device_hist","device",tmp_device,tmp_attribute,tmp_name,al.get_con_nb());
		   }
		   k = k+n_properties*2+2;
		}
	}

	GetTime(after);
	update_timing_stats(before, after, "DbPutDeviceAttributeProperty");
	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_property
 *
 *	description:	method to execute "DbGetClassAttributeProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_property(const Tango::DevVarStringArray *property_names)
{
	TangoSys_MemStream sql_query_stream;
	char n_attributes_str[256];
	char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_class, *tmp_attribute;

	INFO_STREAM << "DataBase::GetAttributeProperty(): get " << property_names->length()-1 << " attributes for class " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_class);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_attribute = (*property_names)[i];
	   sql_query_stream.str("");
	   sql_query_stream << "SELECT name,value FROM property_attribute_class WHERE class = \"" 
	                    << tmp_class << "\" AND attribute LIKE \"" << tmp_attribute << "\" ";
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
	   
	   result = query(sql_query_stream.str(),"db_get_class_attribute_property()");

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetAttributeProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	            n_props = n_props+2;
	            property_list->length(n_props);
	            (*property_list)[n_props-2] = CORBA::string_dup(row[0]);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_attributes += n_rows;
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetClassProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_class_attribute_property
 *
 *	description:	method to execute "DbPutClassAttributeProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_class_attribute_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream sql_query_stream;
	int n_attributes, n_properties=0, n_rows=0, i, j, k;
	const char *tmp_class, *tmp_attribute, *tmp_name;

	sscanf((*property_list)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutAttributeProperty(): put " << n_attributes << " attributes for device " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_attribute_class WRITE, property_attribute_class_hist WRITE,class_attribute_history_id WRITE",this);
		
		k = 2;
		for (i=0; i<n_attributes; i++)
		{
		   tmp_class = (*property_list)[0];
		   tmp_attribute = (*property_list)[k];
		   sscanf((*property_list)[k+1], "%d", &n_properties);
		   for (j=k+2; j<k+n_properties*2+2; j=j+2)
		   {
	    	  tmp_name = (*property_list)[j];
        	  string tmp_escaped_string = escape_string((*property_list)[j+1]);

// first delete the tuple (device,name,count) from the property table
			  sql_query_stream.str("");
			  sql_query_stream << "DELETE FROM property_attribute_class WHERE class LIKE \"" \
													 << tmp_class << "\" AND attribute LIKE \"" << tmp_attribute \
													 << "\" AND name LIKE \"" << tmp_name << "\"";
	    	  DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_put_class_attribute_property()",al.get_con_nb());

// then insert the new value for this tuple
        	  sql_query_stream.str("");
			  sql_query_stream << "INSERT INTO property_attribute_class SET class='" << tmp_class \
													 << "',attribute='" << tmp_attribute \
													 << "',name='" << tmp_name \
													 << "',count='1',value='" << tmp_escaped_string \
													 << "',updated=NULL,accessed=NULL";
	    	  DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_put_class_attribute_property()",al.get_con_nb());

// then insert the new value into the history table

			  unsigned int class_attribute_property_hist_id = get_id("class_attribute",al.get_con_nb());
        	  sql_query_stream.str("");
			  sql_query_stream << "INSERT INTO property_attribute_class_hist SET class='" << tmp_class \
													 << "',attribute='" << tmp_attribute \
													 << "',name='" << tmp_name \
													 << "',id='" << class_attribute_property_hist_id \
													 << "',count='1',value='" << tmp_escaped_string << "'";
	    	  DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_put_class_attribute_property()",al.get_con_nb());

	    	  purge_att_property("property_attribute_class_hist","class",tmp_class,tmp_attribute,tmp_name,al.get_con_nb());
		   }
		   k = k+n_properties*2+2;
		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_list
 *
 *	description:	method to execute "DbGetClassAttributeList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_list(const Tango::DevVarStringArray *class_wildcard)
{

	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_attrs=0, j;
	Tango::DevVarStringArray *attribute_list = new Tango::DevVarStringArray;
	const char *class_name, *wildcard;
	string tmp_wildcard;

	class_name = (*class_wildcard)[0];	
	INFO_STREAM << "DataBase::db_get_class_attribute(): get attributes for class " << class_name << endl;

	wildcard = (*class_wildcard)[1];	
	if (wildcard == NULL)
	{
#ifdef WIN32
		sql_query_stream << "SELECT DISTINCT attribute FROM property_attribute_class WHERE class = \"" << class_name << "\"  AND attribute like \"\\%\"";
#else
		sql_query_stream << "SELECT DISTINCT attribute FROM property_attribute_class WHERE class = \"" << class_name << "\"  AND attribute like \"%%\"";
#endif /* WIN32 */
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT attribute FROM property_attribute_class WHERE class = \"" 
		                 << class_name << "\"  AND attribute like \"" << tmp_wildcard << "\"";
	}
	DEBUG_STREAM << "DataBase::GetClassAttributeList(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_class_attribute_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::GetClassAttributeList(): num_rows() " << n_rows << endl;
	if (n_rows > 0)
	{

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetClassAttributeList(): attribute[ "<< j << "] " << row[0] << endl;
		    	n_attrs++;
		    	attribute_list->length(n_attrs);
	            (*attribute_list)[n_attrs-1] = CORBA::string_dup(row[0]);
	         }
	      }
	}
	mysql_free_result(result);

	DEBUG_STREAM << "DataBase::GetClassAttributeList(): attribute_list->length() "<< attribute_list->length() << endl;

	return(attribute_list);
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_property
 *
 *	description:	method to execute "DbGetClassProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_property(const Tango::DevVarStringArray *property_names)
{
	TangoSys_MemStream sql_query_stream;
	char n_properties_str[256];
	char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_class, *tmp_name;

	INFO_STREAM << "DataBase::GetClassProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[0] = CORBA::string_dup(tmp_class);
	(*property_list)[1] = CORBA::string_dup(n_properties_str);

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_name = (*property_names)[i];
	   sql_query_stream.str("");
	   sql_query_stream << "SELECT count,value FROM property_class WHERE class = \"" 
	                    << tmp_class << "\" AND name LIKE \"" << tmp_name << "\" ORDER BY count";
	   DEBUG_STREAM << "DataBase::GetClassProperty(): sql_query " << sql_query_stream.str() << endl;
	   
	   result = query(sql_query_stream.str(),"db_get_class_property()");

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetClassProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_name);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetClassProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
		    	n_props++;
		    	property_list->length(n_props);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_properties += n_rows;
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetClassProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_class_property
 *
 *	description:	method to execute "DbPutClassProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_class_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream sql_query_stream;
    char tmp_count_str[256];
	int n_properties=0, n_rows=0, i, j, k;
	const char *tmp_class, *tmp_name, *tmp_count;

	TimeVal	before, after;
	GetTime(before);
	
	sscanf((*property_list)[1],"%d",&n_properties);
	INFO_STREAM << "DataBase::PutClassProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_class WRITE, property_class_hist WRITE, class_history_id WRITE",this);
		
		k = 2;
		tmp_class = (*property_list)[0];
		for (i=0; i<n_properties; i++)
		{
			tmp_count = 0;
		   	tmp_name = (*property_list)[k];

// first delete all tuples (device,name) from the property table
			sql_query_stream.str("");
			sql_query_stream << "DELETE FROM property_class WHERE class LIKE \"" << tmp_class \
												<< "\" AND name LIKE \"" << tmp_name << "\"";
												
			DEBUG_STREAM << "DataBase::PutClassProperty(): sql_query " << sql_query_stream.str() << endl;
		   	simple_query(sql_query_stream.str(),"db_put_class_property()",al.get_con_nb());
		   	sscanf((*property_list)[k+1], "%d", &n_rows);
		   	unsigned int class_property_hist_id = get_id("class",al.get_con_nb());
		   	for (j=k+2; j<k+n_rows+2; j++)
		   	{
        	  	string tmp_escaped_string = escape_string((*property_list)[j]);
	    	  	tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// then insert the new value for this tuple
				sql_query_stream.str("");
				sql_query_stream << "INSERT INTO property_class SET class='" << tmp_class \
													 << "',name='" << tmp_name \
													 << "',count='" << tmp_count_str \
													 << "',value='" << tmp_escaped_string \
													 << "',updated=NULL,accessed=NULL";
	    	  	DEBUG_STREAM << "DataBase::PutClassProperty(): sql_query " << sql_query_stream.str() << endl;
  	    	  	simple_query(sql_query_stream.str(),"db_put_class_property()",al.get_con_nb());

// then insert the new value into the history table
				sql_query_stream.str("");
				sql_query_stream << "INSERT INTO property_class_hist SET class='" << tmp_class \
													 << "',name='" << tmp_name \
													 << "',count='" << tmp_count_str \
													 << "',id='" << class_property_hist_id \
													 << "',value='" << tmp_escaped_string << "'";
	    	  	DEBUG_STREAM << "DataBase::PutClassProperty(): sql_query " << sql_query_stream.str() << endl;
  	    	  	simple_query(sql_query_stream.str(),"db_put_class_property()",al.get_con_nb());
		   	}
		   	purge_property("property_class_hist","class",tmp_class,tmp_name,al.get_con_nb());
		   	k = k+n_rows+2;
		}
	}

	GetTime(after);
	update_timing_stats(before, after, "DbPutClassProperty");	
	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_class_property
 *
 *	description:	method to execute "DbDeleteClassProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_class_property(const Tango::DevVarStringArray *property_list)
{
	TangoSys_MemStream sql_query_stream;
	int n_properties=0, i,j;
	const char *tmp_class;
	string tmp_name;
	MYSQL_RES *result;
	MYSQL_ROW row;

	n_properties = property_list->length() - 1;
	INFO_STREAM << "DataBase::DeleteClassProperty(): delete " << n_properties << " properties for class " << (*property_list)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_class WRITE,property_class_hist WRITE,class_history_id WRITE",this);
		  
		for (i=0; i<n_properties; i++)
		{
	    	  tmp_class = (*property_list)[0];
	    	  tmp_name =  replace_wildcard((*property_list)[i+1]);

        	  // Is there something to delete ?

        	  sql_query_stream.str("");
	    	  sql_query_stream << "SELECT DISTINCT name FROM property_class WHERE class=\"" << tmp_class 
		                	   << "\" AND name LIKE \"" << tmp_name << "\"";
			  result = query(sql_query_stream.str(),"db_delete_class_property()",al.get_con_nb());
			  int count = mysql_num_rows(result);

			  for(j=0;j<count;j++) {

		    	row = mysql_fetch_row(result);

            	// delete the tuple (device,name,count) from the property table
		    	sql_query_stream.str("");
		    	sql_query_stream << "DELETE FROM property_class WHERE class=\"" << tmp_class 
		                	   << "\" AND name=\"" << row[0] << "\"";
	        	DEBUG_STREAM << "DataBase::DeleteClassProperty(): sql_query " << sql_query_stream.str() << endl;
		    	simple_query(sql_query_stream.str(),"db_delete_class_property()",al.get_con_nb());

				// Mark this property as deleted
				unsigned int class_property_hist_id = get_id("class",al.get_con_nb());
				sql_query_stream.str("");
				sql_query_stream << "INSERT INTO property_class_hist SET class='" << tmp_class \
													 << "',name='" << row[0] \
													 << "',id='" << class_property_hist_id \
													 << "',count='0',value='DELETED'";
	        	DEBUG_STREAM << "DataBase::PutClassProperty(): sql_query " << sql_query_stream.str() << endl;
  	        	simple_query(sql_query_stream.str(),"db_delete_class_property()",al.get_con_nb());

   	        	purge_property("property_class_hist","class",tmp_class,row[0],al.get_con_nb());

			  }
  	    	  mysql_free_result(result);

		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_export_device
 *
 *	description:	method to execute "DbExportDevice"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_export_device(const Tango::DevVarStringArray *export_info)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0, i;
	const char *tmp_ior, *tmp_host, *tmp_pid, *tmp_version;
	string tmp_device, tmp_server;

	TimeVal	before, after;
	GetTime(before);


	if (export_info->length() < 5) {
   		WARN_STREAM << "DataBase::DbExportDevice(): insufficient export info for device ";
   		WARN_STREAM << tmp_device << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"Insufficient export info for device",
					       (const char *)"DataBase::DbExportDevice()");
	}
	
	INFO_STREAM << "DataBase::ExportDevice(): put " << export_info->length()-1 << " export info for device " << (*export_info)[0] << endl;

	tmp_device = (*export_info)[0];
	for (i=0; i<tmp_device.length(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
	tmp_ior = (*export_info)[1];
	tmp_host = (*export_info)[2];
	tmp_pid = (*export_info)[3];
	if (strcmp(tmp_pid, "null")==0)
		tmp_pid = "-1";
	tmp_version = (*export_info)[4];
//
//	If it is a server stating -> get previous host where running
//
	bool	do_fire = false;
	string	previous_host;
	{
		AutoLock al("LOCK TABLES device WRITE, server WRITE",this);
	
		if (fireToStarter==true)
		{
			if (tmp_device.substr(0,8) == "dserver/")
			{
				omni_mutex_lock oml(starter_mutex);

				//	Get database server name
				//--------------------------------------
				Tango::Util *tg = Tango::Util::instance();
				string	db_serv = tg->get_ds_name();
				transform(db_serv.begin(), db_serv.end(), db_serv.begin(), ::tolower);
				string	adm_dev("dserver/");
				adm_dev += db_serv;

				//	Check if not database or starter servers
				if (tmp_device !=  adm_dev &&
					tmp_device.substr(0,16) != "dserver/starter/" )
				{
					do_fire = true;
					char *tmp_ptr = db_get_device_host((Tango::DevString)tmp_device.c_str(),al.get_con_nb());
					previous_host = tmp_ptr;
					DEBUG_STREAM << tmp_device << " was running on " << previous_host << endl;
					CORBA::string_free(tmp_ptr);
				}
			}
		}
//
// check if device is defined and if so get server name in order to
// update server table 
//
		sql_query_stream << "SELECT server FROM device WHERE name LIKE \"" << tmp_device << "\" ";
		DEBUG_STREAM << "DataBase::ExportDevice(): sql_query " << sql_query_stream.str() << endl;

		result = query(sql_query_stream.str(),"db_export_device()",al.get_con_nb());

		n_rows = mysql_num_rows(result);
		DEBUG_STREAM << "DataBase::ExportDevice(): mysql_num_rows() " << n_rows << endl;

		if (n_rows > 0)
		{
		   if ((row = mysql_fetch_row(result)) != NULL)
		   {
	    	  DEBUG_STREAM << "DataBase::ExportDevice(): device "<< tmp_device << " server name " << row[0] << endl;
	    	  tmp_server = row[0];
		   }
		}
		else
		{
	    	 INFO_STREAM << "DataBase::ExportDevice(): device not defined !" << endl;
  	 		 TangoSys_OMemStream o;
			 o << "device " << tmp_device << " not defined in the database !";
	    	 mysql_free_result(result);
	    	 Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	     				                	o.str(),
					                    	(const char *)"DataBase::ExportDevice()");
		}
		mysql_free_result(result);

// update the new value for this tuple
   		sql_query_stream.str("");
		sql_query_stream << "UPDATE device set exported=1,ior=\'" << tmp_ior 
	                 << "\',host=\'" << tmp_host << "\',pid=\'" << tmp_pid 
					 << "\',version=\'" << tmp_version 
					 << "\',started=NOW() where name LIKE \'" << tmp_device << "\'";
		DEBUG_STREAM << "DataBase::ExportDevice(): sql_query " << sql_query_stream.str() << endl;

		simple_query(sql_query_stream.str(),"db_export_device()",al.get_con_nb());

// update host name in server table

    	sql_query_stream.str("");
		sql_query_stream << "UPDATE server set host=\'" << tmp_host << "\' where name LIKE \'" 
	                 << tmp_server << "\'";
    	DEBUG_STREAM << "DataBase::ExportDevice(): sql_query " << sql_query_stream.str() << endl;

		simple_query(sql_query_stream.str(),"db_export_device()",al.get_con_nb());
	}

	//	Check if a server has been started.
	if (do_fire)
	{
		//	Update host's starter to update controlled servers list
		vector<string>	hosts;
		hosts.push_back(tmp_host);
		DEBUG_STREAM << "New Host is " << tmp_host << endl;

		if (previous_host!=""      &&
			previous_host!="nada"  && 
			previous_host!=tmp_host)
				hosts.push_back(previous_host);

		starter_shared->send_starter_cmd(hosts);
	}
	GetTime(after);
	update_timing_stats(before, after, "DbExportDevice");
	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_un_export_device
 *
 *	description:	method to execute "DbUnExportDevice"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_un_export_device(Tango::DevString devname)
{
	TangoSys_MemStream sql_query_stream;
	long n_rows=0, i;
	char *tmp_device;

	INFO_STREAM << "DataBase::UnExportDevice(): un-export " << devname << " device " << endl;

	tmp_device = (char*)malloc(strlen(devname)+1);
	sprintf(tmp_device,"%s",devname);
	for (i=0; i<strlen(tmp_device); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}

// un-export device from database by seting ior="not exported"
    sql_query_stream << "UPDATE device SET exported=0,stopped=NOW() WHERE name like \"" 
	                 << tmp_device << "\"";
	DEBUG_STREAM << "DataBase::UnExportDevice(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_export_device()");

	free(tmp_device);

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_un_export_server
 *
 *	description:	method to execute "DbUnExportServer"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_un_export_server(Tango::DevString server_name)
{
	TangoSys_MemStream sql_query_stream;
	long n_rows=0, i;
	char *tmp_server;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::UnExportServer(): un-export all device(s) from server " << server_name << " device " << endl;

	tmp_server = (char*)malloc(strlen(server_name)+1);
	sprintf(tmp_server,"%s",server_name);
	for (i=0; i<strlen(tmp_server); i++) { 
		tmp_server[i] = tolower(tmp_server[i]);
	}

// un-export all devices belonging to this server from database by setting exported="false"

	sql_query_stream << "UPDATE device SET exported=0,stopped=NOW() WHERE server like \"" 
	                 << tmp_server << "\"";
	DEBUG_STREAM << "DataBase::UnExportServer(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_un_export_server()");

	free(tmp_server);

	GetTime(after);
	update_timing_stats(before, after, "DbUnExportServer");
	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_import_device
 *
 *	description:	method to execute "DbImportDevice"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongStringArray *DataBase::db_import_device(Tango::DevString devname)
{
	/*
	static TimeVal	t0;
	TimeVal	t1;
	GetTime(t1);
	cout << Elapsed(t0, t1) << "   " ;
	GetTime(t0);
	*/
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, n_svalues=0, n_lvalues=0; 
	int i, exported, pid;
	string tmp_device;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::ImportDevice(): get import info for " << devname << " device " << endl;

	tmp_device = devname;
	for (i=0; i<tmp_device.size(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
//	tmp_device = replace_wildcard(tmp_device.c_str());

// the following query takes too long because the use of OR in the SELECT means the
// indices cannot be used by mysql. therefore do the SELECT in two stages - andy 31may05

//	sprintf(sql_query,"SELECT exported,ior,version,pid,server,host FROM device WHERE name = '%s' or alias = '%s';",
//	        tmp_device.c_str(),tmp_device.c_str());

	{
		AutoLock al("LOCK TABLE device READ",this);
		
		sql_query_stream << "SELECT exported,ior,version,pid,server,host,class FROM device WHERE name = '" 
	                 	<< tmp_device << "';";
		DEBUG_STREAM << "DataBase::ImportDevice(): sql_query " << sql_query_stream.str() << endl;
	
		result = query(sql_query_stream.str(),"db_import_device()",al.get_con_nb());

		n_rows = mysql_num_rows(result);
		DEBUG_STREAM << "DataBase::ImportDevice(): mysql_num_rows() " << n_rows << endl;

		if (n_rows <= 0)
		{
//
// could not find device by name, try to look for by alias
//
   			INFO_STREAM << "DataBase::ImportDevice(): could not find device by name, look for alias !" << endl;
			mysql_free_result(result);
			sql_query_stream.str("");
			sql_query_stream << "SELECT exported,ior,version,pid,server,host,class FROM device WHERE alias = '" 
		                	 << tmp_device << "';";
			DEBUG_STREAM << "DataBase::ImportDevice(): sql_query " << sql_query_stream.str() << endl;

 	    	result = query(sql_query_stream.str(),"db_import_device()",al.get_con_nb());
	
			n_rows = mysql_num_rows(result);
			DEBUG_STREAM << "DataBase::ImportDevice(): mysql_num_rows() " << n_rows << endl;
		}
	}


	Tango::DevVarLongStringArray *import_info = new Tango::DevVarLongStringArray;
	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::ImportDevice(): device exported " << row[0] << " version " << row[2] << " server " << row[4] << " host " << row[5] << endl;
	      n_svalues = n_svalues+6;
	      (import_info->svalue).length(n_svalues);
	      (import_info->svalue)[n_svalues-6] = CORBA::string_dup(tmp_device.c_str());
	      (import_info->svalue)[n_svalues-4] = CORBA::string_dup(row[2]);
	      (import_info->svalue)[n_svalues-3] = CORBA::string_dup(row[4]);
	      (import_info->svalue)[n_svalues-2] = CORBA::string_dup(row[5]);
		  (import_info->svalue)[n_svalues-1] = CORBA::string_dup(row[6]);
			//	IOR Check
		  if (row[1]!=NULL)
	     	(import_info->svalue)[n_svalues-5] = CORBA::string_dup(row[1]);
		  else
		  	(import_info->svalue)[n_svalues-5] = CORBA::string_dup("");
	      exported = -1;
	      if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = exported;
	      pid = -1;
	      if (row[3] != NULL) sscanf(row[3],"%d",&pid);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = pid;
	   }
		else {
	    	 INFO_STREAM << "DataBase::ImportDevice(" << tmp_device << "): info not defined !" << endl;
	    	 mysql_free_result(result);
		     delete import_info;
	    	 Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
								(const char *)"Device import info not found in the database !",
								(const char *)"DataBase::ImportDevice()");
		}
	}
	else {
	     INFO_STREAM << "DataBase::ImportDevice(" << tmp_device << "): device not defined !" << endl;
 	 	 TangoSys_OMemStream o;
		 o << "device " << tmp_device << " not defined in the database !";
	     mysql_free_result(result);
	     delete import_info;
	     Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
							            o.str(),
							            (const char *)"DataBase::ImportDevice()");
	}
	mysql_free_result(result);
	/*
	 * calculate elapsed time and update timing variables
	 */
	GetTime(after);
	update_timing_stats(before, after, "DbImportDevice");
	return(import_info);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_add_server
 *
 *	description:	method to execute "DbAddServer"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_add_server(const Tango::DevVarStringArray *server_device_list)
{
	TangoSys_MemStream sql_query_stream;
	char domain[256], family[256], member[256];
	int n_rows=0;
	const char *tmp_server, *tmp_class;

	if (server_device_list->length() < 3)
	{
	   WARN_STREAM << "DataBase::AddServer(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs at least 3 (server,device,class)",
					  (const char *)"DataBase::AddServer()");
	}
		
	INFO_STREAM << "DataBase::AddServer(): insert " << (*server_device_list)[0] << " server with device " << (*server_device_list)[1] << endl;
	tmp_server = (*server_device_list)[0];
	
	{
		AutoLock al("LOCK TABLE device WRITE",this);
		
		for (int i=0; i<(server_device_list->length()-1)/2; i++)
		{
			string tmp_device((*server_device_list)[i*2+1].in());
			tmp_class = (*server_device_list)[i*2+2];
			if (!check_device_name(tmp_device))
			{
  	 	    	TangoSys_OMemStream o;
	   			o << "device name (" << tmp_device << ") syntax error (should be [tango:][//instance/]domain/family/member)";
	   			Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
						                	   o.str(),
						                	   (const char *)"DataBase::AddServer()");
			}
			device_name_to_dfm(tmp_device, domain, family, member);

// first delete the tuple (device,name,count) from the property table
        	sql_query_stream.str("");
        	sql_query_stream << "DELETE FROM device WHERE server=\"" << tmp_server 
		                	 << "\" AND name=\"" << tmp_device << "\" ";
			DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query_stream.str() << endl;
			simple_query(sql_query_stream.str(),"db_add_server()",al.get_con_nb());

// then insert the new value for this tuple
        	sql_query_stream.str("");
        	sql_query_stream << "INSERT INTO device SET name=\"" << tmp_device 
		                	 << "\",domain=\"" << domain << "\",family=\"" << family 
							 << "\",member=\"" << member 
							 << "\",exported=0,ior=\"nada\",host=\"nada\",server=\"" << tmp_server 
							 << "\",pid=0,class=\"" << tmp_class
							 << "\",version=0,started=NULL,stopped=NULL";
			DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query_stream.str() << endl;
			simple_query(sql_query_stream.str(),"db_add_server()",al.get_con_nb());

		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_add_device
 *
 *	description:	method to execute "DbAddDevice"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_add_device(const Tango::DevVarStringArray *server_device)
{
	TangoSys_MemStream sql_query_stream;
 	char domain[256], family[256], member[256];
	int n_rows=0;
	const char *tmp_server, *tmp_class, *tmp_alias;
	string tmp_device;
	string dserver_name;
	MYSQL_RES *result;


	if (server_device->length() < 3)
	{
	   WARN_STREAM << "DataBase::AddServer(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs at least 3 (server,device,class)",
					  (const char *)"DataBase::AddServer()");
	}
		
	INFO_STREAM << "DataBase::AddDevice(): insert " << (*server_device)[0] << " server with device " << (*server_device)[1] << endl;
	tmp_server = (*server_device)[0];
	tmp_device = (*server_device)[1];
	tmp_class = (*server_device)[2];
	if (server_device->length() > 3)
	{
		tmp_alias = (*server_device)[3];
	}
	if (!check_device_name(tmp_device))
	{
       TangoSys_OMemStream o;
	   o << "device name (" << tmp_device << ") syntax error (should be [tango:][//instance/]domain/family/member)";
	   Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
	   				                  o.str(),
					                  (const char *)"DataBase::AddDevice()");
	}
	device_name_to_dfm(tmp_device, domain, family, member);

	{
		AutoLock al("LOCK TABLE device WRITE",this);
		
// first delete the tuple (device,name) from the device table

		sql_query_stream << "DELETE FROM device WHERE name LIKE \"" << tmp_device << "\"";
		DEBUG_STREAM << "DataBase::AddDevice(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_add_device()",al.get_con_nb());

// then insert the new value for this tuple

    	sql_query_stream.str("");
		if (server_device->length() < 4)
		{
			sql_query_stream << "INSERT INTO device SET name=\"" << tmp_device 
		                	 << "\",domain=\"" << domain << "\",family=\"" << family 
							 << "\",member=\"" << member 
							 << "\",exported=0,ior=\"nada\",host=\"nada\",server=\"" 
							 << tmp_server << "\",pid=0,class=\"" << tmp_class 
							 << "\",version=\"0\",started=NULL,stopped=NULL";
		}
		else
		{
			sql_query_stream << "INSERT INTO device SET name=\"" << tmp_device 
		                	 << "\",domain=\"" << domain << "\",family=\"" << family 
							 << "\",member=\"" << member 
							 << "\",exported=0,ior=\"nada\",host=\"nada\",server=\"" 
							 << tmp_server << "\",pid=0,class=\"" << tmp_class
							 << "\",alias=\"" << tmp_alias 
							 << "\",version=\"0\",started=NULL,stopped=NULL";
		}
		DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_add_device()",al.get_con_nb());

//
// Check if a DServer device entry for the process already exists
//
    	sql_query_stream.str("");
		sql_query_stream << "SELECT name FROM device WHERE server LIKE \"" << tmp_server 
	                	 << "\" AND class LIKE \"DServer\"";
		DEBUG_STREAM << "DataBase::AddDevice(): sql_query " << sql_query_stream.str() << endl;
		result = query(sql_query_stream.str(),"db_add_device()",al.get_con_nb());

		n_rows = mysql_num_rows(result);
		DEBUG_STREAM << "DataBase::AddDevice(): mysql_num_rows() " << n_rows << endl;

//
// If there is no admin device for the device's server, create one
//
	
		if (n_rows == 0)
		{		
			dserver_name = "dserver/";
			dserver_name = dserver_name + string(tmp_server);
			device_name_to_dfm(dserver_name, domain, family, member);
        	sql_query_stream.str("");
			sql_query_stream << "INSERT INTO device SET name=\"dserver/" << tmp_server
		                	 << "\",domain=\"" << domain << "\",family=\"" << family 
							 << "\",member=\"" << member 
							 << "\",exported=0,ior=\"nada\",host=\"nada\",server=\"" << tmp_server 
							 << "\",pid=0,class=\"DServer\",version=\"0\",started=NULL,stopped=NULL";
			DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query_stream.str() << endl;
  	    	simple_query(sql_query_stream.str(),"db_add_device()",al.get_con_nb());

		}

		mysql_free_result(result);
	}
		
	return;
}


//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_list
 *
 *	description:	method to execute "DbGetDeviceAttributeList"
 *	return list of attributes for device which match the
 *	wildcard
 *
 * @param	argin	device name
 * @return	attribute wildcard
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_list(const Tango::DevVarStringArray *device_wildcard)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *device, *wildcard;

	device = (*device_wildcard)[0];
	wildcard = (*device_wildcard)[1];
	INFO_STREAM << "DataBase::db_get_device_attribute_list(): device " << device;
	WARN_STREAM << " wildcard " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT attribute FROM property_attribute_device WHERE device=\"" 
		                 << device << "\" AND attribute LIKE \"%\" ORDER BY attribute";
	}
	else
	{
		tmp_wildcard = replace_wildcard (wildcard);
		sql_query_stream << "SELECT DISTINCT attribute FROM property_attribute_device WHERE device=\""
		                 << device << "\" AND attribute LIKE \"" << tmp_wildcard << "\" ORDER BY attribute";
	}
	DEBUG_STREAM << "DataBase::db_get_device_attrribute_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_attribute_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_attribute_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *attribute_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   attribute_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_attribute_list(): attribute[ "<< i << "] attribute " << row[0] << endl;
	         (*attribute_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		attribute_list->length(0);
	mysql_free_result(result);

	return(attribute_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_server
 *
 *	description:	method to execute "DbDeleteServer"
 *	delete server from the database, do not delete device properties
 *
 * @param	argin	server name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_server(Tango::DevString server)
{
	TangoSys_MemStream sql_query_stream;

	INFO_STREAM << "DataBase::db_delete_server(): delete server " << server << " from database" << endl;

// first check the server name that it has no wildcard '*' and
// that it has at least one slash in it

	string tmp_server = string(server);

	if ((tmp_server.find('*') != string::npos) || 
	    (tmp_server.find('%') != string::npos) ||
	    (tmp_server.find('/') == string::npos))
	{
         	WARN_STREAM << "DataBase::db_delete_server(): server name  " << server << "incorrect ";
         	WARN_STREAM << endl;
         	Tango::Except::throw_exception((const char *)DB_IncorrectServerName,
					       (const char *)"failed to delete server, server name incorrect",
					       (const char *)"DataBase::db_delete_server()");
	}

	tmp_server = replace_wildcard(server);

//
//	get  host where running
//
	string	previous_host("");
	if (fireToStarter==true)
	{
		omni_mutex_lock oml(starter_mutex);
		
		string	adm_dev("dserver/");
		adm_dev += tmp_server;
		char *tmp_ptr = db_get_device_host((Tango::DevString)adm_dev.c_str());
		previous_host = tmp_ptr;
		DEBUG_STREAM << tmp_server << " was running on " << previous_host << endl;
		CORBA::string_free(tmp_ptr);
	}



// then delete the device from the device table

    sql_query_stream << "DELETE FROM device WHERE server LIKE \"" << tmp_server << "\"";
    DEBUG_STREAM << "DataBase::db_delete_server(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_delete_server()");

	//	Update host's starter to update controlled servers list
	if (fireToStarter==true)
	{
		omni_mutex_lock oml(starter_mutex);
		
		vector<string>	hosts;
		if (previous_host!="")
		{
			hosts.push_back(previous_host);
			starter_shared->send_starter_cmd(hosts);
		}
	}


}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device
 *
 *	description:	method to execute "DbDeleteDevice"
 *
 * @param	argin	device name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device(Tango::DevString device)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_device;

	INFO_STREAM << "DataBase::db_delete_device(): delete " << device << " from database" << endl;

// first check the device name

	tmp_device = device;
	if (!check_device_name(tmp_device))
	{
         	WARN_STREAM << "DataBase::db_delete_device(): device name  " << device << " incorrect ";
         	WARN_STREAM << endl;
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
					       (const char *)"failed to delete device, device name incorrect",
					       (const char *)"DataBase::db_delete_device()");
	}

// replace database wildcards (% and _)

	string tmp_wildcard = replace_wildcard(tmp_device.c_str());

	{
		AutoLock al("LOCK TABLES device WRITE, property_device WRITE, property_attribute_device WRITE",this);
		
// then delete the device from the device table

    	sql_query_stream << "DELETE FROM device WHERE name LIKE \"" << tmp_wildcard << "\"";
    	DEBUG_STREAM << "DataBase::db_delete_device(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_delete_device()",al.get_con_nb());

// then delete device from the property_device table

    	sql_query_stream.str("");
		sql_query_stream << "DELETE FROM property_device WHERE device LIKE \"" << tmp_wildcard << "\"";
    	DEBUG_STREAM << "DataBase::db_delete_device(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_delete_device()",al.get_con_nb());

// then delete device from the property_attribute_device table

    	sql_query_stream.str("");
		sql_query_stream << "DELETE FROM property_attribute_device WHERE device LIKE \"" << tmp_wildcard << "\"";
    	DEBUG_STREAM << "DataBase::db_delete_device(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_delete_device()",al.get_con_nb());
	}

    return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_attribute
 *
 *	description:	method to execute "DbDeleteDeviceAttribute"
 *	delete a device attribute and all its properties from
 *	the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_attribute(const Tango::DevVarStringArray *argin)
{
	TangoSys_MemStream sql_query_stream;
	const char *attribute;
	string tmp_device;

	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_delete_device_attribute(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient number of arguments to delete device attribute",
					       (const char *)"DataBase::db_delete_device_attribute()");
	}

	tmp_device = (*argin)[0].in();
	attribute = (*argin)[1];

	INFO_STREAM << "DataBase::db_delete_device(): delete " << tmp_device << " from database" << endl;

// first check the device name

	if (!check_device_name(tmp_device))
	{
         	WARN_STREAM << "DataBase::db_delete_device_attribute(): device name  " << tmp_device << " incorrect ";
         	WARN_STREAM << endl;
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
					       (const char *)"failed to delete device attribute, device name incorrect",
					       (const char *)"DataBase::db_delete_device_attribute()");
	}

// replace database wildcards (% and _)

	string tmp_wildcard = replace_wildcard(tmp_device.c_str());

// then delete device from the property_attribute_device table

    sql_query_stream << "DELETE FROM property_attribute_device WHERE device LIKE \"" 
	                 << tmp_wildcard << "\" AND attribute LIKE \"" << attribute << "\" ";
    DEBUG_STREAM << "DataBase::db_delete_device_attribute(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_delete_device_attribute()");

    return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_class_attribute
 *
 *	description:	method to execute "DbDeleteClassAttribute"
 *	delete a class attribute and all its properties from
 *	the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_class_attribute(const Tango::DevVarStringArray *argin)
{
	TangoSys_MemStream sql_query_stream;
	const char *attribute;
	string tmp_class;

	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_delete_class_attribute(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient number of arguments to delete class attribute",
					       (const char *)"DataBase::db_delete_class_attribute()");
	}

	tmp_class = (*argin)[0];
	attribute = (*argin)[1];

	INFO_STREAM << "DataBase::db_delete_class_attribute(): delete " << tmp_class << " from database" << endl;

// then delete class from the property_attribute_class table

    sql_query_stream << "DELETE FROM property_attribute_class WHERE class LIKE \"" << tmp_class 
	                 << "\" AND attribute LIKE \"" << attribute << "\" ";
    DEBUG_STREAM << "DataBase::db_delete_class_attribute(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_delete_class_attribute()");
	
    return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_attribute_property
 *
 *	description:	method to execute "DbDeleteDeviceAttributeProperty"
 *	delete a device attribute property from the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_attribute_property(const Tango::DevVarStringArray *argin)
{
	TangoSys_MemStream sql_query_stream;
	const char *attribute, *property;
	string tmp_device;
	int i;
	MYSQL_RES *result;
	MYSQL_ROW row;

	if (argin->length() < 3) {
   		WARN_STREAM << "DataBase::db_delete_device_attribute_property(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient number of arguments to delete device attribute property",
					       (const char *)"DataBase::db_delete_device_attribute_property()");
	}

	tmp_device = (*argin)[0];
	if (!check_device_name(tmp_device))
	{
        	WARN_STREAM << "DataBase::db_delete_device_attribute(): device name  " << tmp_device << " incorrect ";
         	WARN_STREAM << endl;
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
				       	(const char *)"failed to delete device attribute, device name incorrect",
				       	(const char *)"DataBase::db_delete_device_attribute()");
	}
	attribute = (*argin)[1];
	
	{
		AutoLock al("LOCK TABLES property_attribute_device WRITE, property_attribute_device_hist WRITE,device_attribute_history_id WRITE",this);
		
		for (i=0; i<argin->length()-2; i++)
		{
			property = (*argin)[i+2];

			INFO_STREAM << "DataBase::db_delete_device_attribute_property(): delete device " << tmp_device ;
			INFO_STREAM << " attribute " << attribute << " property[" << i <<"] " << property << " from database" << endl;

// Is there something to delete ?

			sql_query_stream.str("");
			sql_query_stream << "SELECT count(*) FROM property_attribute_device WHERE device = \"" << tmp_device 
		                	 <<"\" AND attribute = \"" << attribute << "\" AND name = \"" << property << "\" ";
			result = query(sql_query_stream.str(),"db_delete_device_attribute_property()",al.get_con_nb());
 	    	row = mysql_fetch_row(result);
			int count = atoi(row[0]);
  	    	mysql_free_result(result);

			if(count) {

// then delete property from the property_attribute_device table

			  sql_query_stream.str("");
			  sql_query_stream << "DELETE FROM property_attribute_device WHERE device = \"" << tmp_device 
		                	   <<"\" AND attribute = \"" << attribute << "\" AND name = \"" << property << "\" ";
   			  DEBUG_STREAM << "DataBase::db_delete_device_attribute_property(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_delete_device_attribute_property()",al.get_con_nb());		
		
// Mark this property as deleted	

        	  unsigned int device_attribute_property_hist_id = get_id("device_attribute",al.get_con_nb());
			  sql_query_stream.str("");
			  sql_query_stream << "INSERT INTO property_attribute_device_hist SET device='" << tmp_device
							   << "',attribute='" << attribute
							   << "',name='" << property
							   << "',id='" << device_attribute_property_hist_id
							   << "',count='0',value='DELETED'";
	    	  DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_delete_device_attribute_property()",al.get_con_nb());

			}
			purge_att_property("property_attribute_device_hist","device",tmp_device.c_str(),attribute,property,al.get_con_nb());		
		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_class_attribute_property
 *
 *	description:	method to execute "DbDeleteClassAttributeProperty"
 *	delete a class attribute property from the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_class_attribute_property(const Tango::DevVarStringArray *argin)
{
	TangoSys_MemStream sql_query_stream;
	const char *attribute, *property;
	string tmp_class;
	int i;
	MYSQL_RES *result;
	MYSQL_ROW row;

	if (argin->length() < 3) {
   		WARN_STREAM << "DataBase::db_delete_class_attribute_property(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient number of arguments to delete class attribute property",
					       (const char *)"DataBase::db_delete_class_attribute_property()");
	}

	tmp_class = (*argin)[0];
	attribute = (*argin)[1];
	
	{
		AutoLock al("LOCK TABLES property_attribute_class WRITE,property_attribute_class_hist WRITE,class_attribute_history_id WRITE",this);
		 
		for (i=0; i<argin->length()-2; i++)
		{
			property = (*argin)[i+2];

			INFO_STREAM << "DataBase::db_delete_class_attribute_property(): delete class " << tmp_class ;
			INFO_STREAM << " attribute " << attribute << " property[" << i <<"] " << property << " from database" << endl;

// Is there something to delete ?

        	sql_query_stream.str("");
			sql_query_stream << "SELECT count(*) FROM property_attribute_class WHERE class = \"" << tmp_class
		                	 << "\" AND attribute = \"" << attribute << "\" AND name = \"" << property 
							 << "\" ";
   			DEBUG_STREAM << "DataBase::db_delete_class_attribute_property(): sql_query " << sql_query_stream.str() << endl;
			result = query(sql_query_stream.str(),"db_delete_class_attribute_property()",al.get_con_nb());
 	    	row = mysql_fetch_row(result);
			int count = atoi(row[0]);
  	    	mysql_free_result(result);

			if(count) {

// then delete property from the property_attribute_class table

        	  sql_query_stream.str("");
			  sql_query_stream << "DELETE FROM property_attribute_class WHERE class = \"" << tmp_class
		                	   << "\" AND attribute = \"" << attribute << "\" AND name = \"" << property 
							   << "\" ";
   			  DEBUG_STREAM << "DataBase::db_delete_class_attribute_property(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_delete_class_attribute_property()",al.get_con_nb());

// Mark this property as deleted	

        	  unsigned int class_attribute_property_hist_id = get_id("class_attribute",al.get_con_nb());
        	  sql_query_stream.str("");
			  sql_query_stream << "INSERT INTO property_attribute_class_hist SET class='" << tmp_class \
													 << "',attribute='" << attribute \
													 << "',name='" << property \
													 << "',id='" << class_attribute_property_hist_id \
													 << "',count='0',value='DELETED'";
	    	  DEBUG_STREAM << "DataBase::db_delete_class_attribute_property(): sql_query " << sql_query_stream.str() << endl;
			  simple_query(sql_query_stream.str(),"db_delete_class_attribute_property()",al.get_con_nb());

			}
			purge_att_property("property_attribute_class_hist","class",tmp_class.c_str(),attribute,property,al.get_con_nb());
		}
	}

  	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_host_servers_info
 *
 *	description:	method to execute "DbGetHostServersInfo"
 *	return info about all servers running on specified host, name, mode and level
 *
 * @param	argin	host name
 * @return	server info for all servers running on specified host
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_host_servers_info(Tango::DevString argin)
{
	//- struct timeval	t0, t;
	//- gettimeofday(&t0, NULL);
	INFO_STREAM << "DataBase::db_get_host_servers_info(): entering... !" << endl;
	//	Get server list
	Tango::DevVarStringArray	*servers = db_get_host_server_list(argin);
	Tango::DevVarStringArray	*argout  = new Tango::DevVarStringArray();
	argout->length(servers->length()*3);
	int		idx = 0;
	for (int i=0 ; i<servers->length() ; i++)
	{
		//	Get info for each server
		Tango::DevVarStringArray	*info = db_get_server_info((*servers)[i]);
		(*argout)[idx++] = CORBA::string_dup((*servers)[i]);//	Server name
		(*argout)[idx++] = CORBA::string_dup((*info)[2]);	//	Controlled ?
		(*argout)[idx++] = CORBA::string_dup((*info)[3]);	//	Startup level
		delete info;
	}
	delete servers;

	//	Check execution duration
	//- gettimeofday(&t, NULL);
	//- WARN_STREAM << argin << "; " << 1000.0*(t.tv_sec - t0.tv_sec) + 
	//-	((double)t.tv_usec - t0.tv_usec) / 1000 << " ms" << endl;
	return argout;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_server_info
 *
 *	description:	method to execute "DbGetServerInfo"
 *	return info about host, mode and level for specified server
 *
 * @param	argin	server name
 * @return	server info
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_server_info(Tango::DevString server_name)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0;
	Tango::DevVarStringArray *server_info = new Tango::DevVarStringArray;
	string tmp_name;

	INFO_STREAM << "DataBase::db_get_server_info(): server " << server_name << endl;

	server_info->length(4);
	(*server_info)[0] = CORBA::string_dup(server_name);

//	tmp_name = replace_wildcard(server_name);
    sql_query_stream << "SELECT host,mode,level FROM server WHERE name = '" << server_name << "';";
	DEBUG_STREAM << "DataBase::db_get_server_info(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_server_info()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_server_info(): mysql_num_rows() " << n_rows << endl;
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_get_server_info(): host "<< row[0] << " mode " << row[1] << " level " << row[2] << endl;
	      (*server_info)[1] = CORBA::string_dup(row[0]);
	      (*server_info)[2] = CORBA::string_dup(row[1]);
	      (*server_info)[3] = CORBA::string_dup(row[2]);
	   }
	}
	else
	{
	   (*server_info)[1] = CORBA::string_dup(" ");
	   (*server_info)[2] = CORBA::string_dup(" ");
	   (*server_info)[3] = CORBA::string_dup(" ");
	}
	mysql_free_result(result);

	return server_info;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_server_info
 *
 *	description:	method to execute "DbPutServerInfo"
 *	update server info including host, mode and level
 *
 * @param	argin	server info
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_server_info(const Tango::DevVarStringArray *server_info)
{
	TangoSys_MemStream sql_query_stream;
	long n_rows=0, i;
	const char *tmp_host, *tmp_mode, *tmp_level;
	string tmp_server;

	if (server_info->length() < 4) {
   		WARN_STREAM << "DataBase::db_put_server_info(): insufficient info for server ";
   		WARN_STREAM << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient server info",
					       (const char *)"DataBase::db_put_server_info()");
	}
	
	INFO_STREAM << "DataBase::db_put_server_info(): put " << server_info->length()-1 << " export info for device " << (*server_info)[0] << endl;

	tmp_server = (*server_info)[0];
// replace database wildcards (% and _)
//	string tmp_wildcard = replace_wildcard(tmp_server.c_str());
// replace uppercase by lowercase
	for (i=0; i<tmp_server.length(); i++) { 
		tmp_server[i] = tolower(tmp_server[i]);
	}
	tmp_host = (*server_info)[1];
	tmp_mode = (*server_info)[2];
	tmp_level = (*server_info)[3];

//
//	If it is an empty host name -> get previous host where running
//

	string previous_host("");
	{
		AutoLock al("LOCK TABLES device READ, server WRITE",this);
		
		if (fireToStarter==true)
		{
			if (tmp_host == "")
			{
				omni_mutex_lock oml(starter_mutex);

				//	Get database server name
				//--------------------------------------
				Tango::Util *tg = Tango::Util::instance();
				string	db_serv = tg->get_ds_name();
				transform(db_serv.begin(), db_serv.end(), db_serv.begin(), ::tolower);
				string	adm_dev = "dserver";
				adm_dev += tmp_server;

				char *tmp_ptr = db_get_device_host((Tango::DevString)adm_dev.c_str(),al.get_con_nb()); 
				previous_host = tmp_ptr;
				DEBUG_STREAM << tmp_server << " was running on " << previous_host << endl;
				CORBA::string_free(tmp_ptr);
			}
		}
// first delete the server from the server table

		sql_query_stream << "DELETE FROM server WHERE name = \"" << tmp_server << "\"";
		DEBUG_STREAM << "DataBase::db_put_server_info(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_put_server_info()",al.get_con_nb());

// insert the new info for this server

    	sql_query_stream.str("");
		sql_query_stream << "INSERT INTO server SET name=\'" << tmp_server << "\',host=\'" 
	                	 << tmp_host << "\',mode=\'" << tmp_mode << "\',level=\'" << tmp_level << "\'";
		DEBUG_STREAM << "DataBase::db_put_server_info(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_put_server_info()",al.get_con_nb());
	}

	//	Update host's starter to update controlled servers list
	if (fireToStarter==true)
	{
		omni_mutex_lock oml(starter_mutex);
		
		vector<string>	hosts;
		if (previous_host=="")
			hosts.push_back(tmp_host);
		else
			hosts.push_back(previous_host);
		starter_shared->send_starter_cmd(hosts);
	}

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_server_info
 *
 *	description:	method to execute "DbDeleteServerInfo"
 *	delete info related to a server
 *
 * @param	argin	server name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_server_info(Tango::DevString server_name)
{
	TangoSys_MemStream sql_query_stream;

	INFO_STREAM << "DataBase::db_delete_server_info(): delete " << server_name << " from database" << endl;

// replace database wildcards (% and _)

//	string tmp_wildcard = replace_wildcard(server_name);

// then delete the device from the device table
    sql_query_stream << "DELETE FROM server WHERE name = \"" << server_name << "\"";
    DEBUG_STREAM << "DataBase::db_delete_server_info(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_delete_server_info()");

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_exported_list
 *
 *	description:	method to execute "DbGetDeviceExportedList"
 *	Get a list of exported devices whose names satisfy the filter (wildcard is
 *
 * @param	argin	filter
 * @return	list of exported devices
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_exported_list(Tango::DevString filter)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_filter;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::db_get_device_exported_list(): filter " << filter << endl;

	if (filter == NULL)
	{
		sql_query_stream << "SELECT DISTINCT name FROM device WHERE name LIKE \"%\" AND exported=1 ORDER BY name";
	}
	else
	{
		tmp_filter = replace_wildcard(filter);
		sql_query_stream << "SELECT DISTINCT name FROM device WHERE (name LIKE \"" 
		                 << tmp_filter << "\" OR alias LIKE \"" << tmp_filter 
						 << "\") AND exported=1 ORDER BY name";
	}
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_exported_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_exported_list(): device_list[ "<< i << "] alias " << row[0] << endl;
	         (*device_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		device_list->length(0);
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceExportedList");
	return(device_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_exportd_device_list_for_class
 *
 *	description:	method to execute "DbGetExportdDeviceListForClass"
 *	Query the database for device exported for the specified class.
 *
 * @param	argin	Class name
 * @return	Device exported list
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_exportd_device_list_for_class(Tango::DevString classname)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_classname;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_exported_list(): classname " << classname << endl;

	if (classname == NULL)
	{
		sql_query_stream << "SELECT DISTINCT name FROM device WHERE class LIKE \"%\" AND exported=1 ORDER BY name";
	}
	else
	{
		tmp_classname = replace_wildcard(classname);
		sql_query_stream << "SELECT DISTINCT name FROM device WHERE class LIKE \"" << tmp_classname
		                 << "\" AND exported=1 ORDER BY name";
	}
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_exportd_device_list_for_class()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_exported_list(): device_list[ "<< i << "] alias " << row[0] << endl;
	         (*device_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		device_list->length(0);
	mysql_free_result(result);

	return(device_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_server_class_list
 *
 *	description:	method to execute "DbGetDeviceServerClassList"
 *	return list of device classes for a device server
 *
 * @param	argin	device server instance name
 * @return	list of classes for this device server
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_server_class_list(Tango::DevString server)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_server;

	INFO_STREAM << "DataBase::db_get_device_server_class_list(): server " << server << endl;

	tmp_server = replace_wildcard(server);
	sql_query_stream << "SELECT DISTINCT class FROM device WHERE server LIKE \"" << tmp_server 
	                 << "\" ORDER BY class";
	DEBUG_STREAM << "DataBase::db_get_device_server_class_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_server_class_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_server_class_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *class_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   class_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_server_class_list(): row[ "<< i << "] class " << row[0] << endl;
	         (*class_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		class_list->length(0);
	mysql_free_result(result);

	return(class_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_export_event
 *
 *	description:	method to execute "DbExportEvent"
 *
 * @param	argin	event channel or factory
 *
 */
//+------------------------------------------------------------------
void DataBase::db_export_event(const Tango::DevVarStringArray *export_info)
{
	TangoSys_MemStream sql_query_stream;
	long n_rows=0, i;
	const char *tmp_ior, *tmp_host, *tmp_pid, *tmp_version;
	string tmp_event, tmp_server;

	TimeVal	before, after;
	GetTime(before);

	if (export_info->length() < 5) {
   		WARN_STREAM << "DataBase::db_export_event(): insufficient export info for event ";
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient export info for event",
					       (const char *)"DataBase::db_export_event()");
	}
	
	INFO_STREAM << "DataBase::db_export_event(): put " << export_info->length()-1 << " export info for event " << (*export_info)[0] << endl;

	tmp_event = (*export_info)[0];
	for (i=0; i<tmp_event.length(); i++) { 
		tmp_event[i] = tolower(tmp_event[i]);
	}
	tmp_event   = replace_wildcard(tmp_event.c_str());
	tmp_ior     = (*export_info)[1];
	tmp_host    = (*export_info)[2];
	tmp_pid     = (*export_info)[3];
	tmp_version = (*export_info)[4];

// first delete existing information from database

	{
		AutoLock al("LOCK TABLE event WRITE",this);
		
		sql_query_stream << "DELETE FROM event WHERE name=\"" << tmp_event << "\"";
   		DEBUG_STREAM << "DataBase::db_export_event(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_export_event()",al.get_con_nb());

// update the new value for this tuple

		sql_query_stream.str("");
		sql_query_stream << "INSERT event set name=\'" << tmp_event
	                 << "\',exported=1,ior=\'" << tmp_ior << "\',host=\'" << tmp_host 
					 << "\',server=\'" << tmp_event << "\',pid=\'" << tmp_pid 
					 << "\',version=\'" << tmp_version << "\',started=NOW();";
		DEBUG_STREAM << "DataBase::export_event(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_export_event()",al.get_con_nb());
	}

	GetTime(after);
	update_timing_stats(before, after, "DbExportEvent");

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_import_event
 *
 *	description:	method to execute "DbImportEvent"
 *
 * @param	argin	name of event channel or factory
 * @return	export information e.g. IOR
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongStringArray *DataBase::db_import_event(Tango::DevString event_name)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, n_svalues=0, n_lvalues=0; 
	int i, exported, pid;
	string tmp_event;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::db_import_event(): get import info for " << event_name << endl;

	tmp_event = event_name;
	for (i=0; i<tmp_event.size(); i++) { 
		tmp_event[i] = tolower(tmp_event[i]);
	}
	tmp_event = replace_wildcard(tmp_event.c_str());

    sql_query_stream << "SELECT exported,ior,version,pid,host FROM event WHERE name = '" << tmp_event << "';";
	DEBUG_STREAM  << "DataBase::db_import_event(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_import_event()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_import_event(): mysql_num_rows() " << n_rows << endl;

	Tango::DevVarLongStringArray *import_info = new Tango::DevVarLongStringArray;
	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_import_event(): device[ "<< i << "] exported " << row[0] << " IOR  " << row[1] << " version " << row[2] << endl;
	      n_svalues = n_svalues+4;
	      (import_info->svalue).length(n_svalues);
	      (import_info->svalue)[n_svalues-4] = CORBA::string_dup(tmp_event.c_str());
	      (import_info->svalue)[n_svalues-3] = CORBA::string_dup(row[1]);
	      (import_info->svalue)[n_svalues-2] = CORBA::string_dup(row[2]);
	      (import_info->svalue)[n_svalues-1] = CORBA::string_dup(row[4]);
	      exported = -1;
	      if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = exported;
	      pid = -1;
	      if (row[3] != NULL) sscanf(row[3],"%d",&pid);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = pid;
	   }
	}
	else {
	     INFO_STREAM << "DataBase::db_import_event(): event not defined !" << endl;
   	 	 TangoSys_OMemStream o;
		 o << "event " << tmp_event << " not defined in the database !";
	     mysql_free_result(result);
		 delete import_info;
	     Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	     				                o.str(),
					                    (const char *)"DataBase::db_import_event()");
	}
	mysql_free_result(result);

	GetTime(after);
	update_timing_stats(before, after, "DbImportEvent");
	return(import_info);

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_un_export_event
 *
 *	description:	method to execute "DbUnExportEvent"
 *
 * @param	argin	name of event channel or factory to unexport
 *
 */
//+------------------------------------------------------------------
void DataBase::db_un_export_event(Tango::DevString event_name)
{
	TangoSys_MemStream sql_query_stream;
	long n_rows=0, i;
	string tmp_event;

	INFO_STREAM << "DataBase::un_export_event(): un-export " << event_name << " device " << endl;

	tmp_event = event_name;
	for (i=0; i<tmp_event.size(); i++) { 
		tmp_event[i] = tolower(tmp_event[i]);
	}
	tmp_event = replace_wildcard(tmp_event.c_str());

// un-export event from database by seting ior="not exported"
    
	sql_query_stream << "UPDATE event SET exported=0,stopped=NOW() WHERE name like \"" << tmp_event << "\"";
	DEBUG_STREAM << "DataBase::un_export_event(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_un_export_event()");
}

//============================================================================
//============================================================================
Tango::DevString DataBase::db_get_device_host(Tango::DevString argin,int con_nb)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_argin;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows;
	
	tmp_argin = replace_wildcard(argin);
	sql_query_stream << "SELECT host FROM device WHERE name LIKE \"" << tmp_argin << "\"";
	
	result = query(sql_query_stream.str(),"db_get_device_host()",con_nb);

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_host(): mysql_num_rows() " << n_rows << endl;
	Tango::DevString	argout;
	if (n_rows > 0)
	{
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			argout = CORBA::string_dup(row[0]);
		}
	}
	else
	{
 	    TangoSys_OMemStream o;
		o << "No device found for host \'" << argin << "\'";
		WARN_STREAM << o << endl;
		Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	   				                   o.str(),
					                   (const char *)"DataBase::db_get_device_host()");
	}
	mysql_free_result(result); //C.S. 05-10-2004

	return argout;
}


//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_alias_list
 *
 *	description:	method to execute "DbGetDeviceAliasList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_alias_list(Tango::DevString wildcard)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_alias_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sql_query_stream << "SELECT DISTINCT alias FROM device WHERE alias LIKE \"%\" ORDER BY alias";
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sql_query_stream << "SELECT DISTINCT alias FROM device WHERE alias LIKE \"" << tmp_wildcard 
		                 << "\" ORDER BY alias";
	}
	DEBUG_STREAM << "DataBase::db_get_device_alias_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_alias_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_alias_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *alias_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   alias_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
//	         DEBUG_STREAM << "DataBase::db_get_device_alias_list(): row[ "<< i << "] alias " << row[0] << endl;
	         (*alias_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		alias_list->length(0);
	mysql_free_result(result);

	return(alias_list);
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_alias
 *
 *	description:	method to execute "DbGetDeviceAlias"
 *	Return alias for device name if found.
 *
 * @param	argin	The device name
 * @return	The alias found
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_device_alias(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;
	string		devname(argin);
	string		tmp_devname;
	MYSQL_RES	*result;
	MYSQL_ROW	row;
	int			n_rows;

	INFO_STREAM << "DataBase::db_get_device_alias(): devname " << devname << endl;
	if (!check_device_name(devname))
	{
         	WARN_STREAM << "DataBase::db_get_device_alias(): device name  " << devname << " incorrect ";
         	WARN_STREAM << endl;
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
					       (const char *)"failed to delete device, device name incorrect",
					       (const char *)"DataBase::db_get_device_alias()");
	}

	tmp_devname = replace_wildcard(devname.c_str());
	sql_query_stream << "SELECT DISTINCT alias FROM device WHERE name LIKE \"" << tmp_devname 
	                 << "\" ORDER BY alias";
	DEBUG_STREAM << "DataBase::db_get_device_alias(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_alias()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_alias_(): mysql_num_rows() " << n_rows << endl;

	Tango::DevString	argout;
	if (n_rows > 0)
	{
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			if (row[0]==NULL)
			{
				mysql_free_result(result);
  	            TangoSys_OMemStream o;
				o << "No alias found for device \'" << devname << "\'";
				WARN_STREAM << o << endl;
				Tango::Except::throw_exception((const char *)DB_AliasNotDefined,
	   						                   o.str(),
							                   (const char *)"DataBase::db_get_device_alias()");
			}
			else
				argout = CORBA::string_dup(row[0]);
		}
	}
	else
	{
		mysql_free_result(result);
        TangoSys_OMemStream o;
		o << "No alias found for device \'" << devname << "\'";		
		WARN_STREAM << o << endl;
		Tango::Except::throw_exception((const char *)DB_AliasNotDefined,
	   				                   o.str(),
					                   (const char *)"DataBase::db_get_device_alias()");
	}
	mysql_free_result(result);
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_alias
 *
 *	description:	method to execute "DbPutDeviceAlias"
 *
 * @param	argin	device name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_alias(const Tango::DevVarStringArray *device_alias)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	long n_rows=0, i;
	const char *tmp_alias;
	string tmp_device;


	if (device_alias->length() < 2) {
   		WARN_STREAM << "DataBase::db_put_device_alias(): insufficient alias info for device ";
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient alias info for device",
					       (const char *)"DataBase::db_put_device_alias()");
	}

	tmp_device = (*device_alias)[0];
	tmp_alias = (*device_alias)[1];
	for (i=0; i<tmp_device.length(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
	INFO_STREAM << "DataBase::db_put_device_alias(): put " << tmp_alias << " for device " << tmp_device << endl;

// first check to see if this alias exists

	{
		AutoLock al("LOCK TABLE device WRITE",this);
		
    	sql_query_stream << "SELECT alias from device WHERE alias=\'" << tmp_alias 
	                	 << "\' AND name <> \'" << tmp_device << "\'";
		DEBUG_STREAM << "DataBase::db_put_device_alias(): sql_query " << sql_query_stream.str() << endl;

		result = query(sql_query_stream.str(),"db_put_device_alias()",al.get_con_nb());

		n_rows = mysql_num_rows(result);
		DEBUG_STREAM << "DataBase::db_put_device_alias(): mysql_num_rows() " << n_rows << endl;

		mysql_free_result(result);
		if (n_rows > 0)
		{
		   WARN_STREAM << "DataBase::db_put_device_alias(): this alias exists already !" << endl;
    	   TangoSys_OMemStream o;
		   o << "alias " << tmp_alias << " already exists !";
		   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				                	  o.str(),
					                	  (const char *)"DataBase::db_put_device_alias()");
		}

// update the new value for this tuple
    	sql_query_stream.str("");
		sql_query_stream << "UPDATE device set alias=\'" << tmp_alias 
	                	 << "\',started=NOW() where name LIKE \'" << tmp_device << "\'";
		DEBUG_STREAM << "DataBase::db_put_device_alias(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_put_device_alias()",al.get_con_nb());
	}

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_alias_device
 *
 *	description:	method to execute "DbGetAliasDevice"
 *	Return the device name for specified alias.
 *
 * @param	argin	specified alias.
 * @return	Device name found.
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_alias_device(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_argin;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows;

	if (argin == NULL)
	{
		sql_query_stream << "SELECT name FROM device WHERE alias LIKE \"%\"";
	}
	else
	{
		tmp_argin = replace_wildcard(argin);
		sql_query_stream << "SELECT name FROM device WHERE alias LIKE \"" << tmp_argin << "\"";
	}
	DEBUG_STREAM << "DataBase::db_get_alias_device(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_alias_device()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_alias_device(): mysql_num_rows() " << n_rows << endl;
	Tango::DevString	argout;
	if (n_rows > 0)
	{
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			argout = CORBA::string_dup(row[0]);
		}
		mysql_free_result(result);
	}
	else
	{
		mysql_free_result(result);
        TangoSys_OMemStream o;
	    o << "No device found for alias \'" << argin << "\'";
		WARN_STREAM << o << endl;
		Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	   				                   o.str(),
					                   (const char *)"DataBase::db_get_alias_device()");
	}

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_alias
 *
 *	description:	method to execute "DbDeleteDeviceAlias"
 *	Delete a device alias.
 *
 * @param	argin	alias name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_alias(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;

	// first check to see if this alias exists
	sql_query_stream << "UPDATE device SET alias=null WHERE alias=\'" << argin << "\' ";
	DEBUG_STREAM  << "DataBase::db_delete_device_alias(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_delete_device_alias()");
	
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_attribute_alias
 *
 *	description:	method to execute "DbPutAttributeAlias"
 *	Define an alias for an attribute
 *
 * @param	argin	attribute name, alias
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_attribute_alias(const Tango::DevVarStringArray *argin)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	long n_rows=0, i;
	const char *tmp_alias;
	string tmp_name, tmp_attribute, tmp_device;


	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_put_attribute_alias(): insufficient alias info for attribute ";
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient alias info for attribute",
					       (const char *)"DataBase::db_put_attribute_alias()");
	}

	tmp_name = (*argin)[0];
	tmp_alias = (*argin)[1];
	for (i=0; i<tmp_name.length(); i++) { 
		tmp_name[i] = tolower(tmp_name[i]);
	}
	INFO_STREAM << "DataBase::db_put_attribute_alias(): put " << tmp_alias << " for attribute " << tmp_name << endl;

// first check to see if this alias exists

	{
		AutoLock al("LOCK TABLE attribute_alias WRITE",this);
		
    	sql_query_stream << "SELECT alias from attribute_alias WHERE alias=\'" << tmp_alias 
	                	 << "\' AND name <> \'" << tmp_name << "\'";
		DEBUG_STREAM << "DataBase::db_put_attribute_alias(): sql_query " << sql_query_stream.str() << endl;

		result = query(sql_query_stream.str(),"db_put_attribute_alias()",al.get_con_nb());

		n_rows = mysql_num_rows(result);
		DEBUG_STREAM << "DataBase::db_put_attribute_alias(): mysql_num_rows() " << n_rows << endl;

		mysql_free_result(result);
		if (n_rows > 0)
		{
		   WARN_STREAM << "DataBase::db_put_attribute_alias(): this alias exists already !" << endl;
    	   TangoSys_OMemStream o;
		   o << "alias " << tmp_alias << " already exists !";
		   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				                	  o.str(),
					                	  (const char *)"DataBase::db_put_attribute_alias()");
		}

		string::size_type pos=0;
		int nsep=0;
		do 
		{
			if (pos != 0) pos++;
			pos = tmp_name.find("/",pos);
			if (pos != string::npos) nsep++;
			WARN_STREAM << "DataBase::db_put_attribute_alias(): found " << nsep << " separators , remaining string " << tmp_name.substr(pos+1) << endl;
		}
		while (pos != string::npos);
		if (nsep != 3)
		{
		   WARN_STREAM << "DataBase::db_put_attribute_alias(): attribute name has bad syntax, must have 3 / in it" << endl;
    	   TangoSys_OMemStream o;
		   o << "attribute name " << tmp_name << " has bad syntax, must have 3 / in it";
		   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				                	  o.str(),
					                	  (const char *)"DataBase::db_put_attribute_alias()");
		}
		tmp_device = tmp_name.substr(0,tmp_name.rfind("/"));
		tmp_attribute = tmp_name.substr(tmp_name.rfind("/")+1);
// first delete the current entry (if any)

    	sql_query_stream.str("");
		sql_query_stream << "DELETE FROM attribute_alias WHERE name=\'" << tmp_name << "\'";
		DEBUG_STREAM << "DataBase::db_put_attribute_alias(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_put_attribute_alias()",al.get_con_nb());

// update the new value for this tuple

    	sql_query_stream.str("");
		sql_query_stream << "INSERT attribute_alias SET alias=\'" << tmp_alias 
	                	 << "\',name=\'" << tmp_name << "\',device=\'" << tmp_device 
						 << "\',attribute=\'" << tmp_attribute << "\',updated=NOW()";
		DEBUG_STREAM << "DataBase::db_put_attribute_alias(): sql_query " << sql_query_stream.str() << endl;
		simple_query(sql_query_stream.str(),"db_put_attribute_alias()",al.get_con_nb());
	}

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_attribute_alias
 *
 *	description:	method to execute "DbGetAttributeAlias"
 *	Get the device attribute name for the given alias. If no alias corresponds then return an empty string.
 *
 * @param	argin	alias
 * @return	attribute name
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_attribute_alias(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0;
	Tango::DevString	argout  = new char[256];

	INFO_STREAM << "DataBase::db_get_attribute_alias(): put " << argin << endl;

// first check to see if this alias exists

	sql_query_stream << "SELECT name from attribute_alias WHERE alias LIKE \'" << argin << "\' ";
	DEBUG_STREAM << "DataBase::db_get_attribute_alias(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_attribute_alias()");
	
	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_attribute_alias(): mysql_num_rows() " << n_rows << endl;

	if (n_rows > 0)
	{
        if ((row = mysql_fetch_row(result)) != NULL)
        {
            DEBUG_STREAM << "DataBase::db_get_attribute_alias(): attribute name "<< row[0] << endl;
            strcpy(argout,row[0]);
		}
	}
	else
	{
		//strcpy(argout,"");
        TangoSys_OMemStream o;
	   	o << "No attribute found for alias  \'" << argin << "\'";
	   	Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  	               o.str(),
					  	               (const char *)"DataBase::db_get_attribute_alias()");
	}
	//	Add your own code to control device here

	mysql_free_result(result);

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_attribute_alias_list
 *
 *	description:	method to execute "DbGetAttributeAliasList"
 *
 * @param	argin	attribute alias
 * @return	attribute name
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_attribute_alias_list(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_attribute_alias_list(): alias " << argin;
	WARN_STREAM << " wildcard " << argin << endl;

	if (argin == NULL)
	{
		sql_query_stream << "SELECT DISTINCT alias FROM attribute_alias WHERE alias LIKE \"%\" ORDER BY attribute";
	}
	else
	{
		tmp_wildcard = replace_wildcard (argin);
		sql_query_stream << "SELECT DISTINCT alias FROM attribute_alias WHERE alias LIKE \"" 
		                 << tmp_wildcard << "\" ORDER BY attribute";
	}
	DEBUG_STREAM << "DataBase::db_get_attribute_alias_list(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_attribute_alias_list()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_attribute_alias_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *attribute_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   attribute_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_attribute_alias_list(): attribute[ "<< i << "] attribute " << row[0] << endl;
	         (*attribute_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		attribute_list->length(0);
	mysql_free_result(result);

	return(attribute_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_attribute_alias
 *
 *	description:	method to execute "DbDeleteAttributeAlias"
 *	Delete an attribute alias.
 *
 * @param	argin	alias name.
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_attribute_alias(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;

	// first check to see if this alias exists
	sql_query_stream << "DELETE  FROM attribute_alias WHERE alias=\'" << argin << "\' ";
	DEBUG_STREAM << "DataBase::db_delete_attribute_alias(): sql_query " << sql_query_stream.str() << endl;
	simple_query(sql_query_stream.str(),"db_delete_attribute_alias()");

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_attribute_property2
 *
 *	description:	method to execute "DbPutDeviceAttributeProperty2"
 *	Put device attribute property. This command adds the possibility to have attribute property
 *	which are arrays. Not possible with the old DbPutDeviceAttributeProperty command.
 *	This old command is not deleted for compatibility reasons.
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_attribute_property2(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "DataBase::db_put_device_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here
	TangoSys_MemStream sql_query_stream;
	char tmp_count_str[256];
	int n_attributes, n_properties=0, n_rows=0, tmp_count, i, j, k, l, jj;
	const char *tmp_device, *tmp_attribute, *tmp_name;

	TimeVal	before, after;
	GetTime(before);


	sscanf((*argin)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutAttributeProperty2(): put " << n_attributes << " attributes for device " << (*argin)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_attribute_device WRITE, property_attribute_device_hist WRITE,device_attribute_history_id WRITE",this);
	
		k = 2;
		for (i=0; i<n_attributes; i++)
		{
	   		tmp_device = (*argin)[0];
	   		tmp_attribute = (*argin)[k];
	   		sscanf((*argin)[k+1], "%d", &n_properties);
	   		for (jj=0; jj<n_properties; jj++)
	   		{
				j = k + 2;
	      		tmp_name = (*argin)[j];
			
// first delete the tuple (device,name,count) from the property table
				sql_query_stream.str("");
	      		sql_query_stream << "DELETE FROM property_attribute_device WHERE device LIKE \"" 
				                	 << tmp_device << "\" AND attribute LIKE \"" << tmp_attribute 
									 << "\" AND name LIKE \"" << tmp_name << "\" ";
	      		DEBUG_STREAM << "DataBase::PutAttributeProperty2(): sql_query " << sql_query_stream.str() << endl;
				simple_query(sql_query_stream.str(),"db_put_device_attribute_property2()",al.get_con_nb());

				sscanf((*argin)[j+1], "%d", &n_rows);
				tmp_count = 0;
            	unsigned int device_attribute_property_hist_id = get_id("device_attribute",al.get_con_nb());

	   			for (l=j+1; l<j+n_rows+1; l++)
	   			{
          			string tmp_escaped_string = escape_string((*argin)[l+1]);
	      			tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// then insert the new value for this tuple
					sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_attribute_device SET device=\'" 
					                	 << tmp_device << "\',attribute=\'" << tmp_attribute 
										 << "\',name=\'" << tmp_name << "\',count=\'" << tmp_count_str 
										 << "\',value=\'" << tmp_escaped_string << "\',updated=NULL,accessed=NULL";
	      			DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
					simple_query(sql_query_stream.str(),"db_put_device_attribute_property2()",al.get_con_nb());

// then insert the new value into the history table
					sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_attribute_device_hist SET device=\'" 
					                	 << tmp_device << "\',attribute=\'" << tmp_attribute 
										 << "\',name=\'" << tmp_name << "\',count=\'" << tmp_count_str 
										 << "\',id=\'" << device_attribute_property_hist_id 
										 << "\',value=\'" << tmp_escaped_string << "\'";
	      			DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
					simple_query(sql_query_stream.str(),"db_put_device_attribute_property2()",al.get_con_nb());

				}
				purge_att_property("property_attribute_device_hist","device",tmp_device,tmp_attribute,tmp_name,al.get_con_nb());
				k = k + n_rows + 2;
	   		}
	   		k = k+2;
		}
	}

	GetTime(after);
	update_timing_stats(before, after, "DbPutDeviceAttributeProperty2");
	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_property2
 *
 *	description:	method to execute "DbGetDeviceAttributeProperty2"
 *	Retrieve device attribute properties. This command has the possibility to retrieve
 *	device attribute properties which are arrays. It is not possible with the old
 *	DbGetDeviceAttributeProperty command. Nevertheless, the old command has not been
 *	deleted for compatibility reason
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_property2(const Tango::DevVarStringArray *property_names)
{
	DEBUG_STREAM << "DataBase::db_get_device_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here

	TangoSys_MemStream sql_query_stream;
	char n_attributes_str[256];
	char n_rows_str[256];
	char prop_size_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_device, *tmp_attribute;

	TimeVal	before, after;
	GetTime(before);

	INFO_STREAM << "DataBase::GetDeviceAttributeProperty2(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_device);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   	tmp_attribute = (*property_names)[i];
		sql_query_stream.str("");
		sql_query_stream << "SELECT name,value FROM property_attribute_device WHERE device = \""
		                 << tmp_device << "\" AND attribute LIKE \"" << tmp_attribute 
						 << "\" ORDER BY name,count";
	   	DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): sql_query " << sql_query_stream.str() << endl;
		
		result = query(sql_query_stream.str(),"db_get_device_attribute_property2()");

	   	n_rows = mysql_num_rows(result);
	   	DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): mysql_num_rows() " << n_rows << endl;
	   	n_props = n_props+2;
	   	property_list->length(n_props);
	   	(*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
		int prop_number_idx = n_props-1;
		int prop_number = 0;
	   	if (n_rows > 0)
	   	{
			string name, old_name;
			bool new_prop = true;
			int prop_size_idx;
			int prop_size = 0;
	      	for (j=0; j<n_rows; j++)
	      	{
	        	if ((row = mysql_fetch_row(result)) != NULL)
	         	{
					name = row[0];
					if (j == 0)
						old_name = name;
					else
					{
						name = row[0];
						if (name != old_name)
						{
							new_prop = true;
							old_name = name;
						}
						else
							new_prop = false;
					}
//	            			DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
					if (new_prop == true)
					{
						n_props = n_props + 3;
						property_list->length(n_props);
	            		(*property_list)[n_props-3] = CORBA::string_dup(row[0]);
	            		(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						if (prop_size != 0)
						{
							sprintf(prop_size_str,"%d",prop_size);
							(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
							prop_number++;
						}						
						prop_size_idx = n_props - 2;
						prop_size = 1;
					}
					else
					{
						n_props = n_props + 1;
						property_list->length(n_props);
						(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						prop_size++;
					}
	         	}
	      	}
			if (prop_size != 0)
			{
				sprintf(prop_size_str,"%d",prop_size);
				(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
				prop_number++;
			}
	   	}
	   	sprintf(n_rows_str,"%d",prop_number);
		(*property_list)[prop_number_idx] = CORBA::string_dup(n_rows_str);
	   	mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): property_list->length() "<< property_list->length() << endl;
	
	GetTime(after);
	update_timing_stats(before, after, "DbGetDeviceAttributeProperty2");
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_class_attribute_property2
 *
 *	description:	method to execute "DbPutClassAttributeProperty2"
 *	This command adds support for array properties compared to the previous one
 *	called DbPutClassAttributeProperty. The old comman is still there for compatibility reason
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_class_attribute_property2(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "DataBase::db_put_class_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here
	TangoSys_MemStream sql_query_stream;
	char tmp_count_str[256];
	int n_attributes, n_properties=0, n_rows=0, tmp_count, i, j, k, l, jj;
	const char *tmp_class, *tmp_attribute, *tmp_name;

	sscanf((*argin)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutClassAttributeProperty2(): put " << n_attributes << " attributes for device " << (*argin)[0] << endl;

	{
		AutoLock al("LOCK TABLES property_attribute_class WRITE, property_attribute_class_hist WRITE,class_attribute_history_id WRITE",this);
		
		k = 2;
		for (i=0; i<n_attributes; i++)
		{
	   		tmp_class = (*argin)[0];
	   		tmp_attribute = (*argin)[k];
	   		sscanf((*argin)[k+1], "%d", &n_properties);
	   		for (jj=0; jj<n_properties; jj++)
	   		{
				j = k + 2;
	      		tmp_name = (*argin)[j];
			
// first delete the tuple (device,name,count) from the property table

				sql_query_stream.str("");
				sql_query_stream << "DELETE FROM property_attribute_class WHERE class LIKE \"" 
			                	 << tmp_class << "\" AND attribute LIKE \"" << tmp_attribute 
								 << "\" AND name LIKE \"" << tmp_name << "\" ";
	      		DEBUG_STREAM << "DataBase::PutClassAttributeProperty2(): sql_query " << sql_query_stream.str() << endl;
				simple_query(sql_query_stream.str(),"db_put_class_attribute_property2()",al.get_con_nb());

				sscanf((*argin)[j+1], "%d", &n_rows);
				tmp_count = 0;
				unsigned int class_attribute_property_hist_id = get_id("class_attribute",al.get_con_nb());
	   			for (l=j+1; l<j+n_rows+1; l++)
	   			{
          				string tmp_escaped_string = escape_string((*argin)[l+1]);
	      				tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// then insert the new value for this tuple

 			        sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_attribute_class SET class=\'" 
					                 << tmp_class << "\',attribute=\'" << tmp_attribute 
									 << "\',name=\'" << tmp_name << "\',count=\'" << tmp_count_str
									 << "\',value=\'" << tmp_escaped_string 
									 << "\',updated=NULL,accessed=NULL";
	      			DEBUG_STREAM << "DataBase::PutClassAttributeProperty2(): sql_query " << sql_query_stream.str() << endl;
			        simple_query(sql_query_stream.str(),"db_put_class_attribute_property2()",al.get_con_nb());

// then insert the new value into the history table

 			        sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_attribute_class_hist SET class=\'" 
					                 << tmp_class << "\',attribute=\'" << tmp_attribute 
									 << "\',name=\'" << tmp_name << "\',count=\'" << tmp_count_str
									 << "\',id=\'" << class_attribute_property_hist_id 
									 << "\',value=\'" << tmp_escaped_string << "\'"; 
	      			DEBUG_STREAM << "DataBase::PutClassAttributeProperty2(): sql_query " << sql_query_stream.str() << endl;
			        simple_query(sql_query_stream.str(),"db_put_class_attribute_property2()",al.get_con_nb());
					
				}
		        purge_att_property("property_attribute_class_hist","class",tmp_class,tmp_attribute,tmp_name,al.get_con_nb());
				k = k + n_rows + 2;
	   		}
	   		k = k+2;
		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_property2
 *
 *	description:	method to execute "DbGetClassAttributeProperty2"
 *	This command supports array property compared to the old command called
 *	DbGetClassAttributeProperty. The old command has not been deleted from the
 *	server for compatibility reasons.
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_property2(const Tango::DevVarStringArray *property_names)
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter : Writing a TANGO DS / Exchanging data)
	//------------------------------------------------------------
	DEBUG_STREAM << "DataBase::db_get_class_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here

	TangoSys_MemStream sql_query_stream;
	char n_attributes_str[256];
	char n_rows_str[256];
	char prop_size_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_class, *tmp_attribute;

	INFO_STREAM << "DataBase::GetClassAttributeProperty2(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_class);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   	tmp_attribute = (*property_names)[i];
		sql_query_stream.str("");
		sql_query_stream << "SELECT name,value FROM property_attribute_class WHERE class = \"" 
		                 << tmp_class << "\" AND attribute LIKE \"" << tmp_attribute
						 << "\" ORDER BY name,count";
	   	DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): sql_query " << sql_query_stream.str() << endl;

		result = query(sql_query_stream.str(),"db_get_class_attribute_property2()");
		
	   	n_rows = mysql_num_rows(result);
	   	DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): mysql_num_rows() " << n_rows << endl;
	   	n_props = n_props+2;
	   	property_list->length(n_props);
	   	(*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
		int prop_number_idx = n_props-1;
		int prop_number = 0;
	   	if (n_rows > 0)
	   	{
			string name, old_name;
			bool new_prop = true;
			int prop_size_idx;
			int prop_size = 0;
	      		for (j=0; j<n_rows; j++)
	      		{
	        		if ((row = mysql_fetch_row(result)) != NULL)
	         		{
					name = row[0];
					if (j == 0)
						old_name = name;
					else
					{
						name = row[0];
						if (name != old_name)
						{
							new_prop = true;
							old_name = name;
						}
						else
							new_prop = false;
					}
//	            			DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
					if (new_prop == true)
					{
						n_props = n_props + 3;
						property_list->length(n_props);
	            				(*property_list)[n_props-3] = CORBA::string_dup(row[0]);
	            				(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						if (prop_size != 0)
						{
							sprintf(prop_size_str,"%d",prop_size);
							(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
							prop_number++;
						}						
						prop_size_idx = n_props - 2;
						prop_size = 1;
					}
					else
					{
						n_props = n_props + 1;
						property_list->length(n_props);
						(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						prop_size++;
					}
	         		}
	      		}
			if (prop_size != 0)
			{
				sprintf(prop_size_str,"%d",prop_size);
				(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
				prop_number++;
			}
	   	}
	   	sprintf(n_rows_str,"%d",prop_number);
		(*property_list)[prop_number_idx] = CORBA::string_dup(n_rows_str);
	   	mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): property_list->length() "<< property_list->length() << endl;
	
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_info
 *
 *	description:	method to execute "DbGetDeviceInfo"
 *	Returns info from DbImportDevice and started/stopped dates.
 *
 * @param	argin	Device name
 * @return	Info from DbImportDevice and started/stopped dates.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongStringArray *DataBase::db_get_device_info(Tango::DevString argin)
{
	TangoSys_MemStream sql_query_stream;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, n_svalues=0, n_lvalues=0; 
	int i, exported, pid;
	string tmp_device;

	INFO_STREAM << "DataBase::ImportDevice(): get import info for " << argin << " device " << endl;

	tmp_device = argin;
	for (i=0; i<tmp_device.size(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}

	sql_query_stream << "SELECT exported,ior,version,pid,server,host,started,stopped FROM device WHERE name = '" 
	                 << tmp_device << "' or alias = '" << tmp_device << "';";
	DEBUG_STREAM << "DataBase::ImportDevice(): sql_query " << sql_query_stream.str() << endl;
	
	result = query(sql_query_stream.str(),"db_get_device_info()");

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::ImportDeviceList(): mysql_num_rows() " << n_rows << endl;

	Tango::DevVarLongStringArray *argout = new Tango::DevVarLongStringArray;
	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::ImportDeviceList(): device[ "<< i << "] exported " << row[0] << " version " << row[2] << " server " << row[4] << " host " << row[5] << endl;
	      n_svalues = 7;
		  if ((row[4] == NULL) || (row[5] == NULL))
		  {
	         TangoSys_OMemStream o;
			 o << "Wrong info in database for device " << tmp_device;
			 o << "Database entry seems corrupted (server or host column NULL)" << ends;
	    	 mysql_free_result(result);
			 delete argout;
	    	 Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
								            o.str(),
								            (const char *)"DataBase::GetDeviceInfo()");
		  }
	      (argout->svalue).length(n_svalues);
	      (argout->svalue)[0] = CORBA::string_dup(tmp_device.c_str());
	      (argout->svalue)[2] = CORBA::string_dup(row[2]);
	      (argout->svalue)[3] = CORBA::string_dup(row[4]);
	      (argout->svalue)[4] = CORBA::string_dup(row[5]);
			//	IOR Check
		  if (row[1]!=NULL)
	      	(argout->svalue)[1] = CORBA::string_dup(row[1]);
		  else
	      	(argout->svalue)[1] = CORBA::string_dup("");

		//	Convert date format
		char	*format = (char *)"%D %M %Y at %H:%i:%s";
		MYSQL_RES *result2;
		MYSQL_ROW row2;
		for (int x=0 ; x<2 ; x++)
		{
			if (row[x+6]!=NULL)
			{
				sql_query_stream.str("");
				sql_query_stream << "SELECT DATE_FORMAT(\'" << row[x+6] << "\',\'" << format << "\')";
				result2 = query(sql_query_stream.str(),"db_get_device_info()");
				int nb = mysql_num_rows(result2);
				if (nb > 0)
				{
					if ((row2 = mysql_fetch_row(result2)) != NULL && row2[0] != NULL) //add extra check for MySQL 5
						(argout->svalue)[5+x] = CORBA::string_dup(row2[0]);
					else (argout->svalue)[5+x] = CORBA::string_dup("?"); // empty date-> row2[0]==NULL !!!
				}
				else
					(argout->svalue)[5+x] = CORBA::string_dup("?");
             	mysql_free_result(result2);
			}
			else
				(argout->svalue)[5+x] = CORBA::string_dup("?");
		}



	      exported = -1;
	      if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	      n_lvalues++;
	      (argout->lvalue).length(n_lvalues);
	      (argout->lvalue)[n_lvalues-1] = exported;
	      pid = -1;
	      if (row[3] != NULL) sscanf(row[3],"%d",&pid);
	      n_lvalues++;
	      (argout->lvalue).length(n_lvalues);
	      (argout->lvalue)[n_lvalues-1] = pid;
	   }
		else {
	    	 INFO_STREAM << "DataBase::ImportDevice(): info not defined !" << endl;
	         TangoSys_OMemStream o;
			 o << "device " << tmp_device << " import info not found in the database !";
	    	 mysql_free_result(result);
			 delete argout;
	    	 Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
								            o.str(),
								            (const char *)"DataBase::GetDeviceInfo()");
		}
	}
	else {
	     INFO_STREAM << "DataBase::ImportDevice(): device not defined !" << endl;
         TangoSys_OMemStream o;
		 o << "device " << tmp_device << " not defined in the database !";
	     mysql_free_result(result);
		 delete argout;
	     Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
							            o.str(),
							            (const char *)"DataBase::GetDeviceInfo()");
	}
	mysql_free_result(result);

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::reset_timing_values
 *
 *	description:	method to execute "ResetTimingValues"
 *	Reset the timing attribute values.
 *
 *
 */
//+------------------------------------------------------------------
void DataBase::reset_timing_values()
{
	DEBUG_STREAM << "DataBase::reset_timing_values(): entering... !" << endl;

	timing_stats_mutex.lock();
	
	//	Loop over map to re-initialise remaining variables
	std::map<std::string,TimingStatsStruct*>::iterator iter;
	for (iter=timing_stats_map.begin(); iter!=timing_stats_map.end(); iter++)
	{
		iter->second->average       = 
		iter->second->minimum       =
		iter->second->maximum       =
		iter->second->total_elapsed =
		iter->second->calls         = 0.0;
	}
	
	timing_stats_mutex.unlock();
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_property_hist
 *
 *	description:	method to execute "DbGetDevicePropertyHist"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_property_hist(const Tango::DevVarStringArray *argin)
{

	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *ids;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Tango::DevVarStringArray *property_hist = new Tango::DevVarStringArray;
	const char *tmp_device;
	string      tmp_name;

	if (argin->length() != 2)
	{
	   WARN_STREAM << "DataBase::GetDevicePropertyHist(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 2 (device,property)",
					  (const char *)"DataBase::GetDevicePropertyHist()");
	}

	tmp_device = (*argin)[0];
	tmp_name   = replace_wildcard((*argin)[1]);
	
	// Get id list

    sql_query_stream << "SELECT DISTINCT id FROM property_device_hist WHERE device = \"" 
	                 << tmp_device << "\" AND name LIKE \"" << tmp_name << "\" ORDER by date ASC";

	{
		AutoLock al("LOCK TABLE property_device_hist READ",this);
							 
		ids = query(sql_query_stream.str(),"db_get_device_property_hist()",al.get_con_nb());
	
		// Retreive history
		int i,j;
		int nb_item = 0;
		property_hist->length(0);

		for (i=0; i<mysql_num_rows(ids); i++)
		{
		   row = mysql_fetch_row(ids);
		   unsigned int id = atoi(row[0]);
		   sql_query_stream.str("");
		   sql_query_stream << "SELECT DATE_FORMAT(date,'%Y-%m-%d %H:%i:%s'),value,name,count FROM property_device_hist WHERE id = \"" 
	                    	<< id << "\" AND device = \"" << tmp_device << "\" ORDER BY count ASC";

		   result = query(sql_query_stream.str(),"db_get_device_property_hist()",al.get_con_nb());

		   int count = mysql_num_rows(result);

    	   row = mysql_fetch_row(result);
  		   int deleted = (atoi(row[3]) == 0); // count=0 for deleted property
  		   if(deleted) count = 0;
		   char n_rows_str[256];
		   sprintf(n_rows_str,"%d",count);

    	   property_hist->length(nb_item+3+count);
		   (*property_hist)[nb_item+0] = CORBA::string_dup(row[2]);
    	   (*property_hist)[nb_item+1] = CORBA::string_dup(row[0]);
    	   (*property_hist)[nb_item+2] = CORBA::string_dup(n_rows_str);

		   for(j=0;j<count;j++) {
        	 (*property_hist)[nb_item+3+j] = CORBA::string_dup(row[1]);
        	 row = mysql_fetch_row(result);
		   }

		   nb_item += 3+count;
		   mysql_free_result(result);
		}
	}
	
	mysql_free_result(ids);

	return(property_hist);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_property_hist
 *
 *	description:	method to execute "DbGetDeviceAttributePropertyHist"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_property_hist(const Tango::DevVarStringArray *argin)
{

	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *ids;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Tango::DevVarStringArray *property_hist = new Tango::DevVarStringArray;
	const char *tmp_device;
	string      tmp_attribute;
	string      tmp_name;

	if (argin->length() != 3)
	{
	   WARN_STREAM << "DataBase::DbGetDeviceAttributePropertyHist(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 3 (device,attribute,property)",
					  (const char *)"DataBase::DbGetDeviceAttributePropertyHist()");
	}

	tmp_device    = (*argin)[0];
	tmp_attribute = replace_wildcard((*argin)[1]);
	tmp_name      = replace_wildcard((*argin)[2]);
	
	// Get id list

    sql_query_stream << "SELECT DISTINCT id FROM property_attribute_device_hist WHERE device = \"" 
	                 << tmp_device << "\" AND attribute LIKE \"" << tmp_attribute 
 			         << "\" AND name LIKE \"" << tmp_name << "\" ORDER by date ASC";

	{
		AutoLock al("LOCK TABLE property_attribute_device_hist READ",this);
							 
		ids = query(sql_query_stream.str(),"db_get_device_attribute_property_hist()",al.get_con_nb());

		// Retreive history
		int i,j;
		int nb_item = 0;
		property_hist->length(0);

		for (i=0; i<mysql_num_rows(ids); i++)
		{
		   row = mysql_fetch_row(ids);
		   unsigned int id = atoi(row[0]);
		   sql_query_stream.str("");
		   sql_query_stream << "SELECT DATE_FORMAT(date,'%Y-%m-%d %H:%i:%s'),value,attribute,name,count FROM property_attribute_device_hist WHERE id = \"" 
	                    	<< id << "\" AND device = \"" << tmp_device << "\" ORDER BY count ASC";

		   result = query(sql_query_stream.str(),"db_get_device_attribute_property_hist()",al.get_con_nb());

		   int count = mysql_num_rows(result);

    	   row = mysql_fetch_row(result);
  		   int deleted = (atoi(row[4]) == 0); // count=0 for deleted property
  		   if(deleted) count = 0;
		   char n_rows_str[256];
		   sprintf(n_rows_str,"%d",count);

    	   property_hist->length(nb_item+4+count);
		   (*property_hist)[nb_item+0] = CORBA::string_dup(row[2]);
    	   (*property_hist)[nb_item+1] = CORBA::string_dup(row[3]);
    	   (*property_hist)[nb_item+2] = CORBA::string_dup(row[0]);
    	   (*property_hist)[nb_item+3] = CORBA::string_dup(n_rows_str);

		   for(j=0;j<count;j++) {
        	 (*property_hist)[nb_item+4+j] = CORBA::string_dup(row[1]);
        	 row = mysql_fetch_row(result);
		   }

		   nb_item += 4+count;
		   mysql_free_result(result);
		}
	}
	
	mysql_free_result(ids);

	return(property_hist);

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_property_hist
 *
 *	description:	method to execute "DbGetClassAttributePropertyHist"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_property_hist(const Tango::DevVarStringArray *argin)
{

	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *ids;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Tango::DevVarStringArray *property_hist = new Tango::DevVarStringArray;
	const char *tmp_class;
	string      tmp_attribute;
	string      tmp_name;

	if (argin->length() != 3)
	{
	   WARN_STREAM << "DataBase::DbGetClassAttributePropertyHist(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 3 (class,attribute,property)",
					  (const char *)"DataBase::DbGetClassAttributePropertyHist()");
	}

	tmp_class     = (*argin)[0];
	tmp_attribute = replace_wildcard((*argin)[1]);
	tmp_name      = replace_wildcard((*argin)[2]);
	
	// Get id list

    sql_query_stream << "SELECT DISTINCT id FROM property_attribute_class_hist WHERE class = \"" 
                     << tmp_class << "\" AND attribute LIKE \"" << tmp_attribute 
				     << "\" AND name LIKE \"" << tmp_name << "\" ORDER by date ASC";

	{
		AutoLock al("LOCK TABLE property_attribute_class_hist READ",this);
							 
		ids = query(sql_query_stream.str(),"db_get_class_attribute_property_hist()",al.get_con_nb());
	
		// Retreive history
		int i,j;
		int nb_item = 0;
		property_hist->length(0);

		for (i=0; i<mysql_num_rows(ids); i++)
		{
		   row = mysql_fetch_row(ids);
		   unsigned int id = atoi(row[0]);
		   sql_query_stream.str("");
		   sql_query_stream << "SELECT DATE_FORMAT(date,'%Y-%m-%d %H:%i:%s'),value,attribute,name,count FROM property_attribute_class_hist WHERE id = \"" 
	                    	<< id << "\" AND class = \"" << tmp_class << "\" ORDER BY count ASC";

		   result = query(sql_query_stream.str(),"db_get_class_attribute_property_hist()",al.get_con_nb());

		   int count = mysql_num_rows(result);

    	   row = mysql_fetch_row(result);
  		   int deleted = (atoi(row[4]) == 0); // count=0 for deleted property
  		   if(deleted) count = 0;
		   char n_rows_str[256];
		   sprintf(n_rows_str,"%d",count);

    	   property_hist->length(nb_item+4+count);
		   (*property_hist)[nb_item+0] = CORBA::string_dup(row[2]);
    	   (*property_hist)[nb_item+1] = CORBA::string_dup(row[3]);
    	   (*property_hist)[nb_item+2] = CORBA::string_dup(row[0]);
    	   (*property_hist)[nb_item+3] = CORBA::string_dup(n_rows_str);

		   for(j=0;j<count;j++) {
            	 (*property_hist)[nb_item+4+j] = CORBA::string_dup(row[1]);
            	 row = mysql_fetch_row(result);
		   }

		   nb_item += 4+count;
		   mysql_free_result(result);
		}
	}
	
	mysql_free_result(ids);

	return(property_hist);

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_property_hist
 *
 *	description:	method to execute "DbGetClassPropertyHist"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_property_hist(const Tango::DevVarStringArray *argin)
{

	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *ids;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Tango::DevVarStringArray *property_hist = new Tango::DevVarStringArray;
	const char *tmp_class;
	string      tmp_name;

	if (argin->length() != 2)
	{
	   WARN_STREAM << "DataBase::DbGetClassPropertyHist(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 2 (class,property)",
					  (const char *)"DataBase::DbGetClassPropertyHist()");
	}

	tmp_class  = (*argin)[0];
	tmp_name   = replace_wildcard((*argin)[1]);
	
	// Get id list

    sql_query_stream << "SELECT DISTINCT id FROM property_class_hist WHERE class = \"" 
	                 << tmp_class << "\" AND name LIKE \"" << tmp_name << "\" ORDER by date ASC";

	{
		AutoLock al("LOCK TABLE property_class_hist READ",this);
							 
		ids = query(sql_query_stream.str(),"db_get_class_property_hist()",al.get_con_nb());

		// Retreive history
		int i,j;
		int nb_item = 0;
		property_hist->length(0);

		for (i=0; i<mysql_num_rows(ids); i++)
		{
		   row = mysql_fetch_row(ids);
		   unsigned int id = atoi(row[0]);
		   sql_query_stream.str("");
		   sql_query_stream << "SELECT DATE_FORMAT(date,'%Y-%m-%d %H:%i:%s'),value,name,count FROM property_class_hist WHERE id = \"" 
	                    	<< id << "\" AND class = \"" << tmp_class << "\" ORDER BY count ASC";

		   result = query(sql_query_stream.str(),"db_get_class_property_hist()",al.get_con_nb());

		   int count = mysql_num_rows(result);

    	   row = mysql_fetch_row(result);
  		   int deleted = (atoi(row[3]) == 0); // count=0 for deleted property
  		   if(deleted) count = 0;
		   char n_rows_str[256];
		   sprintf(n_rows_str,"%d",count);

    	   property_hist->length(nb_item+3+count);
		   (*property_hist)[nb_item+0] = CORBA::string_dup(row[2]);
    	   (*property_hist)[nb_item+1] = CORBA::string_dup(row[0]);
    	   (*property_hist)[nb_item+2] = CORBA::string_dup(n_rows_str);

		   for(j=0;j<count;j++) {
        	 (*property_hist)[nb_item+3+j] = CORBA::string_dup(row[1]);
        	 row = mysql_fetch_row(result);
		   }

		   nb_item += 3+count;
		   mysql_free_result(result);
		}
	}
	
	mysql_free_result(ids);

	return(property_hist);

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_property_hist
 *
 *	description:	method to execute "DbGetPropertyHist"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_property_hist(const Tango::DevVarStringArray *argin)
{

	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *ids;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Tango::DevVarStringArray *property_hist = new Tango::DevVarStringArray;
	const char *tmp_object;
	string      tmp_name;

	if (argin->length() != 2)
	{
	   WARN_STREAM << "DataBase::DbGetPropertyHist(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"incorrect no. of input arguments, needs 2 (object,property)",
					  (const char *)"DataBase::DbGetPropertyHist()");
	}

	tmp_object  = (*argin)[0];
	tmp_name   = replace_wildcard((*argin)[1]);
	
	// Get id list

    sql_query_stream << "SELECT DISTINCT id FROM property_hist WHERE object = \"" 
	                 << tmp_object << "\" AND name LIKE \"" << tmp_name << "\" ORDER by date";

	{
		AutoLock al("LOCK TABLE property_hist READ",this);
							 
		ids = query(sql_query_stream.str(),"db_get_property_hist()",al.get_con_nb());

		// Retreive history
		int i,j;
		int nb_item = 0;
		property_hist->length(0);

		for (i=0; i<mysql_num_rows(ids); i++)
		{
		   row = mysql_fetch_row(ids);
		   unsigned int id = atoi(row[0]);
		   sql_query_stream.str("");
		   sql_query_stream << "SELECT DATE_FORMAT(date,'%Y-%m-%d %H:%i:%s'),value,name,count FROM property_hist WHERE id = \"" 
	                    	<< id << "\" AND object = \"" << tmp_object << "\" ORDER BY count";

		   result = query(sql_query_stream.str(),"db_get_property_hist()",al.get_con_nb());

		   int count = mysql_num_rows(result);

    	   row = mysql_fetch_row(result);
  		   int deleted = (atoi(row[3]) == 0); // count=0 for deleted property
  		   if(deleted) count = 0;
		   char n_rows_str[256];
		   sprintf(n_rows_str,"%d",count);

    	   property_hist->length(nb_item+3+count);
		   (*property_hist)[nb_item+0] = CORBA::string_dup(row[2]);
    	   (*property_hist)[nb_item+1] = CORBA::string_dup(row[0]);
    	   (*property_hist)[nb_item+2] = CORBA::string_dup(n_rows_str);

		   for(j=0;j<count;j++) {
        	 (*property_hist)[nb_item+3+j] = CORBA::string_dup(row[1]);
        	 row = mysql_fetch_row(result);
		   }

		   nb_item += 3+count;
		   mysql_free_result(result);
		}
	}
	
	mysql_free_result(ids);

	return(property_hist);

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_for_device
 *
 *	description:	method to execute "DbGetClassForDevice"
 *	Search the class of the specified device.
 *
 * @param	argin	Device name
 * @return	Class off the specified device
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_class_for_device(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_get_class_for_device(): entering... !" << endl;
	//	Get class for device
	TangoSys_MemStream	tms;
	tms << "SELECT DISTINCT class FROM device WHERE name=\""
				<< argin <<  "\"";
	DEBUG_STREAM << "DataBase::db_get_class_for_device(): sql_query " << tms.str() << endl;
	
	Tango::DevString	argout;
	MYSQL_RES *result = query(tms.str(), "db_get_class_for_device()");
	int	n_rows = mysql_num_rows(result);
	if (n_rows==0)
	{
		TangoSys_MemStream	tms;
		tms << "Class not found for " << argin;
		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)tms.str().c_str(),
					  (const char *)"DataBase::db_get_class_for_device()");	
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
 *	method:	DataBase::db_get_class_inheritance_for_device
 *
 *	description:	method to execute "DbGetClassInheritanceForDevice"
 *	Search the class inheritance of the specified device.
 *
 * @param	argin	Device name
 * @return	Classes off the specified device.\n[0] - is the class of the device.\n[1] - is the class from the device class is inherited.\n........and so on
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_inheritance_for_device(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_get_class_inheritance_for_device(): entering... !" << endl;

	//	Get class for the specified device
	string	classname = db_get_class_for_device(argin);
	
	//	Get its inheritance
	Tango::DevVarStringArray	*array = new Tango::DevVarStringArray();
	array->length(2);
	(*array)[0] = CORBA::string_dup(classname.c_str());
	(*array)[1] = CORBA::string_dup("InheritedFrom");
	Tango::DevVarStringArray	*props = db_get_class_property(array);

	//	Put in argout
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray();
	argout->length(props->length()-3);
	(*argout)[0] = CORBA::string_dup(classname.c_str());
	for (int i=4 ; i<props->length() ; i++)
		(*argout)[i-3] = CORBA::string_dup((*props)[i]);

	delete array;
	delete props;
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_data_for_server_cache
 *
 *	description:	method to execute "DbGetDataForServerCache"
 *	This command returns all the data needed by a device server process during its
 *	startup sequence. The aim of this command is to minimize database access during
 *	device server startup sequence.
 *
 * @param	argin	Elt[0] = DS name (exec_name/inst_name), Elt[1] = Host name
 * @return	All the data needed by the device server during its startup sequence. Precise list depend on the device server
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_data_for_server_cache(const Tango::DevVarStringArray *argin)
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter : Writing a TANGO DS / Exchanging data)
	//------------------------------------------------------------
	Tango::DevVarStringArray	*argout  = new Tango::DevVarStringArray();
	DEBUG_STREAM << "DataBase::db_get_data_for_server_cache(): entering... !" << endl;

	//	Add your own code to control device here
	
	if (argin->length() != 2)
	{
	   WARN_STREAM << "DataBase::DbGetDataForServerCache(): incorrect number of input arguments " << endl;
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)"Incorrect no. of input arguments, needs 2 (ds_name,host_name)",
					  (const char *)"DataBase::DbGetDataForServerCache()");
	}

	if (mysql_svr_version < 50000)
	{
		WARN_STREAM << "DataBase::DbGetDataForServerCache(): MySQL server too old for this command" << endl;
		Tango::Except::throw_exception((const char *)"DB_MySQLServerTooOld",
						(const char *)"The MySQL server release does not support stored procedure. Update MySQL to release >= 5",
						(const char *)"DataBase::DbGetDataForServerCache()");
	} 

	TimeVal	before, after;
	GetTime(before);
		
	string	sql_query;
	string svc((*argin)[0]);
	string host((*argin)[1]);
	MYSQL_RES *res;
	MYSQL_ROW row;
	unsigned int i;

	Tango::Util *tg = Tango::Util::instance();
	string	&db_inst_name = tg->get_ds_inst_name();
	string tmp_var_name("@param_out");
	tmp_var_name = tmp_var_name + db_inst_name;
			
//
// Do not use methods query() or simple_query() because we are
// calling a stored procedure.
// Calling a stored procedure needs special care to retrieve its OUT
// parameter(s). We have to code a loop using mysql_next_result
// function. The first result with data is the one we are 
// interested in
//

	sql_query = "CALL tango.ds_start('" + svc + "','" + host + "'," + tmp_var_name + ")";
	sql_query = sql_query + ";SELECT " + tmp_var_name;
//	cout << "Query = " << sql_query << endl;

	int con_nb = get_connection();
	if (mysql_real_query(conn_pool[con_nb].db, sql_query.c_str(),sql_query.length()) != 0)
	{
		TangoSys_OMemStream o;

		WARN_STREAM << "DataBase::db_get_data_for_server_cache failed to query TANGO database:" << endl;
		WARN_STREAM << "  query = " << sql_query << endl;
		WARN_STREAM << " (SQL error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;

		o << "Failed to query TANGO database (error=" << mysql_error(conn_pool[con_nb].db) << ")";
		o << "\nThe query was: " << sql_query << ends;

		release_connection(con_nb);

		Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),
									   (const char *)"DataBase::DbGetDataForServerCache()");
	}

	int status;
	do
	{
		if ((res = mysql_store_result(conn_pool[con_nb].db)) != NULL)
		{
			break;
		}
		else
		{
			if (mysql_field_count(conn_pool[con_nb].db) != 0)
			{
				TangoSys_OMemStream o;

				WARN_STREAM << "DataBase::db_get_data_for_server_cache: mysql_store_result() failed  (error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;

				o << "mysql_store_result() failed (error=" << mysql_error(conn_pool[con_nb].db) << ")";

				release_connection(con_nb);

				Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),
											   (const char *)"DataBase::DbGetDataForServerCache()");
			}

			if ((status = mysql_next_result(conn_pool[con_nb].db)) > 0)
			{
				TangoSys_OMemStream o;

				WARN_STREAM << "DataBase::db_get_data_for_server_cache: mysql_next_result() failed  (error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;

				o << "mysql_next_result() failed (error=" << mysql_error(conn_pool[con_nb].db) << ")";

				release_connection(con_nb);

				Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),
											   (const char *)"DataBase::DbGetDataForServerCache()");
			}
		}
	}while (status == 0);

	release_connection(con_nb);
	
	row = mysql_fetch_row(res);
	unsigned long *length_ptr = mysql_fetch_lengths(res);
	string str(row[0],length_ptr[0]);
	int nb_field = count(str.begin(),str.end(),'\0');
	
	if (nb_field == 0)
	{
		if (str.size() == 0)
		{
			mysql_free_result(res);
			WARN_STREAM << "DataBase::DbGetDataForServerCache(): Stored procedure does not return any result!!!" << endl;
			Tango::Except::throw_exception((const char *)"DB_StoredProcedureNoResult",
						(const char *)"The stored procedure did not return any results!!!",
						(const char *)"DataBase::DbGetDataForServerCache()");
		}
		else
		{
			mysql_free_result(res);
			WARN_STREAM << "DataBase::DbGetDataForServerCache(): Stored procedure failed with a MySQL error!!!" << endl;
			Tango::Except::throw_exception((const char *)"DB_StoredProcedureFailed",
						(const char *)"The stored procedure failed with a MySQL error!!!",
						(const char *)"DataBase::DbGetDataForServerCache()");
		}
	}
	
	argout->length(nb_field + 1);
	
	string::size_type pos = 0;
	string::size_type start = 0;
	int idx = 0;
	string tmp_elt;
	
	pos = str.find('\0');		
	while (pos != string::npos)
	{
		tmp_elt = str.substr(start,pos - start);
		(*argout)[idx] = CORBA::string_dup(tmp_elt.c_str());
		start = pos + 1;
		idx++;
		pos = str.find('\0',start);
	}
	tmp_elt = str.substr(start);
	(*argout)[idx] = CORBA::string_dup(tmp_elt.c_str());
		
	mysql_free_result(res);

	GetTime(after);
	update_timing_stats(before, after, "DbGetDataForServerCache");		
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_all_device_attribute_property
 *
 *	description:	method to execute "DbDeleteAllDeviceAttributeProperty"
 *	Delete all attribute properties for the specified device attribute(s)
 *
 * @param	argin	str[0] = device name, str[1]...str[n] = attribute name(s)
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_all_device_attribute_property(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "DataBase::db_delete_all_device_attribute_property(): entering... !" << endl;

	//	Add your own code to control device here
	
	TangoSys_MemStream sql_query_stream;
	const char *attribute, *property;
	string tmp_device;
	int i,j;
	MYSQL_RES *result;
	MYSQL_ROW row;

	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_delete_all_device_attribute_property(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)"insufficient number of arguments to delete all device attribute(s) property",
					       (const char *)"DataBase::db_delete_all_device_attribute_property()");
	}

	tmp_device = (*argin)[0];
	if (!check_device_name(tmp_device))
	{
        	WARN_STREAM << "DataBase::db_delete_all_device_attribute(): device name  " << tmp_device << " incorrect ";
         	WARN_STREAM << endl;
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
				       	(const char *)"Failed to delete all device attribute(s) property, device name incorrect",
				       	(const char *)"DataBase::db_delete_all_device_attribute()");
	}
	
	
	{
		AutoLock al("LOCK TABLES property_attribute_device WRITE, property_attribute_device_hist WRITE,device_attribute_history_id WRITE",this);
		
		for (i=0; i<argin->length()-1; i++)
		{
			attribute = (*argin)[i+1];

			INFO_STREAM << "DataBase::db_delete_all_device_attribute_property(): delete device " << tmp_device ;
			INFO_STREAM << " attribute " << attribute << " property(ies) from database" << endl;

// Is there something to delete ?

			sql_query_stream.str("");
			sql_query_stream << "SELECT DISTINCT name FROM property_attribute_device WHERE device = \"" << tmp_device 
		                	 <<"\" AND attribute = \"" << attribute << "\" ";
			result = query(sql_query_stream.str(),"db_delete_all_device_attribute_property()",al.get_con_nb());
			my_ulonglong count = mysql_num_rows(result);
			
			if (count != 0)
			{
				
// then delete property from the property_attribute_device table

				sql_query_stream.str("");
				sql_query_stream << "DELETE FROM property_attribute_device WHERE device = \"" << tmp_device 
		       					<<"\" AND attribute = \"" << attribute << "\" ";
				DEBUG_STREAM << "DataBase::db_delete_all_device_attribute_property(): sql_query " << sql_query_stream.str() << endl;
				simple_query(sql_query_stream.str(),"db_delete_all_device_attribute_property()",al.get_con_nb());		
		
// Mark this property as deleted	

				for(j=0;j<count;j++)
				{
					row = mysql_fetch_row(result);
					unsigned int device_attribute_property_hist_id = get_id("device_attribute",al.get_con_nb());
					sql_query_stream.str("");
					sql_query_stream << "INSERT INTO property_attribute_device_hist SET device='" << tmp_device
			   					 << "',attribute='" << attribute
			   					 << "',name='" << row[0]
			   					 << "',id='" << device_attribute_property_hist_id
			   					 << "',count='0',value='DELETED'";
					DEBUG_STREAM << "DataBase::DeletAllDeviceAttributeProperty(): sql_query " << sql_query_stream.str() << endl;
					simple_query(sql_query_stream.str(),"db_delete_all_device_attribute_property()",al.get_con_nb());
					purge_att_property("property_attribute_device_hist","device",tmp_device.c_str(),attribute,row[0],al.get_con_nb());		
				}
			}
			mysql_free_result(result);
		}
	}

	return;

}


}	//	namespace
