// NativeApi.h : main header file for the NativeApi DLL


#pragma once

#ifndef NativeApi_H
#define NativeApi_H


#ifdef __cplusplus
extern "C" {
#endif

//define the underlying transport being used
#define CAMERALINK  //USB1

//define import for applications using the natveapi library.
#define DLLINEXP dllimport



/////////////////////////////////////////////////
#ifdef CAMERALINK
/// EPIX board error numbers returned by functions that don't use Derr
/// transport layer error messages. See EPIX help for full descriptions.
/// 
#define PXERROR         -1      /* use error. bad or inconsistent args ...  */
#define PXERMALLOC      -2      /* memory allocation error          */
#define PXERLOGIC       -5      /* internal logic error         */
#define PXERSEARCH      -9      /* specified name could not be matched  */
#define PXERBADMNEMONIC     -9      /* specified name could not be matched  */
#define PXERHARD        -3      /* hardware device error            */
#define PXERBREAK       -4      /* canceled by user (Ctrl+Break)         */
#define PXERNOOPTION        -6      /* hardware option not installed/available  */
#define PXERNOPTION     -6      /* PXERNOOPTION, historical (mis)spelling   */
#define PXERDOSIO       -7      /* generic dos file read/write error    */
#define PXERDRIVER      -8      /* undetermined driver error        */
#define PXERFILEFORM        -10     /* invalid file format          */
#define PXERNOMODE      -11     /* Option/Feature/Operation not (currently) supported */
#define PXERNOITEM      -12     /* no such item/object          */
#define PXERNOFILE      -13     /* file not found               */
#define PXERNEWFILE     -14     /* file not creatable           */
#define PXERTOOBIG      -15     /* insufficient table space         */
#define PXERBADEXP      -16     /* ill formed expression, constraint    */
#define PXERMATHOP      -17     /* invalid mathematical operation       */
#define PXERBADPARM     -18     /* invalid parameter or option specification*/
#define PXERNODATA      -19     /* data type(s) not supported       */
#define PXERNOTNOW      -20     /* feature/operation not available in current environment */
#define PXERNOCOLOR     -21     /* color(s) or data component(s) not supported */
#define PXERDEVIO       -22     /* generic device (COM, PRT, I2C) access error   */
#define PXERISOPEN      -23     /* item in use, driver already open     */
#define PXERINUSE       -23     /* historical name, deprecated      */
#define PXERTIMEOUT     -24     /* operation timed out          */
#define PXERNOTOPEN     -25     /* driver/resource not opened for use   */
#define PXERNODEVMEM        -26     /* insufficent memory on device     */
#define PXERNOFEATURE       -27     /* feature is not installed         */
#define PXERFUNCSTUB        -28     /* function is an incomplete stub       */
#define PXERNOKEY       -29     /* authorization key invalid or not found   */
#define PXERTRIGGER     -30     /* terminated by trigger            */
#define PXERTOOSMALL        -31     /* area too small               */
#define PXERUNITS       -32     /* not available for multiple units, or currently selected units */
#define PXERVIDFORM     -33     /* invalid video format             */
#define PXERCOMPRESS        -34     /* compression mode not supported           */
#define PXERMORESPACE       -35     /* more space needed                */
#define PXERTOOLATE     -36     /* timed command arrived too late for execution */
#define PXERNORESOURCE      -37     /* required resource not available          */
#define PXERNOMODULE        -38     /* required library/module not found        */
#define PXERNOFUNCTION      -39     /* required function in library/module not found*/
#define PXERBADSTRUCT       -40     /* wrong (version of) parameter         */
#define PXERBADCHAIN        -41     /* internal error, bad chain            */
#define PXERRESOURCEBUSY    -42     /* video resource already in use              */
#define PXERDEVFAULT        -43     /* unknown device fault             */
#define PXEROPSYS       -44     /* undetermined operating system error      */
#define PXERBUFFERSPACE     -45     /* insufficient frame buffer memory         */
#define PXERSTATECHANGED    -46     /* state changed during operation           */
#define PXERWRONGHARDWARE   -47     /* parameter(s) invalid for selected hardware   */
#define PXERX11NOMODE       -48     /* XWindows/X11 mode not supported          */
#define PXERX11ERROR        -49     /* XWindows/X11 error               */
#define PXERNOFEATSOFTVERS  -50     /* feature not available in current version of software */
#define PXERUNKNOWNERROR    -51     /* error code is undefined              */
#define PXERBADDIM      -52     /* invalid image dimensions             */
#define PXERNOVIDSTATE      -53     /* video state not found/defined            */
#define PXERDRIVERBADVERS   -54     /* driver is wrong version              */
#define PXERDRIVERNOTFOUND  -55     /* driver not installed or accessible       */
#define PXERNOOPERSYS       -56     /* current operating system not supported       */
#define PXEROPENFILE        -57     /* file not found or creatable          */
#define PXERRARG1       -1      /* currently, same as PXERROR           */
#define PXERRARG2       -1      /* ditto                        */
#define PXERRARG3       -1      /* ditto                        */
#define PXERRARG4       -1      /* ditto                        */
#define PXERRARG5       -1      /* ditto                        */
#define PXERRARG6       -1      /* ditto                        */
#define PXERRARG7       -1      /* ditto                        */
#define PXERRARG8       -1      /* ditto                        */
#define PXERRARG9       -1      /* ditto                        */
#define PXERRARG10      -1      /* ditto                        */
#define PXERRARG11      -1      /* ditto                        */
#define PXERRARG12      -1      /* ditto                        */
#define PXERRARG13      -1      /* ditto                        */
#define PXERRARG14      -1      /* ditto                        */
#define PXERRARG15      -1      /* ditto                        */
#define PXERRARG16      -1      /* ditto                        */
#define PXERRARG17      -1      /* ditto                        */
#define PXERRARG18      -1      /* ditto                        */
#define PXERRARG19      -1      /* ditto                        */
#define PXERUSER        -8192   /* external additions start here            */
#define PXERONDSP       -16384  /* modifier: error from DSP, not host       */
#endif
////////////////////Enumerations//////////////////////////

/// <summary>
/// Enumeration of the available communication methods
/// </summary>
typedef enum //CommunicationMethod
{
    /// <summary>
    /// CameraLink transport DLL should be used
    /// </summary>
    CameraLink,
    /// <summary>
    /// USB transport DLL should be used
    /// </summary>
    USB
} CommunicationMethod;
/// <summary>
/// An enumeration of the different bin levels available
/// </summary>
typedef enum //bins // horizontal by vertical
{
    /// <summary>
    /// Unbinned
    /// </summary>
    x11 = 1,
    /// <summary>
    ///  Binned vertically by 2
    /// </summary>
    x12,
    /// <summary>
    ///  Binned vertically by 4
    /// </summary>
    x14,
    /// <summary>
    ///  Binned horizontally by 2
    /// </summary>
    x21,
    /// <summary>
    /// Binned horizontally by 2 and vertically by 2
    /// </summary>
    x22,
    /// <summary>
    /// Binned horizontally by 2 and vertically by 4
    /// </summary>
    x24,
    /// <summary>
    /// Binned horizontally by 4
    /// </summary>
    x41,
    /// <summary>
    /// Binned horizontally by 4 and vertically by 2
    /// </summary>
    x42,
    /// <summary>
    /// Binned horizontally by 4 and vertically by 4
    /// </summary>
    x44,
    /// <summary>
    /// Indcates an error
    /// </summary>
    binsError

} bins;


/// <summary>Enumration for error codes returned from the API functions</summary>
typedef enum //Derr
{
    /// <summary>The operation was successful</summary>
    SUCCESS,
    /// <summary>The image pointer was NULL</summary>
    NULL_IMAGE,
    /// <summary>The image pixel type was wrong for the operation requested</summary>
    WRONG_TYPE,
    /// <summary>The image dimesions were wrong for the operation requested</summary>
    WRONG_DIMS,
    /// <summary>One or more parameters were incorrect</summary>
    BAD_PARAM,
    /// <summary>The communications channel is not open or could not be openned</summary>
    BAD_COMMS,
    /// <summary>An invalid trigger source was requested</summary>
    BAD_TRIGGER,
    /// <summary>The communications channel failed to open</summary>
    BAD_COMMS_OPEN,
    /// <summary>A failure in a detector write command occurred</summary>
    BAD_COMMS_WRITE,
    /// <summary>A failure in a detector read command occurred</summary>
    BAD_COMMS_READ,
    /// <summary>An error occurred openning or reading from a file</summary>
    BAD_FILE_IO,
    /// <summary>The software failed to open the PC driver or frame grabber</summary>
    BAD_BOARD,
    /// <summary>A function call was not able to reserve the memory it required</summary>
    OUT_OF_MEMORY


} Derr;


/// <summary>
/// An enumeration of the available full well modes
/// </summary>
typedef enum //FullWellModes
{
    /// <summary>
    /// The low noise reduced dynamic range mode
    /// </summary>
    Low=0,
    /// <summary>
    /// The normal full well mode
    /// </summary>
    High,
    /// <summary>
    /// Indicates an error
    /// </summary>
    FullWellModesError
} FullWellModes;

/// <summary>
/// An enumeration of detector modes. Not currently used
/// </summary>
typedef enum //DetectorModes
{
    /// <summary>
    /// The default detector mode
    /// </summary>
    normal,
    /// <summary>
    /// The detector test mode
    /// </summary>
    test,
    /// <summary>
    /// The detector is in line shading mode: Not currently implemneted
    /// </summary>
    line_shading,
    /// <summary>
    /// The detector is in frame shading mode: Not currently implemneted
    /// </summary>
    frame_shading,
    /// <summary>
    /// Indicates an error
    /// </summary>
    DetectorModesError
} DetectorModes;



/// <summary>
/// Enumneration of pixel types
/// </summary>
typedef enum //pType
{
    /// <summary>
    /// A pixel type of unsigned short
    /// </summary>
    u16 = 2,
    /// <summary>
    /// A pixel type of floating point
    /// </summary>
    flt = 4
} pType;
///<summary>
/// A structure to hold image dimensions
///</summary>
typedef struct ImageDimensions
{
    /// <summary>
    /// The width in pixels of the image
    /// </summary>
    int width;
    /// <summary>
    /// The height in pixels of the image
    /// </summary>
    int height;
    /// <summary>
    /// The amountof images of width * height in this stack
    /// </summary>
    int depth;
    /// <summary>
    /// The data type of the pixels in the image
    /// </summary>
    pType pixelType;

} ImgDims;

/// <summary>
/// An enumeration of exposure modes.
/// </summary>
typedef enum //ExposureModes
{
    /// <summary>
    /// The detector should slow down line reading in order to achieve exposure time
    /// </summary>
    Line_Delay,
    /// <summary>
    /// The detector should clear the sensor and wait for exposure time to pass before reading the detector image.
    /// </summary>
    Expose_and_read,
    /// <summary>
    /// The detector should take a sequence of images with no gaps.
    /// </summary>
	Sequence_Exposure,
	/// <summary>
    /// The detector should take a sequence of images with a specified gap no less than the minimum exposure timeforthe bin level.
    /// </summary>
	Frame_Rate_exposure,
	/// <summary>
    /// The detector should take a number of images with preset exposure times without a gap..
    /// </summary>
	Preprogrammed_exposure,
    /// <summary>
    /// Not currently used
    /// </summary>
    Auto_trigger,
    /// <summary>
    /// Not currently used
    /// </summary>
    Automatic_exposure,
    /// <summary>
    /// Indicates an error
    /// </summary>
    ExposureModesError
} ExposureModes;


/// <summary>
/// An enumeration of automatic exposure mode setups. Not currently used
/// </summary>
typedef enum //AutomoaticExposureModesSetup
{
    /// <summary>
    /// Exposure is triggered by reaching a threshold
    /// </summary>
    Select_threshold,
    /// <summary>
    /// Exposure is triggered by the resut of averaging
    /// </summary>
    Select_Averaging_method,
    /// <summary>
    /// Indicates an error
    /// </summary>
    AutomoaticExposureModesSetupError
} AutomoaticExposureModesSetup;


/// <summary>
/// An enumeration of exposure trigger sources. Not currently used.
/// </summary>
typedef enum //ExposureTriggerSource
{
    /// <summary>
    /// Trigger on positive edge - Not Used
    /// </summary>
    Ext_pos_edge_trig,
    /// <summary>
    /// Trigger on negative edge
    /// </summary>
    Ext_neg_edge_trig,
    /// <summary>
    /// Trigger using software
    /// </summary>
    Internal_Software,
    /// <summary>
    /// Trigger automatically - Not used
    /// </summary>
    Internal_Auto,
    /// <summary>
    /// Detector exposure duration and trigger contorlled externally
    /// </summary>
    Ext_Duration_Trig,
    /// <summary>
    /// Indicates an error
    /// </summary>
    ExposureTriggerSourceError
} ExposureTriggerSource;


/// <summary>
/// An enumeration of detector hardware output functions. Not currently used.
/// </summary>
typedef enum //DetectorHardwareOutputFunction
{
    /// <summary>
    /// On exposure
    /// </summary>
    Exposure,
    /// <summary>
    /// On pre-exposure
    /// </summary>
    Pre_exposure,
    /// <summary>
    /// on post exposure
    /// </summary>
    Post_exposure,
    /// <summary>
    /// On full sensor exposure
    /// </summary>
    Full_sensor_exposure,
    /// <summary>
    /// On read
    /// </summary>
    Read,
    /// <summary>
    /// Indicates an error
    /// </summary>
    DetectorHardwareOutputFunctionError
} DetectorHardwareOutputFunction;


/// <summary>
/// An enumeration of offset modes. Not currently used.
/// </summary>
typedef enum //OffsetModeSelection
{
    /// <summary>
    /// Offset will be entered externally.
    /// </summary>
    Manual,
    /// <summary>
    /// Offsets will be adjusted in hardware.
    /// </summary>
    Automatic,
    /// <summary>
    /// Indicates an error
    /// </summary>
    OffsetModeSelectionError
} OffsetModeSelection;
  


//////////////////////////////////////////////////////

///////////////////STRUCTURES////////////////////////////

/// <summary>
/// Structure that describes the detector sensor layout and sensor specification
/// </summary>
typedef struct SensorDescription
{
    /// <summary>
    /// The amount of sensors in the detector
    /// </summary>
    int numSensors;
    /// <summary>
    /// The amount of strips that a sensor has
    /// </summary>
    int sensorStrips;
    /// <summary>
    /// The amountof pixels in a strip
    /// </summary>
    int stripLength;
    /// <summary>
    /// The width in pixels of the individual sensor
    /// </summary>
    int sensorX;
    /// <summary>
    /// The height in pixels of the individual sensor
    /// </summary>
    int sensorY;
    /// <summary>
    /// The amount of sensors tiled horizontally
    /// </summary>
    int sensorsH;
    /// <summary>
    /// The amount of sensors tiled vertically
    /// </summary>
    int sensorsV;
    /// <summary>
    /// The width of the image buffer required in pixels
    /// </summary>
    int imageBufferX;
    /// <summary>
    /// The height of the image buffer required in pixels
    /// </summary>
    int imageBufferY;

} SensorDesc;

/// <summary>
/// A structure for forming a list of exposures including the exposure time and bin level.
/// </summary>
typedef struct ExposureList
{
    /// <summary>
    /// The number of exposures this structure represents
    /// </summary>
    int numberOfExposures;
    /// <summary>
    /// An array containing the bin level for each exposure which should be initialized using Utils.initializeExp
    /// </summary>
    bins* Binning;
    /// <summary>
    /// An array containing the exposure time in ms for each exposure which should be initialized using Utils.initializeExp
    /// </summary>
    float* t_expms;

}exposureList;




/// <summary>
/// A structure used to specify the new image dimensions for geometry correction
/// </summary>
typedef struct GeometryCorrectionParamsTag
{
    /// <summary>
    /// The new image width
    /// </summary>
    int iRefX; //1536
    /// <summary>
    /// The new image height
    /// </summary>
    int iRefY; //1944

}GeometryCorrectionParams;

/// <summary>
/// Structure to hold the detector current status.
/// </summary>
typedef struct Status
{

    /// <summary>
    /// The currently set exposure mode
    /// </summary>
    ExposureModes exposureMode;
    /// <summary>
    /// The currently set Full Well mode
    /// </summary>
    FullWellModes fullWellMode;
    /// <summary>
    /// The currently set exposure time
    /// </summary>
    float exposureTime;
    /// <summary>
    /// The curently set bin level
    /// </summary>
    bins binLevel;
    /// <summary>
    /// The currenlty set Trigger Source
    /// </summary>
    ExposureTriggerSource triggerSource;
    /// <summary>
    /// True if the detector test mode is set to on
    /// </summary>
    BOOL testMode;
    /// <summary>
    /// The current temperature of the detector: Not currently available
    /// </summary>
    float temperature;

} DetStatus;

//////////////////////////////////////////////////////////

////////////////////////////////////////////////////


__declspec( DLLINEXP )Derr FixFlood(void* img,float *fixed ,ImgDims* imgDims);
__declspec( DLLINEXP )char  *GetTransportMethod(void);
__declspec( DLLINEXP )int Snap(int unit , int buffer, int timeout);
__declspec( DLLINEXP )char*  GetSerialError(int error);
__declspec( DLLINEXP )int  GetCapturedBuffer(void);
__declspec( DLLINEXP ) int GetFieldCount(void);
__declspec( DLLINEXP ) Derr openComms(void);
__declspec( DLLINEXP ) Derr sendCommand(char* command, char* returnStr);
__declspec( DLLINEXP ) Derr sendCommand1(char* command);
__declspec( DLLINEXP ) int SerialRead( int unit, char* data, int count);
__declspec( DLLINEXP ) int SerialWrite(int unit, char* data, int cnt );
__declspec( DLLINEXP ) int WriteBufferUchar(int unit, int frame, int startx, int starty, int xdim, int ydim, byte* buffer,int length, char* PixelType);
__declspec( DLLINEXP ) int ReadBufferUchar(int unit, int frame, int startx, int starty, int xdim, int ydim, byte* buffer, int length,char* PixelType);
__declspec( DLLINEXP ) int ReadBufferUshort(int unit, int frame, int startx, int starty, int xdim, int ydim, unsigned short* buffer, int length, char* PixelType);
__declspec( DLLINEXP ) int mesgFault(int unit);
__declspec( DLLINEXP ) int imageXdim(void);
__declspec( DLLINEXP ) int imageYdim(void);
__declspec( DLLINEXP ) Derr DefectCorrection(void* img, ImgDims* input, void* defectMap , ImgDims* defectDims , int planeToCorrect );
__declspec( DLLINEXP ) Derr GetFirmwareVersion(char* version , int lengthOfVersion);
__declspec( DLLINEXP ) Derr SetFullWellMode(FullWellModes fwm);
__declspec( DLLINEXP ) void GetTimeStamp(char* timestamp,int lengthofTimestamp);
__declspec( DLLINEXP ) Derr  SetExposureMode( ExposureModes mode);
__declspec( DLLINEXP ) Derr SetExposureTime(float timems);
__declspec( DLLINEXP ) Derr unscrambleALL(int buf,void* scrambleBuffer ,ImgDims* scrambleBufferDims, void* output,ImgDims* outputDims, SensorDesc* sensorDetail , bins currentBinning);
__declspec( DLLINEXP ) ExposureModes GetExposureMode(void);
__declspec( DLLINEXP ) float GetExposureTime(void);
__declspec( DLLINEXP ) Derr SetBinningMode(bins flag);
__declspec( DLLINEXP ) Derr ClearCameraBuffer(int i);
__declspec( DLLINEXP ) Derr ClearBuffers( int buffers );
__declspec( DLLINEXP ) int  FlushCC1(void);
__declspec( DLLINEXP ) DetStatus  GetDetectorStatus(void);
__declspec( DLLINEXP ) Derr SetTestMode(BOOL SetTestOn);
__declspec( DLLINEXP ) Derr ReadRS232Cmd(int addr, char* returnString, int lengthOfReturnString);
__declspec( DLLINEXP ) Derr loadSensorConfigFile(char* filename);
__declspec( DLLINEXP ) Derr SendRS232Cmd1(char* command_and_value);
__declspec( DLLINEXP ) Derr SendRS232Cmd(int addr, int data);
__declspec( DLLINEXP ) Derr openBoard(char* configFileName, HANDLE* h);
__declspec( DLLINEXP ) Derr CloseBoard(HANDLE h);
__declspec( DLLINEXP ) Derr findMedianofPlanes(unsigned short* img,unsigned short* median,ImgDims* imgDims);
__declspec( DLLINEXP ) Derr SubtractDark(void* img,ImgDims* imgDims, void* dark,ImgDims* darkDims, int darkOffset);
__declspec( DLLINEXP ) Derr FloodCorrection(void* Src, ImgDims* srcimgDims,void* flood, ImgDims* imgDims,int darkOffset);
__declspec( DLLINEXP ) Derr DarkFloodCorrection(void* pSrc,ImgDims* srcDims, void* flood,ImgDims* floodDims, void* dark,ImgDims* darkDims, int DarkOffset, int exposurenum);
__declspec( DLLINEXP ) Derr SoftReset(void);
__declspec( DLLINEXP ) Derr unscrambleThread(int starth, int hcount,unsigned short* pDsti , unsigned short* pSrci ,SensorDesc* sdet);
__declspec( DLLINEXP ) Derr PowerSwitch(bool on);
__declspec( DLLINEXP ) Derr PowerCLInterface(BOOL flag);
__declspec( DLLINEXP ) ExposureTriggerSource GetTriggerSource(void );
__declspec( DLLINEXP ) Derr  SetTriggerSource( ExposureTriggerSource ets);
__declspec( DLLINEXP ) BOOL GetTestMode(void);
__declspec( DLLINEXP ) FullWellModes GetFullWellMode(void);
__declspec( DLLINEXP ) bins GetBinningMode(void);
__declspec( DLLINEXP ) unsigned short LinearSectionCorrection( int value ,int MeasuredEnd,int MeasuredStart, int MeasuredStop ,int sectionNum, int DarkOffset);
__declspec( DLLINEXP ) Derr LinearizeData(unsigned short* image , ImgDims* imgdims , int *MeasuredStarts , int MSLen, int DarkOffset);
__declspec( DLLINEXP ) int GoLiveSeq(int unit , int start, int stop,int inc,int numBuf, int period);
__declspec( DLLINEXP ) int  setExsyncPrin(int unitmap, unsigned int exsync, unsigned int prin);
__declspec( DLLINEXP ) int  getExsync(int unitmap);
__declspec( DLLINEXP ) int  getPrin(int unitmap);
__declspec( DLLINEXP ) int GoLivePair(int unit , int buffer1  , int buffer2);
__declspec( DLLINEXP ) int GoUnLive(int unit);
__declspec( DLLINEXP ) Derr SetNumOfExposures(int num);
__declspec( DLLINEXP ) Derr  SetExposureTime2(float timems, int AddrExposureTimeLow, int AddrExposureTimeHigh);
__declspec( DLLINEXP ) Derr SetGapTime(float timems);
__declspec( DLLINEXP ) Derr setSlowedDown(int serial);
/// <summary>
/// Correction of bad pixels and lines based on a defect map image. Floating point image.
/// </summary>
/// <param name="inBuf">A float pointer to the input image</param>
/// <param name="outBuf">A float pointer to the output image</param>
/// <param name="MapBuf">A unsigned short pointer to the defect pixel map image</param>
/// <param name="imageWidth">The width in pixels of the image.</param>
/// <param name="imageHeight">The height in pixels of the image.</param>
//extern "C" void BadPixelCorrectionFloat(float *inBuf, float *outBuf, unsigned short *MapBuf, int imageWidth, int imageHeight);
void BadPixelCorrectionFloat(float *inBuf, float *outBuf, unsigned short *MapBuf, int imageWidth, int imageHeight);

/// <summary>
/// Correction of bad pixels and lines based on a defect map image. Unsigned short image.
/// </summary>
/// <param name="inBuf">A unsigned short pointer to the input image</param>
/// <param name="outBuf">A unsigned short pointer to the output image</param>
/// <param name="MapBuf">A unsigned short pointer to the defect pixel map image</param>
/// <param name="imageWidth">The width in pixels of the image.</param>
/// <param name="imageHeight">The height in pixels of the image.</param>
//extern "C" void BadPixelCorrectionUShort(unsigned short *inBuf, unsigned short *outBuf, unsigned short *MapBuf, int imageWidth, int imageHeight);
void BadPixelCorrectionUShort(unsigned short *inBuf, unsigned short *outBuf, unsigned short *MapBuf, int imageWidth, int imageHeight);

/// <summary>
/// Geometry correction for a floating point image
/// </summary>
/// <param name="inBuf">A float pointer to the input image</param>
/// <param name="outBuf">A float pointer to the output image</param>
/// <param name="imageWidth">The width in pixels of the image.</param>
/// <param name="imageHeight">The height in pixels of the image.</param>
/// <param name="Params">A GeometryCorrcetionParams structure</param>
//extern "C" void GeometryCorrectionFloat(float *inBuf, float *outBuf, int imageWidth, int imageHeight, GeometryCorrectionParams  Params);
void GeometryCorrectionFloat(float *inBuf, float *outBuf, int imageWidth, int imageHeight, GeometryCorrectionParams  Params);


/// <summary>
/// Geometry correction for a unsigned short image
/// </summary>
/// <param name="inBuf">A unsigned short pointer to the input image</param>
/// <param name="outBuf">A unsigned short pointer to the output image</param>
/// <param name="imageWidth">The width in pixels of the image.</param>
/// <param name="imageHeight">The height in pixels of the image.</param>
/// <param name="Params">A GeometryCorrcetionParams structure</param>
//extern "C" void GeometryCorrectionUShort(unsigned short *inBuf, unsigned short *outBuf, int imageWidth, int imageHeight,GeometryCorrectionParams  Params);
void GeometryCorrectionUShort(unsigned short *inBuf, unsigned short *outBuf, int imageWidth, int imageHeight,GeometryCorrectionParams  Params);

#ifdef __cplusplus
}
#endif

#endif