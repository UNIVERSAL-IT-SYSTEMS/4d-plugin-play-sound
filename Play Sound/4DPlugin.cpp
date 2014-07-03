/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.c
 #	source generated by 4D Plugin Wizard
 #	Project : Play Sound
 #	author : miyako
 #	2014/07/03
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

#if VERSIONWIN
#pragma comment(lib, "winmm.lib")
#include <tchar.h> //for _stprintf_s
#else
NSMutableDictionary *aliases;
#endif

void PluginMain(int32_t selector, PA_PluginParameters params)
{
	try
	{
		int32_t pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (int32_t pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
#if VERSIONMAC	
		case kInitPlugin :
		case kServerInitPlugin :			
			aliases = [[NSMutableDictionary alloc]init];			
			break;
			
		case kDeinitPlugin :
		case kServerDeinitPlugin :			
			[aliases release];
			break;		
#endif	
// --- Play Sound

		case 1 :
			SOUND_Open(pResult, pParams);
			break;

		case 2 :
			SOUND_Pause(pResult, pParams);
			break;

		case 3 :
			SOUND_Play(pResult, pParams);
			break;

		case 4 :
			SOUND_Stop(pResult, pParams);
			break;

		case 5 :
			SOUND_Close(pResult, pParams);
			break;

		case 6 :
			SOUND_Resume(pResult, pParams);
			break;

	}
}

// ---------------------------------- Play Sound ----------------------------------

#if VERSIONMAC
void removeAliasKey(NSString *fileAlias){
	NSSound *sound = [aliases valueForKey:fileAlias];
	if(sound){
		[sound stop];		
		[sound release];
		[aliases removeObjectForKey:fileAlias];
	}	
}
#endif

void SOUND_Open(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT Param2;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);

#if VERSIONWIN
	wchar_t commandString[_MAX_PATH + 128] = {0};
	_stprintf_s(commandString, TEXT("open \"%s\" alias \"%s\" %s"), (wchar_t *)Param1.getUTF16StringPtr(), (wchar_t *)Param2.getUTF16StringPtr(), L"wait");
	sendCommand(commandString, returnValue);	
#else
	NSURL *url = Param1.copyUrl();
	if(url){
		NSString *fileAlias = Param2.copyUTF16String();
		removeAliasKey(fileAlias);
		NSSound* sound = [[NSSound alloc]initWithContentsOfURL:url byReference:YES];
		[aliases setValue:sound forKey:fileAlias];
		[url release];
		[fileAlias release];
	}
#endif

	returnValue.setReturn(pResult);
}

#if VERSIONMAC
void sendCommand(C_TEXT &Param1, NSString *selectorName){
	NSString *fileAlias = Param1.copyUTF16String();
	NSSound *sound = [aliases valueForKey:fileAlias];
	if(sound){
		[sound performSelector:NSSelectorFromString(selectorName)];
	}
	[fileAlias release];
}
#endif

void SOUND_Pause(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);

#if VERSIONWIN
	wchar_t commandString[_MAX_PATH + 128] = {0};
	_stprintf_s(commandString, TEXT("pause \"%s\" %s"), (wchar_t *)Param1.getUTF16StringPtr(), L"wait");
	sendCommand(commandString, returnValue);	
#else
	sendCommand(Param1, @"pause");
#endif

	returnValue.setReturn(pResult);
}

void SOUND_Play(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);

#if VERSIONWIN
	wchar_t commandString[_MAX_PATH + 128] = {0};
	_stprintf_s(commandString, TEXT("play \"%s\""), (wchar_t *)Param1.getUTF16StringPtr());
	sendCommand(commandString, returnValue);	
#else
	sendCommand(Param1, @"play");	
#endif

	returnValue.setReturn(pResult);
}

void SOUND_Stop(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);

#if VERSIONWIN
	wchar_t commandString[_MAX_PATH + 128] = {0};
	_stprintf_s(commandString, TEXT("stop \"%s\" %s"), (wchar_t *)Param1.getUTF16StringPtr(), L"wait");
	sendCommand(commandString, returnValue);	
#else
	sendCommand(Param1, @"stop");	
#endif

	returnValue.setReturn(pResult);
}

void SOUND_Close(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);

#if VERSIONWIN
	wchar_t commandString[_MAX_PATH + 128] = {0};
	_stprintf_s(commandString, TEXT("close \"%s\" %s"), (wchar_t *)Param1.getUTF16StringPtr(), L"wait");
	sendCommand(commandString, returnValue);	
#else
	NSString *fileAlias = Param1.copyUTF16String();
	removeAliasKey(fileAlias);
	[fileAlias release];
#endif

	returnValue.setReturn(pResult);
}

void SOUND_Resume(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;	
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);

#if VERSIONWIN
	wchar_t commandString[_MAX_PATH + 128] = {0};
	_stprintf_s(commandString, TEXT("resume \"%s\" %s"), (wchar_t *)Param1.getUTF16StringPtr(), L"wait");
	sendCommand(commandString, returnValue);
#else
	sendCommand(Param1, @"resume");	
#endif	

	returnValue.setReturn(pResult);
}

#if VERSIONWIN
void sendCommand(wchar_t *commandString, C_TEXT &returnValue){
	wchar_t errorText[128] = {0};
	MCIERROR errorCode = mciSendString(commandString, NULL, 0, NULL);
	if(mciGetErrorString(errorCode, errorText, 128)){
		CUTF16String err = CUTF16String((PA_Unichar *)errorText);
		returnValue.setUTF16String(&err);
	}
}
#endif	