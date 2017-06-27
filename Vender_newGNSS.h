#pragma once

// .17 Change Beidou Ephemeris format. Modify V8 AGPS Command.
// .18 Fix altitude parsing error when its value is negative.
// .18 Add warning message when download image is not exist.
// .18 [Set Ephemeris] will use the previous saved file automatically.
// .18 Earth View can hide satellite in the specified system.
// .19 Fix rmc speed display error.
// .19 Add setting dialog for adjust delay before binsize.
// .20 Add new download command, fix rmc speed display, set no nmea then download will no nmea output.
// .21 Mutex between [Configure Position Update Rate] and [Configure SAEE].
// .22 Show 12 Channel GPGSV info in theirs GNSS system.
// .23 Fix NMEA Parser error when it has two GNGSA statements.
// .24 Fix NMEA Parser error when it has no GPGGA statements (Unicore pure BD mode).
// .25 Fix [Query Power Saving Parameter] timeout issue.
// .26 Add tag loader process. 
// .26 Hide com port selection in [Configure Serial Port] dialog when using Customer Release version.
// .27 Add [Query NMEA Talker ID] in menu [Binary]. Modify VTG interval default to 1 second.
// .28 Support new loader srec BINSIZE / BINSIZ2 / BINSIZ3 command.
// .28 Fix lon, lat, alt dispear issue after reconnection.
// .29 Remove Query / Configure 1PPS NMEA Delay, and remove AGPS item in general version.
// .30 Add Query / Configure Power Saving Parameter(Rom), and modified Query / Configure Power Saving Parameter in Binary menu.
// .31 NMEA Player added PreprocessInputLine function for normalizing the nmea string.
// .32 Fixed Beidou nmea status bugs, the gsa information is not clear after reconnection.
// .33 Change some strings and menu items based on meeting conclusions. 
// .34 Change some strings and menu items based on meeting conclusions. 
// .35 Fixed Log Decompress and KML Conver open file dialog style. 
// .36 Added GNSS Doze Mode command in Venus 8.
// .37 Added SWCFG_VENDOR_GNSS_GENERAL_OLINKSTAR_PROTECT.
// .38 Modify CheckOlinkstarFirmware() rule.
// .39 Modify "Skytraq" string for customer.
// .39 Remove 1PPS clock pluse and modify Set GLONASS Time Correction.
// .40 Modify AGPS function in General version.
// .41 Add support add tag version for OLinkStart version
// .42 Remove "Skytraq" in About dialog.
// .43 Using "OLINKSTAR IMAGE" for external loader download.
// .44 Fixed CSerial::GetString in Prolific can't download issue.
// .45 Add [Configure / Query 1PPS Frequency Output].
// .46~.47 Add support add tag version for SWID version.
// .48 Add SPI download option and modify GetString time interval.
// .49 Add setting [Boost Get/Set Ephemeris] for disable baud rate change in Get / Set Ephemeris.
// .50 Modify [Query/Configure SBAS] function, using combo box for Ranging options.
// .51 Fix [Query Cable Delay] in wrong unit issue.
// .52 Add SWCFG_VENDOR_GNSS_GENERAL_FKHL, and new NMEA_PRN_TYPE define.
// .53 Fix Query Navigation Mode issue.
// .54 Fix ShowGPGSVmsg2 parsing bugs.
// .55 Add Ericsson customer branch.
// .56 Fix first nmea no output issue.
// .57 Add TIMING_MONITORING,TIMING_OUTPUT_ALIGN, SHOW_ELEV_AND_CNR_MASK_IN_GEN for customize version.
// .58 Clean NMEA data after output mode change.
// .59 Make Scatter View larger and show last point in blue.
// .60 Add [Query / Configure Binary Measurement Data Out] in Binary menu.
// .61 Fix scatter view delay issue. Modify Beidou Prn range for Ericsson Version.
// .62 Change SHOW_ELEV_AND_CNR_MASK_IN_GEN default to 1.
// .63 Add NMEA error detection function in respond window.
// .64 Fix error NMEA detect function issue.
// .65 Support multi hz datalog.
// .66 Query Customer ID display in hex format.
// .67 Add start address in DataLog decompress.
// .68 Fix start address issue in DataLog decompress.
// .69 Fix leap seconds issue and second issue in DataLog decompress.
// .70 Add Enu scale 0.1m, 0.2m, 0.5m. Defined by MORE_ENU_SCALE.
// .71 Add "Send" button in [NMEA Checksum Calc] and [Binary Checksum Calc] dialog.
// .72 Add Serial Number Command in OLinkStar version.
// .73 Scatter scale will not change after reconnected.
// .74 Add 2 seconds delay after plug-in detected.
// .75 Adjust removing device message show time to 3500 ms.
// .76 Change [Configure Binary Measurement Data Out], [Query Binary Measurement Data Out]
// .76 Add [Query DGPS], [Configure DGPS].
// .76 Add [Query Carrier Smooth Mode], [Configure Carrier Smooth Mode].
// .76 Add [Query Time Stamping], [Configure Time Stamping].
// .76 Improve time out response. Update date no check.
// .77 Change [Configure DGPS], [Configure Carrier Smooth Mode], [Configure Time Stamping].
// .78 Add [Query GPS Time] and [Configure GPS/UTC Leap Seconds] in Venus 8 menu.
// .79 Fix QueryTimeStamping tigger mode error.
// .80 Fix QueryTimeStamping valid error.
// .81 Fix QueryTimeStamping Sub Time of Week error.
// .82 Save NMEA support raw measurement binary output decode.
// .83 Block save NMEA for multi hz.
// .84 Add Surveying and mapping in Navigation Mode.
// .85 Add time sync in NMEA Player.
// .86 Support [Enable All] in SBAS Command.
// .86 Add Test External SREC in utility.
// .87 Add Parameter Search Engine Sleep Critria command.
// .88 Modify Parameter Search Engine Sleep Critria command.
// .89 Modify Configure Timming command packet size.
// .90 Setting add [Download Testing] and [Log Response].
// .91 Support Gaileo NMEA.
// .92 Add binary message error detection.
// .93 Add [Configure Binary Measurement Data Out] in Customer Release Version.
// .94 Add Query/Configure Datum Index in Venus 8 menu.
// .95 Add [Save Binary] in File menu, it can record any output from device.
// .96 Fix [Save Binary] issue.
// .97 Version and Boot Status in Information will be updated after user query them in menu.
// .98 Fix garbage codes shown after NMEA sentences issue.
// .99 Fix [Configure DGPS] packet size error issue.
// .100 Fix NMEA output un-smooth behavior when configue the position update rate higher than 20Hz.
// .101 Add version extension string after kernel version, add [Query SHA1 String] in [Binary].
// .102 Add function [Query/Configure Search Engine Number] to customer release version.
// .103 Add SUP800 Tester configure, and modify 1pps Frequency Output default value.
// .104 Modify PSTI 004_001. parser spec, add valide field.
// .105 Adjust Frequency default to 10M in Configure 1PPS Frequency Output.
// .106 Save NMEA can support all binary output message description.
// .107 Add SUP800 Customer Release branch.
// .108 KML Convert support both GGA and RMC, and add progress bar when converting.
// .109 Fix Speed and Direction maybe show 0.0 and never update after re-connection.
// .110 Add Query GNSS Constellation Capability function.
// .111 Fix KML Converter bug, corrected segmentation conditions.
// .112 Fix CUSTOMER_ID define error.
// .113 Switch Earth bitmap by coordinates.
// .114 Change KR Ver to SW Ver
// .115 Add [Query SHA1 String] to General version.
// .116 Add [Raw Measurement Binary Convert] in [Convert] menu.
// .117 Fix Power Mode in [Configure Power Mode] can't change issue.
// .118 Fix Fixed Raw Measurement output parsing issue.
// .119 Add specify Scatter Center Point function in Setup.
// .120 Add SWCFG_VENDOR_GNSS_SWID_NEW_DOWNLOAD.
// .121 Add Set / Query UART Pass-through in Venus8 Menu.
// .122 Add GPSDO sub-menu in Venus8 Menu.
// .123 Add GPSDO sub-menu in Venus8 Menu.
// .124 Modify GPSDO sub-menu in Venus8 Menu.
// .125 Modify KML converter output default folder to same of source.
// .125 Fixed binary output satellite prn duplicate issue.
// .126 Add Signal Disturbance Test menu in [Venus 8] menu.
// .127 Modify Signal Disturbance Test command.
// .128 Fixed NMEA Player crash in XP when scatter starting draw point.
// .129 Support special NMEA(RMC and GGA), add [Query / Configure Interference Detect Control] to General.
// .130 Support [Get / Set Beidou almanac].
// .131 Never change baud rate when configure serial port fail.
// .132 Fixed DataLog Decompress crash bug when date number < 10.
// .133 Add [Configure GPS/UTC Leap Seconds in UTC] and download in external loader when boot in ROM code.
// .134 Extend timeout when query version before download.
// .135 Disable check SAEE and Multi-Hz can't be both on.
// .136 Add [Configure / Query Position Fix Navgation Mask] to Customer Release version.
// .137 Fix [Query CRC] fail issue and [Configure Position Update Rate] NACK issue.
// .138 Add [GPSDO Download] panel.
// .139 Fix DataLog read fail issue
// .140 Add [Noise Power Control] command in [Venus 8].
// .141 Adjust all configure dialog in [Venus 8]. Fixed download V6 fail issue.
// .142 Fix external download in binary command issue.
// .143 Support 3D KML convert.
// .144 Remove [Query / Configure Power Saving Parameters(Rom)] and modify [Query / Configure Power Saving Parameters]
// .145 [Conveter/KML] support GNS message.
// .146 Modify Antenna Detection command for fw spec.
// .147 Add GAGAN option in [Query / Configure SBAS].
// .148 Fixed GAGAN option error issue.
// .149 20150720 Add iq plot functions.
// .150 20150721 Update IQPlot for Windows XP.
// .151 20150804 Add SWCFG_VENDOR_GNSS_TAKUJI version for Oliver's customer upgrade.
// .152 20150804 Modify SWCFG_VENDOR_GNSS_TAKUJI version add tag.
// .153 20150805 Add Master/Slave function.
// .154 20150807 KML support Point List, and Scatter support 0.05m, 0.01m.
// .155 20150807 KML point list support name tag in UTC time.
// .156 20150817 Setup scatter center support 7 decimal Point.
// .157 20150819 Support Fix RTK and Float RTK quality mode and modify KML converter.
// .158 20150819 Support Fix RTK and Float RTK quality mode and modify KML converter.
// .159 20150824 Modify [Configure Timing] for RTK.
// .160 20150831 Release for Customer Release.
// .161 20150911 KML remove point title.
// .162 20150917 Add Geo-Fencing command.
// .163 20150922 Add [Default Timeout] in setup dialog. Request from Andrew and Justin.
// .164 20150923 Fixed 0x64, 0x03 size error issue. Report by Andrew.
// .165 20150923 Add RTK mode command. Request from Andrew.
// .166 20150925 Fix altitude issue for Scatter.
// .167 20151001 IQ Plot support multi process.
// .168 20151006 Fixed ShowBinaryOutput() issue.
// .169 20151013 Add %012.9lf to KML converter. Request from Ken
// .170 20151016 Remove REBOOT notify for scatter center changed. Request from Oliver
// .171 20151026 Fix WGS84 XYZ not update issue. Report by forum user.
// .172 20151029 Scatter will not change the center point after re-connect or restart, request from Oliver
// .173 20151110 Add [Query/Configure RTK Parameters], [Reset RTK Engine] in [Venus 8 / RTK] menu.
// .173 20151110 Fixed command ack lose issue when its contant 0xa0 0xa1.
// .174 20151120 Modify Lat/Lon display.
// .175 20151126 Merge com and baudrate setting to g_setting structure. 
// .175 20151126 Modify [Configure Position Update Rate] change baudrate rule.
// .176 20151126 Modify Scatter Count default value for slow computer. Request from Andrew
// .177 20151127 KML converter support GSA / GSV. Request from Michael
// .177 20151130 Add RTK field and modify 2DRMS & CEP50% display.
// .177 20151130 Add MORE_INFO define and new profile [GNSS_General_RTK][GNSS_NMEAPLAYER_RTK].
// .178 20151204 Fixed baud rate not restore after download.
// .179 20151208 Fixed KML converter GSA issue.
// .180 20151209 Fixed KML converter Beidou issue.
// .181 20151217 Remove Geo-fecing in General version. request from Oliver.
// .181 20151221 Fixed degree symbol display in English issue, report from fourm user.
// .182 20151224 Add [Query Binary Measurement Data Out] in binary menu. Request form Andrew and Ryan.
// .184 20151230 Modify _ALWAYS_USE_EXTERNAL_SREC_ to 1, using 115200 dl srec. Request form Leo by 錦輪  
// .185 20160105 SAINTMAX customer version QWP002001-1050104. Request form Leo
// .186 20160112 SAINTMAX customer version QWP002001-1050104. Request form Leo
// .187 20160120 Player fixed set origin error(In west earth) and timing control issue, Request form Oliver and Andrew.
// .188 20160123 Modify some typo text, Request from forum user.
// .189 20160126 Send 1 Hz cmd after config to RTK base mode, Request from Oliver.
// .190 20160202 Add SPECIAL_TEST for eten special command test, Request from Leo and Andrew.
// .191 20160203 Add SPECIAL_TEST random test, Request from Leo and Andrew.
// .192 20160225 Modify GeoFencing to support multiple Geo-Fencing polgons. Request from Oliver and Andrew.
// .193 20160226 Brench Kalman version, add RTK baseline display. Request from Leo.
// .194 20160301 Show baseline to three decimal. Request from Leo.
// .195 20160310 Support RTK Mode and operational function. Request from Andrew and Ryan Yang.
// .196 20160317 Reconstruction Configure Message Type functions and modify QueryRtkMode2. Request from Ryan Yang and Ken.
// .197 20160323 Modify RTK static mode altitude prompt text. Request from Ryan Yang.
// .197 20160324 Fixed scan port / baudrate bugs.
// .197 20160330 ETEN customize version, add PSTM menu, Request from Leo.
// .198 20160331 Add Utility function [Read 0x50000000 to File], Request from Andrew.
// .199 20160331 KML Converter support DR Mode, Request from Ken.
// .199 20160411 Add binary output player in Player version, Request from Leo.
// .200 20160412 Fix download issue when raw mode download will fail, Report from Andrew and Ken.
// .200 20160413 Fix crash issue when UI fast operation, Report from Andrew and Ryan.
// .201 20160414 Read prom bin fast when download, Report from Ken.
// .202 20160419 Support DJI NAVIGATION DATA MESSAGE(ID: 0x7A, CUSTOMER ID: 0xB, SID: 0x80), Report from Andrew, Roger.
// .203 20160422 Modify RTK Mode and operational function, add Baseline length, requast from Andrew, Ryan.
// .203 20160422 Remove GPS Measurement Mode command, requast from Andrew, Oliver.
// .204 20160428 Add FormatError ACK support, requast from Andrew.
// .205 20160429 Fix Ack error issue, requast from Ken.
// .206 20160509 Add command [Pinning Of Kernel Very Low Speed] to all version, requast from Ryan.
// .207 20160511 Clear PSTI032 info; Correct Lon/Lat display, requast from Ken.
// .208 20160512 Setup dialog add recentScatterCenter list, requast from Ken.
// .208 20160512 Binary Measurement Data Out support 8Hz, requast from Andrew.
// .208 20160516 Remove Query / Configure Datum in Binary menu, requast from Andrew.
// .209 20160527 Modify [Pinning Of Kernel Very Low Speed] text, requast from Andrew.
// .210 20160615 Add SWCFG_VENDOR_NSHP_FIX_TOOL for NS-HP user patch, requast from Andrew and Oliver.
// .211 20160616 Add Write 0x50000000 to a File for Kayyui, requast from Andrew.
// .212 20160704 Support GNGSV token for special customer output, requast from Leo.
// .213 20160707 Add new command DEVICE UNIQUE ID for Dofun, requast from Andrew and Michael Chuang.
// .214 20160712 Add IQPlot Log function, requast from Terrance.
// .215 20160719 RTK pacth tool version, requast from Andrew.
// .215 20160720 Reorganization resource ID, requast from Alex.
// .216 20160728 Add [Configure/Query PSTI030/PSTI032 Interval] command, requast from Andrew and forum user.
// .216 20160728 Disable [Configure Proprietary NMEA] in Customer Release version, requast from Andrew and Alex.
// .217 20160728 Add tooltips in response window, Reauest from Alex.
// .217 20160801 Remove [Surveying and mapping] in Configure Navigation Mode, requast from Alex.
// .217 20160808 Fix SUP800 parsing issue in multi-hz, requast from Andrew.
// .217 20160808 Add [Quadcopter] in Configure Navigation Mode, requast from Terrance.
// .218 20160809 New Configure geo-fencing data UI, requast from Ken.
// .219 20160810 SUP800 multi-hz maxuim baud rate in 230400, requast from Andrew.
// .220 20160816 Add CWQX version, baudrate support 1200, 2400, requast from Andrew, Leo
// .221 20160830 Remove SHOW_ELEV_AND_CNR_MASK_IN_GEN.
// .221 20160901 Remove restart after [Configure Position Update Rate]. request from Andrew.
// .222 20160905 Modify Zenlane protocol. request from Leo.
// .223 20160905 Modify Zenlane protocol. request from Leo.
// .224 20160905 Modify block transfer in Zenlane protocol. request from Leo.
// .225 20160907 Add [Configure/Query PSTI0004 Interval] command, requast from Andrew.
// .226 20160914 Modify for Glonass RTK version(support 0xE5 output, change bin output format), requast from Andrew, Ryan.
// .226 20160914 1. Add new command to “Re-calculate GLONASS IFB”, the structure is as follow:
// .226 20160914    A0 A1 00 02 6B 04 6F 0D 0A
// .226 20160914 2. Modify binary command 0x1E to add Extended_ Raw_Meas Enabling field.
// .226 20160914 3. Modify binary command 0x89 to add Extended_ Raw_Meas Enabling field.
// .226 20160914 4. Parse EXT_RAW_MEAS – Extended Raw Measurement Data v.1 (0xE5), the protocol is defined in attached file.
// .227 20160919 Add [UART Pass through] to RTK menu, request from Andrew and Ryan.
// .228 20160930 Add Ins DR menu, request from Andrew and Gin.
// .229 20161004 Modify InsDR menu message, request from Andrew and Gin.
// .230 20161007 Add RTK On/OFF SV channel functions, request from Ryan and Andrew.
// .231 20161013 Modify ellipsoid altitude and sea level altitude from U32 to S32, request from Ken and Andrew.
// .232 20161014 NMEA Player support only RMC no GGA, request from Andrew.
// .233 20161018 Release L2 support and fix PSTI issue, request from Patrick and Ken.
// .234 20161018 Fix L2 size error, request from Patrick.
// .235 20161020 Add Save Host Log function from Andrew.
// .236 20161026 Add Configure RTK Reference Static Started Position, request from Andrew and Ryan.
// .237 20161103 Add [Check all] and [Uncheck all] buttons in RTK On/OFF SV channel, request from Andrew.
// .237 20161103 Redesign all save function for more stable, request from Andrew.
// .238 20161116 Add [HostLog Output To NMEA] in Convert menu, request from Terrance and Andrew.
// .239 20161117 Add [Get 16-IO Register] and [Configure 16-IO Register] in Binary menu, request from Patrick.
// .240 20161123 Fixed [Get 16-IO Register] and [Configure 16-IO Register] id error issue, request from Patrick.
// .241 20161123 Add [Query RTCM Measurement Data Output] and [Configure RTCM Measurement Data Output] in RTK menu, request from Ryan.
// .242 20161123 Fix 0xA8 speed issue, request from Andrew.
// .243 20161124 Modify [RTCM Measurement Data Output] for checking setting value and module type, request from Andrew and Ryan.
// .244 20161124 Modify [RTCM Measurement Data Output] for UI status issue, request from Ken.
// .245 20161128 Add [Write Clock Offset], request from Oliver.
// .246 20161130 Add [INS DR Test], request from Angus.
// .247 20161201 Add Raw menu and add BASE POSITION commands, request from Ryan.
// .248 20161202 Fixed some bugs in BASE POSITION commands.
// .249 20161202 Fixed some text in BASE POSITION commands.
// .250 20161212 Add RTK precisely kinematic base mode, request from Ryan.
// .251 20161212 Parser supports RAW+NMEA, [Configure RTK Mode And Operational Function] add description, request from Ryan.
// .252 20161228 New Save HostLog function.
// .253 20170104 Fixed compatibility issues with Windows XP, reprot from Andrew.
// .254 20170105 Display time when 0xE5 output, reprot from Ryan.
// .254 20170105 Fix NMEA sentence error issue in Binary output mode, report from Leo.
// .254 20170106 Add [Query Clock Offset] in Binary menu, request from Oliver.
// .255 20170106 New parser for multiple message RTCM_NEW_PARSER.
// .256 20170112 Add [Show Beidou Almanac] in menu, requast from forum user.
// .257 20170117 Add SHOW_COM_SELECT flag for show COM port select, requast from forum Leo.
// .258 20170117 Add RTCM / Ublox convert, requast from Roger and Andrew.
// .259 20170220 Move INS DR menu to customer release version, requast from Oliver.
// .260 20170120 Fix some crash issue in parser, requast from Alex.
// .260 20170224 Add DR prodeuction commands in INS DR menu, requast from Angus.
// .261 20170309 Fix KML converter bugs in quality mode, requast from Kevin.
// .262 20170309 Fix KML converter bugs (GSA parsing) in quality mode, requast from Kevin.
// .263 20170315 Add GPS L2 support(GPGSV2), requast from Eric.
// .264 20170322 Automatically fill in fields, requast from Ryan and Ken.
// .265 20170328 Support new 0xA8 format, size is 0x3F, requast from Andrew.
// .266 20170328 Fix 0xA8 time seconds not continuious issue, requast from Andrew.
// .267 20170405 Add all baud rate in download panel, requast from Leo.
// .268 20170407 XN120 change SNR test range (38 to 36), requast from Leo.
// .269 20170407 Add [Configure / Query SBAS Advance] and [Query SBAS Default], requast from Kevin.
// .270 20170413 Fix NMEA Parser bug, report from Ardrew.
// .271 20170413 Modify some Query / Configure SBAS Advance strings, requast from Kevin.
// .272 20170421 Modify [Reset RTK Engine], requast from Ryan.
// .273 20170426 Add [Read 0x60000000 to a File (1KB)], requast from Patrick.
// .273 20170426 Fix issue in GetBinaryAck(), when receive 0xA0, 0xA1 in content will no response. 
// .274 20170524 Test EON Flash Loader and modify some flow.
// .275 20170525 Support EON Flash download and check chipmode.
// .276 20170531 Add LITEON customer version, request from Oliver.
// .277 20170531 Support EON Flash download and protected 3/4 flassh, request from Patrick.
// .278 20170608 Add profile for NAVIC support, request from Oliver and Terrance.
// .279 20170608 Add FLOAT_SNR to support NMEA GSV output flaot in CN, request from Oliver.
// .280 20170622 Navic support NMEA 183 4.1, modify GSA/GSV parser, request from Oliver.
// .281 20170622 Fix NMEA Parser bugs, request from Terrance.
// .282 20170623 Support master/slave download, request from Oliver.

#define SW_FUN_DATALOG		        0x0001
#define SW_FUN_AGPS				        0x0002
#define SW_FUN_DR				          0x0004                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

#define SOFTWARE_FUNCTION		      (SW_FUN_DATALOG | SW_FUN_AGPS)
#define IS_DEBUG				          0
//title.Format("%s %s V%s for %s", APP_CAPTION, APP_TITLE, APP_VERSION, APP_MODULE);
#define APP_CAPTION				        "GNSS Viewer"
#define APP_VERSION				        "2.0.282"
#define APP_TITLE				          ""
#define APP_MODULE				        "Venus 8"

#define	Sktyraq					          0x0000
#define SWID					            0x0001		//重慶西南集成電路設計
#define Ericsson				          0x0004
#define OlinkStar				          0x4F4C
#define Eten					            0x000A

//For customer upgrade
#define UPGRADE_DOWNLOAD		      0
#define UPGRADE_CRC				        0x0000
#define UPGRADE_DUEDATE_Y		      0
#define UPGRADE_DUEDATE_M		      0
#define UPGRADE_DUEDATE_D		      0
#define UPGRADE_ADD_TAG			      0
#define UPGRADE_TAG_VALUE		      0

//Default download boost baudrate
#define BAUDRATE_DEFAULT		      5		//8=921600,7=460800,6=230400,5=115200,4=57600,3=38400,2=19200,1=9600,0=4800
//Define FIRMWARE_DOWNLOAD 0 to disable download UI in Viewer
#define FIRMWARE_DOWNLOAD		      1
//Default scatter count
#define MAX_SCATTER_COUNT		      300

#define ODOMETER_SUPPORT		      0
//#define BINARY_MESSAGE_INTERVAL	0
#define ACTIVATE_MINIHOMER		    0
#define NMEA_INPUT				        0
#define GSA_MAX_SATELLITE		      12
//Download using resend protocol
#define RESEND_DOWNLOAD			      1
//Display binary command in and ack in respond view.
#define _SHOW_BINARY_DATA_		    1
//Using external SREC file directly, no prompt.
#define _ALWAYS_USE_EXTERNAL_SREC_	0
//Add a tag for DR Firmware.
//#define _CREATE_LICENSE_TAG_	    0
//for [Reset Motion Sensor] Command
#define RESET_MOTION_SENSOR		    0
//Show eCompass calibration UI in Viewer
#define ECOMPASS_CALIBRATION	    0
#define TIMING_MODE				        1   //20170531, Alex modified from 0 to 1.
//Information has multiple pages
#define _TAB_LAYOUT_			        0

//#define _BAUTRATE_IDX_6_		    891200
#define OPEN_PINNING_RESERVE	    1
#define RESET_MOTION_SENSOR		    0
#define _V8_SUPPORT				        1
#define GG12A					            0
#define SHOW_CLOCK_OFFSET		      0
#define SHOW_NOISE				        0
#define DATA_POI				          0
#define TWIN_DATALOG			        0
#define CUSTOMER_DOWNLOAD		      0
#define CUSTOMER_ID				        Sktyraq

//NMEA PRN parsing type
//Type 0 - GL: 65~96; GP: 1~64, 183~188, 193~197; BD: others
//Type 1 - GL: 201~300; GP: 101~200; BD: 1~100; GA: 301~400
//Type 2 - GP: 1~85; BD: others
//Type 3 - GL: 65~96; GP: 1~64, 183~188, 193~197; BD: others; Don't care GPS_183_188 flag.
#define NMEA_PRN_TYPE			        0

#define TIMING_MONITORING		      1
#define TIMING_OUTPUT_ALIGN		    1
#define	SHOW_ERROR_NMEA_NOTIFY	  0
#define	MORE_ENU_SCALE			      1
#define	GPS_183_188				        1
#define	_MODULE_SUP_800_		      0
#define	_RESOURCE_LOADER_ID_	    0		//specify a special download loader for customer.
#define CHECK_SAEE_MULTIHZ_ON	    0		//Check SAEE and Multi-Hz can't both on.
#define INVERT_LON_LAT			      1		//Final spec for GeoFecing spec.
#define MORE_INFO				          0		//More information field for RTK
#define RAW_MENU				          1		//Show RAW menu 20161201 request from Ryan
#define RTK_MENU				          1		//Show RTK menu
#define INS_DR_MENU				        1	  //Show INS_DR menu
#define CLIENT_WIDTH			        1008	//Viewer window client width
#define CLIENT_HEIGHT			        678	//Viewer window client height
#define AUTO_QUERY_VERSION		    0
#define DOWNLOAD_IMMEDIATELY	    0		//start download immediately when nmea come in.
#define SPECIAL_TEST			        0		//Test ETEN case 20160202
#define GEO_FENCING_CMD			      1		//0 - old geo-fencing cmd, 1 - new geo-fencing cmd
#define SHOW_RTK_BASELINE		      0
#define SHOW_PATCH_MENU			      0		//Use for user patch.
#define DOFUN_UNIQUE_ID			      0		//Show DOFUN UNIQUE ID menu
#define _DEVELOPER_			          0		//Developer version.
#define SPECIAL_BAUD_RATE			    0		//For RTK GLONASS EVK, baud rate * 1.5
#define CUSTOMER_CWQX_160815  		0   //上海長望氣象, Add 1200, 2400 bps, maximum 115200 bps
#define FIX_DOWNLOAD_115200   		0   //Download can only use 115200 bps
#define CUSTOMER_ZENLANE_160808  	0   //善領科技, Add customized commands
#define SUPPORT_BDL2_GSV2         0   //Support BDL2 information in GSV2 token.
#define RTCM_NEW_PARSER         	1   //Use RTCM new parser.
#define SHOW_COM_SELECT           1   //Show COM port select in [Configure Serial Port]
#define SMALL_UI                  0   //Small UI, No Earth View, Scatter and menu
#define KML_USE_CHECKLISTBOX      1   //Use CCheckListBox in CKmlDlg
#define LITEON_CUSTOMIZE          0   //Use LITEON special version
#define FLOAT_SNR                 0   //Use FLOAT SNR in GSV token



//#define _NAVIC_CONVERT_           1   //Use FLOAT SNR in GSV token

////////////////////////////////////////////////////////////////////////////////////////////
#if defined(SWCFG_VENDOR_NAVIC_INTERNALUSE) //20170608 for Oliver demo
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef MAX_SCATTER_COUNT
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			        "GNSS Viewer(NAVIC)"
 #define APP_TITLE				        "Internal Use"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          1
 #define BAUDRATE_DEFAULT		      7
 #define SHOW_ERROR_NMEA_NOTIFY   1
 #define MAX_SCATTER_COUNT		    1000
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define _NAVIC_CONVERT_			    1	  //Please add _NAVIC_CONVERT_ in resource prheader

#elif defined(SWCFG_VENDOR_NAVIC_GENERAL)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			        "GNSS Viewer(NAVIC)"
 #define APP_TITLE				        "Customer Release"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          0
 #define BAUDRATE_DEFAULT		      7
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define _NAVIC_CONVERT_			    1	  //Please add _NAVIC_CONVERT_ in resource prheader

#elif defined(SWCFG_VENDOR_GNSS_LITEON_170531) //20170531 for LITEON
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef BAUDRATE_DEFAULT
 #undef _TAB_LAYOUT_
 #undef LITEON_CUSTOMIZE

 #define APP_CAPTION			          "GNSS Viewer"
 #define APP_TITLE				          "Customer Release"
 #define APP_MODULE				          ""
 #define BAUDRATE_DEFAULT		        5
 #define _TAB_LAYOUT_			          1 //Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define LITEON_CUSTOMIZE           1

#elif defined(SWCFG_VENDOR_GNSS_XN120_TESTER_BEIDOU) //20170308 for Leo
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef SMALL_UI
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT

 #define APP_CAPTION			          "XN120 Tester Viewer"
 #define APP_TITLE				          "(GPS + BEIDOU2)"
 #define APP_MODULE				          ""
 #define GNSS_VIEWER			          1
 #define IS_DEBUG				            0
 #define BAUDRATE_DEFAULT		        7
 #define TIMING_MODE			          1
 #define MORE_INFO				          0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			          0		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define SMALL_UI			              1
 #define CLIENT_WIDTH			    577
 #define CLIENT_HEIGHT			  341
 #define XN120_TESTER
 #define XN120_TESTER_BEIDOU

#elif defined(SWCFG_VENDOR_GNSS_XN120_TESTER) //20170308 for Leo
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef SMALL_UI
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT

 #define APP_CAPTION			          "XN120 Tester Viewer"
 #define APP_TITLE				          "(GPS Only)"
 #define APP_MODULE				          ""
 #define GNSS_VIEWER			          1
 #define IS_DEBUG				            0
 #define BAUDRATE_DEFAULT		        7
 #define TIMING_MODE			          1
 #define MORE_INFO				          0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			          0		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define SMALL_UI			              1
 #define CLIENT_WIDTH			    577
 #define CLIENT_HEIGHT			  341
 #define XN120_TESTER

#elif defined(SWCFG_VENDOR_GNSS_CUSTOMER_PRODUCTION_OLIVER20161128) //20161128 for Oliver
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_VERSION				    
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define APP_VERSION			"2.1.245"

 #define GNSS_VIEWER			1
 #define IS_DEBUG				  0
 #define BAUDRATE_DEFAULT	7
 #define TIMING_MODE			1
 #define MORE_INFO				0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define PRODUCTION_OLIVER20161128
 #define CUSTOMIZE_COSDSTART_BUTTON

#elif defined(SWCFG_VENDOR_GNSS_NMEAPLAYER_BDL2) //20161018 For Patrick and Evan
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_INPUT
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef SUPPORT_BDL2_GSV2

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "NMEA Player"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          0
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define NMEA_INPUT				        1
 #define SHOW_ERROR_NMEA_NOTIFY   1
 #define CLIENT_WIDTH			        1008	
 #define CLIENT_HEIGHT			      614
 #define MORE_INFO				        0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define SUPPORT_BDL2_GSV2          1

#elif defined(SWCFG_VENDOR_GNSS_BDL2_NMEA)  //20161018 For Patrick and Evan
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef SUPPORT_BDL2_GSV2

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "Internal Use L2"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          1
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define SHOW_ERROR_NMEA_NOTIFY   1
 #define MORE_INFO				        0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define SUPPORT_BDL2_GSV2          1

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_ZENLANE_160808)  //For 善領科技, 20160808, Request from Leo
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef CUSTOMER_ZENLANE_160808

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "Zenlane Release"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          0
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define MORE_INFO				        0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define CUSTOMER_ZENLANE_160808	1

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_CWQX_160815)  //For 上海長望氣象, 20160815, Request from Leo, Andrew
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef CUSTOMER_CWQX_160815
 #undef FIX_DOWNLOAD_115200

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "CWQX Release"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          0
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define MORE_INFO				        0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define CUSTOMER_CWQX_160815			1
 #define FIX_DOWNLOAD_115200			1

#elif defined(SWCFG_VENDOR_GNSS_SUP800_NMEAPLAYER)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_INPUT
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT
 #undef _MODULE_SUP_800_

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "SUP800 Player"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          0
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define NMEA_INPUT				        1
 #define SHOW_ERROR_NMEA_NOTIFY   1
 #define CLIENT_WIDTH			        1008	
 #define CLIENT_HEIGHT			      614
 #define _MODULE_SUP_800_         1

#elif defined(SWCFG_VENDOR_GNSS_SPECIAL_BAUDRATE)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef _TAB_LAYOUT_
 #undef _DEVELOPER_
 #undef SPECIAL_BAUD_RATE

 #define APP_CAPTION			  "GNSS Viewer"
 #define APP_TITLE				  "Special Baud Rate"
 #define GNSS_VIEWER			  1
 #define IS_DEBUG				    0
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			  1
 #define _TAB_LAYOUT_			  1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define _DEVELOPER_	      1
 #define SPECIAL_BAUD_RATE	1

#elif defined(SWCFG_VENDOR_GNSS_DEVELOPER)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef _TAB_LAYOUT_
 #undef _DEVELOPER_

 #define APP_CAPTION			  "GNSS Viewer"
 #define APP_TITLE				  "Developer Release"
 #define GNSS_VIEWER			  1
 #define IS_DEBUG				    0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			  1
 #define _TAB_LAYOUT_			  1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define _DEVELOPER_	      1

#elif defined(SWCFG_VENDOR_GNSS_CUSTOMER_DOFUN_20160707)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_
 #undef DOFUN_UNIQUE_ID

 #define APP_CAPTION			  "GNSS Viewer"
 #define APP_TITLE				  "Unique ID Release"
 #define GNSS_VIEWER			  1
 #define IS_DEBUG				    0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			  1
 #define MORE_INFO				  0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			  1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.
 #define DOFUN_UNIQUE_ID		1		

#elif defined(SWCFG_VENDOR_NSHP_FIX_TOOL)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef MAX_SCATTER_COUNT
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT
 #undef SHOW_PATCH_MENU
 #undef SHOW_PATCH_MENU		
 #undef UPGRADE_DOWNLOAD		
 #undef UPGRADE_CRC			
 #undef UPGRADE_DUEDATE_Y		
 #undef UPGRADE_DUEDATE_M	
 #undef UPGRADE_DUEDATE_D	
 #undef UPGRADE_ADD_TAG	
 #undef UPGRADE_TAG_VALUE	

 #define APP_CAPTION			    "RTK Patch Tool"
 #define APP_TITLE				    ""
 #define GNSS_VIEWER			    1
 #define IS_DEBUG				      0
 #define BAUDRATE_DEFAULT		  6
 #define MAX_SCATTER_COUNT		100
 #define CLIENT_WIDTH			    292		//Viewer window client width
 #define CLIENT_HEIGHT			  398		//Viewer window client height
 #define SHOW_PATCH_MENU		  1
 #define UPGRADE_DOWNLOAD		  0
 #define UPGRADE_CRC			    0xFFFFFFFF
 #define UPGRADE_DUEDATE_Y		2017
 #define UPGRADE_DUEDATE_M		1
 #define UPGRADE_DUEDATE_D		31
 #define UPGRADE_ADD_TAG		  1
 #define UPGRADE_TAG_VALUE		0xA012

#elif defined(SWCFG_VENDOR_GNSS_NMEAPLAYER_NEW)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_INPUT
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT
 #undef MORE_INFO
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			      "GNSS Viewer"
 #define APP_TITLE				      "NMEA Player"
 #define GNSS_VIEWER			      1
 #define IS_DEBUG				        0
 #define BAUDRATE_DEFAULT		    7
 #define TIMING_MODE			      1
 #define NMEA_INPUT				      1
 #define SHOW_ERROR_NMEA_NOTIFY 1
 #define CLIENT_WIDTH			      1008	
 #define CLIENT_HEIGHT			    614
 #define MORE_INFO				      0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			      1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_NEW)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define MORE_INFO				0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.

#elif defined(SWCFG_VENDOR_GNSS_INTERNALUSE_NEW)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef MAX_SCATTER_COUNT
 #undef MORE_INFO
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "Internal Use"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          1
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define SHOW_ERROR_NMEA_NOTIFY   1
 #define MAX_SCATTER_COUNT		    1000
 #define MORE_INFO				        0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			        1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.

#elif defined(SWCFG_VENDOR_GNSS_ADDTAG)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SMALL_UI
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT
 #undef AUTO_QUERY_VERSION

 #define APP_CAPTION			    "GNSS Viewer"
 #define APP_TITLE				    "SANITMAX Release"
 #define GNSS_VIEWER			    1
 #define IS_DEBUG				      0
 #define BAUDRATE_DEFAULT		  7
 #define TIMING_MODE			    1
 #define SMALL_UI			        1
 #define CLIENT_WIDTH			    577
 #define CLIENT_HEIGHT			  341
 #define AUTO_QUERY_VERSION		1

#elif defined(SWCFG_VENDOR_GNSS_ETEN20160330)	//Request from Leo
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef SHOW_RTK_BASELINE
 #undef CUSTOMER_ID
 #undef RTK_MENU

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define APP_MODULE				"Venus 8"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define MORE_INFO				0		//Please define _MORE_INFO_ for rc2 too.
 #define SHOW_RTK_BASELINE		0		//Please define SHOW_RTK_BASELINE for rc2 too.
 #define CUSTOMER_ID			Eten
 #define RTK_MENU				0

#elif defined(SWCFG_VENDOR_GNSS_KALMAN)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO
 #undef SHOW_RTK_BASELINE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define APP_MODULE				"Venus 8 RTK"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define MORE_INFO				1		//Please define _MORE_INFO_ for rc2 too.
 #define SHOW_RTK_BASELINE		1		//Please define SHOW_RTK_BASELINE for rc2 too.

#elif defined(SWCFG_VENDOR_GNSS_SAINTMAX)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SMALL_UI
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT
 #undef AUTO_QUERY_VERSION

 #define APP_CAPTION			    "GNSS Viewer"
 #define APP_TITLE				    "SANITMAX Release"
 #define GNSS_VIEWER			    1
 #define IS_DEBUG				      0
 #define BAUDRATE_DEFAULT		  7
 #define TIMING_MODE			    1
 #define SMALL_UI			        1
 #define CLIENT_WIDTH			    577
 #define CLIENT_HEIGHT			  341
 #define AUTO_QUERY_VERSION		1
 #define SAINTMAX_UI			    1

#elif defined(SWCFG_VENDOR_GNSS_NMEAPLAYER_RTK)	//20151130 request from Oliver
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_INPUT
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef MORE_INFO
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"NMEA Player"
 #define APP_MODULE				"Venus 8 RTK"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define NMEA_INPUT				1
 #define SHOW_ERROR_NMEA_NOTIFY 1
 #define MORE_INFO				1		//Please define _MORE_INFO_ for rc2 too.
 #define CLIENT_WIDTH			1008
 #define CLIENT_HEIGHT			614

#elif defined(SWCFG_VENDOR_GNSS_RTK)	//20151130 request from Oliver
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef APP_MODULE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef MORE_INFO

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define APP_MODULE				"Venus 8 RTK"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define MORE_INFO				1		//Please define _MORE_INFO_ for rc2 too.

#elif defined(SWCFG_VENDOR_GNSS_TAKUJI)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef UPGRADE_DOWNLOAD
 #undef UPGRADE_CRC
 #undef UPGRADE_DUEDATE_Y		
 #undef UPGRADE_DUEDATE_M		
 #undef UPGRADE_DUEDATE_D		
 #undef UPGRADE_ADD_TAG
 #undef UPGRADE_TAG_VALUE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Takuji Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define UPGRADE_DOWNLOAD		1
 #define UPGRADE_CRC			0xce72
 #define UPGRADE_DUEDATE_Y		2015
 #define UPGRADE_DUEDATE_M		8
 #define UPGRADE_DUEDATE_D		31
 #define UPGRADE_ADD_TAG		1
 #define UPGRADE_TAG_VALUE		0x0A01

#elif defined(SWCFG_VENDOR_GNSS_SWID_NEW_DOWNLOAD)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef _RESOURCE_LOADER_ID_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define _RESOURCE_LOADER_ID_	1351

#elif defined(SWCFG_VENDOR_GNSS_SUP800_GENERAL)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef _MODULE_SUP_800_
 #undef MAX_SCATTER_COUNT

 #define APP_CAPTION			        "GNSS Viewer"
 #define APP_TITLE				        "SUP800 Customer Release"
 #define GNSS_VIEWER			        1
 #define IS_DEBUG				          0
 #define BAUDRATE_DEFAULT		      7
 #define TIMING_MODE			        1
 #define SHOW_ERROR_NMEA_NOTIFY   1
 #define _MODULE_SUP_800_		      1     //Please also define _MODULE_SUP_800_ in resource header
 #define MAX_SCATTER_COUNT        100

#elif defined(SWCFG_VENDOR_GNSS_SUP800)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef _MODULE_SUP_800_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"SUP800 Tester"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				1
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define SHOW_ERROR_NMEA_NOTIFY 1
 #define _MODULE_SUP_800_		1

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_OLINKSTAR_PROTECT)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef CUSTOMER_DOWNLOAD
 #undef DefaultCuteomer

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"OLinkStar Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define CUSTOMER_DOWNLOAD		1
 #define DefaultCuteomer		OlinkStar

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_ERICSSON)
//ERICSSON from 重慶西南集成電路設計
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef CUSTOMER_DOWNLOAD
 #undef DefaultCuteomer
 #undef TIMING_MONITORING
 #undef TIMING_OUTPUT_ALIGN
 #undef NMEA_PRN_TYPE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Ericsson Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define CUSTOMER_DOWNLOAD		0
 #define DefaultCuteomer		Ericsson
 #define TIMING_MONITORING		0
 #define TIMING_OUTPUT_ALIGN	0
 #define NMEA_PRN_TYPE			2

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_RDRNSS_PROTECT)
//SWID 重慶西南集成電路設計
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef CUSTOMER_DOWNLOAD
 #undef DefaultCuteomer
 #undef TIMING_MONITORING
 #undef TIMING_OUTPUT_ALIGN

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"RDRNSS Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define CUSTOMER_DOWNLOAD		0
 #define DefaultCuteomer		SWID
 #define TIMING_MONITORING	0
 #define TIMING_OUTPUT_ALIGN	0

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_SWID_PROTECT)
//SWID 重慶西南集成電路設計
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef CUSTOMER_DOWNLOAD
 #undef DefaultCuteomer
 #undef TIMING_MONITORING
 #undef TIMING_OUTPUT_ALIGN

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"SWID Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define CUSTOMER_DOWNLOAD		0
 #define DefaultCuteomer			SWID
 #define TIMING_MONITORING	0
 #define TIMING_OUTPUT_ALIGN	0

#elif defined(SWCFG_VENDOR_GNSS_INTERNALUSE_BDV8)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef MAX_SCATTER_COUNT
 #undef MORE_INFO
 #undef _TAB_LAYOUT_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Internal Use"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				1
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define SHOW_ERROR_NMEA_NOTIFY 1
 #define MAX_SCATTER_COUNT		1000
 #define MORE_INFO				0		//Please define _MORE_INFO_ in resource preprocessor for rc2.
 #define _TAB_LAYOUT_			1		//Please define _TAB_LAYOUT_ in resource preprocessor for rc2.

#elif defined(SWCFG_VENDOR_GNSS_INTERNALUSE_GPS_183)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef NMEA_PRN_TYPE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Internal Use GPS 183-188"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				1
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define SHOW_ERROR_NMEA_NOTIFY 1
 #define NMEA_PRN_TYPE			3

#elif defined(SWCFG_VENDOR_GNSS_BDV8)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1

#elif defined(SWCFG_VENDOR_GNSS_FKHL)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_PRN_TYPE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release FKHL"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define NMEA_PRN_TYPE			1

#elif defined(SWCFG_VENDOR_GNSS_FKHL_Test)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_PRN_TYPE
 #undef NMEA_INPUT

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release FKHL"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define NMEA_PRN_TYPE			1
 #define NMEA_INPUT				1

#elif defined(SWCFG_VENDOR_GNSS_BDV8_NMEAPLAYER)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef NMEA_INPUT
 #undef SHOW_ERROR_NMEA_NOTIFY
 #undef CLIENT_WIDTH
 #undef CLIENT_HEIGHT

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"NMEA Player"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define NMEA_INPUT				1
 #define SHOW_ERROR_NMEA_NOTIFY 1
 #define CLIENT_WIDTH			1008	
 #define CLIENT_HEIGHT			614

#elif defined(SWCFG_VENDOR_GNSS_V8)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Internal Use"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				1
 #define BAUDRATE_DEFAULT		6
 #define TIMING_MODE			1

#elif defined(SWCFG_VENDOR_CREATETAG)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef _CREATE_LICENSE_TAG_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Create Tag"
 #define GNSS_VIEWER				1
 #define IS_DEBUG					0
 #define BAUDRATE_DEFAULT			7
 #define TIMING_MODE				1
 #define _CREATE_LICENSE_TAG_		1

#elif defined(SWCFG_VENDOR_GENERAL_DR)
 #undef APP_TITLE				
 #undef BAUDRATE_DEFAULT
 #undef SOFTWARE_FUNCTION

 #define APP_TITLE				    "Customer Release DR"
 #define BAUDRATE_DEFAULT		  5
 #define SOFTWARE_FUNCTION		(SW_FUN_DATALOG | SW_FUN_AGPS | SW_FUN_DR)

#else
 #error Unknow SWCFG_VENDER_TYPE!!!
#endif

