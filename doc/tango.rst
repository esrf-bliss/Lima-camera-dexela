Dexela Tango device
=====================

This is the reference documentation of the Dexela Tango device.

you can also find some useful information about the camera models/prerequisite/installation/configuration/compilation in the :ref:`Dexela camera plugin <camera-dexela>` section.


Properties
----------

=============== =============== ===================== =========================================================================
Property name	Mandatory	Default value	      Description
=============== =============== ===================== =========================================================================
database_path	Yes		DexelaConfig.cfg      The database path file, e.g C:\DexelaConfig.cfg
sensor_format	Yes		sensor2923	      The detector model
=============== =============== ===================== =========================================================================

Attributes
----------
======================= ======= ======================= ======================================================================
Attribute name		RW	Type			Description
======================= ======= ======================= ======================================================================
full_well_mode		ro	DevString	 	The well-mode, can be set to HIGH or LOW	
======================= ======= ======================= ======================================================================

Commands
--------

=======================	=============== =======================	===========================================
Command name		Arg. in		Arg. out		Description
=======================	=============== =======================	===========================================
Init			DevVoid 	DevVoid			Do not use
State			DevVoid		DevLong			Return the device state
Status			DevVoid		DevString		Return the device state as a string
getAttrStringValueList	DevString:	DevVarStringArray:	Return the authorized string value list for
			Attribute name	String value list	a given attribute name
=======================	=============== =======================	===========================================

