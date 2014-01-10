Dexela
----------

.. image:: Dexela.jpg 

Introduction
````````````
The Dexela detector is a brand product of  PerkinElmer. PerkinElmer has recently Acquired Dexela Limited a
manufacturer of CMOS flat panel. Nevertheless the Dexela detector SDK still remains not compatible with the other PerkinElmer
detector SDK (see perkinelemer plugin) and one need to use this camera plugin instead.

Prerequisite
````````````
The Dexela detector model sensor2923 has only been tested at ESRF and it only works under **Windows 7 64 bits**.

The software Suite called XCAP Imaging version 3.0 has been installed on the computer. XCAP provides the SDK 
SDK-1.0.8.10-20.0.14.

The detector is controlled via an acquisition board: PIXCI(R) E4 PCIExpress Camera Link board (EPIX,Inc.).

The detector is now used in operation on the Swiss-Norvegian beamline BM01.


Installation & Module configuration
````````````````````````````````````

-  follow first the steps for the windows installation :ref:`windows_installation`

The minimum configuration file is *config.inc* :

.. code-block:: sh

 COMPILE_CORE=1
 COMPILE_SIMULATOR=0
 COMPILE_SPS_IMAGE=1
 COMPILE_ESPIA=0
 COMPILE_FRELON=0
 COMPILE_MAXIPIX=0
 COMPILE_PILATUS=0
 COMPILE_BASLER=0
 COMPILE_PROSILICA=0
 COMPILE_ROPERSCIENTIFIC=0
 COMPILE_MYTHEN=0
 COMPILE_ADSC=0
 COMPILE_UEYE=0
 COMPILE_XH=0
 COMPILE_XSPRESS3=0
 COMPILE_XPAD=0
 COMPILE_PERKINELMER=0
 COMPILE_ANDOR=0
 COMPILE_PHOTONICSCIENCE=0
 COMPILE_PCO=0
 COMPILE_MARCCD=0
 COMPILE_POINTGREY=0
 COMPILE_IMXPAD=0
 COMPILE_DEXELA=1
 COMPILE_RAYONIXHS=0
 COMPILE_CBF_SAVING=0
 COMPILE_NXS_SAVING=0
 COMPILE_FITS_SAVING=0
 COMPILE_EDFGZ_SAVING=0
 COMPILE_TIFF_SAVING=0
 COMPILE_CONFIG=1
 LINK_STRICT_VERSION=0
 export COMPILE_CORE COMPILE_SPS_IMAGE COMPILE_SIMULATOR \
       COMPILE_ESPIA COMPILE_FRELON COMPILE_MAXIPIX COMPILE_PILATUS \
       COMPILE_BASLER COMPILE_PROSILICA COMPILE_ROPERSCIENTIFIC COMPILE_ADSC \
       COMPILE_MYTHEN COMPILE_UEYE COMPILE_XH COMPILE_XSPRESS3 COMPILE_XPAD COMPILE_PERKINELMER \
       COMPILE_ANDOR COMPILE_PHOTONICSCIENCE COMPILE_PCO COMPILE_MARCCD COMPILE_DEXELA\
       COMPILE_POINTGREY COMPILE_IMXPAD COMPILE_RAYONIXHS COMPILE_CBF_SAVING COMPILE_NXS_SAVING \
       COMPILE_FITS_SAVING COMPILE_EDFGZ_SAVING COMPILE_TIFF_SAVING COMPILE_CONFIG\
       LINK_STRICT_VERSION

-  start the windows compilation :ref:`windows_compilation`

-  finally with the Tango server installation :ref:`tango_installation`

Initialisation and Capabilities
````````````````````````````````
In order to help people to understand how the camera plugin has been implemented in LImA this section
provide some important information about the developer's choices.

Camera initialisation
......................

The camera will be initialized   within the **DexelaInterface**  object. There are 2 parameters to pass
to the DexelaInterface() constructor:
 - database_path (e.g. "C:\\DexelaConfig.cfg")
 - sensor_format (e.g. "sensor2923")


Std capabilites
................

This plugin has been implemented in respect of the mandatory capabilites but with limitations according 
due to the detector specific features and with some programmer's  choices.  We do not explain here the standard Lima capabilites
but you can find in this section the useful information on the Dexela specfic features.

* HwDetInfo
 The Dexela detector as a pixel size of 74.8e-6 m (74.8 um) and the image data type is fixed to 16bpp (bit per pixel).
  

* HwSync
 The supported trigger modes are IntTrig, IntTrigMult, ExtTrigMult and ExtGate.

 The exposure time range is 0.0116 (1/86) to 120 seconds.

 The latency time is not manage.


Optional capabilites
........................
In addition to the standard capabilities, we make the choice to implement some optional capabilities in order to 
have an improved simulation.

* HwShutter
 There is no shutter capability. 

* HwRoi
 There is no hardware capability, but Lima provides the sofware Roi as well.

* HwBin 
 The supported hardware binning factors are 1x1, 1x2, 1x4, 2x1, 2x2, 2x4, 4x1, 4x2 and 4x4.

Configuration
`````````````

The main configuration will consist in providing the correct "DexelaConfig.cfg" file to the detector API.
The file has to be provided by the manufacturer with a second file like "sensor2923.fmt". The .fmt file contains
some calibration data.


How to use
````````````
The LimaCCDs tango server provides a complete interface to the dexela plugin so feel free to test.

For a quick test one can use python, is this a short code example:

.. code-block:: python

  from Lima import Dexela
  from lima impor Core
  import time

  hwint = Dexela.Interface('c:\DexelaConfig.cfg','sensor2923')
  control = Core.CtControl(hwint)

  acq = control.acquisition()

  # setting new file parameters and autosaving mode
  saving=control.saving()

  pars=saving.getParameters()
  pars.directory='/tmp/'
  pars.prefix='testdexela_'
  pars.suffix='.edf'
  pars.fileFormat=Core.CtSaving.EDF
  pars.savingMode=Core.CtSaving.AutoFrame
  saving.setParameters(pars)


  # now ask for 2 sec. exposure and 10 frames
  acq.setAcqExpoTime(2)
  acq.setNbImages(10) 
  
  acq.prepareAcq()
  acq.startAcq()

  # wait for last image (#9) ready
  lastimg = control.getStatus().ImageCounters.LastImageReady
  while lastimg !=9:
    time.sleep(1)
    lastimg = control.getStatus().ImageCounters.LastImageReady
 
  # read a image
  im0 = control.ReadImage(0)
  

  
